#ifndef IMPLS_H
#define IMPLS_H

#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

#define MAX_VECTOR_SIZE 256

#include "vecmath/vectorclass.h"
#include "vecmath/vectormath_exp.h"

#include "fmath/fmath.h"

#define Real float
#define VecSize 8
#define NumValues 7

void normalExp(std::vector<Real> & vec, std::vector<Real> & out_vec);
void valarrayExp(std::vector<Real> & vec, std::vector<Real> & out_vec);
void fmathExp(std::vector<Real> & vec, std::vector<Real> & out_vec);
void vectorizedExp(std::vector<Real> & vec, std::vector<Real> & out_vec);
void mklExp(std::vector<Real> & vec, std::vector<Real> & out_vec);

#ifdef USE_IPP
void ippExp(std::vector<Real> & vec, std::vector<Real> & out_vec);
#endif

#if defined(__INTEL_COMPILER)
void svmlExp(std::vector<Real> & vec, std::vector<Real> & out_vec);
#endif

#endif
