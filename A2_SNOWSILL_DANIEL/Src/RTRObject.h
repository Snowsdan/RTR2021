//-----------------------------------------------------------------------------
// RTRObject.h
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#pragma once
#include "RTRLighting.h"
#include "RTRShader.h"

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
    RTRPoint_t* m_VertexPoints{ nullptr };
    RTRFace_t* m_Faces{ nullptr };
    RTRMaterial_t m_Material{ {0.19225, 0.19225, 0.19225 }, { 0.50754, 0.50754, 0.50754 }, { 0.508273, 0.508273, 0.508273 }, 128.0 };
    unsigned int m_VertexBuffer{ 0 };
    unsigned int m_VertexArray{ 0 };
    unsigned int m_FaceElementBuffer{ 0 };
};

//-----------------------------------------------------------------------------
class RTRCube : public RTRObject
{
public:
    RTRCube() : RTRObject() {}
    ~RTRCube() {}
    virtual void Init();
    virtual const char* GetName() { return "RTRCube"; }
};
