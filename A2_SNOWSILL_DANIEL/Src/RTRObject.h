//-----------------------------------------------------------------------------
// RTRObject.h
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#pragma once
#include "RTRLighting.h"
#include "RTRShader.h"
#include "Collider.h"
#include <vector>
struct RTRPoint_t {
    float x, y, z;
};

struct RTRFace_t {
    unsigned int v0, v1, v2;
};

//-----------------------------------------------------------------------------
class RTRObject
{
public:
    RTRObject() {}
    ~RTRObject() {}
    virtual void Init();
    virtual void Render(RTRShader* shader);
    virtual void End();
    void SetMaterial(RTRMaterial_t mat) { m_Material = mat; }
    virtual const char* GetName() { return "RTRObject"; }

public:
    unsigned int m_NumVertices{ 0 };
    unsigned int m_NumFaces{ 0 };
    std::vector<float> vertices;
    std::vector<unsigned int> faces;
    RTRPoint_t* m_VertexPoints{ nullptr };
    RTRFace_t* m_Faces{ nullptr };
    RTRMaterial_t m_Material{ {0.19225, 0.19225, 0.19225 }, { 0.50754, 0.50754, 0.50754 }, { 0.508273, 0.508273, 0.508273 }, 128.0 };
    unsigned int m_VertexBuffer = 0;
    unsigned int m_VertexArray = 0;
    unsigned int m_FaceElementBuffer = 0;

    glm::mat4 model = glm::mat4(1.0);
    glm::vec3 position = glm::vec3(0.0);
    glm::vec3 rotation = glm::vec3(0.0);
    glm::vec3 scale = glm::vec3(0.0);

    Collider* collider;
    bool checkCollision(BoundingBox* first, BoundingBox* second);
    bool checkCollision(BoundingBox* first, BoundingSphere* second);
    bool checkCollision(BoundingSphere* first, BoundingSphere* second);

    void handleCollision(glm::vec3 colPoint);

    
};

//-----------------------------------------------------------------------------
class RTRCube : public RTRObject
{
public:
    RTRCube() : RTRObject() {}
    ~RTRCube() {}
    virtual void Init();
    virtual const char* GetName() { return "RTRCube"; }
    void Translate(glm::vec3 translation);
    void Scale(glm::vec3 scaleFactor);

    
};
