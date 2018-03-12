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

#ifndef FLOATVECTORCLASSFLATFLUX_H
#define FLOATVECTORCLASSFLATFLUX_H

#include "flat_flux_common.h"

#define MAX_VECTOR_SIZE 256

#include "../vecmath/vectorclass.h"
#include "../vecmath/vectormath_exp.h"

#include <vector>

class FloatVectorClassFlatFlux
{
public:
  FloatVectorClassFlatFlux(std::vector<float> & scalar_flux, std::vector<float> & fsr_solution, std::vector<float> & Q);
  virtual ~FloatVectorClassFlatFlux();

  /**
   * Called on each Segment
   * @param start The beginning of the segment
   * @param end The end of the segment
   */
  virtual void onSegment();

protected:
  /// Distance to travel before accumulating into scalar flux
  const float _dead_zone;

  const unsigned int _num_groups;

  const unsigned int _num_polar;

  /// Offest into the vectors
  unsigned int _current_offset;

  /// Offset into the FSR vectors
  unsigned int _current_fsr_offset;

  float * _scalar_flux;

  float * _fsr_volumes;

  float * _Q;

  /// The Azimuthal spacing for the Ray
  float _azimuthal_spacing = 0.01;

  /// The Azimuthal weight for the Ray
  float _azimuthal_weight = 0.02;

  /// Pointer to the beginning of the Ray's data
  float _angular_flux[NUM_POLAR * NUM_GROUPS];

  /// Polar spacing
  float _polar_spacing = 0.02;

  /// Sin of the polar angle
  float _polar_sins[NUM_POLAR];

  /// Weights for the polar angles
  float _polar_weights[NUM_POLAR];

  std::vector<float> _delta_angular_flux;

  std::vector<float> _exp_tau;

  unsigned long _counter = 0;

  std::vector<float> _sigma_t;

private:
  Vec8f _current_sigma_t;
  Vec8f _current_exp_tau;
  Vec8f _current_Q;
  Vec8f _current_delta_angular_flux;
  Vec8f _current_angular_flux;
  Vec8f _current_scalar_flux;
};

#endif /* FLOATVECTORCLASSFLATFLUX_H */
