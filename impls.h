#ifndef IMPLS_H
#define IMPLS_H

#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

#include "vectorclass.h"
#include "vectormath_exp.h"
#include "fmath.h"

#define Real double
#define VecSize 4
#define NumValues 64

void vectorizedExp(std::vector<Real> & vec, std::vector<Real> & out_vec);
void normalExp(std::vector<Real> & vec, std::vector<Real> & out_vec);
void fmathExp(std::vector<Real> & vec, std::vector<Real> & out_vec);
void mklExp(std::vector<Real> & vec, std::vector<Real> & out_vec);

#endif
