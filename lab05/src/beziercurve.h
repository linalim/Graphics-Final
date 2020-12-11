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
    void draw();
    void init();

    std::vector<glm::vec3> getCurve();
private:
    const float m_numRows, m_numCols;
    std::unique_ptr<OpenGLShape> m_shape;
    int nChoosek(int n, int k);
    glm::vec3 getPoint(std::vector<Point3d> points, float t);


};

#endif // BEZIERCURVE_H
