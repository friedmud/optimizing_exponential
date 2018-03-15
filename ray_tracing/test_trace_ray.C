#include "trace_ray.h"

#include "libmesh/point.h"

#include <chrono>

using namespace libMesh;

void test_trace_ray()
{
  Point o(0,0,0);
  Point d(2.,0.0,0);

  Point V00(1,-1,-1);
  Point V10(1,-1,1);
  Point V11(1,1,1);
  Point V01(1,1,-1);

  Real u, v, t;

  unsigned long int num_its = 1e8;

  std::cout << "Starting Vanilla" << std::endl;
  auto start = std::chrono::high_resolution_clock::now();
  for (unsigned long int i=0; i < num_its; i++)
    intersectQuad<Point>(o, d, V00, V10, V11, V01, u, v, t);
  std::chrono::duration<Real> vanilla_duration = std::chrono::high_resolution_clock::now() - start;

  std::cout << t << std::endl;

  std::cout << "vanilla: " << vanilla_duration.count() << std::endl;
}
