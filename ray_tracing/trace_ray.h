#include "libmesh/point.h"

// VectorClass Includes
#include "vectorclass.h"
#include "vector3d.h"

#include <vector>

using namespace libMesh;

extern unsigned int count;

// https://people.cs.kuleuven.be/~ares.lagae/publications/LD05ERQIT/LD05ERQIT_code.cpp
template<typename PointType>
bool
intersectQuad(const PointType & O,
              const PointType & D,
              const PointType & V00,
              const PointType & V10,
              const PointType & V11,
              const PointType & V01,
              Real & u,
              Real & v,
              Real & t)
{
  count++;

  // Reject rays using the barycentric coordinates of // the intersection point with respect to T.
  auto E01 = V10;
  E01 -= V00;

  auto E03 = V01;
  E03 -= V00;

  auto P = D.cross(E03);

  auto det = E01 * P;

  //  libMesh::err<<"Count: "<<count<<std::endl;

  if (std::abs(det) < TOLERANCE)
  {
#ifdef USE_DEBUG_RAY
    if (ray->id() == debug_ray_id)
      libMesh::err << "1 Rejecting because " << std::abs(det) << " < " << TOLERANCE << std::endl;
#endif
    return false;
  }

  auto T = O;
  T -= V00;

  auto alpha = (T * P) / det;

  if (alpha < -1e-12)
  {
#ifdef USE_DEBUG_RAY
    if (ray->id() == debug_ray_id)
      libMesh::err << "2 Rejecting because " << alpha << " < " << 0 << std::endl;
#endif
    return false;
  }

  /*
  if (alpha > 1)
  {

      libMesh::err<<"3 Rejecting because "<<alpha<<" > "<<1<<std::endl;

    return false;
  }
  */

  auto Q = T.cross(E01);

  // Compute the ray parameter of the intersection // point.
  t = (E03 * Q) / det;

  if (t < -1e-12)
  {
#ifdef USE_DEBUG_RAY
    if (ray->id() == debug_ray_id)
      libMesh::err << "3 Rejecting because " << t << " < " << 0 << std::endl;
#endif

    return false;
  }

  auto beta = (D * Q) / det;

  if (beta < -1e-12)
  {
#ifdef USE_DEBUG_RAY
    if (ray->id() == debug_ray_id)
      libMesh::err << "4 Rejecting because " << beta << " < " << 0 << std::endl;
#endif

    return false;
  }

  /*
    if (beta > 1)
    {

        libMesh::err<<"5 Rejecting because "<<beta<<" > "<<1+TOLERANCE<<std::endl;

      return false;
    }
  */

  // Reject rays using the barycentric coordinates of // the intersection point with respect to
  // T′.
  if ((alpha + beta) > 1)
  {
    auto E23 = V01;
    E23 -= V11;

    auto E21 = V10;
    E21 -= V11;

    auto P_prime = D.cross(E21);

    auto det_prime = E23 * P_prime;

    if (std::abs(det_prime) < TOLERANCE)
    {
#ifdef USE_DEBUG_RAY
      if (ray->id() == debug_ray_id)
        libMesh::err << "5 Rejecting because " << std::abs(det_prime) << " < " << TOLERANCE
                     << std::endl;
#endif

      return false;
    }

    auto T_prime = O;
    T_prime -= V11;

    auto alpha_prime = (T_prime * P_prime) / det_prime;

    if (alpha_prime < -1e-12)
    {
#ifdef USE_DEBUG_RAY
      if (ray->id() == debug_ray_id)
        libMesh::err << "6 Rejecting because " << alpha_prime << " < " << 0 << std::endl;
#endif

      return false;
    }

    auto Q_prime = T_prime.cross(E23);

    auto beta_prime = (D * Q_prime) / det_prime;

    if (beta_prime < -1e-12)
    {
#ifdef USE_DEBUG_RAY
      if (ray->id() == debug_ray_id)
        libMesh::err << "7 Rejecting because " << beta_prime << " < " << 0 << std::endl;
#endif

      return false;
    }
  }

  // Compute the barycentric coordinates of V11. E02 = V11 - V00
  auto E02 = V11;
  E02 -= V00;

  auto N = E01.cross(E03);

  Real alpha11;
  Real beta11;

  if ((std::abs(N(0)) >= std::abs(N(1))) && (std::abs(N(0)) >= std::abs(N(2))))
  {
    alpha11 = (E02(1) * E03(2) - E02(2) * E03(1)) / N(0);
    beta11 = (E01(1) * E02(2) - E01(2) * E02(1)) / N(0);
  }
  else if ((std::abs(N(1)) >= std::abs(N(0))) && (std::abs(N(1)) >= std::abs(N(2))))
  {
    alpha11 = (E02(2) * E03(0) - E02(0) * E03(2)) / N(1);
    beta11 = (E01(2) * E02(0) - E01(0) * E02(2)) / N(1);
  }
  else
  {
    alpha11 = (E02(0) * E03(1) - E02(1) * E03(0)) / N(2);
    beta11 = (E01(0) * E02(1) - E01(1) * E02(0)) / N(2);
  }

  // Compute the bilinear coordinates of the // intersection point.
  if (std::abs(alpha11 - 1) < TOLERANCE)
  {
    u = alpha;
    if (std::abs(beta11 - 1) < TOLERANCE)
      v = beta;
    else
      v = beta / (u * (beta11 - 1) + 1);
  }
  else if (std::abs(beta11 - 1) < TOLERANCE)
  {
    v = beta;
    u = alpha / (v * (alpha11 - 1) + 1);
  }
  else
  {
    auto A = -(beta11 - 1);
    auto B = alpha * (beta11 - 1) - beta * (alpha11 - 1) - 1;
    auto C = alpha;

    auto delta = (B * B) - (4 * A * C);
    auto Q = -0.5 * (B + ((B < 0.0 ? -1.0 : 1.0) * std::sqrt(delta)));
    u = Q / A;
    if ((u < 0) || (u > 1))
      u = C / Q;
    v = beta / (u * (beta11 - 1) + 1);
  }
  return true;
}

// https://people.cs.kuleuven.be/~ares.lagae/publications/LD05ERQIT/LD05ERQIT_code.cpp
template<typename PointType>
bool
intersectQuadTuned(const PointType & O,
                    const PointType & D,
                    const PointType & V00,
                    const PointType & V10,
                    const PointType & V11,
                    const PointType & V01,
                    Real & u,
                    Real & v,
                    Real & t)
{
  // Reject rays using the barycentric coordinates of // the intersection point with respect to T.
  auto E01 = V10;
  E01 -= V00;

  auto E03 = V01;
  E03 -= V00;

  auto P = D.cross(E03);

  auto det = E01 * P;

  //  libMesh::err<<"Count: "<<count<<std::endl;

  if (std::abs(det) < TOLERANCE)
  {
#ifdef USE_DEBUG_RAY
    if (ray->id() == debug_ray_id)
      libMesh::err << "1 Rejecting because " << std::abs(det) << " < " << TOLERANCE << std::endl;
#endif
    return false;
  }

//  std::cout<<"Here 1"<<std::endl;

  auto T = O;
  T -= V00;

  auto alpha = (T * P) / det;

  if (alpha < -1e-12)
  {
#ifdef USE_DEBUG_RAY
    if (ray->id() == debug_ray_id)
      libMesh::err << "2 Rejecting because " << alpha << " < " << 0 << std::endl;
#endif
    return false;
  }

//  std::cout<<"Here 2"<<std::endl;

  /*
  if (alpha > 1)
  {

      libMesh::err<<"3 Rejecting because "<<alpha<<" > "<<1<<std::endl;

    return false;
  }
  */

  auto Q = T.cross(E01);

  // Compute the ray parameter of the intersection // point.
  t = (E03 * Q) / det;

  if (t < -1e-12)
  {
#ifdef USE_DEBUG_RAY
    if (ray->id() == debug_ray_id)
      libMesh::err << "3 Rejecting because " << t << " < " << 0 << std::endl;
#endif

    return false;
  }

//  std::cout<<"Here 3"<<std::endl;


  auto beta = (D * Q) / det;

  if (beta < -1e-12)
  {
#ifdef USE_DEBUG_RAY
    if (ray->id() == debug_ray_id)
      libMesh::err << "4 Rejecting because " << beta << " < " << 0 << std::endl;
#endif

    return false;
  }

//  std::cout<<"Here 4"<<std::endl;


  /*
    if (beta > 1)
    {

        libMesh::err<<"5 Rejecting because "<<beta<<" > "<<1+TOLERANCE<<std::endl;

      return false;
    }
  */

  // Reject rays using the barycentric coordinates of // the intersection point with respect to
  // T′.
  if ((alpha + beta) > 1)
  {
    auto E23 = V01;
    E23 -= V11;

    auto E21 = V10;
    E21 -= V11;

    auto P_prime = D.cross(E21);

    auto det_prime = E23 * P_prime;

    if (std::abs(det_prime) < TOLERANCE)
    {
#ifdef USE_DEBUG_RAY
      if (ray->id() == debug_ray_id)
        libMesh::err << "5 Rejecting because " << std::abs(det_prime) << " < " << TOLERANCE
                     << std::endl;
#endif

      return false;
    }

//    std::cout<<"Here 5"<<std::endl;


    auto T_prime = O;
    T_prime -= V11;

    auto alpha_prime = (T_prime * P_prime) / det_prime;

    if (alpha_prime < -1e-12)
    {
#ifdef USE_DEBUG_RAY
      if (ray->id() == debug_ray_id)
        libMesh::err << "6 Rejecting because " << alpha_prime << " < " << 0 << std::endl;
#endif

      return false;
    }

//    std::cout<<"Here 6"<<std::endl;


    auto Q_prime = T_prime.cross(E23);

    auto beta_prime = (D * Q_prime) / det_prime;

    if (beta_prime < -1e-12)
    {
#ifdef USE_DEBUG_RAY
      if (ray->id() == debug_ray_id)
        libMesh::err << "7 Rejecting because " << beta_prime << " < " << 0 << std::endl;
#endif

      return false;
    }
//    std::cout<<"Here 7"<<std::endl;
  }

  // Compute the barycentric coordinates of V11. E02 = V11 - V00
  auto E02 = V11;
  E02 -= V00;

  auto N = E01.cross(E03);

  Real alpha11;
  Real beta11;

  Real temp_N[4];
  N.getValues(temp_N);

  Real temp_E01[4];
  E01.getValues(temp_E01);

  Real temp_E02[4];
  E02.getValues(temp_E02);

  Real temp_E03[4];
  E03.getValues(temp_E03);

  if ((std::abs(temp_N[0]) >= std::abs(temp_N[1])) && (std::abs(temp_N[0]) >= std::abs(temp_N[2])))
  {
//    std::cout<<"Here 8"<<std::endl;
    alpha11 = (temp_E02[1] * temp_E03[2] - temp_E02[2] * temp_E03[1]) / temp_N[0];
    beta11 = (temp_E01[1] * temp_E02[2] - temp_E01[2] * temp_E02[1]) / temp_N[0];
  }
  else if ((std::abs(temp_N[1]) >= std::abs(temp_N[0])) && (std::abs(temp_N[1]) >= std::abs(temp_N[2])))
  {
//    std::cout<<"Here 9"<<std::endl;
    alpha11 = (temp_E02[2] * temp_E03[0] - temp_E02[0] * temp_E03[2]) / temp_N[1];
    beta11 = (temp_E01[2] * temp_E02[0] - temp_E01[0] * temp_E02[2]) / temp_N[1];
  }
  else
  {
//    std::cout<<"Here 10"<<std::endl;
    alpha11 = (temp_E02[0] * temp_E03[1] - temp_E02[1] * temp_E03[0]) / temp_N[2];
    beta11 = (temp_E01[0] * temp_E02[1] - temp_E01[1] * temp_E02[0]) / temp_N[2];
  }

  // Compute the bilinear coordinates of the // intersection point.
  if (std::abs(alpha11 - 1) < TOLERANCE)
  {
//    std::cout<<"Here 11"<<std::endl;
    u = alpha;
    if (std::abs(beta11 - 1) < TOLERANCE)
      v = beta;
    else
      v = beta / (u * (beta11 - 1) + 1);
  }
  else if (std::abs(beta11 - 1) < TOLERANCE)
  {
//    std::cout<<"Here 12"<<std::endl;
    v = beta;
    u = alpha / (v * (alpha11 - 1) + 1);
  }
  else
  {
//    std::cout<<"Here 13"<<std::endl;
    auto A = -(beta11 - 1);
    auto B = alpha * (beta11 - 1) - beta * (alpha11 - 1) - 1;
    auto C = alpha;

    auto delta = (B * B) - (4 * A * C);
    auto Q = -0.5 * (B + ((B < 0.0 ? -1.0 : 1.0) * std::sqrt(delta)));
    u = Q / A;
    if ((u < 0) || (u > 1))
      u = C / Q;
    v = beta / (u * (beta11 - 1) + 1);
  }

//  std::cout<<"Here 14"<<std::endl;

  return true;
}




template<typename PointType>
bool
intersectQuadHandVectorized(const PointType & in_O,
                            const PointType & in_D,
                            const PointType & in_V00,
                            const PointType & in_V10,
                            const PointType & in_V11,
                            const PointType & in_V01,
                            Real & u,
                            Real & v,
                            Real & t)
{
  Real temp_O[4];
  Real temp_D[4];
  Real temp_V00[4];
  Real temp_V10[4];
  Real temp_V11[4];
  Real temp_V01[4];

  for (unsigned int i = 0; i < LIBMESH_DIM; i++)
  {
    temp_O[i] = in_O(i);
    temp_D[i] = in_D(i);
    temp_V00[i] = in_V00(i);
    temp_V10[i] = in_V10(i);
    temp_V11[i] = in_V11(i);
    temp_V01[i] = in_V01(i);
  }

  Vec3d O, D, V00, V10, V11, V01;

  O.load(temp_O);
  D.load(temp_D);
  V00.load(temp_V00);
  V10.load(temp_V01);
  V11.load(temp_V11);
  V01.load(temp_V01);


  // Reject rays using the barycentric coordinates of // the intersection point with respect to T.
  auto E01 = V10 - V00;
  auto E03 = V01 - V00;

  auto P = cross_product(D, E03);

  auto det = dot_product(E01, P);

  //  libMesh::err<<"Count: "<<count<<std::endl;

  if (std::abs(det) < TOLERANCE)
  {
#ifdef USE_DEBUG_RAY
    if (ray->id() == debug_ray_id)
      libMesh::err << "1 Rejecting because " << std::abs(det) << " < " << TOLERANCE << std::endl;
#endif
    return false;
  }

//  std::cout<<"Here 1"<<std::endl;

  auto T = O - V00;

  auto alpha = dot_product(T,P) / det;

  if (alpha < -1e-12)
  {
#ifdef USE_DEBUG_RAY
    if (ray->id() == debug_ray_id)
      libMesh::err << "2 Rejecting because " << alpha << " < " << 0 << std::endl;
#endif
    return false;
  }

//  std::cout<<"Here 2"<<std::endl;

  /*
  if (alpha > 1)
  {

      libMesh::err<<"3 Rejecting because "<<alpha<<" > "<<1<<std::endl;

    return false;
  }
  */

  auto Q = cross_product(T, E01);

  // Compute the ray parameter of the intersection // point.
  t = dot_product(E03, Q) / det;

  if (t < -1e-12)
  {
#ifdef USE_DEBUG_RAY
    if (ray->id() == debug_ray_id)
      libMesh::err << "3 Rejecting because " << t << " < " << 0 << std::endl;
#endif

    return false;
  }

//  std::cout<<"Here 3"<<std::endl;


  auto beta = dot_product(D, Q) / det;

  if (beta < -1e-12)
  {
#ifdef USE_DEBUG_RAY
    if (ray->id() == debug_ray_id)
      libMesh::err << "4 Rejecting because " << beta << " < " << 0 << std::endl;
#endif

    return false;
  }

//  std::cout<<"Here 4"<<std::endl;


  /*
    if (beta > 1)
    {

        libMesh::err<<"5 Rejecting because "<<beta<<" > "<<1+TOLERANCE<<std::endl;

      return false;
    }
  */

  // Reject rays using the barycentric coordinates of // the intersection point with respect to
  // T′.
  if ((alpha + beta) > 1)
  {
    auto E23 = V01 - V11;

    auto E21 = V10 - V11;

    auto P_prime = cross_product(D, E21);

    auto det_prime = dot_product(E23, P_prime);

    if (std::abs(det_prime) < TOLERANCE)
    {
#ifdef USE_DEBUG_RAY
      if (ray->id() == debug_ray_id)
        libMesh::err << "5 Rejecting because " << std::abs(det_prime) << " < " << TOLERANCE
                     << std::endl;
#endif

      return false;
    }

//    std::cout<<"Here 5"<<std::endl;


    auto T_prime = O - V11;

    auto alpha_prime = dot_product(T_prime, P_prime) / det_prime;

    if (alpha_prime < -1e-12)
    {
#ifdef USE_DEBUG_RAY
      if (ray->id() == debug_ray_id)
        libMesh::err << "6 Rejecting because " << alpha_prime << " < " << 0 << std::endl;
#endif

      return false;
    }

//    std::cout<<"Here 6"<<std::endl;


    auto Q_prime = cross_product(T_prime, E23);

    auto beta_prime = dot_product(D, Q_prime) / det_prime;

    if (beta_prime < -1e-12)
    {
#ifdef USE_DEBUG_RAY
      if (ray->id() == debug_ray_id)
        libMesh::err << "7 Rejecting because " << beta_prime << " < " << 0 << std::endl;
#endif

      return false;
    }
//    std::cout<<"Here 7"<<std::endl;
  }

  // Compute the barycentric coordinates of V11. E02 = V11 - V00
  auto E02 = V11 - V00;

  auto N = cross_product(E01, E03);

  Real alpha11;
  Real beta11;

  Real temp_N[4];
  N.store(temp_N);

  Real temp_E01[4];
  E01.store(temp_E01);

  Real temp_E02[4];
  E02.store(temp_E02);

  Real temp_E03[4];
  E03.store(temp_E03);

  if ((std::abs(temp_N[0]) >= std::abs(temp_N[1])) && (std::abs(temp_N[0]) >= std::abs(temp_N[2])))
  {
//    std::cout<<"Here 8"<<std::endl;
    alpha11 = (temp_E02[1] * temp_E03[2] - temp_E02[2] * temp_E03[1]) / temp_N[0];
    beta11 = (temp_E01[1] * temp_E02[2] - temp_E01[2] * temp_E02[1]) / temp_N[0];
  }
  else if ((std::abs(temp_N[1]) >= std::abs(temp_N[0])) && (std::abs(temp_N[1]) >= std::abs(temp_N[2])))
  {
//    std::cout<<"Here 9"<<std::endl;
    alpha11 = (temp_E02[2] * temp_E03[0] - temp_E02[0] * temp_E03[2]) / temp_N[1];
    beta11 = (temp_E01[2] * temp_E02[0] - temp_E01[0] * temp_E02[2]) / temp_N[1];
  }
  else
  {
//    std::cout<<"Here 10"<<std::endl;
    alpha11 = (temp_E02[0] * temp_E03[1] - temp_E02[1] * temp_E03[0]) / temp_N[2];
    beta11 = (temp_E01[0] * temp_E02[1] - temp_E01[1] * temp_E02[0]) / temp_N[2];
  }

  // Compute the bilinear coordinates of the // intersection point.
  if (std::abs(alpha11 - 1) < TOLERANCE)
  {
//    std::cout<<"Here 11"<<std::endl;
    u = alpha;
    if (std::abs(beta11 - 1) < TOLERANCE)
      v = beta;
    else
      v = beta / (u * (beta11 - 1) + 1);
  }
  else if (std::abs(beta11 - 1) < TOLERANCE)
  {
//    std::cout<<"Here 12"<<std::endl;
    v = beta;
    u = alpha / (v * (alpha11 - 1) + 1);
  }
  else
  {
//    std::cout<<"Here 13"<<std::endl;
    auto A = -(beta11 - 1);
    auto B = alpha * (beta11 - 1) - beta * (alpha11 - 1) - 1;
    auto C = alpha;

    auto delta = (B * B) - (4 * A * C);
    auto Q = -0.5 * (B + ((B < 0.0 ? -1.0 : 1.0) * std::sqrt(delta)));
    u = Q / A;
    if ((u < 0) || (u > 1))
      u = C / Q;
    v = beta / (u * (beta11 - 1) + 1);
  }

//  std::cout<<"Here 14"<<std::endl;

  return true;
}
