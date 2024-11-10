
#include "scene.h"
#include "terraingen.h"

void Scene::Init()
{

	// SHIP
	shipTexture = gdl::LoadImage("assets/spaceship.png", gdl::TextureFilterModes::Nearest);
    gdl::FBXFile* shipFbx = new gdl::FBXFile();
    shipScene = shipFbx->LoadFile("assets/spaceship.fbx");
    shipScene->SetMaterialTexture("spaceship", shipTexture);

	// BG
	spacebg = gdl::LoadImage("assets/spacebg.png", gdl::TextureFilterModes::Linear);

    // Heightmap
    gdl::PNGFile* terrainPNG = new gdl::PNGFile();
    terrainPNG->ReadFile("assets/testmap.png");
    terrain = TerrainGenerator::GenerateFromPNG(1.0f, 3.0f, terrainPNG);

    heightMap = new gdl::Image();
    heightMap->LoadPNG(terrainPNG, gdl::TextureFilterModes::Nearest);

    delete terrainPNG;

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

    glTranslatef(0.0f, -2.0f, -5.0f);
    glRotatef(gdl::GetElapsedSeconds()* 30.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(gdl::GetElapsedSeconds()* 55.0f, 1.0f, 0.0f, 0.0f);
    //glRotatef(gdl::GetElapsedSeconds()* 25.0f, 0.0f, 0.0f, 1.0f);

    // shipScene->Draw();
    // glScalef(0.1f, 0.1f, 0.1f);
    // Draw the terrain

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, heightMap->GetTextureId());
    glColor3f(1.0f, 1.0f, 1.0f);
    glScalef(1.1f, 1.1f, 1.1f);

    terrain->DrawElements();

    /*
	glDisable(GL_TEXTURE_2D);
    glColor3f(0.2f, 1.0f, 0.2f);
    terrain->DrawLines();

    glColor3f(1.0f, 0.3f, 0.2f);
    terrain->DrawNormals();
    */

    glPopMatrix();


}
