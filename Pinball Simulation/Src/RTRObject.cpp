//-----------------------------------------------------------------------------
// RTRObject.cpp
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------

#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>

#include "RTRObject.h"

//----------------------------------------------------------------------------
void RTRObject::Init()
{
    glGenBuffers(1, &m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    //glBufferData(GL_ARRAY_BUFFER, m_NumVertices * sizeof(RTRPoint_t), m_VertexPoints, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_VertexArray);
    glBindVertexArray(m_VertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

    //5 numbers for each vertex (3 for position, 2 for texture coords
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &m_FaceElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_FaceElementBuffer);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_NumFaces * sizeof(RTRFace_t), m_Faces, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(unsigned int), &faces[0], GL_STATIC_DRAW);
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
    scaleFactor = glm::vec2(scale.x, scale.z);
    model = glm::scale(model, scale);
    collider->Scale(scale);
}

#pragma region RTRCube
//-----------------------------------------------------------------------------
void RTRCube::Init(const char* texturePath)
{

    vertices = {
            //Vertices        //Texture Coordinates
         -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
    m_NumVertices = 36;

    collider = new BoundingBox(-0.5, 0.5, -0.5, 0.5, glm::vec3(0.0));
    if (texturePath != nullptr) {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);


        unsigned char* data = stbi_load(texturePath, &width, &height, &numChannels, 0);
        if (data)
        {

            //Check for number of colour channels
            GLenum format;
            if (numChannels == 1)
                format = GL_RED;
            else if (numChannels == 3)
                format = GL_RGB;
            else if (numChannels == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "FAILED TO LOAD TEXTURE: " << texturePath <<  std::endl;
            stbi_image_free(data);
        }
    }
    
    RTRObject::Init();
}
void RTRCube::Render(RTRShader* shader)
{
    shader->SetMaterial("u_ObjectMaterial", m_Material);
    //shader->SetInt("u_ObjectMaterial.Diffuse", 0);
    glBindVertexArray(m_VertexArray);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    //glDrawElements(GL_TRIANGLES, m_NumFaces, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
#pragma endregion 

#pragma region RTRSphere

//This code was taken from: http://www.songho.ca/opengl/gl_sphere.html
void RTRSphere::Init() {

    float radius = 0.25f;
    float x, y, z, xy, k1, k2;                      // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
    float s, t;                                     // vertex texCoord
    
    float sectorCount = 75.0f;
    float stackCount = 75.0f;
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
    velocity = glm::vec3(0.003, 0.0, -0.015);
    collider = new BoundingSphere(sphereRadius, glm::vec3(0.0));

    glGenBuffers(1, &m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_VertexArray);
    glBindVertexArray(m_VertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &m_FaceElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_FaceElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(unsigned int), &faces[0], GL_STATIC_DRAW);

}

void RTRSphere::Render(RTRShader* shader)
{
    shader->SetMaterial("u_ObjectMaterial", m_Material);
    shader->SetInt("boxTexture", 0);

    glBindVertexArray(m_VertexArray);
    glDrawElements(GL_TRIANGLES, m_NumFaces, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
#pragma endregion

#pragma region Cylinder
void RTRCylinder::Init(const char* texturePath) {
    
}

#pragma endregion