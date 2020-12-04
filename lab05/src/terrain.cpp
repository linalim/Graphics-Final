#include "terrain.h"

#include <math.h>
#include "gl/shaders/ShaderAttribLocations.h"
#include <iostream>

Terrain::Terrain() : m_numRows(100), m_numCols(m_numRows)
{
}


/**
 * Returns a pseudo-random value between -1.0 and 1.0 for the given row and column.
 */
float Terrain::randValue(int row, int col) {
    return -1.0 + 2.0 * glm::fract(sin(row * 127.1f + col * 311.7f) * 43758.5453123f);
}


/**
 * Returns the object-space position for the terrain vertex at the given row and column.
 */
glm::vec3 Terrain::getPosition(int row, int col) {
    glm::vec3 position;
    position.x = 10 * row/m_numRows - 5;
    position.y = 0;
    position.z = 10 * col/m_numCols - 5;

    // TODO: Adjust position.y using value noise.
    float scale = 20.f;
    for (int octave = 0; octave < 3; octave++) {
        float new_row = float(row)/scale;
        float new_col = float(col)/scale;

        float a = randValue(glm::floor(new_row), glm::floor(new_col));
        float b = randValue(glm::floor(new_row), glm::floor(new_col) + 1);
        float c = randValue(glm::floor(new_row) + 1, glm::floor(new_col));
        float d = randValue(glm::floor(new_row) + 1, glm::floor(new_col) + 1);

        float y = glm::fract(new_row);
        float x = glm::fract(new_col);
        position.y += glm::mix(glm::mix(a,b,x*x*(3-2*x)),glm::mix(c,d,x*x*(3-2*x)), y) / glm::pow(2.0f, float(octave));
        scale /= 2;
    }

    return position;

}


/**
 * Returns the normal vector for the terrain vertex at the given row and column.
 */
glm::vec3 Terrain::getNormal(int row, int col) {
    // TODO: Compute the normal at the given row and column using the positions of the
    //       neighboring vertices.

    glm::vec3 P = getPosition(row, col);
    glm::vec3 n0 = getPosition(row, col + 1);
    glm::vec3 n1 = getPosition(row + 1, col + 1);
    glm::vec3 n2 = getPosition(row + 1, col);
    glm::vec3 n3 = getPosition(row + 1, col - 1);
    glm::vec3 n4 = getPosition(row, col - 1);
    glm::vec3 n5 = getPosition(row - 1, col - 1);
    glm::vec3 n6 = getPosition(row - 1, col);
    glm::vec3 n7 = getPosition(row - 1, col + 1);

    glm::vec3 normal(0, 0, 0);
    normal += glm::normalize(glm::cross(n0 - P, n1 - P));
    normal += glm::normalize(glm::cross(n1 - P, n2 - P));
    normal += glm::normalize(glm::cross(n2 - P, n3 - P));
    normal += glm::normalize(glm::cross(n3 - P, n4 - P));
    normal += glm::normalize(glm::cross(n4 - P, n5 - P));
    normal += glm::normalize(glm::cross(n5 - P, n6 - P));
    normal += glm::normalize(glm::cross(n6 - P, n7 - P));
    normal += glm::normalize(glm::cross(n7 - P, n0 - P));

    return glm::normalize(normal);

}


/**
 * Initializes the terrain by storing positions and normals in a vertex buffer.
 */
void Terrain::init() {
    // TODO: Change from GL_LINE to GL_FILL in order to render full triangles instead of wireframe.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    // Initializes a grid of vertices using triangle strips.
    int numVertices = (m_numRows - 1) * (2 * m_numCols + 2);
    std::vector<glm::vec3> data(2 * numVertices);
    int index = 0;
    for (int row = 0; row < m_numRows - 1; row++) {
        for (int col = m_numCols - 1; col >= 0; col--) {
            data[index++] = getPosition(row, col);
            data[index++] = getNormal  (row, col);
            data[index++] = getPosition(row + 1, col);
            data[index++] = getNormal  (row + 1, col);
        }
        data[index++] = getPosition(row + 1, 0);
        data[index++] = getNormal  (row + 1, 0);
        data[index++] = getPosition(row + 1, m_numCols - 1);
        data[index++] = getNormal  (row + 1, m_numCols - 1);
    }

    // Initialize OpenGLShape.
    m_shape = std::make_unique<OpenGLShape>();
    m_shape->setVertexData(&data[0][0], data.size() * 3, VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, numVertices);
    m_shape->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_shape->setAttribute(ShaderAttrib::NORMAL, 3, sizeof(glm::vec3), VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_shape->buildVAO();
}


/**
 * Draws the terrain.
 */
void Terrain::draw()
{
    m_shape->draw();
}
