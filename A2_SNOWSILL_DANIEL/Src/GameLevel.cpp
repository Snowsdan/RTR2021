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
        .Ambient = glm::vec3(1.0, 0.2, 0.2),
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

   
    InitGameBoard();
    dynamicObjects.push_back(AddSphere(glm::vec3(0.0, 0.75, 0.0)));

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
    UpdateState(deltaTime);

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
    for (RTRObject* obj : defaultShaderObjects) {
        glUseProgram(defaultShader->GetId());
        defaultShader->SetFloat("u_CurTime", (float)currentTime);
        defaultShader->SetFloat("u_TimeDelta", (float)deltaTime);
        defaultShader->SetMat4("u_ModelMatrix", obj->model);
        defaultShader->SetMat4("u_ViewMatrix", viewMatrix);
        defaultShader->SetMat4("u_ProjectionMatrix", projectionMatrix);
        defaultShader->SetCamera("u_Camera", *camera);
        defaultShader->SetLightingModel(*lightingModel);

        obj->Render(defaultShader);
    }

   

}

//Test all dyanamic objects against all static objects for collisions
void GameLevel::UpdateState(float dt) {
    for (RTRObject* dynamicObj : dynamicObjects) {
        dynamicObj->Translate(dynamicObj->velocity * dt);
        for (RTRObject* staticObj : staticObjects) {
            if (dynamicObj->collider->checkCollision(staticObj->collider)) {
                std::cout << "COLLISION" << std::endl;
            }
            /*else {
                std::cout << "NO COLLISION" << std::endl;

            }*/

        }
    }
}

RTRObject* GameLevel::AddCube(glm::vec3 startingPosition, glm::vec3 scale) {
    RTRCube* cube = new RTRCube();
    cube->Init();
    cube->Translate(startingPosition);
    cube->Scale(scale);
    defaultShaderObjects.push_back(cube);

    return cube;
}
RTRObject* GameLevel::AddSphere(glm::vec3 startingPosition) {
    RTRSphere* sphere = new RTRSphere();
    sphere->Init();
    sphere->Translate(startingPosition);
    plasmaShaderObjects.push_back(sphere);

    return sphere;
}

void GameLevel::InitGameBoard() {
    //Base
    //staticObjects.push_back(AddCube(glm::vec3(0.0, 0.0, 0.0), glm::vec3(5.0, 0.5, 5.0)));

    //Four Walls
    staticObjects.push_back(AddCube(glm::vec3(0.0, 0.5, 2.75), glm::vec3(5.0, 0.5, 0.5)));
    //staticObjects.push_back(AddCube(glm::vec3(0.0, 0.5, 0.0), glm::vec3(5.0, 0.5, 0.5)));

    staticObjects.push_back(AddCube(glm::vec3(0.0, 0.5, -2.75), glm::vec3(5.0, 0.5, 0.5)));

    staticObjects.push_back(AddCube(glm::vec3(2.75, 0.5, 0.0), glm::vec3(1.0, 0.5, 5.0)));
    staticObjects.push_back(AddCube(glm::vec3(-2.75, 0.5, 0.0), glm::vec3(1.0, 0.5, 5.0)));

}

void GameLevel::CreateUniformGrid() {
    
    float gridWidth = 5.0;
    float gridHeight = 5.0;
    float cellSize = 0.5;

    for (int i = 0; i < 5; i++) {
        BoundingBox* cell = new BoundingBox();
    }
}

