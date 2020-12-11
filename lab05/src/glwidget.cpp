#include "glwidget.h"

#include "cs123_lib/resourceloader.h"
#include "cs123_lib/errorchecker.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <iostream>

#define PI 3.14159265f

GLWidget::GLWidget(QGLFormat format, QWidget *parent)
    : QGLWidget(format, parent), m_angleX(0), m_angleY(0.5f), m_zoom(10.f),
      m_program(0),
      m_timer(this),
      m_fps(60.0f),
      m_increment(0)
{
    // Set up 60 FPS draw loop.
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));
    m_timer.start(1000.0f / m_fps);
    m_tick = 1.0;
    m_angle = 0.f;
}

GLWidget::~GLWidget()
{
}

/** Repaints the canvas. Called 60 times per second. */
void GLWidget::tick()
{
    m_tick += 1;

    m_angle += M_PI / 60.f;

    if(m_angle >= 2 * M_PI) {
        m_angle = 0.f;
    }

    update();
}

void GLWidget::initializeGL() {
    ResourceLoader::initializeGlew();
    resizeGL(width(), height());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Set the color to set the screen when the color buffer is cleared.
    glClearColor(0.f, 0.f, 0.f, 0.f);

//    m_program = ResourceLoader::createShaderProgram(":/shaders/shader.vert", ":/shaders/shader.frag");
    m_program = ResourceLoader::createShaderProgram(":/shaders/texture.vert", ":/shaders/texture.frag");

    m_terrain.init();
    m_bezier.init();
    m_sphere = std::make_unique<sphere>(30, 30);

    rebuildMatrices();
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float time = m_increment++ / (float) m_fps;      // Time in seconds.

    // Bind shader program.
    glUseProgram(m_program);

    std::vector<glm::vec3> bezier_curve = m_bezier.getCurve();
    glm::mat4 viewT;
    glm::mat4 rotT;
    if (5*time > bezier_curve.size()) { // edge case
        m_increment = 0;
        m_translation = glm::translate(bezier_curve[0]);
    }

    m_translation = glm::translate(bezier_curve[5*time]);

    viewT = glm::translate(-bezier_curve[5*time-2]);

    rotT = orientLook(glm::vec4(bezier_curve[5*time-2], 1.f), glm::vec4(bezier_curve[5*time-1], 1.f), glm::vec4(0.f, 1.f, 0.f, 0.f));
    m_view = rotT * viewT;
    // Set uniforms.
    glUniformMatrix4fv(glGetUniformLocation(m_program, "model"), 1, GL_FALSE, glm::value_ptr(m_model));
    glUniformMatrix4fv(glGetUniformLocation(m_program, "view"), 1, GL_FALSE, glm::value_ptr(m_view));
    glUniformMatrix4fv(glGetUniformLocation(m_program, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));

    // Draw terrain.
    m_terrain.draw();
    m_bezier.draw();

    //set another uniform called T and update matrix
    GLint uniform_loc = glGetUniformLocation(m_program, "model");
    glUniformMatrix4fv(uniform_loc, 1, GL_FALSE, glm::value_ptr(m_translation));
    m_sphere->draw();

    // Unbind shader program.
    glUseProgram(0);

}

glm::mat4 GLWidget::orientLook(const glm::vec4 &eye, const glm::vec4 &look, const glm::vec4 &up) {
    // @TODO: [CAMTRANS] Fill this in...
    glm::vec4 m_w = -glm::vec4(glm::vec3(glm::normalize(look-eye)), 0.f); // look
    glm::vec4 m_up = glm::vec4(glm::vec3(glm::normalize(up)), 0.f);
    glm::vec4 m_v = glm::vec4(glm::vec3(glm::normalize(m_up - glm::dot(m_up, m_w) * m_w)), 0.f);
    glm::vec4 m_u = glm::vec4(glm::cross(glm::vec3(m_v), glm::vec3(m_w)), 0.f);
    glm::mat4 rotationMatrix = glm::mat4(m_u.x, m_u.y, m_u.z, 0.0,
                                 m_v.x, m_v.y, m_v.z, 0.0,
                                 m_w.x, m_w.y, m_w.z, 0.0,
                                 0.0, 0.0, 0.0, 1.0);
    rotationMatrix = glm::transpose(rotationMatrix);
    return rotationMatrix;
}

void GLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    rebuildMatrices();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    m_prevMousePos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    m_angleX += 10 * (event->x() - m_prevMousePos.x()) / (float) width();
    m_angleY += 10 * (event->y() - m_prevMousePos.y()) / (float) height();
    m_prevMousePos = event->pos();
    rebuildMatrices();
}

void GLWidget::wheelEvent(QWheelEvent *event) {
    m_zoom -= event->delta() / 100.f;
    rebuildMatrices();
}

void GLWidget::rebuildMatrices() {
    m_model = glm::mat4(1.f);
    m_view = glm::translate(glm::vec3(0, 0, -m_zoom)) *
             glm::rotate(m_angleY, glm::vec3(1,0,0)) *
             glm::rotate(m_angleX, glm::vec3(0,1,0));
    m_projection = glm::perspective(0.8f, (float)width()/height(), 0.1f, 100.f);    
    update();
}
