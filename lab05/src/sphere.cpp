#include "sphere.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <cmath>
#include "glm/gtc/constants.hpp"
#include "gl/datatype/VAO.h"
#include "gl/datatype/VBO.h"
#include "gl/datatype/VBOAttribMarker.h"
#include "gl/shaders/ShaderAttribLocations.h"
#include <iostream>
#include <QImage>


using namespace CS123::GL;

sphere::sphere() :
    m_VAO(nullptr)
{

}

sphere::sphere(int param1, int param2) :
    m_param1(param1),
    m_param2(param2)
{
    /**
     * We build in vertex data for a cube, in this case they are handwritten.
     * You'll want to expand upon or refactor this code heavily to take advantage
     * of polymorphism, vector math/glm library, and utilize good software design
     *
     */
    setVertexData();
    /** build the VAO so that the shape is ready to be drawn */
    buildVAO();
}

sphere::~sphere()
{

}

void sphere::setVertexData() {
    m_vertexData.reserve((6 * m_param1 - 6) * m_param2);
    drawWalls();

}

void sphere::drawWalls() {
    float radius = 0.1f;
    for (int i = 0; i < m_param1; i++) {
        for (int j = 0; j < m_param2; j++) {
            if (m_param1 < 2) { // setting minimum tessellation parameters
                m_param1 = 2;
            }
            if (m_param2 < 3) { // setting minimum tessellation parameters
                m_param2 = 3;
            }
            float theta = 2 * glm::pi<float>() / m_param2 * j;
            float phi = glm::pi<float>() / m_param1 * i;
            float dtheta = 2 * glm::pi<float>() / m_param2;
            float dphi = glm::pi<float>() / m_param1;

            glm::vec3 vertex1 = glm::vec3(radius * glm::sin(phi) * glm::cos(theta), -radius * glm::cos(phi), radius * glm::sin(phi) * glm::sin(theta));
            glm::vec3 vertex2 = glm::vec3(radius * glm::sin(phi) * glm::cos(theta + dtheta), -radius * glm::cos(phi), radius * glm::sin(phi) * glm::sin(theta + dtheta));
            glm::vec3 vertex3 = glm::vec3(radius * glm::sin(phi + dphi) * glm::cos(theta + dtheta), -radius * glm::cos(phi + dphi), radius * glm::sin(phi + dphi) * glm::sin(theta + dtheta));
            glm::vec3 vertex4 = glm::vec3(radius * glm::sin(phi + dphi) * glm::cos(theta), -radius * glm::cos(phi + dphi), radius * glm::sin(phi + dphi) * glm::sin(theta));

            insertVec3(m_vertexData, vertex1);
            insertVec3(m_vertexData, calculateNormal(theta, phi));
            insertVec3(m_vertexData, vertex4);
            insertVec3(m_vertexData, calculateNormal(theta, phi + dphi));
            insertVec3(m_vertexData, vertex3);
            insertVec3(m_vertexData, calculateNormal(theta + dtheta, phi + dphi));
            insertVec3(m_vertexData, vertex1);
            insertVec3(m_vertexData, calculateNormal(theta, phi));
            insertVec3(m_vertexData, vertex3);
            insertVec3(m_vertexData, calculateNormal(theta + dtheta, phi + dphi));
            insertVec3(m_vertexData, vertex2);
            insertVec3(m_vertexData, calculateNormal(theta + dtheta, phi));

        }
    }
}

void sphere::draw() {
    if (m_VAO) {
        m_VAO->bind();
        m_VAO->draw();
        m_VAO->unbind();
    }
}

void sphere::buildVAO() {
    const int numFloatsPerVertex = 6;
    const int numVertices = m_vertexData.size() / numFloatsPerVertex;

    std::vector<VBOAttribMarker> markers;
    markers.push_back(VBOAttribMarker(ShaderAttrib::POSITION, 3, 0));
    markers.push_back(VBOAttribMarker(ShaderAttrib::NORMAL, 3, 3*sizeof(float)));
    VBO vbo = VBO(m_vertexData.data(), m_vertexData.size(), markers);
    m_VAO = std::make_unique<VAO>(vbo, numVertices);

}

glm::vec3 sphere::calculateNormal(float theta, float phi) {
    glm::vec3 normal = glm::vec3(glm::sin(phi) * glm::cos(theta), -glm::cos(phi), glm::sin(phi) * glm::sin(theta));
    return normal;
}
