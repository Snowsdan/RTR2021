//-----------------------------------------------------------------------------
// RTRObject.cpp
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------

#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>

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

//Translates the object by translating model matrix and modifying other attributes
void RTRObject::Translate(glm::vec3 translation) {
    model = glm::translate(model, translation);
    collider->Translate(translation);
}

void RTRObject::Scale(glm::vec3 scale) {
    model = glm::scale(model, scale);
    collider->Scale(scale);
}

#pragma region RTRCube
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

    collider = new BoundingBox(-0.5, 0.5, -0.5, 0.5, glm::vec3(0.0));
   
    RTRObject::Init();
}

#pragma endregion 

#pragma region RTRSphere

//This code was taken from: http://www.songho.ca/opengl/gl_sphere.html
void RTRSphere::Init() {

    float radius = 0.5f;
    float x, y, z, xy, k1, k2;                      // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
    float s, t;                                     // vertex texCoord
    
    float sectorCount = 50.0f;
    float stackCount = 50.0f;
    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = M_PI / 2.0f - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectorCount; ++j, ++k1, ++k2)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0)
            {
                faces.push_back(k1);
                faces.push_back(k2);
                faces.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if (i != (stackCount - 1))
            {
                faces.push_back(k1 + 1);
                faces.push_back(k2);
                faces.push_back(k2 + 1);
            }
        }
    }

    m_NumFaces = faces.size();
    m_NumVertices = vertices.size();

    sphereRadius = radius;
    velocity = glm::vec3(0.001, 0.0, 0.001);
    collider = new BoundingSphere(sphereRadius, glm::vec3(0.0));

    RTRObject::Init();
}
#pragma endregion