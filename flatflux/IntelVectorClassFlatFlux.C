/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#if defined(__INTEL_COMPILER)

#include "IntelVectorClassFlatFlux.h"

#include<cmath>
#include<iostream>

IntelVectorClassFlatFlux::IntelVectorClassFlatFlux(std::vector<Real> & scalar_flux, std::vector<Real> & fsr_solution, std::vector<Real> & Q) :
    _dead_zone(0),
    _num_groups(NUM_GROUPS),
    _num_polar(NUM_POLAR),
    _current_offset(0),
    _current_fsr_offset(0),
    _scalar_flux(scalar_flux.data()),
    _fsr_volumes(fsr_solution.data()),
    _Q(Q.data()),
    _delta_angular_flux(_num_groups),
    _exp_tau(_num_groups),
    _sigma_t(_num_groups)
{
  for (unsigned int i = 0; i < NUM_GROUPS; i++)
    _sigma_t[i] = (double)i/(double)1000;

  for (unsigned int i = 0; i < NUM_POLAR * NUM_GROUPS; i++)
    _angular_flux[i] = (double)i/(double)230;
}

IntelVectorClassFlatFlux::~IntelVectorClassFlatFlux() {}

namespace
{
Vec4d a;
Vec4d b;
}


#define VecSize 4

inline void vectorizedExp(std::vector<Real> & vec, std::vector<Real> & out_vec)
{
  auto total_size = vec.size();

  unsigned int num_chunks = total_size / VecSize;

  unsigned int remainder = total_size % VecSize;

  auto out_array = out_vec.data();

  for (unsigned int chunk = 0; chunk < num_chunks; chunk++)
  {
    a.load(&vec[chunk*VecSize]);
    b = exp(a);
    b.store(out_array + (chunk * VecSize));
  }

  // The remaineder
  if (remainder)
  {
    a.load_partial(remainder, &vec[num_chunks*VecSize]);
    b = exp(a);
    b.store_partial(remainder, (out_array + (num_chunks * VecSize)));
  }
}

void
IntelVectorClassFlatFlux::onSegment()
{
  Real tracking_segment_length = 1.1;

  for (unsigned int p = 0; p < _num_polar; p++)
  {
    // Storing this locally enables vectorization
    const unsigned int current_offset = _current_offset;

    const Real polar_sin = _polar_sins[p];

    const Real polar_weight = _polar_weights[p];

    const unsigned int angular_offset = p * _num_groups;

    auto current_angular_flux = &_angular_flux[angular_offset];

    auto current_scalar_flux = &_scalar_flux[current_offset];

    auto current_Q = &_Q[current_offset];

    auto current_delta_angular_flux = &_delta_angular_flux[0];

    Real segment_length = tracking_segment_length / polar_sin;

    const Real scalar_flux_multiplier = 4.0 * PI * _azimuthal_spacing * _polar_spacing *
      _azimuthal_weight * polar_weight * polar_sin;

    // For vectorization
    unsigned int num_chunks = _num_groups / VecSize;

    unsigned int remainder = _num_groups % VecSize;

    // Loop over vectorized operations
    for (unsigned int chunk = 0; chunk < num_chunks; chunk++)
    {
      auto group_index = chunk * VecSize;

      _current_sigma_t.load(&_sigma_t[group_index]);
      _current_Q.load(&current_Q[group_index]);
      _current_angular_flux.load(&current_angular_flux[group_index]);
      _current_scalar_flux.load(&current_scalar_flux[group_index]);

      // _exp_tau[g] = -segment_length * _sigma_t[g];
      _current_exp_tau = _current_sigma_t * -segment_length;

      // _exp_tau[g] = std::exp(_exp_tau[g]);
      _current_exp_tau = expSVML(_current_exp_tau); // exp(_current_exp_tau);

      // _exp_tau[g] = 1. - _exp_tau[g];
      _current_exp_tau = 1 - _current_exp_tau;

      // current_delta_angular_flux[g] = (current_angular_flux[g] - current_Q[g]) * _exp_tau[g];
      _current_delta_angular_flux = (_current_angular_flux - _current_Q) * _current_exp_tau;

      // current_angular_flux[g] -= current_delta_angular_flux[g];
      _current_angular_flux -= _current_delta_angular_flux;

      // current_scalar_flux[g] += scalar_flux_multiplier * current_delta_angular_flux[g];
      _current_scalar_flux =  mul_add(scalar_flux_multiplier, _current_delta_angular_flux, _current_scalar_flux);

      // Store the results back out
      _current_angular_flux.store(&current_angular_flux[group_index]);
      _current_scalar_flux.store(&current_scalar_flux[group_index]);
    }

    _fsr_volumes[_current_fsr_offset] += _azimuthal_spacing * _azimuthal_weight * tracking_segment_length * 0.5 * _polar_spacing * polar_weight;
  }

  auto integrated_distance = 10023;

#pragma clang loop vectorize_width(4) interleave_count(4)
  for (unsigned int p = 0; p < _num_polar; p++)
    integrated_distance += tracking_segment_length;
}

#endif
