#include "OptimizedFlatFlux.h"
#include "FMathFlatFlux.h"
#include "MKLFlatFlux.h"
#include "VectorExpFlatFlux.h"
#include "VectorClassFlatFlux.h"
#include "IntelVectorClassFlatFlux.h"
#include "FloatVectorClassFlatFlux.h"

#include <chrono>
#include <iostream>

#include <cstdlib>

#define NUM_SEGMENTS 1e7

void test_flat_flux()
{
  std::vector<Real> scalar_flux(10 * NUM_POLAR * NUM_GROUPS);
  std::vector<Real> fsr_solution(10 * NUM_POLAR * NUM_GROUPS);
  std::vector<Real> Q(10 * NUM_POLAR * NUM_GROUPS);

  std::vector<float> float_scalar_flux(10 * NUM_POLAR * NUM_GROUPS);
  std::vector<float> float_fsr_solution(10 * NUM_POLAR * NUM_GROUPS);
  std::vector<float> float_Q(10 * NUM_POLAR * NUM_GROUPS);

  for (auto & val : scalar_flux)
    val = (double)rand()/(double)RAND_MAX;

  for (auto & val : fsr_solution)
    val = (double)rand()/(double)RAND_MAX;

  for (auto & val : Q)
    val = (double)rand()/(double)RAND_MAX;


  for (auto & val : float_scalar_flux)
    val = (float)rand()/(float)RAND_MAX;

  for (auto & val : float_fsr_solution)
    val = (float)rand()/(float)RAND_MAX;

  for (auto & val : float_Q)
    val = (float)rand()/(float)RAND_MAX;


  OptimizedFlatFlux off(scalar_flux, fsr_solution, Q);

  std::cout<<"Starting Optimized"<<std::endl;
  auto start = std::chrono::high_resolution_clock::now();
  for (unsigned long int s = 0; s < NUM_SEGMENTS; s++)
    off.onSegment();
  std::chrono::duration<Real> optimized_duration = std::chrono::high_resolution_clock::now() - start;

  FMathFlatFlux fmff(scalar_flux, fsr_solution, Q);

  std::cout<<"Starting FMath"<<std::endl;
  start = std::chrono::high_resolution_clock::now();
  for (unsigned long int s = 0; s < NUM_SEGMENTS; s++)
    fmff.onSegment();
  std::chrono::duration<Real> fmath_duration = std::chrono::high_resolution_clock::now() - start;

  MKLFlatFlux mklff(scalar_flux, fsr_solution, Q);

  std::cout<<"Starting MKL"<<std::endl;
  start = std::chrono::high_resolution_clock::now();
  for (unsigned long int s = 0; s < NUM_SEGMENTS; s++)
    mklff.onSegment();
  std::chrono::duration<Real> mkl_duration = std::chrono::high_resolution_clock::now() - start;

  VectorExpFlatFlux veff(scalar_flux, fsr_solution, Q);

  std::cout<<"Starting VectorExp"<<std::endl;
  start = std::chrono::high_resolution_clock::now();
  for (unsigned long int s = 0; s < NUM_SEGMENTS; s++)
    veff.onSegment();
  std::chrono::duration<Real> vector_exp_duration = std::chrono::high_resolution_clock::now() - start;

  VectorClassFlatFlux vcff(scalar_flux, fsr_solution, Q);

  std::cout<<"Starting VectorClass"<<std::endl;
  start = std::chrono::high_resolution_clock::now();
  for (unsigned long int s = 0; s < NUM_SEGMENTS; s++)
    vcff.onSegment();
  std::chrono::duration<Real> vector_class_duration = std::chrono::high_resolution_clock::now() - start;

#if defined(__INTEL_COMPILER)

  IntelVectorClassFlatFlux ivcff(scalar_flux, fsr_solution, Q);

  std::cout<<"Starting IntelVectorClass"<<std::endl;
  start = std::chrono::high_resolution_clock::now();
  for (unsigned long int s = 0; s < NUM_SEGMENTS; s++)
    ivcff.onSegment();
  std::chrono::duration<Real> intel_vector_class_duration = std::chrono::high_resolution_clock::now() - start;
#endif

  FloatVectorClassFlatFlux fvcff(float_scalar_flux, float_fsr_solution, float_Q);

  std::cout<<"Starting FloatVectorClass"<<std::endl;
  start = std::chrono::high_resolution_clock::now();
  for (unsigned long int s = 0; s < NUM_SEGMENTS; s++)
    fvcff.onSegment();
  std::chrono::duration<Real> float_vector_class_duration = std::chrono::high_resolution_clock::now() - start;

  std::cout<<"optmized: "<<optimized_duration.count()<<std::endl;
  std::cout<<"fmath: "<<fmath_duration.count()<<std::endl;
  std::cout<<"mkl: "<<mkl_duration.count()<<std::endl;
  std::cout<<"vector exp: "<<vector_exp_duration.count()<<std::endl;
  std::cout<<"vector class: "<<vector_class_duration.count()<<std::endl;

#if defined(__INTEL_COMPILER)
  std::cout<<"intel vector class: "<<intel_vector_class_duration.count()<<std::endl;
#endif

  std::cout<<"float vector class: "<<float_vector_class_duration.count()<<std::endl;
}
