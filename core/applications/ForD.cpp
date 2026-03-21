#include "ForD.h"
#include "../rendering/Shader.h"
#include "../rendering/Skybox.h"
#include "../rendering/VertexData.h"
#include "../rendering/4d/Mesh4D.h"
#include "../utils/Time.h"
#include "../rendering/Material.h"
#include "../rendering/PointLight.h"

ForD::ForD() {
    myScene.skybox = new Skybox(Skybox::BOX, assetPath + "/skybox");
    Skybox::current = myScene.skybox;
    litShader = std::make_unique<Shader>(
        shaderPath + "/lit.glvs",
        shaderPath + "/lit.glfs");
    geomShader = std::make_unique<Shader>(
        shaderPath + "/geomTest.glvs",
        shaderPath + "/geomTest.glgs",
        shaderPath + "/geomTest.glfs");
    forDShader = std::make_unique<Shader>(
        shaderPath + "/lit4D.glvs",
        shaderPath + "/lit4D.glgs",
        shaderPath + "/lit4D.glfs");
    forDWireShader = std::make_unique<Shader>(
        shaderPath + "/lit4D_wireframe.glvs",
        shaderPath + "/lit4D_wireframe.glfs");

    GameObject* go = new GameObject("Harold");
    myScene.AddObject(*go);
    auto surfCol = glm::vec3(0.9f,0.9f,0.9f);
    copper = std::make_unique<Material>(
        surfCol,
        surfCol,
        surfCol);
    mesh4D = std::make_unique<Mesh4D>();
    // mesh4D->AddTetrahedron(Tetrahedron4D(
    //     {-2.0f, 2.0f, 2.0f, -1.0f},
    //     {-1.2f, -1.5f, -3.5f, -0.7f},
    //     {3.0f, -3.2f, 2.5f, 0.5f},
    //     {-0.5f, 1.5f, -3.5f, 1.0f}));

    mesh4D->AddCuboid(
        {0.0, 0.0, 0.0, -1.0},
        {1.0, 0.0, 0.0, 0.3},
        {0.3, 0.6, 0.0, 0.7},
        {0.2, 0.1, 0.5, 1.0});
    tesseract = Mesh4D::MakeTesseract({-1,-1,-1,-1},
        {2,0,0,0},
        {0,2,0,0},
        {0,0,2,0},
        {0,0,0,2});

    auto* pointLight1 = new GameObject("Point light");
    auto* pointLight2 = new GameObject("Point light");
    glm::vec3 pl1Col = glm::vec3(0.3, 0.3, 1.0);
    glm::vec3 pl2Col = glm::vec3(1.0, 0.8, 0.2);
    pointLight1->AddComponent<PointLight>(pl1Col * 0.1f, pl1Col, pl1Col * 0.7f);
    pointLight2->AddComponent<PointLight>(pl2Col * 0.1f, pl2Col, pl2Col * 0.7f);
    pointLight1->GetTransform()->SetPosition(glm::vec3(-1, 1, 2));
    pointLight2->GetTransform()->SetPosition(glm::vec3(1, 1, -2));

    myScene.AddObject(*pointLight1);
    myScene.AddObject(*pointLight2);

    auto r4dH = go->AddComponent<Renderer4D>(tesseract.get(), forDShader.get());
    // renderer4D = std::make_unique<Renderer4D>(tesseract.get(), forDShader.get());
    // renderer4D->SetSectionPlanePosition(glm::normalize(glm::vec4(0.3,0.5,0.7,1)));
    // myScene.InjectRenderer("4D", [&]() {
    //     renderer4D->Render(ViewProvider::GetCurrent(), glm::mat4(1.0));
    // });
    auto r4d = go->GetComponent<Renderer4D>();
    r4d->wireframeShader = forDWireShader.get();
    r4d->material = copper.get();
    TETRA_USE_EDITOR_INPUT
    input->AddListener (KeyEvent::GetLink(GLFW_PRESS, GLFW_KEY_P), &ForD::PauseToggle, *this);
}

void ForD::Update() {
    Application::Update();
    if (enableAnimation) {
        timer += Time::deltaTime;
        float fac = 1 + 2 * glm::sin(timer);
    }
}

void ForD::PauseToggle(const Event<InputEvent>& ev) {
    enableAnimation = !enableAnimation;
}
