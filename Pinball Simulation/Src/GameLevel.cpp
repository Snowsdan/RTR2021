#include "GameLevel.h"
#include <typeinfo>

GameLevel::GameLevel(glm::mat4 projectionMat) {

    projectionMatrix = projectionMat;
    lightingModel = new RTRLightingModel();

    // Add directional light to follow camera view direction
    
    RTRLight_t* light1 = new RTRLight_t{ 
        .Type = RTRLightType::DirectionalLight,
        .Ambient = glm::vec3(0.2, 0.2, 0.2),
        .Diffuse = glm::vec3(0.2, 0.2, 1.0),
        .Specular = glm::vec3(0.5, 0.5, 0.5),
        .Direction = glm::vec3(1.0, 0.0, 0.0) };
    lightingModel->AddLight(*light1);

    RTRLight_t* light2 = new RTRLight_t{
        .Type = RTRLightType::PointLight,
        .Ambient = glm::vec3(0.5, 0.5, 0.5),
        .Diffuse = glm::vec3(0.5, 0.5, 0.5),
        .Specular = glm::vec3(1.0, 1.0, 1.0),
        .Position = glm::vec3(5.0, 2.0, 5.0),
        .Constant = 1.0f,
        .Linear = 0.35f,
        .Quadratic = 0.44f
    };
    
    lightingModel->AddLight(*light2);
    AddDebugLight(light2);

    RTRLight_t* light3 = new RTRLight_t{
        .Type = RTRLightType::PointLight,
        .Ambient = glm::vec3(0.5, 0.2, 0.2),
        .Diffuse = glm::vec3(0.5, 0.0, 0.0),
        .Specular = glm::vec3(1.0, 1.0, 1.0),
        .Position = glm::vec3(10.0, 0.0, -10.0),
        .Constant = 1.0f,
        .Linear = 0.35f,
        .Quadratic = 0.44f
        };
    lightingModel->AddLight(*light3);
    AddDebugLight(light3);


    basicShader = new RTRShader();
    if (basicShader->Load("Src/BasicShader.vert", "Src/BasicShader.frag") != 0) {
        std::cout << "COULD NOT LOAD BASIC SHADER" << std::endl;

    }
    defaultShader = new RTRShader();
    if (defaultShader->Load("Src/RTRDefault.vert", "Src/RTRDefault.frag", "Src/RTRDefault.geom") != 0) {
        std::cout << "COULD NOT LOAD DEFAULT SHADER" << std::endl;
    }

    plasmaShader = new RTRShader();
    if (plasmaShader->Load("Src/RTRPlasma.vert", "Src/RTRPlasma.frag") != 0) {
        std::cout << "COULD NOT LOAD PLASMA SHADER" << std::endl;

    }

    skyBoxShader = new RTRShader();
    if (skyBoxShader->Load("Src/SkyBoxShader.vert", "Src/SkyBoxShader.frag") != 0) {
        std::cout << "COULD NOT LOAD SKYBOX SHADER" << std::endl;

    }
    skyBox = new SkyBox();
    
    reflectiveShader = new RTRShader();
    //The RTRDefault geometry shader has everything we need, so no need to write one specifically for reflections
    if (reflectiveShader->Load("Src/ReflectiveShader.vert", "Src/ReflectiveShader.frag", "Src/RTRDefault.geom") != 0) {
        std::cout << "COULD NOT LOAD REFLECTIVE SHADER" << std::endl;

    }
    
   
    InitGameBoard();
    
    CreateUniformGrid();
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


    glUseProgram(defaultShader->GetId());
    
    for (RTRObject* obj : defaultShaderObjects) {
        defaultShader->SetVec2("scaleFactor", obj->scaleFactor);
        defaultShader->SetInt("RTRMaterial.Diffuse", 0);
        defaultShader->SetFloat("u_CurTime", (float)currentTime);
        defaultShader->SetFloat("u_TimeDelta", (float)deltaTime);
        defaultShader->SetMat4("u_ModelMatrix", obj->model);
        defaultShader->SetMat4("u_ViewMatrix", viewMatrix);
        defaultShader->SetMat4("u_ProjectionMatrix", projectionMatrix);
        defaultShader->SetCamera("u_Camera", *camera);
        defaultShader->SetLightingModel(*lightingModel);

        obj->Render(defaultShader);
    }
    defaultShader->SetMat4("u_ModelMatrix", gameBoardBase->model);
    defaultShader->SetVec2("scaleFactor", gameBoardBase->scaleFactor);

    gameBoardBase->Render(defaultShader);
    
    
    //Render Skybox
    glDepthFunc(GL_LEQUAL);  
    glUseProgram(skyBoxShader->GetId());

    skyBoxShader->SetMat4("view", glm::mat4(glm::mat3(viewMatrix)));
    skyBoxShader->SetMat4("projection", projectionMatrix);
    skyBoxShader->SetInt("skybox", 0);
    skyBox->Render(skyBoxShader);

    glDepthFunc(GL_LESS);
    
    glUseProgram(reflectiveShader->GetId());
    for (RTRObject* obj : reflectiveShaderObjects) {
        reflectiveShader->SetMat4("model", obj->model);
        reflectiveShader->SetMat4("view", viewMatrix);
        reflectiveShader->SetMat4("projection", projectionMatrix);
        reflectiveShader->SetVec3("cameraPos", camera->m_Position);
        reflectiveShader->SetInt("skyBox", 0);
        obj->Render(reflectiveShader);
    }

    if(debug)
        DrawDebug(viewMatrix, projectionMatrix);

}

//Test all dyanamic objects against all static objects for collisions
void GameLevel::UpdateState(float dt) {
    for (RTRObject* dynamicObj : dynamicObjects) {

        //Constantly apply a downward force
        dynamicObj->velocity.z += 0.00002 * dt;    
        dynamicObj->Translate(dynamicObj->velocity * dt);
        
        //An attempt at using the Uniform Grid to test for collisions
        /*std::tuple<int, int> coords = { floor(dynamicObj->transform.position.x), floor(dynamicObj->transform.position.z)};
        std::vector<RTRObject*>* objs = uniformGrid.at(coords);
        for (RTRObject* staticObj : *objs) {
            CollisionInfo* collision = dynamicObj->collider->checkCollision(staticObj->collider);
            if (collision != nullptr) {
                HandleCollision((RTRSphere*)dynamicObj, collision);
            }
        }*/

        for (RTRObject* staticObj : staticObjects) {
            CollisionInfo* collision = dynamicObj->collider->checkCollision(staticObj->collider);
            if (collision != nullptr) {
                HandleCollision((RTRSphere*)dynamicObj, collision);
            }
        }
        

        for (RTRObject* otherDynamic : dynamicObjects) {
            if (dynamicObj == otherDynamic)
                continue;
            CollisionInfo* collision = dynamicObj->collider->checkCollision(otherDynamic->collider);
            if (collision != nullptr) {
                HandleCollision((RTRSphere*)dynamicObj, collision);
                HandleCollision((RTRSphere*)otherDynamic, 
                    otherDynamic->collider->checkCollision(dynamicObj->collider));
            }
        }
    }

}

RTRObject* GameLevel::AddCube(glm::vec3 startingPosition, glm::vec3 scale, const char* texturePath) {
    RTRCube* cube = new RTRCube();
    cube->Init(texturePath);
    cube->Translate(startingPosition);
    cube->Scale(scale);
    defaultShaderObjects.push_back(cube);

    return cube;
}
RTRObject* GameLevel::AddSphere(glm::vec3 startingPosition) {
    RTRSphere* sphere = new RTRSphere();
    sphere->Init();
    sphere->Translate(startingPosition);
    reflectiveShaderObjects.push_back(sphere);

    return sphere;
}

void GameLevel::LaunchBall() {
    dynamicObjects.push_back(AddSphere(glm::vec3(4.3, 0.5, 2.0)));
}

void GameLevel::InitGameBoard() {
    //Base
    //staticObjects.push_back(AddCube(glm::vec3(0.0, 0.0, 0.0), glm::vec3(5.0, 0.5, 5.0)));

    //Four Walls
    //Bottom Walls
    staticObjects.push_back(AddCube(glm::vec3(3.0, 0.5, 5.0), glm::vec3(4.0, 0.5, 0.5), "Src/Textures/Metallic.png"));
    staticObjects.push_back(AddCube(glm::vec3(-3.0, 0.5, 5.0), glm::vec3(4.0, 0.5, 0.5), "Src/Textures/Metallic.png"));

    //Upper Wall
    staticObjects.push_back(AddCube(glm::vec3(0.0, 0.5, -5.0), glm::vec3(10.0, 0.5, 0.5), "Src/Textures/Metallic.png"));
    //Right Wall
    staticObjects.push_back(AddCube(glm::vec3(5.0, 0.5, 0.0), glm::vec3(0.5, 0.5, 10.0), "Src/Textures/Metallic.png"));
    //Left Wall
    staticObjects.push_back(AddCube(glm::vec3(-5.0, 0.5, 0.0), glm::vec3(0.5, 0.5, 10.0), "Src/Textures/Metallic.png"));

    //Right Hand Side Guard Rail
    staticObjects.push_back(AddCube(glm::vec3(3.8, 0.5, 1.5), glm::vec3(0.25, 0.25, 7.8), "Src/Textures/Metallic.png"));

    //Pegs. Should be within x: -5 and 4. z: +-5
    staticObjects.push_back(AddCube(glm::vec3(3.0, 0.5, 1.3), glm::vec3(0.2, 0.5, 1.0), "Src/Textures/Metallic.png"));
    staticObjects.push_back(AddCube(glm::vec3(-2.5, 0.5, 2.5), glm::vec3(0.5, 0.5, 1.4), "Src/Textures/Metallic.png"));
    staticObjects.push_back(AddCube(glm::vec3(-2.5, 0.5, -3.6), glm::vec3(0.5, 0.5, 1.0), "Src/Textures/Metallic.png"));
    staticObjects.push_back(AddCube(glm::vec3(1.5, 0.5, 0.5), glm::vec3(4.0, 0.5, 0.3), "Src/Textures/Metallic.png"));

    //Initialise Flippers


    //Initialise the base
    gameBoardBase = new RTRCube();
    gameBoardBase->Init("Src/Textures/EmeraldTex.jpg");
    gameBoardBase->Scale(glm::vec3(10.0, 0.5, 10.0));
}

void GameLevel::CreateUniformGrid() {
    
    //Loop through each cell in the grid
    for (int i = 0; i < uniformRows; i++) {
        for (int j = 0; j < uniformColumns; j++) {

            //Create a list to store the objects that intersect with the cell
            std::vector<RTRObject*>* objectList = new std::vector<RTRObject*>;
            float x = j + startingPosition.x;
            float z = i + startingPosition.z;

            //Create a bounding box for said cell
            BoundingBox* cell = 
                new BoundingBox(x, x + cellSize, z, z + cellSize, glm::vec3(0.0));

            //Check each static object in the scene to see if it interesects with the cell
            for (RTRObject* staticObj : staticObjects) {
                //If there is a collision, store a reference to the object in the cells object list
                if (cell->checkCollision(staticObj->collider)) {
                    objectList->push_back(staticObj);
                }
            }
            uniformGrid.insert({ std::make_tuple(x,z), objectList });
        }
    }
}



void GameLevel::HandleCollision(RTRSphere* obj, CollisionInfo* collision)
{
    //If there was a horizontal collision, move object outside of collider
    //and then reverse x velocity
    if (collision->direction == LEFT || collision->direction == RIGHT) 
    {
        obj->velocity.x = -obj->velocity.x; // reverse
        // relocate
        float penetration = obj->sphereRadius -
            std::abs(collision->differenceVec.x);
        if (collision->direction == LEFT)
            obj->transform.position.x += penetration * 1.1; // move right
        else
            obj->transform.position.x -= penetration * 1.1; // move left;
    }
    //If there was a veritcal collision, move object outside of collider
    //and then reverse z velocity
    else 
    {
        obj->velocity.z = -obj->velocity.z; // reverse
        // relocate
        float penetration = obj->sphereRadius -
            std::abs(collision->differenceVec.y);
        if (collision->direction == UP)
            obj->transform.position.z -= penetration * 1.1; // move up
        else
            obj->transform.position.z += penetration * 1.1; // move down
    }

}

#pragma region Debug
void GameLevel::DrawDebug(glm::mat4 view, glm::mat4 projection) {

    //Render Light Models
    glUseProgram(basicShader->GetId());
    basicShader->SetMat4("view", view);
    basicShader->SetMat4("projection", projection);

    for (RTRObject* light : debugObjects) {
        basicShader->SetMat4("model", light->model);
        basicShader->SetVec3("colour", light->colour);
        light->Render(basicShader);
    }


    glUseProgram(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(&projectionMatrix[0][0]);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    glPushMatrix();
    glLoadMatrixf(&view[0][0]);
    float len = 2.0;

    glBegin(GL_LINES);
    glColor3f(5.0, 0.01, 0.0);
    glVertex3f(-5.0, 0.01, 0.0);
    glVertex3f(5.0, 0.01, 0.0);
    glColor3f(0.0, 1.01, 0.0);
    glVertex3f(0.0, -5.01, 0.0);
    glVertex3f(0.0, 5.01, 0.0);
    glColor3f(0.0, 0.01, 1.0);
    glVertex3f(0.0, 0.01, -5.0);
    glVertex3f(0.0, 0.01, 5.0);
    glEnd();

    DrawBoundingVolumes();
    DrawUniformGrid();
    glPopMatrix();
}

void GameLevel::DrawBoundingVolumes()
{
    for (RTRObject* dynamicObj : dynamicObjects) {
        dynamicObj->collider->DrawVolume();
    }

    for (RTRObject* staticObj : staticObjects) {
        staticObj->collider->DrawVolume();
    }
}

void GameLevel::DrawUniformGrid() {
    glBegin(GL_LINES);

    glColor3f(0.0, 1.0, 0.0);

    std::map<std::tuple<int, int>, std::vector<RTRObject*>*>::iterator it;

    for (it = uniformGrid.begin(); it != uniformGrid.end(); it++){
        glVertex3f(std::get<0>(it->first), 0.6, std::get<1>(it->first));
        glVertex3f(std::get<0>(it->first) + cellSize, 0.6, std::get<1>(it->first));

        glVertex3f(std::get<0>(it->first), 0.6, std::get<1>(it->first));
        glVertex3f(std::get<0>(it->first), 0.6, std::get<1>(it->first) + cellSize);
    }
    glEnd();
}
void GameLevel::AddDebugLight(RTRLight_t* light)
{
    RTRCube* cube = new RTRCube();
    cube->Init(nullptr);
    cube->Translate(light->Position);
    cube->colour = light->Ambient;
    debugObjects.push_back(cube);

}


#pragma endregion

