//-----------------------------------------------------------------------------
// RTRObject.cpp
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#include <iostream>
#include "RTRObject.h"

// README / TODO - This is just a simple start.
// * For your assignment, think carefully about how Shaders, Materials and Objects will hang together.
// * Have a hierarchy of differnet object types (static object, moving object, hierarchical object, object with force, etc...
// * Don't worry too much about optimising the buffer handling by, for example, merging all objects into a single 
// vertex buffer. Or sorting object based on shader or material to minimize switches. Spend your time on more imprtant 
// optimisation techniques that will add marks - for example your uniform grid implementation. 
// * For this assignment you can hardcode the vertex buffers for the differnet shapes (see cube) and then 
// scale, place and animate them with transformation matrices.

//-----------------------------------------------------------------------------
void RTRObject::Init()
{
    float sizeofFace = faces.size() * sizeof(float);
    float mult = m_NumFaces * sizeof(float);
    glGenBuffers(1, &m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    //glBufferData(GL_ARRAY_BUFFER, m_NumVertices * sizeof(RTRPoint_t), m_VertexPoints, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, m_NumVertices * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_VertexArray);
    glBindVertexArray(m_VertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &m_FaceElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_FaceElementBuffer);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_NumFaces * sizeof(RTRFace_t), m_Faces, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(unsigned int), &faces[0], GL_STATIC_DRAW);
}

void RTRObject::Render(RTRShader *shader)
{
    shader->SetMaterial("u_ObjectMaterial", m_Material);
    glBindVertexArray(m_VertexArray);
    glDrawElements(GL_TRIANGLES, m_NumFaces, GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, m_NumVertices);
    glBindVertexArray(0);
}

void RTRObject::End()
{
    glDeleteVertexArrays(1, &m_VertexArray); m_VertexArray = 0;
    glDeleteBuffers(1, &m_VertexBuffer); m_VertexBuffer = 0;
    glDeleteBuffers(1, &m_FaceElementBuffer); m_FaceElementBuffer = 0;
    if (m_VertexPoints != nullptr) { delete m_VertexPoints; m_VertexPoints = nullptr; }
    if (m_Faces != nullptr) { delete m_Faces; m_Faces = nullptr; }
}

bool RTRObject::checkCollision(BoundingBox* first, BoundingBox* second) {

    return (first->maxX >= second->minX && first->minX <= second->maxX)
        && (first->maxZ >= second->minZ && first->minZ <= second->maxZ);

}


bool RTRObject::checkCollision(BoundingBox* first, BoundingSphere* second) {
    return false;
}


bool RTRObject::checkCollision(BoundingSphere* first, BoundingSphere* second) {

    float distance = sqrt((first->center.x - second->center.x) * (first->center.x - second->center.x) +
        (first->center.z - second->center.z) * (first->center.z - second->center.z));

    return distance < (first->radius + second->radius);
}

//-----------------------------------------------------------------------------
void RTRCube::Init()
{

    vertices = {
        //Front Face
       //     points        
       -0.5f, 0.5f, 0.5f,
         0.5f, 0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
       -0.5f, -0.5f, 0.5f,

       //Left Face
       //     points        
        -0.5f, 0.5f, -0.5f,
       -0.5f, 0.5f, 0.5f,
      -0.5f, -0.5f, 0.5f,
       -0.5f, -0.5f, -0.5f,

       //Back Face
       //     points        
       0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
       -0.5f, -0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,

      //Right Face
      //     points        
      0.5f, 0.5f, 0.5f,
     0.5f, 0.5f, -0.5f,
     0.5f, -0.5f,-0.5f,
     0.5f, -0.5f, 0.5f,

     //Top Face
     //     points        
     -0.5f, 0.5f, -0.5f,
      0.5f, 0.5f, -0.5f,
       0.5f, 0.5f, 0.5f,
      -0.5f, 0.5f, 0.5f,

      //Bottom Face
      //     points        
      0.5f, -0.5f, -0.5f,
     -0.5f, -0.5f, -0.5f,
      -0.5f, -0.5f, 0.5f,
       0.5f, -0.5f, 0.5f,
    };
    
    faces = {
        //Front face indices
       0, 2,1,
       0, 3, 2,
       //Left face indices
       4, 6, 5,
       4, 7, 6,
       //Back face indices
       8, 10, 9,
       8, 11, 10,
       //Right face indices
       12, 14, 13,
       12, 15, 14,
       ////Top face indices
       16, 18, 17,
       16, 19, 18,
       //Bottom face indices
       20, 22, 21,
       20, 23, 22
    };

    m_NumFaces = faces.size();
    m_NumVertices = vertices.size();

    BoundingBox* AABB = new BoundingBox();
    AABB->minX = -0.5;
    AABB->maxX = 0.5;
    AABB->minZ = -0.5;
    AABB->maxZ = 0.5;

    collider = AABB;
    
    RTRObject::Init();
}

//Translates the object by translating model matrix and modifying other attributes
void RTRCube::Translate(glm::vec3 translation) {
    model = glm::translate(model, translation);
    ((BoundingBox*)collider)->minX += translation.x;
    ((BoundingBox*)collider)->maxX += translation.x;
    ((BoundingBox*)collider)->minZ += translation.z;
    ((BoundingBox*)collider)->maxZ += translation.z;
}

void  RTRCube::Scale(glm::vec3 scaleFactor) {
    model = glm::scale(model, scaleFactor);
    ((BoundingBox*)collider)->minX *= scaleFactor.x;
    ((BoundingBox*)collider)->maxX *= scaleFactor.x;
    ((BoundingBox*)collider)->minZ *= scaleFactor.z;
    ((BoundingBox*)collider)->minZ *= scaleFactor.z;
}
