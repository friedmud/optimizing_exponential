#ifndef IMPLS_H
#define IMPLS_H

#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

#include "vecmath/vectorclass.h"
#include "vecmath/vectormath_exp.h"

#include "fmath/fmath.h"

#define Real float
#define VecSize 8
#define NumValues 64

void vectorizedExp(std::vector<Real> & vec, std::vector<Real> & out_vec);
void normalExp(std::vector<Real> & vec, std::vector<Real> & out_vec);
void fmathExp(std::vector<Real> & vec, std::vector<Real> & out_vec);
void mklExp(std::vector<Real> & vec, std::vector<Real> & out_vec);

#ifdef USE_IPP
void ippExp(std::vector<Real> & vec, std::vector<Real> & out_vec);
#endif

#endif
