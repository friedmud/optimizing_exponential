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

#include "FMathFlatFlux.h"

#include "fmath.h"

#include<cmath>
#include<iostream>

FMathFlatFlux::FMathFlatFlux(std::vector<Real> & scalar_flux, std::vector<Real> & fsr_solution, std::vector<Real> & Q) :
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

FMathFlatFlux::~FMathFlatFlux() {}

void
FMathFlatFlux::onSegment()
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

#pragma clang loop vectorize_width(8) interleave_count(8)
    for (unsigned int g = 0; g < _num_groups; g++)
      _exp_tau[g] = -segment_length * _sigma_t[g];

#pragma clang loop vectorize_width(8) interleave_count(8)
    for (unsigned int g = 0; g < _num_groups; g++)
      _exp_tau[g] = fmath::expd(_exp_tau[g]);

#pragma clang loop vectorize_width(8) interleave_count(8)
    for (unsigned int g = 0; g < _num_groups; g++)
      _exp_tau[g] = 1. - _exp_tau[g];

#pragma clang loop vectorize_width(8) interleave_count(8)
    for (unsigned int g = 0; g < _num_groups; g++)
      current_delta_angular_flux[g] = (current_angular_flux[g] - current_Q[g]) * _exp_tau[g];

#pragma clang loop vectorize_width(8) interleave_count(8)
    for (unsigned int g = 0; g < _num_groups; g++)
      current_angular_flux[g] -= current_delta_angular_flux[g];

    if (-1 > _dead_zone)
    {
      const Real scalar_flux_multiplier = 4.0 * PI * _azimuthal_spacing * _polar_spacing *
                                          _azimuthal_weight * polar_weight * polar_sin;

      /*
#pragma clang loop vectorize_width(8) interleave_count(8)
      for (unsigned int g = 0; g < _num_groups; g++)
        current_scalar_flux[g] += scalar_flux_multiplier * current_delta_angular_flux[g];
      */

      /*
      _fsr_volumes[_current_fsr_offset] += _azimuthal_spacing * _azimuthal_weight *
                                           tracking_segment_length * 0.5 * _polar_spacing *
                                           polar_weight;
      */
    }
  }
/*
  auto integrated_distance = 10023;

  if (-1 > _dead_zone)
#pragma clang loop vectorize_width(8) interleave_count(8)
    for (unsigned int p = 0; p < _num_polar; p++)
      integrated_distance += tracking_segment_length;
*/
}
