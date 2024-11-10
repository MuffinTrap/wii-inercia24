
#include "scene.h"
#include "terraingen.h"

void Scene::Init()
{

	// SHIP
	shipTexture = gdl::LoadImage("assets/spaceship.png", gdl::TextureFilterModes::Nearest);
    gdl::FBXFile* shipFbx = new gdl::FBXFile();
    shipScene = shipFbx->LoadFile("assets/spaceship.fbx");
    shipScene->SetMaterialTexture("spaceship.png", shipTexture);

	// BG
	spacebg = gdl::LoadImage("assets/spacebg.png", gdl::TextureFilterModes::Linear);

    // Heightmap
    gdl::PNGFile* terrainPNG = new gdl::PNGFile();
    terrainPNG->ReadFile("assets/testmap.png");
    terrain = TerrainGenerator::GenerateFromPNG(256.0f, 256.0f, terrainPNG);
    heightMap = new gdl::Image();
    heightMap->LoadPNG(terrainPNG, gdl::TextureFilterModes::Linear);

    // Make terrain part of the ship scene
    shipScene->SetActiveParentNode(shipScene->GetRootNode());
    gdl::Node* terrainNode = new gdl::Node("terrain", terrain, new gdl::Material("terrain", heightMap));
    terrainNode->transform.scale = 1.0f;
    terrainNode->transform.Translate(gdl::vec3(-100.0f, -4.0f, -300.0f));
    shipScene->PushChildNode(terrainNode);
    delete terrainPNG;



    debugFont = gdl::LoadFont("assets/font8x16.png", 8, 16, ' ');

	// Init rendering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glShadeModel(GL_FLAT);
}

void Scene::Draw()
{
	// Draw Full screen bg
	glDisable(GL_DEPTH_TEST);
	gdl::InitOrthoProjection();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	spacebg->Draw2DAbsolute(-1, -1, gdl::GetScreenWidth()+1, gdl::GetScreenHeight()+1);

    glEnable(GL_DEPTH_TEST);
	// Draw ship
	gdl::InitPerspectiveProjection(75.0f, 0.1f, 500.0f);
    gdl::InitCamera(gdl::vec3(0.0f, 0.0f, 0.0f), gdl::vec3(0.0f, 0.0f, -1.0f), gdl::vec3(0.0f, 1.0f, 0.0f));
    glPushMatrix();

    glTranslatef(0.0f, -4.5f, -48.0f);
    glRotatef(gdl::GetElapsedSeconds()* 5.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(gdl::GetElapsedSeconds()* 10.0f, 0.0f, 1.0f, 0.0f);
    //glRotatef(gdl::GetElapsedSeconds()* 25.0f, 0.0f, 0.0f, 1.0f);
    glScalef(0.1f, 0.1f, 0.1f);

    shipScene->Draw();

    glPopMatrix();

    glDisable(GL_DEPTH_TEST);
    gdl::InitOrthoProjection();
    shipScene->DebugDraw(debugFont, 10, gdl::GetScreenHeight() - 10);
}
