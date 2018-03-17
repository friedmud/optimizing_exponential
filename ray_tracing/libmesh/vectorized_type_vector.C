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




// C++ includes
#include <iostream>
#include <iomanip> // for std::setw, std::setiosflags

// Local includes
#include "libmesh/vectorized_type_vector.h"

namespace libMesh
{




// ------------------------------------------------------------
// VectorizedTypeVector<T> class member functions
template <typename T>
VectorizedTypeVector<T> VectorizedTypeVector<T>::unit() const
{

  const Real length = norm();

  libmesh_assert_not_equal_to (length, static_cast<Real>(0.));

#if LIBMESH_DIM == 1
  return VectorizedTypeVector<T>(_coords[0]/length);
#endif

#if LIBMESH_DIM == 2
  return VectorizedTypeVector<T>(_coords[0]/length,
                       _coords[1]/length);
#endif

#if LIBMESH_DIM == 3
  return VectorizedTypeVector<T>(_coords[0]/length,
                       _coords[1]/length,
                       _coords[2]/length);
#endif

}



template <typename T>
void VectorizedTypeVector<T>::print(std::ostream & os) const
{
#if LIBMESH_DIM == 1

  os << "x=" << (*this)(0);

#endif
#if LIBMESH_DIM == 2

  os << "(x,y)=("
     << std::setw(8) << (*this)(0) << ", "
     << std::setw(8) << (*this)(1) << ")";

#endif
#if LIBMESH_DIM == 3

  os <<  "(x,y,z)=("
     << std::setw(8) << (*this)(0) << ", "
     << std::setw(8) << (*this)(1) << ", "
     << std::setw(8) << (*this)(2) << ")";
#endif
}





template <typename T>
void VectorizedTypeVector<T>::write_unformatted (std::ostream & os,
                                       const bool newline) const
{
  libmesh_assert (os);

  os << std::setiosflags(std::ios::showpoint)
     << (*this)(0) << " "
     << (*this)(1) << " "
     << (*this)(2) << " ";

  if (newline)
    os << '\n';
}



template <typename T>
bool VectorizedTypeVector<T>::operator < (const VectorizedTypeVector<T> & rhs) const
{
  for (unsigned int i=0; i<LIBMESH_DIM; i++)
    {
      if ((*this)(i) < rhs(i))
        return true;
      if ((*this)(i) > rhs(i))
        return false;
    }
  return false;
}


template <typename T>
bool VectorizedTypeVector<T>::operator <= (const VectorizedTypeVector<T> & rhs) const
{
  for (unsigned int i=0; i<LIBMESH_DIM; i++)
    {
      if ((*this)(i) < rhs(i))
        return true;
      if ((*this)(i) > rhs(i))
        return false;
    }
  return true;
}



template <typename T>
bool VectorizedTypeVector<T>::operator > (const VectorizedTypeVector<T> & rhs) const
{
  for (unsigned int i=0; i<LIBMESH_DIM; i++)
    {
      if ((*this)(i) > rhs(i))
        return true;
      if ((*this)(i) < rhs(i))
        return false;
    }
  return false;
}


template <typename T>
bool VectorizedTypeVector<T>::operator >= (const VectorizedTypeVector<T> & rhs) const
{
  for (unsigned int i=0; i<LIBMESH_DIM; i++)
    {
      if ((*this)(i) > rhs(i))
        return true;
      if ((*this)(i) < rhs(i))
        return false;
    }
  return true;
}


#ifdef LIBMESH_USE_COMPLEX_NUMBERS
template <>
bool VectorizedTypeVector<Complex>::operator < (const VectorizedTypeVector<Complex> & rhs) const
{
  for (unsigned int i=0; i<LIBMESH_DIM; i++)
    {
      if ((*this)(i).real() < rhs(i).real())
        return true;
      if ((*this)(i).real() > rhs(i).real())
        return false;
      if ((*this)(i).imag() < rhs(i).imag())
        return true;
      if ((*this)(i).imag() > rhs(i).imag())
        return false;
    }
  return false;
}



template <>
bool VectorizedTypeVector<Complex>::operator <= (const VectorizedTypeVector<Complex> & rhs) const
{
  for (unsigned int i=0; i<LIBMESH_DIM; i++)
    {
      if ((*this)(i).real() < rhs(i).real())
        return true;
      if ((*this)(i).real() > rhs(i).real())
        return false;
      if ((*this)(i).imag() < rhs(i).imag())
        return true;
      if ((*this)(i).imag() > rhs(i).imag())
        return false;
    }
  return true;
}



template <>
bool VectorizedTypeVector<Complex>::operator > (const VectorizedTypeVector<Complex> & rhs) const
{
  for (unsigned int i=0; i<LIBMESH_DIM; i++)
    {
      if ((*this)(i).real() > rhs(i).real())
        return true;
      if ((*this)(i).real() < rhs(i).real())
        return false;
      if ((*this)(i).imag() > rhs(i).imag())
        return true;
      if ((*this)(i).imag() < rhs(i).imag())
        return false;
    }
  return false;
}



template <>
bool VectorizedTypeVector<Complex>::operator >= (const VectorizedTypeVector<Complex> & rhs) const
{
  for (unsigned int i=0; i<LIBMESH_DIM; i++)
    {
      if ((*this)(i).real() > rhs(i).real())
        return true;
      if ((*this)(i).real() < rhs(i).real())
        return false;
      if ((*this)(i).imag() > rhs(i).imag())
        return true;
      if ((*this)(i).imag() < rhs(i).imag())
        return false;
    }
  return true;
}

#endif



// ------------------------------------------------------------
// Explicit instantiations
template class VectorizedTypeVector<Real>;

#ifdef LIBMESH_USE_COMPLEX_NUMBERS
template class VectorizedTypeVector<Complex>;
#endif

} // namespace libMesh
