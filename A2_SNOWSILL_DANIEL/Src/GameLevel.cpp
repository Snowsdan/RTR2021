#include "GameLevel.h"
#include <typeinfo>

GameLevel::GameLevel(glm::mat4 projectionMat) {

    projectionMatrix = projectionMat;
    lightingModel = new RTRLightingModel();

    // Add directional light to follow camera view direction
    lightingModel->AddLight({
        .Type = RTRLightType::DirectionalLight,
        .Ambient = glm::vec3(0.2, 0.2, 0.2),
        .Diffuse = glm::vec3(1.0, 1.0, 1.0),
        .Specular = glm::vec3(0.5, 0.5, 0.5),
        .Direction = glm::vec3(1.0, 0.0, 0.0)
        });
    // Add x-axis red spot lights (palce all at x+3.0 so they are positioned around shaded cube)
    lightingModel->AddLight({
        .Type = RTRLightType::PointLight,
        .Ambient = glm::vec3(1.0, 0.0, 0.0),
        .Diffuse = glm::vec3(1.0, 0.0, 0.0),
        .Specular = glm::vec3(1.0, 1.0, 1.0),
        .Position = glm::vec3(2.0 + 3.0, 0.0, 0.0),
        .Constant = 1.0f,
        .Linear = 0.35f,
        .Quadratic = 0.44f
        });

    defaultShader = new RTRShader();
    if (defaultShader->Load("Src/RTRDefault.vert", "Src/RTRDefault.frag", "Src/RTRDefault.geom") != 0) {
        std::cout << "COULD NOT LOAD DEFAULT SHADER" << std::endl;
    }

    plasmaShader = new RTRShader();
    if (plasmaShader->Load("Src/RTRPlasma.vert", "Src/RTRPlasma.frag") != 0) {
        std::cout << "COULD NOT LOAD PLASMA SHADER" << std::endl;

    }

    AddCube(glm::vec3(6.0, 0.0, -1.0));
    AddCube(glm::vec3(3.0, 0.0, 0.0));

}
GameLevel::~GameLevel() {
    for (RTRObject* obj : defaultShaderObjects) {
        delete(obj);
    }
    defaultShaderObjects.clear();
    for (RTRObject* obj : plasmaShaderObjects) {
        delete(obj);
    }
    plasmaShaderObjects.clear();
	delete plasmaShader;
	delete defaultShader;
}

void GameLevel::RenderScene(float currentTime, float deltaTime, glm::mat4 viewMatrix, RTRCamera* camera) {
    UpdateState();

    glUseProgram(plasmaShader->GetId());
    for (RTRObject* obj : plasmaShaderObjects) {
        plasmaShader->SetFloat("u_CurTime", currentTime);
        plasmaShader->SetFloat("u_TimeDelta", deltaTime);
        plasmaShader->SetMat4("u_ModelMatrix", obj->model);
        plasmaShader->SetMat4("u_ViewMatrix", viewMatrix);
        plasmaShader->SetMat4("u_ProjectionMatrix", projectionMatrix);
        plasmaShader->SetCamera("u_Camera", *camera);
        obj->Render(plasmaShader);
    }

}

//Test all dyanamic objects against all static objects for collisions
void GameLevel::UpdateState() {
    for (RTRObject* obj : dynamicObjects) {
        for (RTRObject* staticObj : staticObjects) {
            //Check if the collider is a AABB, if so then cast to AABB and resolve collision
            if (typeid(*staticObj->collider) == typeid(BoundingBox)) {
                if (obj->checkCollision((BoundingBox*)obj->collider, (BoundingBox*)staticObj->collider)) {
                    std::cout << "TWO BOX COLLISION" << std::endl;
                }
            }
            else if (typeid(*staticObj->collider) == typeid(BoundingSphere)) {
                obj->checkCollision((BoundingSphere*)obj->collider, (BoundingSphere*)staticObj->collider);

            }
        }
    }
}

void GameLevel::AddCube(glm::vec3 startingPosition) {
    RTRCube* cube = new RTRCube();
    cube->Init();
    cube->Translate(startingPosition);
    defaultShaderObjects.push_back(cube);
}
void AddSphere(glm::vec3 startingPosition) {

}