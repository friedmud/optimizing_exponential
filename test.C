#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

#include "vectorclass.h"
#include "vectormath_exp.h"
#include "fmath.h"

#define Real double
#define VecSize 4
#define NumValues 16

Vec4d a;
Vec4d b;

void vectorizedExp(std::vector<Real> & vec, std::vector<Real> & out_vec)
{
  auto total_size = vec.size();

  unsigned int num_chunks = total_size / VecSize;

  unsigned int remainder = total_size % VecSize;

  for (unsigned int chunk = 0; chunk < num_chunks; chunk++)
  {
    a.load(&vec[chunk*VecSize]);
    b = exp(a);

    auto current_out_vec = &out_vec[chunk * VecSize];

#pragma clang loop vectorize_width(VecSize) interleave_count(VecSize)
    for (unsigned int i = 0; i < VecSize; i++)
      current_out_vec[i] = b[i];
  }

  // The remaineder
  if (remainder)
  {
    a.load_partial(remainder, &vec[num_chunks*VecSize]);
    b = exp(a);

    auto current_out_vec = &out_vec[num_chunks*VecSize];

#pragma clang loop vectorize_width(VecSize) interleave_count(VecSize)
    for (unsigned int i = 0; i < remainder; i++)
      current_out_vec[i] = b[i];
  }
}

void normalExp(std::vector<Real> & vec, std::vector<Real> & out_vec)
{
  auto size = vec.size();

#pragma clang loop vectorize_width(VecSize) interleave_count(VecSize)
  for (unsigned int i = 0; i < size; i++)
    out_vec[i] = std::exp(vec[i]);
}


void fmathExp(std::vector<Real> & vec, std::vector<Real> & out_vec)
{
  auto size = vec.size();

#pragma clang loop vectorize_width(VecSize) interleave_count(VecSize)
  for (unsigned int i = 0; i < vec.size(); i++)
    out_vec[i] = fmath::exp(vec[i]);
}

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
