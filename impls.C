#include "impls.h"

#include "mkl.h"

#ifdef USE_IPP
#include "ippcore.h"
#include "ippvm.h"
#endif

Vec8f a;
Vec8f b;

void normalExp(std::vector<Real> & vec, std::vector<Real> & out_vec)
{
  auto size = vec.size();

#pragma clang loop vectorize_width(VecSize) interleave_count(VecSize)
#pragma ivdep
#pragma vector aligned
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

void vectorizedExp(std::vector<Real> & vec, std::vector<Real> & out_vec)
{
  auto total_size = vec.size();

  unsigned int num_chunks = total_size / VecSize;

  unsigned int remainder = total_size % VecSize;

  auto out_array = &out_vec[0];

  for (unsigned int chunk = 0; chunk < num_chunks; chunk++)
  {
    a.load(&vec[chunk*VecSize]);
    b = exp(a);
    b.store(out_array + (chunk * VecSize));
  }

  // The remaineder
  if (remainder)
  {
    a.load_partial(remainder, &vec[num_chunks*VecSize]);
    b = exp(a);
    b.store_partial(remainder, (out_array + (num_chunks * VecSize)));
  }
}

void mklExp(std::vector<Real> & vec, std::vector<Real> & out_vec)
{
  vsExp(vec.size(), &vec[0], &out_vec[0]);
}

#ifdef USE_IPP

void ippExp(std::vector<Real> & vec, std::vector<Real> & out_vec)
{
//  ippsExp_32f_A11(&vec[0], &out_vec[0], vec.size());
  ippsExp_32f_A24(&vec[0], &out_vec[0], vec.size());
}

#endif
