#include "trace_ray_2d.h"

template <>
bool
lineLineIntersect2DVanilla(const Point & o,
                           const Point & d,
                           const Point & v0,
                           const Point & v1,
                           Real & u,
                           Real & t)

{
  const Point & p = o;
  const Point & q = v0;

  auto qmp = q - p;

  auto r = (d - o);
  auto s = (v1 - v0);

  auto rxs = r(0)*s(1) - r(1)*s(0);

  if (std::abs(rxs) < 1e-10) // Lines are parallel or colinear
    false;

  t = /*twodcross(qmp, s)*/ (qmp(0)*s(1)-qmp(1)*s(0)) / rxs;
  u = /*twodcross(qmp, r)*/ (qmp(0)*r(1)-qmp(1)*r(0)) / rxs;

  if ((0 < t + 4e-9 && t - 4e-9 <= 1.0) &&
      (0 < u + 4e-9 && u - 4e-9 <= 1.0)) // Do they intersect
    return true;

  // Not parallel, but don't intersect
  return false;
}

template <>
bool
lineLineIntersect2DVanilla(const VectorizedPoint & o,
                           const VectorizedPoint & d,
                           const VectorizedPoint & v0,
                           const VectorizedPoint & v1,
                           Real & u,
                           Real & t)

{
  const VectorizedPoint & p = o;
  const VectorizedPoint & q = v0;

  auto qmp = q - p;

  auto r = (d - o);
  auto s = (v1 - v0);

  auto rxs = r(0)*s(1) - r(1)*s(0);

  if (std::abs(rxs) < 1e-10) // Lines are parallel or colinear
    false;

  t = /*twodcross(qmp, s)*/ (qmp(0)*s(1)-qmp(1)*s(0)) / rxs;
  u = /*twodcross(qmp, r)*/ (qmp(0)*r(1)-qmp(1)*r(0)) / rxs;

  if ((0 < t + 4e-9 && t - 4e-9 <= 1.0) &&
      (0 < u + 4e-9 && u - 4e-9 <= 1.0)) // Do they intersect
    return true;

  // Not parallel, but don't intersect
  return false;
}

bool
lineLineIntersect2DTuned(const VectorizedPoint & o,
                           const VectorizedPoint & d,
                           const VectorizedPoint & v0,
                           const VectorizedPoint & v1,
                           Real & u,
                           Real & t)
{
  const VectorizedPoint & p = o;
  const VectorizedPoint & q = v0;

  auto r = (d - o);
  auto s = (v1 - v0);

  Real temp_r[4];
  r.getValues(temp_r);

  Real temp_s[4];
  s.getValues(temp_s);

  auto rxs = temp_r[0]*temp_s[1] - temp_r[1]*temp_s[0];

  if (std::abs(rxs) < 1e-10) // Lines are parallel or colinear
    false;

  auto qmp = q - p;

  Real temp_qmp[4];
  qmp.getValues(temp_qmp);

  t = /*twodcross(qmp, s)*/ (temp_qmp[0]*temp_s[1]-temp_qmp[1]*temp_s[0]) / rxs;
  u = /*twodcross(qmp, r)*/ (temp_qmp[0]*temp_r[1]-temp_qmp[1]*temp_r[0]) / rxs;

  if ((0 < t + 4e-9 && t - 4e-9 <= 1.0) &&
      (0 < u + 4e-9 && u - 4e-9 <= 1.0)) // Do they intersect
    return true;

  // Not parallel, but don't intersect
  return false;
}

bool
lineLineIntersect2DHand(const Point & o_in,
                        const Point & d_in,
                        const Point & v0_in,
                        const Point & v1_in,
                        Real & u,
                        Real & t)

{
  Vec2d p(o_in(0), o_in(1));
  Vec2d q(v0_in(0), v0_in(1));

  // Do this to do the two subtractions at the same time
  // auto r = (d - o);
  // auto s = (v1 - v0);

  Vec4d dv1(d_in(0), d_in(1), v1_in(0), v1_in(1));
  Vec4d pq(p, q);

  auto rs = dv1 - pq;

  // Pull r and s out
  auto r = rs.get_low();
  auto s = rs.get_high();

//  std::cout<<"r[0]: "<<r[0]<<std::endl;
//  std::cout<<"r[1]: "<<r[1]<<std::endl;

//  std::cout<<"s[0]: "<<s[0]<<std::endl;
//  std::cout<<"s[1]: "<<s[1]<<std::endl;

  auto sp = permute2d<1,0>(s);

  auto begin_cross = r * sp;

  Real temp[2];

  begin_cross.store(temp);

  auto rxs = temp[0] - temp[1];

//  std::cout<<"rxs: "<<rxs<<std::endl;

  if (std::abs(rxs) < 1e-10) // Lines are parallel or colinear
    false;

  auto qmp = q - p;
  auto rp = permute2d<1,0>(r);

  Vec4d twoqmp(qmp, qmp);
  Vec4d sprp(sp, rp);

  //  t = (qmp(0)*s(1)-qmp(1)*s(0)) / rxs;
  //  u = (qmp(0)*r(1)-qmp(1)*r(0)) / rxs;

  auto begin_big_cross = twoqmp * sprp;

  auto bbcp = permute4d<0,2,1,3>(begin_big_cross);

  auto lhs = bbcp.get_low();
  auto rhs = bbcp.get_high();

  auto top = lhs - rhs;

  Real temp_top[2];
  top.store(temp_top);

//  std::cout<<"rcross: "<<temp_top[0]<<std::endl;
//  std::cout<<"scross: "<<temp_top[1]<<std::endl;

  top /= rxs;

//  Real temp_top[2];
  top.store(temp_top);

  t = temp_top[0];
  u = temp_top[1];

//  std::cout<<"t: "<<t<<std::endl;
//  std::cout<<"u: "<<u<<std::endl;

  if ((0 < t + 4e-9 && t - 4e-9 <= 1.0) &&
      (0 < u + 4e-9 && u - 4e-9 <= 1.0)) // Do they intersect
    return true;

  // Not parallel, but don't intersect
  return false;
}
