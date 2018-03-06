#include "impls.h"

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
