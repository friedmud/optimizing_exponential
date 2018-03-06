#include "impls.h"

int main()
{
  std::vector<Real> vals(NumValues);

  for (unsigned int i = 0; i < NumValues; i++)
    vals[i] = i;

  std::cout<<"Vector Size: "<<vals.size()<<std::endl;

  std::vector<Real> outvals(vals.size());

  long unsigned int its = 1e8;

  auto start = std::chrono::high_resolution_clock::now();
  for (long unsigned int i = 0; i < its; i++)
    fmathExp(vals, outvals);
  std::chrono::duration<Real> fmath_duration = std::chrono::high_resolution_clock::now() - start;

  start = std::chrono::high_resolution_clock::now();
  for (long unsigned int i = 0; i < its; i++)
    normalExp(vals, outvals);
  std::chrono::duration<Real> normal_duration = std::chrono::high_resolution_clock::now() - start;

  for (auto & val : outvals)
    std::cout<<val<<" ";

  std::cout<<std::endl;

  start = std::chrono::high_resolution_clock::now();
  for (long unsigned int i = 0; i < its; i++)
    vectorizedExp(vals, outvals);
  std::chrono::duration<Real> vectorized_duration = std::chrono::high_resolution_clock::now() - start;

  std::cout<<"normal: "<<normal_duration.count()<<std::endl;
  std::cout<<"fmath: "<<fmath_duration.count()<<std::endl;
  std::cout<<"vectorized: "<<vectorized_duration.count()<<std::endl;
}
