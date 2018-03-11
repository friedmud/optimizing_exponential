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

#ifndef INTELVECTORCLASSFLATFLUX_H
#define INTELVECTORCLASSFLATFLUX_H

#include "flat_flux_common.h"

#define MAX_VECTOR_SIZE 256

#include "../vecmath/vectorclass.h"
#include "../vecmath/vectormath_exp.h"

#include <vector>

class IntelVectorClassFlatFlux
{
public:
  IntelVectorClassFlatFlux(std::vector<Real> & scalar_flux, std::vector<Real> & fsr_solution, std::vector<Real> & Q);
  virtual ~IntelVectorClassFlatFlux();

  /**
   * Called on each Segment
   * @param start The beginning of the segment
   * @param end The end of the segment
   */
  virtual void onSegment();

protected:
  /// Distance to travel before accumulating into scalar flux
  const Real _dead_zone;

  const unsigned int _num_groups;

  const unsigned int _num_polar;

  /// Offest into the vectors
  unsigned int _current_offset;

  /// Offset into the FSR vectors
  unsigned int _current_fsr_offset;

  Real * _scalar_flux;

  Real * _fsr_volumes;

  Real * _Q;

  /// The Azimuthal spacing for the Ray
  Real _azimuthal_spacing = 0.01;

  /// The Azimuthal weight for the Ray
  Real _azimuthal_weight = 0.02;

  /// Pointer to the beginning of the Ray's data
  Real _angular_flux[NUM_POLAR * NUM_GROUPS];

  /// Polar spacing
  Real _polar_spacing = 0.02;

  /// Sin of the polar angle
  Real _polar_sins[NUM_POLAR];

  /// Weights for the polar angles
  Real _polar_weights[NUM_POLAR];

  std::vector<Real> _delta_angular_flux;

  std::vector<Real> _exp_tau;

  unsigned long _counter = 0;

  std::vector<double> _sigma_t;

private:
  Vec4d _current_sigma_t;

  Vec4d _current_exp_tau;

  Vec4d _current_Q;

  Vec4d _current_delta_angular_flux;

  Vec4d _current_angular_flux;

  Vec4d _current_scalar_flux;
};

#endif /* INTELVECTORCLASSFLATFLUX_H */

#endif
