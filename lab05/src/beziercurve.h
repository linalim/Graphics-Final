#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/type_ptr.hpp"   // glm::value_ptr
#include <vector>

#include "openglshape.h"

struct Point3d
{
    float x, y, z;
};

struct Point2d {
    float x, y;
};

class BezierCurve
{
public:
    BezierCurve();
    Point2d bezier2D(std::vector<Point2d> points, float t);

    void draw();
    void init();

private:
    void drawDot(Point2d point);
    void drawLine(Point2d p1, Point2d p2);
};

#endif // BEZIERCURVE_H
