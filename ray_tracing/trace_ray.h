#include "libmesh/point.h"

using namespace libMesh;

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
