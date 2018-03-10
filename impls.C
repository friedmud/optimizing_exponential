#include "impls.h"

#include <valarray>

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
  for (unsigned int i = 0; i < size; i++)
    out_vec[i] = std::exp(vec[i]);
}

void valarrayExp(std::vector<Real> & vec, std::vector<Real> & out_vec)
{
  std::valarray<Real> va(vec.data(), vec.size());

  std::valarray<Real> result = std::exp(va);

  auto va_data = &result[0];

  auto size = out_vec.size();
  auto data = out_vec.data();

#pragma clang loop vectorize_width(VecSize) interleave_count(VecSize)
  for (decltype(size) i = 0; i < size; i++)
    data[i] = va_data[i];
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

  auto out_array = out_vec.data();

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
  vsExp(vec.size(), vec.data(), out_vec.data());
}

#ifdef USE_IPP

void ippExp(std::vector<Real> & vec, std::vector<Real> & out_vec)
{
  ippsExp_32f_A24(vec.data(), out_vec.data(), vec.size());
}

#endif


#if defined(__INTEL_COMPILER)

#include <immintrin.h>

void svmlExp(std::vector<Real> & vec, std::vector<Real> & out_vec)
{
  auto total_size = vec.size();

  unsigned int num_chunks = total_size / VecSize;

  unsigned int remainder = total_size % VecSize;

  auto out_array = out_vec.data();

  for (unsigned int chunk = 0; chunk < num_chunks; chunk++)
  {
    a.load(&vec[chunk*VecSize]);
    b = expSVML(a);
    b.store(out_array + (chunk * VecSize));
  }

  // The remaineder
  if (remainder)
  {
    a.load_partial(remainder, &vec[num_chunks*VecSize]);
    b = expSVML(a);
    b.store_partial(remainder, (out_array + (num_chunks * VecSize)));
  }
}
#endif
