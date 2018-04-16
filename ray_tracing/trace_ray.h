#include "libmesh/libmesh_common.h"
#include "libmesh/point.h"
#include "libmesh/vectorized_point.h"

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
              Real & t);

template<>
bool
intersectQuad(const Point & O,
              const Point & D,
              const Point & V00,
              const Point & V10,
              const Point & V11,
              const Point & V01,
              Real & u,
              Real & v,
              Real & t);

template<>
bool
intersectQuad(const VectorizedPoint & O,
              const VectorizedPoint & D,
              const VectorizedPoint & V00,
              const VectorizedPoint & V10,
              const VectorizedPoint & V11,
              const VectorizedPoint & V01,
              Real & u,
              Real & v,
              Real & t);


// https://people.cs.kuleuven.be/~ares.lagae/publications/LD05ERQIT/LD05ERQIT_code.cpp
bool
intersectQuadTuned(const VectorizedPoint & O,
                    const VectorizedPoint & D,
                    const VectorizedPoint & V00,
                    const VectorizedPoint & V10,
                    const VectorizedPoint & V11,
                    const VectorizedPoint & V01,
                    Real & u,
                    Real & v,
                   Real & t);


bool
intersectQuadHandVectorized(const Point & in_O,
                            const Point & in_D,
                            const Point & in_V00,
                            const Point & in_V10,
                            const Point & in_V11,
                            const Point & in_V01,
                            Real & u,
                            Real & v,
                            Real & t);

// https://en.wikipedia.org/wiki/Möller–Trumbore_intersection_algorithm
template<typename PointType>
bool
rayIntersectsTriangle(const PointType & O,
                      const PointType & D,
                      const PointType & V00,
                      const PointType & V10,
                      const PointType & V11,
                      Real & u,
                      Real & v,
                      Real & t);


template<>
bool
rayIntersectsTriangle(const Point & O,
                      const Point & D,
                      const Point & V00,
                      const Point & V10,
                      const Point & V11,
                      Real & u,
                      Real & v,
                      Real & t);

template<>
bool
rayIntersectsTriangle(const VectorizedPoint & O,
                      const VectorizedPoint & D,
                      const VectorizedPoint & V00,
                      const VectorizedPoint & V10,
                      const VectorizedPoint & V11,
                      Real & u,
                      Real & v,
                      Real & t);


template<typename PointType>
bool
intersectQuadUsingTriangles(const PointType & O,
                            const PointType & D,
                            const PointType & V00,
                            const PointType & V10,
                            const PointType & V11,
                            const PointType & V01,
                            Real & u,
                            Real & v,
                            Real & t);


template<>
bool
intersectQuadUsingTriangles(const Point & O,
                            const Point & D,
                            const Point & V00,
                            const Point & V10,
                            const Point & V11,
                            const Point & V01,
                            Real & u,
                            Real & v,
                            Real & t);

template<>
bool
intersectQuadUsingTriangles(const VectorizedPoint & O,
                            const VectorizedPoint & D,
                            const VectorizedPoint & V00,
                            const VectorizedPoint & V10,
                            const VectorizedPoint & V11,
                            const VectorizedPoint & V01,
                            Real & u,
                            Real & v,
                            Real & t);


class Hand
{
public:

  Vec3d edge1, edge2, h, s, q;
  Vec3d O, D, V00, V10, V11, V01;

  Real temp_O[4];
  Real temp_D[4];
  Real temp_V00[4];
  Real temp_V10[4];
  Real temp_V11[4];
  Real temp_V01[4];

// https://en.wikipedia.org/wiki/Möller–Trumbore_intersection_algorithm
bool
rayIntersectsTriangleHand(const Vec3d & O,
                          const Vec3d & D,
                          const Vec3d & V00,
                          const Vec3d & V10,
                          const Vec3d & V11,
                          Real & u,
                          Real & v,
                          Real & t);


bool
intersectQuadUsingTrianglesHand(const Point & in_O,
                                const Point & in_D,
                                const Point & in_V00,
                                const Point & in_V10,
                                const Point & in_V11,
                                const Point & in_V01,
                                Real & u,
                                Real & v,
                                Real & t);

};
