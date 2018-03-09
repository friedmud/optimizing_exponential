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

  std::chrono::duration<Real> valarray_duration;

  if (its * NumValues < 1e8)
  {
    std::cout<<"Starting Valarray"<<std::endl;
    start = std::chrono::high_resolution_clock::now();
    for (long unsigned int i = 0; i < its; i++)
      valarrayExp(vals, outvals);
    valarray_duration = std::chrono::high_resolution_clock::now() - start;
  }
  else
    std::cout<<"Skipping Valarray because it's slow!"<<std::endl;

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

#ifdef USE_IPP
  std::cout<<"Starting IPP"<<std::endl;
  start = std::chrono::high_resolution_clock::now();
  for (long unsigned int i = 0; i < its; i++)
    ippExp(vals, outvals);
  std::chrono::duration<Real> ipp_duration = std::chrono::high_resolution_clock::now() - start;
#endif

  std::cout<<"normal: "<<normal_duration.count()<<std::endl;

  if (its * NumValues < 1e8)
    std::cout<<"valarray: "<<valarray_duration.count()<<std::endl;

  std::cout<<"fmath: "<<fmath_duration.count()<<std::endl;
  std::cout<<"vectorized: "<<vectorized_duration.count()<<std::endl;
  std::cout<<"mkl: "<<mkl_duration.count()<<std::endl;

#ifdef USE_IPP
  std::cout<<"ipp: "<<ipp_duration.count()<<std::endl;
#endif
}
