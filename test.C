#include "impls.h"

int main()
{
  std::vector<Real> vals(NumValues);

  for (unsigned int i = 0; i < NumValues; i++)
    vals[i] = i * 1e-2;

  std::cout<<"Vector Size: "<<vals.size()<<std::endl;

  std::vector<Real> outvals(vals.size());

  long unsigned int its = 1e8;

  std::cout<<"Starting Normal"<<std::endl;
  auto start = std::chrono::high_resolution_clock::now();
  for (long unsigned int i = 0; i < its; i++)
    normalExp(vals, outvals);
  std::chrono::duration<Real> normal_duration = std::chrono::high_resolution_clock::now() - start;

  std::cout<<"Starting FMath"<<std::endl;
  start = std::chrono::high_resolution_clock::now();
  for (long unsigned int i = 0; i < its; i++)
    fmathExp(vals, outvals);
  std::chrono::duration<Real> fmath_duration = std::chrono::high_resolution_clock::now() - start;

  std::cout<<"Starting Vector Lib"<<std::endl;
  start = std::chrono::high_resolution_clock::now();
  for (long unsigned int i = 0; i < its; i++)
    vectorizedExp(vals, outvals);
  std::chrono::duration<Real> vectorized_duration = std::chrono::high_resolution_clock::now() - start;

  std::cout<<"Starting MKL"<<std::endl;
  start = std::chrono::high_resolution_clock::now();
  for (long unsigned int i = 0; i < its; i++)
    mklExp(vals, outvals);
  std::chrono::duration<Real> mkl_duration = std::chrono::high_resolution_clock::now() - start;

  std::cout<<"Starting IPP"<<std::endl;
  start = std::chrono::high_resolution_clock::now();
  for (long unsigned int i = 0; i < its; i++)
    ippExp(vals, outvals);
  std::chrono::duration<Real> ipp_duration = std::chrono::high_resolution_clock::now() - start;

  std::cout<<"normal: "<<normal_duration.count()<<std::endl;
  std::cout<<"fmath: "<<fmath_duration.count()<<std::endl;
  std::cout<<"vectorized: "<<vectorized_duration.count()<<std::endl;
  std::cout<<"mkl: "<<mkl_duration.count()<<std::endl;
  std::cout<<"ipp: "<<ipp_duration.count()<<std::endl;
}
