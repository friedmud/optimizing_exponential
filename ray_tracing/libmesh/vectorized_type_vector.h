// The libMesh Finite Element Library.
// Copyright (C) 2002-2018 Benjamin S. Kirk, John W. Peterson, Roy H. Stogner

// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA



#ifndef LIBMESH_VECTORIZED_TYPE_VECTOR_H
#define LIBMESH_VECTORIZED_TYPE_VECTOR_H

// Local includes
#include "libmesh/libmesh_common.h"
#include "libmesh/compare_types.h"
#include "libmesh/tensor_tools.h"

// VectorClass Includes
#include "vectorclass.h"
#include "vector3d.h"

// C++ includes
#include <cstdlib> // *must* precede <cmath> for proper std:abs() on PGI, Sun Studio CC
#include <cmath>
#include <complex>

namespace libMesh
{
template <typename T>
class SetIndex;


// Forward declarations
template <typename T> class TypeTensor;
template <typename T> class VectorValue;
template <typename T> class TensorValue;

/**
 * This class defines a vector in \p LIBMESH_DIM dimensional space of
 * type T.  T may either be Real or Complex.  The default constructor
 * for this class is protected, suggesting that you should not
 * instantiate one of these directly.  Instead use one of the derived
 * types such as \p Point or \p Node.
 *
 * \author Benjamin S. Kirk
 * \date 2003
 */
template <typename T>
class VectorizedTypeVector
{
  template <typename T2>
  friend class VectorizedTypeVector;

  friend class TypeTensor<T>;

protected:

  /**
   * Empty constructor.  Gives the vector 0 in \p LIBMESH_DIM
   * dimensions.
   */
  VectorizedTypeVector  ();

  /**
   * Constructor-from-T.  By default sets higher dimensional
   * entries to 0.
   */
  VectorizedTypeVector (const T x,
              const T y=0,
              const T z=0);

  /**
   * Construct a TypeVector using the vector class object
   */
  VectorizedTypeVector (const Vec3d & in_vec);

  /**
   * Constructor-from-scalars.  By default sets higher dimensional
   * entries to 0.
   */
  template <typename Scalar1, typename Scalar2, typename Scalar3>
  VectorizedTypeVector (typename
              boostcopy::enable_if_c<ScalarTraits<Scalar1>::value,
              const Scalar1>::type x,
              typename
              boostcopy::enable_if_c<ScalarTraits<Scalar2>::value,
              const Scalar2>::type y=0,
              typename
              boostcopy::enable_if_c<ScalarTraits<Scalar3>::value,
              const Scalar3>::type z=0);

  /**
   * Constructor-from-scalar.  Sets higher dimensional entries to 0.
   * Necessary because for some reason the constructor-from-scalars
   * alone is insufficient to let the compiler figure out
   * VectorizedTypeVector<Complex> v = 0;
   */
  template <typename Scalar>
  VectorizedTypeVector (const Scalar x,
              typename
              boostcopy::enable_if_c<ScalarTraits<Scalar>::value,
              const Scalar>::type * sfinae = libmesh_nullptr);

public:

  /**
   * Helper typedef for C++98 generic programming
   */
  typedef T value_type;

  /**
   * Copy-constructor.
   */
  template <typename T2>
  VectorizedTypeVector (const VectorizedTypeVector<T2> & p);

  /**
   * Destructor.
   */
  ~VectorizedTypeVector ();

  /**
   * Assign to this vector without creating a temporary.
   */
  template <typename T2>
  void assign (const VectorizedTypeVector<T2> &);

  /**
   * Assignment-from-scalar operator.  Used only to zero out vectors.
   */
  template <typename Scalar>
  typename boostcopy::enable_if_c<
    ScalarTraits<Scalar>::value,
    VectorizedTypeVector &>::type
  operator = (const Scalar & libmesh_dbg_var(p))
  { libmesh_assert_equal_to (p, Scalar(0)); this->zero(); return *this; }

  /**
   * \returns A const reference to the \f$ i^{th} \f$ entry of the vector.
   */
  const T operator () (const unsigned int i) const;
  const T get (const unsigned int i) const;
  void getValues (Real vals[]) const;
  const T & slice (const unsigned int i) const { return (*this)(i); }

  /**
   * \returns A writable reference to the \f$ i^{th} \f$ entry of the vector.
   */
  SetIndex<T> operator () (const unsigned int i);
  T & slice (const unsigned int i) { return (*this)(i); }

  /**
   * Add two vectors.
   *
   * \returns A copy of the result, this vector is unchanged.
   */
  template <typename T2>
  VectorizedTypeVector<typename CompareTypes<T, T2>::supertype>
  operator + (const VectorizedTypeVector<T2> &) const;

  /**
   * Add to this vector.
   *
   * \returns A reference to *this.
   */
  template <typename T2>
  const VectorizedTypeVector<T> & operator += (const VectorizedTypeVector<T2> &);

  /**
   * Add to this vector without creating a temporary.
   */
  template <typename T2>
  void add (const VectorizedTypeVector<T2> &);

  /**
   * Add a scaled value to this vector without creating a temporary.
   */
  template <typename T2>
  void add_scaled (const VectorizedTypeVector<T2> &, const T);

  /**
   * Subtract from this vector.
   *
   * \returns A copy of the result, this vector is unchanged.
   */
  template <typename T2>
  VectorizedTypeVector<typename CompareTypes<T, T2>::supertype>
  operator - (const VectorizedTypeVector<T2> &) const;

  /**
   * Subtract from this vector.
   *
   * \returns A reference to *this.
   */
  template <typename T2>
  const VectorizedTypeVector<T> & operator -= (const VectorizedTypeVector<T2> &);

  /**
   * Subtract from this vector without creating a temporary.
   */
  template <typename T2>
  void subtract (const VectorizedTypeVector<T2> &);

  /**
   * Subtract a scaled value from this vector without creating a
   * temporary.
   */
  template <typename T2>
  void subtract_scaled (const VectorizedTypeVector<T2> &, const T);

  /**
   * \returns The negative of this vector in a separate copy.
   */
  VectorizedTypeVector<T> operator - () const;

  /**
   * Multiply this vector by a scalar value.
   *
   * \returns A copy of the result, this vector is unchanged.
   */
  template <typename Scalar>
  typename boostcopy::enable_if_c<
    ScalarTraits<Scalar>::value,
    VectorizedTypeVector<typename CompareTypes<T, Scalar>::supertype>>::type
  operator * (const Scalar) const;

  /**
   * Multiply this vector by a scalar value.
   *
   * \returns A reference to *this.
   */
  const VectorizedTypeVector<T> & operator *= (const T);

  /**
   * Divide each entry of this vector by scalar value.
   *
   * \returns A copy of the result, this vector is unchanged.
   */
  template <typename Scalar>
  typename boostcopy::enable_if_c<
    ScalarTraits<Scalar>::value,
    VectorizedTypeVector<typename CompareTypes<T, Scalar>::supertype>>::type
  operator / (const Scalar) const;

  /**
   * Divide each entry of this vector by scalar value.
   *
   * \returns A reference to *this.
   */
  const VectorizedTypeVector<T> & operator /= (const T);

  /**
   * \returns The dot-product of this vector with another vector.
   *
   * \note The complex conjugate is *not* taken in the complex-valued case.
   * \note The vectors may contain different numeric types.
   */
  template <typename T2>
  typename CompareTypes<T, T2>::supertype
  operator * (const VectorizedTypeVector<T2> &) const;

  /**
   * \returns The result of VectorizedTypeVector::operator*().
   */
  template <typename T2>
  typename CompareTypes<T, T2>::supertype
  contract (const VectorizedTypeVector<T2> &) const;

  /**
   * \returns The cross product of this vector with \p v.
   */
  template <typename T2>
  VectorizedTypeVector<typename CompareTypes<T, T2>::supertype>
  cross(const VectorizedTypeVector<T2> & v) const;

  /**
   * \returns A unit vector in the direction of *this.
   */
  VectorizedTypeVector<T> unit() const;

  /**
   * \returns The magnitude of the vector, i.e. the square-root of the
   * sum of the elements squared.
   *
   * \deprecated Use the norm() function instead.
   */
#ifdef LIBMESH_ENABLE_DEPRECATED
  Real size() const;
#endif

  /**
   * \returns The magnitude of the vector, i.e. the square-root of the
   * sum of the elements squared.
   */
  Real norm() const;

  /**
   * \returns The magnitude of the vector squared, i.e. the sum of the
   * element magnitudes squared.
   *
   * \deprecated Use the norm_sq() function instead.
   */
#ifdef LIBMESH_ENABLE_DEPRECATED
  Real size_sq() const;
#endif

  /**
   * \returns The magnitude of the vector squared, i.e. the sum of the
   * element magnitudes squared.
   */
  Real norm_sq() const;

  /**
   * Set all entries of the vector to 0.
   */
  void zero();

  /**
   * \returns \p true if two vectors are equal to within a relative
   * tolerance of \p tol.
   */
  bool relative_fuzzy_equals(const VectorizedTypeVector<T> & rhs,
                             Real tol = TOLERANCE) const;

  /**
   * \returns \p true if two vectors are equal to within an absolute
   * tolerance of \p tol.
   */
  bool absolute_fuzzy_equals(const VectorizedTypeVector<T> & rhs,
                             Real tol = TOLERANCE) const;

  /**
   * \returns \p true if this(i)==rhs(i) for each component of the
   * vector.
   *
   * \note For floating point types T, the function \p absolute_fuzzy_equals()
   * may be a more appropriate choice.
   */
  bool operator == (const VectorizedTypeVector<T> & rhs) const;

  /**
   * \returns !(*this == rhs)
   */
  bool operator != (const VectorizedTypeVector<T> & rhs) const;

  /**
   * \returns \p true if this vector is "less" than \p rhs.
   *
   * Useful for sorting.  Also used for choosing some arbitrary basis
   * function orientations.
   */
  bool operator < (const VectorizedTypeVector<T> & rhs) const;

  /**
   * \returns \p true if this vector is <= to \p rhs.
   *
   * Useful for sorting.  Also used for choosing some arbitrary
   * constraint equation directions.
   */
  bool operator <= (const VectorizedTypeVector<T> & rhs) const;

  /**
   * \returns \p true if this vector is "greater" than \p rhs.
   *
   * Useful for sorting.  Also used for choosing some arbitrary basis
   * function orientations.
   */
  bool operator > (const VectorizedTypeVector<T> & rhs) const;

  /**
   * \returns \p true if this vector is >= \p rhs.
   *
   * Useful for sorting.  Also used for choosing some arbitrary
   * constraint equation directions.
   */
  bool operator >= (const VectorizedTypeVector<T> & rhs) const;

  /**
   * Formatted print, by default to \p libMesh::out.
   */
  void print(std::ostream & os = std::cout) const;

  /**
   * Formatted print as above but supports the syntax:
   *
   * \code
   * Point p(1,2,3);
   * std::cout << p << std::endl;
   * \endcode
   */
  friend std::ostream & operator << (std::ostream & os, const VectorizedTypeVector<T> & t)
  {
    t.print(os);
    return os;
  }

  /**
   * Unformatted print to the stream \p out.  Simply prints the
   * elements of the vector separated by spaces.  Also prints a
   * newline by default, however, this behavior can be controlled with
   * the \p newline parameter.
   */
  void write_unformatted (std::ostream & out, const bool newline = true) const;


  void cacheValues();

protected:

  /**
   * The coordinates of the \p VectorizedTypeVector.
   */
  Vec3d _coords;

  friend SetIndex<T>;
};





//------------------------------------------------------
// Inline functions

template <typename T>
inline
VectorizedTypeVector<T>::VectorizedTypeVector ()
{
  _coords[0] = 0;

#if LIBMESH_DIM > 1
  _coords[1] = 0;
#endif

#if LIBMESH_DIM > 2
  _coords[2] = 0;
#endif
}



template <typename T>
inline
VectorizedTypeVector<T>::VectorizedTypeVector (const T x,
                           const T y,
                           const T z)
{
  _coords.insert(0, x);

#if LIBMESH_DIM > 1
  _coords.insert(1, y);
#else
  libmesh_assert_equal_to (y, 0);
#endif

#if LIBMESH_DIM > 2
  _coords.insert(2, z);
#else
  libmesh_assert_equal_to (z, 0);
#endif
}

template <typename T>
inline
VectorizedTypeVector<T>::VectorizedTypeVector(const Vec3d & in_vec):
    _coords(in_vec)
{
}

template <typename T>
template <typename Scalar1, typename Scalar2, typename Scalar3>
inline
VectorizedTypeVector<T>::VectorizedTypeVector (typename
                           boostcopy::enable_if_c<ScalarTraits<Scalar1>::value,
                           const Scalar1>::type x,
                           typename
                           boostcopy::enable_if_c<ScalarTraits<Scalar2>::value,
                           const Scalar2>::type y,
                           typename
                           boostcopy::enable_if_c<ScalarTraits<Scalar3>::value,
                           const Scalar3>::type z)
{
  _coords.insert(0, x);

#if LIBMESH_DIM > 1
  _coords.insert(1, y);
#else
  libmesh_assert_equal_to (y, 0);
#endif

#if LIBMESH_DIM > 2
  _coords.insert(2, z);
#else
  libmesh_assert_equal_to (z, 0);
#endif
}



template <typename T>
template <typename Scalar>
inline
VectorizedTypeVector<T>::VectorizedTypeVector (const Scalar x,
                           typename
                           boostcopy::enable_if_c<ScalarTraits<Scalar>::value,
                           const Scalar>::type * /*sfinae*/)
{
  _coords.insert(0, x);
}



template <typename T>
template <typename T2>
inline
VectorizedTypeVector<T>::VectorizedTypeVector (const VectorizedTypeVector<T2> & p)
{
  _coords = p._coords;
}



template <typename T>
inline
VectorizedTypeVector<T>::~VectorizedTypeVector ()
{
}



template <typename T>
template <typename T2>
inline
void VectorizedTypeVector<T>::assign (const VectorizedTypeVector<T2> & p)
{
  _coords = p._coords;
}



template <typename T>
inline
const T VectorizedTypeVector<T>::operator () (const unsigned int i) const
{
  libmesh_assert_less (i, LIBMESH_DIM);

  _coords.extract(i);
}

template <typename T>
inline
const T VectorizedTypeVector<T>::get (const unsigned int i) const
{
  libmesh_assert_less (i, LIBMESH_DIM);

  _coords.extract(i);
}

template <typename T>
inline
void VectorizedTypeVector<T>::getValues (Real vals[]) const
{
  _coords.store(vals);
}



template <typename T>
class SetIndex
{
public:
  SetIndex(VectorizedTypeVector<T> & receiver, const unsigned int i) :
      _receiver(receiver),
      _i(i)
  {}

  VectorizedTypeVector<T> & operator=(Real value)
  {
    _receiver._coords.insert(_i, value);

    return _receiver;
  }

  operator T()
  {
    return _receiver._coords.extract(_i);
  }

private:
  VectorizedTypeVector<T> & _receiver;
  unsigned int _i;
};

template <typename T>
inline
SetIndex<T> VectorizedTypeVector<T>::operator () (const unsigned int i)
{
  libmesh_assert_less (i, LIBMESH_DIM);

  return SetIndex<T>(*this, i);
}



template <typename T>
template <typename T2>
inline
VectorizedTypeVector<typename CompareTypes<T, T2>::supertype>
VectorizedTypeVector<T>::operator + (const VectorizedTypeVector<T2> & p) const
{
  typedef typename CompareTypes<T, T2>::supertype TS;

  return VectorizedTypeVector<TS> (_coords + p._coords);
}



template <typename T>
template <typename T2>
inline
const VectorizedTypeVector<T> & VectorizedTypeVector<T>::operator += (const VectorizedTypeVector<T2> & p)
{
  _coords += p._coords;

  return *this;
}



template <typename T>
template <typename T2>
inline
void VectorizedTypeVector<T>::add (const VectorizedTypeVector<T2> & p)
{
  _coords += p._coords;
}



template <typename T>
template <typename T2>
inline
void VectorizedTypeVector<T>::add_scaled (const VectorizedTypeVector<T2> & p, const T factor)
{
  _coords = mul_add(factor, p._coords, _coords);
}



template <typename T>
template <typename T2>
inline
VectorizedTypeVector<typename CompareTypes<T, T2>::supertype>
VectorizedTypeVector<T>::operator - (const VectorizedTypeVector<T2> & p) const
{
  typedef typename CompareTypes<T, T2>::supertype TS;

  return VectorizedTypeVector<TS> (_coords - p._coords);
}



template <typename T>
template <typename T2>
inline
const VectorizedTypeVector<T> & VectorizedTypeVector<T>::operator -= (const VectorizedTypeVector<T2> & p)
{
  _coords -= p._coords;

  return *this;
}



template <typename T>
template <typename T2>
inline
void VectorizedTypeVector<T>::subtract (const VectorizedTypeVector<T2> & p)
{
  _coords -= p._coords;
}



template <typename T>
template <typename T2>
inline
void VectorizedTypeVector<T>::subtract_scaled (const VectorizedTypeVector<T2> & p, const T factor)
{
  _coords = nmul_add(factor, p._coords, _coords);
}



template <typename T>
inline
VectorizedTypeVector<T> VectorizedTypeVector<T>::operator - () const
{
  return VectorizedTypeVector<T> (-_coords);
}



template <typename T>
template <typename Scalar>
inline
typename boostcopy::enable_if_c<
  ScalarTraits<Scalar>::value,
  VectorizedTypeVector<typename CompareTypes<T, Scalar>::supertype>>::type
VectorizedTypeVector<T>::operator * (const Scalar factor) const
{
  return _coords * factor;
}


/* WTF
template <typename T, typename Scalar>
inline
typename boostcopy::enable_if_c<
  ScalarTraits<Scalar>::value,
  VectorizedTypeVector<typename CompareTypes<T, Scalar>::supertype>>::type
operator * (const Scalar factor,
            const VectorizedTypeVector<T> & v)
{
  return v * factor;
}
*/



template <typename T>
inline
const VectorizedTypeVector<T> & VectorizedTypeVector<T>::operator *= (const T factor)
{
  _coords *= factor;

  return *this;
}



template <typename T>
template <typename Scalar>
inline
typename boostcopy::enable_if_c<
  ScalarTraits<Scalar>::value,
  VectorizedTypeVector<typename CompareTypes<T, Scalar>::supertype>>::type
VectorizedTypeVector<T>::operator / (const Scalar factor) const
{
  libmesh_assert_not_equal_to (factor, static_cast<T>(0.));

  typedef typename CompareTypes<T, Scalar>::supertype TS;

  return VectorizedTypeVector<TS>(_coords/factor);
}




template <typename T>
inline
const VectorizedTypeVector<T> &
VectorizedTypeVector<T>::operator /= (const T factor)
{
  libmesh_assert_not_equal_to (factor, static_cast<T>(0.));

  _coords /= factor;

  return *this;
}




template <typename T>
template <typename T2>
inline
typename CompareTypes<T, T2>::supertype
VectorizedTypeVector<T>::operator * (const VectorizedTypeVector<T2> & p) const
{
  return dot_product(_coords, p._coords);
}

template <typename T>
template <typename T2>
inline
typename CompareTypes<T, T2>::supertype
VectorizedTypeVector<T>::contract(const VectorizedTypeVector<T2> & p) const
{
  return dot_product(_coords, p._coords);
}



template <typename T>
template <typename T2>
VectorizedTypeVector<typename CompareTypes<T, T2>::supertype>
VectorizedTypeVector<T>::cross(const VectorizedTypeVector<T2> & p) const
{
  typedef typename CompareTypes<T, T2>::supertype TS;
  libmesh_assert_equal_to (LIBMESH_DIM, 3);

  return VectorizedTypeVector<TS>(cross_product(_coords, p._coords));
}



#ifdef LIBMESH_ENABLE_DEPRECATED
template <typename T>
inline
Real VectorizedTypeVector<T>::size() const
{
  libmesh_deprecated();

  return vector_length(_coords);
}
#endif



template <typename T>
inline
Real VectorizedTypeVector<T>::norm() const
{
  return vector_length(_coords);
}



template <typename T>
inline
void VectorizedTypeVector<T>::zero()
{
  _coords = 0.;
}



#ifdef LIBMESH_ENABLE_DEPRECATED
template <typename T>
inline
Real VectorizedTypeVector<T>::size_sq() const
{
  return horizontal_add(square(_coords));
}
#endif



template <typename T>
inline
Real VectorizedTypeVector<T>::norm_sq() const
{
  return horizontal_add(square(_coords));
}



template <typename T>
inline
bool VectorizedTypeVector<T>::absolute_fuzzy_equals(const VectorizedTypeVector<T> & rhs, Real tol) const
{
  return abs(_coords - rhs._coords) <= tol;
}



template <typename T>
inline
bool VectorizedTypeVector<T>::relative_fuzzy_equals(const VectorizedTypeVector<T> & rhs, Real tol) const
{
  return this->absolute_fuzzy_equals(rhs, tol * (abs(_coords) + abs(rhs._coords)));
}



template <typename T>
inline
bool VectorizedTypeVector<T>::operator == (const VectorizedTypeVector<T> & rhs) const
{
  return _coords == rhs._coords;
}



template <typename T>
inline
bool VectorizedTypeVector<T>::operator != (const VectorizedTypeVector<T> & rhs) const
{
  return _coords != rhs._coords;
}


//------------------------------------------------------
// Non-member functions on VectorizedTypeVectors

// Compute a * (b.cross(c)) without creating a temporary
// for the cross product.  Equivalent to the determinant
// of the 3x3 tensor:
// [a0, a1, a2]
// [b0, b1, b2]
// [c0, c1, c2]
template <typename T>
inline
T triple_product(const VectorizedTypeVector<T> & a,
                 const VectorizedTypeVector<T> & b,
                 const VectorizedTypeVector<T> & c)
{
  // We only support cross products when LIBMESH_DIM==3, same goes for this.
  libmesh_assert_equal_to (LIBMESH_DIM, 3);

  return dot_product(a, cross_product(b, c));
}



/**
 * Compute |b x c|^2 without creating the extra temporary produced by
 * calling b.cross(c).norm_sq().
 */
template <typename T>
inline
T cross_norm_sq(const VectorizedTypeVector<T> & b,
                const VectorizedTypeVector<T> & c)
{
  // We only support cross products when LIBMESH_DIM==3, same goes for this.
  libmesh_assert_equal_to (LIBMESH_DIM, 3);

  return horizontal_add(square(cross(b, c)));
}



/**
 * Calls cross_norm_sq() and takes the square root of the result.
 */
template <typename T>
inline
T cross_norm(const VectorizedTypeVector<T> & b,
             const VectorizedTypeVector<T> & c)
{
  // We only support cross products when LIBMESH_DIM==3, same goes for this.
  libmesh_assert_equal_to (LIBMESH_DIM, 3);

  return vector_length(square(cross(b,c)));
}

} // namespace libMesh

#endif // LIBMESH_TYPE_VECTOR_H
