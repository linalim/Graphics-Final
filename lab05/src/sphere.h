#ifndef SPHERE_H
#define SPHERE_H

#include <glm/glm.hpp>
#include "GL/glew.h"

#include<memory>
#include <vector>

inline void insertVec3(std::vector<float> &data, glm::vec3 v){
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}

namespace CS123 { namespace GL {
class VAO;
}}

class sphere
{
public:
    sphere();
    sphere(int param1, int param2);
    ~sphere();

    void setVertexData();
    void drawWalls();
    void draw();
    glm::vec3 calculateNormal(float theta, float phi);
private:
    int m_param1;
    int m_param2;

protected:
    /** builds the VAO, pretty much the same as from lab 1 */
    void buildVAO();

    std::vector<GLfloat> m_vertexData;
    std::unique_ptr<CS123::GL::VAO> m_VAO;
};

#endif // SPHERE_H
