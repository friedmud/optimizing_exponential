#ifndef TRACE_RAY_2D_H
#define TRACE_RAY_2D_H

#include <cmath>
#include <iostream>

#include "libmesh/point.h"
#include "libmesh/vectorized_point.h"

#define Real double

using namespace libMesh;

/**
 * Pass in:
 * Ray: o -> d
 * Side: v0 -> v1
 *
 * Internally:
 * Ray: p -> t*r
 * Side: q -> u*s
 *
 * From: https://stackoverflow.com/a/565282
 */
template <typename PointType>
bool
lineLineIntersect2DVanilla(const PointType & o,
                           const PointType & d,
                           const PointType & v0,
                           const PointType & v1,
                           Real & u,
                           Real & t);

template <>
bool
lineLineIntersect2DVanilla(const Point & o,
                           const Point & d,
                           const Point & v0,
                           const Point & v1,
                           Real & u,
                           Real & t);

template <>
bool
lineLineIntersect2DVanilla(const VectorizedPoint & o,
                           const VectorizedPoint & d,
                           const VectorizedPoint & v0,
                           const VectorizedPoint & v1,
                           Real & u,
                           Real & t);

/**
 * Pass in:
 * Ray: o -> d
 * Side: v0 -> v1
 *
 * Internally:
 * Ray: p -> t*r
 * Side: q -> u*s
 *
 * From: https://stackoverflow.com/a/565282
 */
bool
lineLineIntersect2DTuned(const VectorizedPoint & o,
                         const VectorizedPoint & d,
                         const VectorizedPoint & v0,
                         const VectorizedPoint & v1,
                         Real & u,
                         Real & t);



bool
lineLineIntersect2DHand(const Point & o,
                        const Point & d,
                        const Point & v0,
                        const Point & v1,
                        Real & u,
                        Real & t);

/**
 * Derived from: http://stackoverflow.com/a/565282/2042320
 * q -> q + u*s: Ray
 * p -> p + t*r: Side
 */
// template <typename T>
// int
// sideIntersectedByLine2D(const Elem * current_elem,
//                         unsigned int incoming_side,
//                         const Point & incoming_point,
//                         const std::shared_ptr<Ray> & ray,
//                         const std::map<const Elem *, std::vector<Point>> & elem_normals,
//                         Point & intersection_point,
//                         Point & /*boundaryintersection_point*/)
// {
//   int intersected_side = -1;
//   unsigned int n_sides = current_elem->n_sides();

//   std::vector<double> stuff;

//   Point ray_direction = (ray->end() - ray->start());
//   ray_direction /= ray_direction.norm();

//   {
//   //    int boundary_side = -1;

// #ifdef USE_DEBUG_RAY
//     if (DEBUG_IF)
//     {
//       std::cerr << "Incoming point: " << incoming_point << std::endl;
//       std::cerr << "Incoming side: " << incoming_side << std::endl;
//       std::cerr << "Ray end: " << ray->end() << std::endl;
//     }

// #endif

//     // Bump the starting point down the path a bit
//     double q0 = incoming_point(0) + 1e-9 * (ray->end()(0) - incoming_point(0));
//     double q1 = incoming_point(1) + 1e-9 * (ray->end()(1) - incoming_point(1));

//     double s0 = ray->end()(0) - q0;
//     double s1 = ray->end()(1) - q1;

//     double intersection_distance = std::numeric_limits<Real>::max();
//     double centroid_distance = std::numeric_limits<Real>::max();

//     /*
//     if (ray_count == 2071730)
//       libMesh::err<<std::endl;
//     */
//     auto & normals = elem_normals.at(current_elem);

//     for (unsigned int i = 0; i < n_sides; i++)
//     {
//       if (i == incoming_side) // Don't search backwards
//         continue;

//       // Backface culling
//       if (normals[i] * ray_direction < -TOLERANCE)
//         continue;

//         /*
//         //if (ray_count == 2071730)
//         {
//           libMesh::err<<"side: "<<i<<std::endl;
//         }
//         */

//         //      bool intersect = false;

// #ifdef USE_DEBUG_RAY
//       if (DEBUG_IF)
//       {
//         std::cerr << "Side: " << i << std::endl;
//         std::cerr << " Side nodes: " << std::endl;
//         std::cerr << "  " << current_elem->point(T::side_nodes_map[i][0]) << std::endl;
//         std::cerr << "  " << current_elem->point(T::side_nodes_map[i][1]) << std::endl;
//       }
// #endif

//       double p0 = current_elem->point(T::side_nodes_map[i][0])(0);
//       double p1 = current_elem->point(T::side_nodes_map[i][0])(1);

//       double r0 = current_elem->point(T::side_nodes_map[i][1])(0) - p0;
//       double r1 = current_elem->point(T::side_nodes_map[i][1])(1) - p1;

//       double rxs = r0 * s1 - r1 * s0;
//       double sxr = s0 * r1 - s1 * r0;

//       if (std::abs(rxs) < 1e-10 || std::abs(sxr) < 1e-10) // Lines are parallel or colinear
//         continue;

//       double t = (((q0 - p0) * s1) - ((q1 - p1) * s0)) / rxs;
//       double u = (((p0 - q0) * r1) - ((p1 - q1) * r0)) / sxr;

//       /*
//       //      if (ray_count == 2071730)
//             {
//               libMesh::err<<"Tolerance: "<<TOLERANCE<<std::endl;

//               libMesh::err<<"t: "<<t<<std::endl;
//               libMesh::err<<"u: "<<u<<std::endl;

//               libMesh::err<<"t + TOLERANCE: "<<t + TOLERANCE<<std::endl;
//               libMesh::err<<"t - TOLERANCE: "<<t - TOLERANCE<<std::endl;
//               libMesh::err<<"u + TOLERANCE: "<<u + TOLERANCE<<std::endl;
//               libMesh::err<<"u - TOLERANCE: "<<u - TOLERANCE<<std::endl;
//             }
//       */

//       if ((0 < t + 4e-9 && t - 4e-9 <= 1.0) &&
//           (0 < u + 4e-9 && u - 4e-9 <= 1.0)) // Do they intersect
//       {
//         double current_centroid_distance = std::abs(
//             t - 0.5); // We want to prefer intersections that go through the middle of sides

//         if (current_centroid_distance < centroid_distance)
//         {
//           //          Elem * neighbor = current_elem->neighbor(i);

//           /*
//           //if (ray_count == 2071730)
//             libMesh::err<<"Intersected side: "<<i<<std::endl;
//           */

//           intersected_side = i;
//           intersection_distance = u;
//           centroid_distance = current_centroid_distance;

//           // Bump just a bit down the Ray to reduce edge cases
//           intersection_point(0) = q0 + ((u /*+1e-9*/) * s0);
//           intersection_point(1) = q1 + ((u /*+1e-9*/) * s1);
//         }
//       }
//       else
//       {
// #ifdef USE_DEBUG_RAY
//         if (DEBUG_IF)
//         {
//           std::cerr << "\nSide: " << i << std::endl;
//           std::cerr << " 0 < t + 4e-9: " << t + 4e-9 << std::endl;
//           std::cerr << " t - 4e-9 <= 1.0: " << t - 4e-9 << std::endl;
//           std::cerr << " 0 < u + 4e-9: " << u + 4e-9 << std::endl;
//           std::cerr << " u - 4e-9 <= 1.0: " << u - 4e-9 << std::endl;
//         }
// #endif
//       }
//     }
//   }

//   return intersected_side;
// }

#endif
