
#include "scene.h"
#include "terraingen.h"
#include "../rocket/mgdl-rocket.h"
#include "../tracks.h"

#ifndef SYNC_PLAYER
    static ROCKET_TRACK camera_x;
    static ROCKET_TRACK camera_y;
    static ROCKET_TRACK camera_z;
    static ROCKET_TRACK camera_orbit_on;
    static ROCKET_TRACK camera_orbit_x;
    static ROCKET_TRACK camera_orbit_y;
    static ROCKET_TRACK camera_orbit_dist;
#endif

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
    if (shipScene->GetRootNode() != nullptr)
    {
        shipNode = shipScene->GetNode("spaceship");
        if (shipNode == nullptr)
        {
            printf("No spaceship node!\n");
        }
    }
    else
    {
        printf("No root node set!\n");
    }




    delete terrainPNG;



    debugFont = gdl::LoadFont("assets/font8x16.png", 8, 16, ' ');

    camera = new Camera();

	// Init rendering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glShadeModel(GL_FLAT);

    // Init Rocket
#ifdef GEKKO
    spaceMusic = gdl::LoadOgg("assets/spacemusic.ogg");
#else
    spaceMusic = gdl::LoadSound("assets/spacemusic.wav");
#endif
    bool rocketInit = gdl::RocketSync::InitRocket(spaceMusic, 72, 4);
    if (rocketInit)
    {
#ifndef SYNC_PLAYER
        camera_x = gdl::RocketSync::GetTrack("camera:x");
        camera_y = gdl::RocketSync::GetTrack("camera:y");
        camera_z = gdl::RocketSync::GetTrack("camera:z");
        camera_orbit_on = gdl::RocketSync::GetTrack("camera:orbitON");
        camera_orbit_x = gdl::RocketSync::GetTrack("camera:orbitX");
        camera_orbit_y = gdl::RocketSync::GetTrack("camera:orbitY");
        camera_orbit_dist = gdl::RocketSync::GetTrack("camera:orbitDistance");
#endif

        gdl::RocketSync::StartSync();
    }
}

void Scene::Update()
{
    gdl::RocketSync::UpdateRow();

    if (gdl::RocketSync::GetBool(camera_orbit_on))
    {
        if (shipNode != nullptr)
        {
            // TODO: Calculate actual position from parent nodes
            camera->target = shipNode->transform.position;
        }
        // Offsets to the target
        camera->target.x += gdl::RocketSync::GetFloat(camera_x);
        camera->target.y += gdl::RocketSync::GetFloat(camera_y);
        camera->target.z += gdl::RocketSync::GetFloat(camera_z);

        camera->Orbit(gdl::RocketSync::GetFloat(camera_orbit_y),
                      gdl::RocketSync::GetFloat(camera_orbit_x),
                      gdl::RocketSync::GetFloat(camera_orbit_dist));

    }
    else
    {
        camera->position.x = gdl::RocketSync::GetFloat(camera_x);
        camera->position.y = gdl::RocketSync::GetFloat(camera_y);
        camera->position.z = gdl::RocketSync::GetFloat(camera_z);
    }

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

    camera->LookAt();

    glPushMatrix();

    shipScene->Draw();

    glPopMatrix();

    glDisable(GL_DEPTH_TEST);
    gdl::InitOrthoProjection();
    shipScene->DebugDraw(debugFont, 10, gdl::GetScreenHeight() - 10);
    camera->DebugDraw(10, gdl::GetScreenHeight()-gdl::GetScreenHeight()/2, debugFont);
}
void Scene::SaveTracks()
{
    gdl::RocketSync::SaveAllTracks();
}

void Scene::DebugDraw3DSpace(float sideLength)
{
    glBegin(GL_LINES);
    // ORIGO
        glColor3f(0.2f, 1.0f, 0.2f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, sideLength/2.0f, 0.0f);

        glColor3f(1.0f, 0.2f, 0.2f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(sideLength/2.0f, 0.0f, 0.0f);

        glColor3f(0.2f, 0.2f, 1.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, sideLength/2.0f);

    // BOX
        glColor3f(0.7f, 0.7f, 0.7f);
        glVertex3f(sideLength/2.0f, 0.0f, sideLength/2.0f);
        glVertex3f(-sideLength/2.0f, 0.0f, sideLength/2.0f);

        glVertex3f(-sideLength/2.0f, 0.0f, sideLength/2.0f);
        glVertex3f(-sideLength/2.0f, 0.0f, -sideLength/2.0f);

        glVertex3f(-sideLength/2.0f, 0.0f, -sideLength/2.0f);
        glVertex3f(sideLength/2.0f, 0.0f, -sideLength/2.0f);

        glVertex3f(sideLength/2.0f, sideLength/2.0f, sideLength/2.0f);
        glVertex3f(-sideLength/2.0f, sideLength/2.0f, sideLength/2.0f);

        glVertex3f(-sideLength/2.0f, sideLength/2.0f, sideLength/2.0f);
        glVertex3f(-sideLength/2.0f, sideLength/2.0f, -sideLength/2.0f);

        glVertex3f(-sideLength/2.0f, sideLength/2.0f, -sideLength/2.0f);
        glVertex3f(sideLength/2.0f, sideLength/2.0f, -sideLength/2.0f);
    glEnd();
}

