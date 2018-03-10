#include "FlatFlux.h"

#include <chrono>
#include <iostream>

#include <cstdlib>

#define NUM_SEGMENTS 1e7

void test_flat_flux()
{
  std::vector<Real> scalar_flux(10 * NUM_POLAR * NUM_GROUPS);
  std::vector<Real> fsr_solution(10 * NUM_POLAR * NUM_GROUPS);
  std::vector<Real> Q(10 * NUM_POLAR * NUM_GROUPS);

  for (auto & val : scalar_flux)
    val = (double)rand()/(double)RAND_MAX;

  for (auto & val : fsr_solution)
    val = (double)rand()/(double)RAND_MAX;

  for (auto & val : Q)
    val = (double)rand()/(double)RAND_MAX;

  FlatFlux ff(scalar_flux, fsr_solution, Q);

  std::cout<<"Starting Optimized"<<std::endl;
  auto start = std::chrono::high_resolution_clock::now();
  for (unsigned long int s = 0; s < NUM_SEGMENTS; s++)
    ff.onSegment();
  std::chrono::duration<Real> optimized_duration = std::chrono::high_resolution_clock::now() - start;

  std::cout<<"optmized: "<<optimized_duration.count()<<std::endl;
}
