#include "trace_ray.h"

#include "libmesh/point.h"
#include "libmesh/vectorized_point.h"

#include <chrono>

using namespace libMesh;

void test_trace_ray()
{
  std::chrono::duration<Real> vanilla_duration;
  std::chrono::duration<Real> vectorized_duration;
  std::chrono::duration<Real> tuned_duration;
  std::chrono::duration<Real> hand_duration;
  std::chrono::duration<Real> vanilla_triangles_duration;
  std::chrono::duration<Real> vectorized_triangles_duration;
  std::chrono::duration<Real> hand_triangles_duration;

  unsigned long int num_its = 1e8;;

  {
    Point o(0,0,0);
    Point d(2.,0.0,0);

    Point V00(1,-1,-1);
    Point V10(1,-1,1);
    Point V11(1,1,1);
    Point V01(1,1,-1);

    Real u, v, t;

    std::cout << "Starting Vanilla" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    for (unsigned long int i=0; i < num_its; i++)
      intersectQuad<Point>(o, d, V00, V10, V11, V01, u, v, t);
    vanilla_duration = std::chrono::high_resolution_clock::now() - start;

    std::cout<<t<<std::endl;
  }

  {
    VectorizedPoint o(0,0,0);
    VectorizedPoint d(2.,0.0,0);

    VectorizedPoint V00(1,-1,-1);
    VectorizedPoint V10(1,-1,1);
    VectorizedPoint V11(1,1,1);
    VectorizedPoint V01(1,1,-1);

    Real u, v, t;

    std::cout << "Starting Vectorized" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    for (unsigned long int i=0; i < num_its; i++)
      intersectQuad<VectorizedPoint>(o, d, V00, V10, V11, V01, u, v, t);
    vectorized_duration = std::chrono::high_resolution_clock::now() - start;

    std::cout<<t<<std::endl;
  }

  {
    VectorizedPoint o(0,0,0);
    VectorizedPoint d(2.,0.0,0);

    VectorizedPoint V00(1,-1,-1);
    VectorizedPoint V10(1,-1,1);
    VectorizedPoint V11(1,1,1);
    VectorizedPoint V01(1,1,-1);

    Real u, v, t;

    std::cout << "Starting Tuned" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    for (unsigned long int i=0; i < num_its; i++)
      intersectQuadTuned(o, d, V00, V10, V11, V01, u, v, t);
    tuned_duration = std::chrono::high_resolution_clock::now() - start;

    std::cout<<t<<std::endl;
  }

  {
    Point o(0,0.1,0);
    Point d(2.,0.0,0);

    Point V00(1,-1,-1);
    Point V10(1,-1,1);
    Point V11(1,1,1);
    Point V01(1,1,-1);

    Real u, v, t;

    std::cout << "Starting Hand" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    for (unsigned long int i=0; i < num_its; i++)
      intersectQuadHandVectorized(o, d, V00, V10, V11, V01, u, v, t);
    hand_duration = std::chrono::high_resolution_clock::now() - start;

    std::cout<<t<<std::endl;
  }

  {
    Point o(0,0.1,0);
    Point d(2.,0,0);

    Point V00(1,-1,-1);
    Point V10(1,-1,1);
    Point V11(1,1,1);
    Point V01(1,1,-1);

    Real u, v, t;

    Real running_t = 0.0;

    std::cout << "Starting Vanilla Triangles" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    for (unsigned long int i=0; i < num_its; i++)
    {
//      o(0) += 1e-15;
      intersectQuadUsingTriangles(o, d, V00, V10, V11, V01, u, v, t);
//      running_t += t;
    }
    vanilla_triangles_duration = std::chrono::high_resolution_clock::now() - start;

    std::cout<<"t: "<<t<<std::endl;
  }

  {
    VectorizedPoint o(0,0.1,0);
    VectorizedPoint d(2.,0,0);

    VectorizedPoint V00(1,-1,-1);
    VectorizedPoint V10(1,-1,1);
    VectorizedPoint V11(1,1,1);
    VectorizedPoint V01(1,1,-1);

    Real u, v, t;

    Real running_t = 0.0;

    std::cout << "Starting Vectorized Triangles" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    for (unsigned long int i=0; i < num_its; i++)
    {
//      o(0) += 1e-15;
      intersectQuadUsingTriangles(o, d, V00, V10, V11, V01, u, v, t);
//      running_t += t;
    }
    vectorized_triangles_duration = std::chrono::high_resolution_clock::now() - start;

    std::cout<<"t: "<<t<<std::endl;
  }

  {
    Point o(0,-0.1,0);
    Point d(2.,0,0);

    Point V00(1,-1,-1);
    Point V10(1,-1,1);
    Point V11(1,1,1);
    Point V01(1,1,-1);

    Real u, v, t;

    Real running_t = 0.0;

    Hand h;

    std::cout << "Starting Hand Triangles" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    for (unsigned long int i=0; i < num_its; i++)
    {
//      o(0) += 1e-15;
      h.intersectQuadUsingTrianglesHand(o, d, V00, V10, V11, V01, u, v, t);
//      running_t += t;
    }
    hand_triangles_duration = std::chrono::high_resolution_clock::now() - start;

    std::cout<<"t: "<<t<<std::endl;
    std::cout<<"running t: "<<running_t<<std::endl;
  }

  std::cout << "vanilla: " << vanilla_duration.count() << std::endl;
  std::cout << "vectorized: " << vectorized_duration.count() << std::endl;
  std::cout << "tuned: " << tuned_duration.count() << std::endl;
  std::cout << "hand: " << hand_duration.count() << std::endl;
  std::cout << "vanilla triangles: " << vanilla_triangles_duration.count() << std::endl;
  std::cout << "vectorized triangles: " << vectorized_triangles_duration.count() << std::endl;
  std::cout << "hand triangles: " << hand_triangles_duration.count() << std::endl;
}
