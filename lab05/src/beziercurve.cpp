#include "beziercurve.h"
#include <string>
#include "gl/shaders/ShaderAttribLocations.h"
#include <cmath>


BezierCurve::BezierCurve() :
    m_numRows(100),
    m_numCols(m_numRows)
{

}

int BezierCurve::nChoosek(int n, int k )
{
    if (k > n) return 0;
    if (k * 2 > n) k = n-k;
    if (k == 0) return 1;

    int result = n;
    for(int i = 2; i <= k; ++i ) {
        result *= (n-i+1);
        result /= i;
    }
    return result;
}


glm::vec3 BezierCurve::getPoint(std::vector<Point3d> points, float t) {
    Point3d point = {0, 0, 0};
    int n = points.size() - 1;
    for (int i = 0; i < n+1; i ++) {
        point.x += float(nChoosek(n, i)) * float(pow(t, i)) * float(pow((1-t), (n-i))) * points[i].x;
        point.y += float(nChoosek(n, i)) * float(pow(t, i)) * float(pow((1-t), (n-i))) * points[i].y;
        point.z += float(nChoosek(n, i)) * float(pow(t, i)) * float(pow((1-t), (n-i))) * points[i].z;
    }
    return glm::vec3(point.x, point.y, point.z);
}


void BezierCurve::init() {
    // initialize using quads
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    int numVertices = 100;
    std::vector<float> data(numVertices * 3);
    Point3d p1 = {-4, 1, -2};
    Point3d p2 = {-3, 3, 0};
    Point3d p3 = {-1, 3, 0};
    Point3d p4 = {1, 0, 0};
    Point3d p5 = {3, 2, 0};
    Point3d p6 = {5, 5, -2};
    Point3d p7 = {7, 1, -3};
    Point3d p8 = {5, 0, -5};
    Point3d p9 = {2, 1, -4};
    Point3d p10 = {0, 3, -3};
    Point3d p11 = {-1, 4, -3};
    Point3d p12 = {-2, 5, -2};
    Point3d p13 = {-3, 2, -2};
    Point3d p14 = {-3, -1, -1};
    Point3d p15 = {-3, -1, 0};
    Point3d p16 = {-2, 1, 1};
    Point3d p17 = {0, 3, 1};
    Point3d p18 = {3, 1, 1};
    Point3d p19 = {4, 2, 2};
    Point3d p20 = {3, 4, 3};
    Point3d p21 = {1, 2, 3};
    Point3d p22 = {0, 2, 3};
    Point3d p23 = {-1, 1, 3};
    Point3d p24 = {-3, 1, 3};
    Point3d p25 = {-4, 1, 3};
    std::vector<Point3d> points = {p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16,
                                  p17, p18, p19, p20, p21, p22, p23, p24, p25};
    for (int i = 0; i < numVertices; i++) {
        float t = float(1.f/float(numVertices)) * i;
        data[3*i] = getPoint(points, t)[0];
        data[1 + 3*i] = getPoint(points, t)[1];
        data[2 + 3*i] = getPoint(points, t)[2];
    }

    // Initialize OpenGLShape.
    m_shape = std::make_unique<OpenGLShape>();
    m_shape->setVertexData(&data[0], data.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_LINE_STRIP, numVertices);
    m_shape->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);

    m_shape->buildVAO();

}

std::vector<glm::vec3> BezierCurve::getCurve() {
    std::vector<glm::vec3> curve;
    int numVertices = 100;
    std::vector<float> data(numVertices * 3);
    Point3d p1 = {-4, 1, -2};
    Point3d p2 = {-3, 3, 0};
    Point3d p3 = {-1, 3, 0};
    Point3d p4 = {1, 0, 0};
    Point3d p5 = {3, 2, 0};
    Point3d p6 = {5, 5, -2};
    Point3d p7 = {7, 1, -3};
    Point3d p8 = {5, 0, -5};
    Point3d p9 = {2, 1, -4};
    Point3d p10 = {0, 3, -3};
    Point3d p11 = {-1, 4, -3};
    Point3d p12 = {-2, 5, -2};
    Point3d p13 = {-3, 2, -2};
    Point3d p14 = {-3, -1, -1};
    Point3d p15 = {-3, -1, 0};
    Point3d p16 = {-2, 1, 1};
    Point3d p17 = {0, 3, 1};
    Point3d p18 = {3, 1, 1};
    Point3d p19 = {4, 2, 2};
    Point3d p20 = {3, 4, 3};
    Point3d p21 = {1, 2, 3};
    Point3d p22 = {0, 2, 3};
    Point3d p23 = {-1, 1, 3};
    Point3d p24 = {-3, 1, 3};
    Point3d p25 = {-4, 1, 3};
    std::vector<Point3d> points = {p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16,
                                  p17, p18, p19, p20, p21, p22, p23, p24, p25};
    for (int i = 0; i < numVertices; i++) {
        float t = float(1.f/float(numVertices)) * i;
        data[3*i] = getPoint(points, t)[0];
        data[1 + 3*i] = getPoint(points, t)[1];
        data[2 + 3*i] = getPoint(points, t)[2];
        curve.push_back(glm::vec3(data[3*i], data[1 + 3*i], data[2 + 3*i]));
    }
    return curve;
}


/**
 * Draws the curve.
 */
void BezierCurve::draw()
{
    m_shape->draw();
}
