#include "beziercurve.h"
#include <string>

BezierCurve::BezierCurve()
{

}


Point2d BezierCurve::bezier2D(std::vector<Point2d> points, float t) {
//    for (int i = 0 ; i < int(points.size()); i++) {

//    }
    Point2d point;
    point.x = (1-t) * points[0].x + t * points[1].x;
    point.y = (1-t) * points[0].y + t * points[1].y;
    return point;
}

void BezierCurve::drawDot(Point2d point) {
    glBegin(GL_POINTS);
    glVertex2i(point.x,point.y);
    glEnd();
    glFlush();

}

void BezierCurve::drawLine(Point2d p1, Point2d p2) {
    glBegin(GL_LINES);
    glVertex2f(p1.x, p1.y);
    glVertex2f(p2.x, p2.y);
    glEnd();
    glFlush();
}

void BezierCurve::init() {
    // initialize using quads
}


/**
 * Draws the curve.
 */
void BezierCurve::draw()
{
    std::vector<Point2d> test = {{1.f, 2.f}, {1.f, 3.f}};
    Point2d bezier_point = bezier2D(test, 0.5f);
    drawDot(test[0]);
    drawDot(bezier_point);
    drawLine(test[0], bezier_point);
}
