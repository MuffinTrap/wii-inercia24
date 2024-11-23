
#include "scene.h"
#include "terraingen.h"
#include "../rocket/mgdl-rocket.h"
#include "../tracks.h"
#include <glm/gtc/matrix_transform.hpp>

#ifndef SYNC_PLAYER
    static ROCKET_TRACK camera_x;
    static ROCKET_TRACK camera_y;
    static ROCKET_TRACK camera_z;
    static ROCKET_TRACK camera_orbit_on;
    static ROCKET_TRACK camera_orbit_x;
    static ROCKET_TRACK camera_orbit_y;
    static ROCKET_TRACK camera_orbit_dist;


    // Ship hangar doors
    static ROCKET_TRACK door_hangar;
    static ROCKET_TRACK door_hangar_x;

    // Runway elevator doors
    static ROCKET_TRACK door_elevator;
    static ROCKET_TRACK door_elevator_x;

    // Platform that moves the ship around
    static ROCKET_TRACK platform_x;
    static ROCKET_TRACK platform_y;
    static ROCKET_TRACK platform_z;
    static ROCKET_TRACK platform_rotY;

    // Ship itself
    static ROCKET_TRACK ship_x;
    static ROCKET_TRACK ship_y;
    static ROCKET_TRACK ship_z;
    static ROCKET_TRACK ship_rotX;
    static ROCKET_TRACK ship_rotY;
    static ROCKET_TRACK ship_rotZ;
#endif

void Scene::Init()
{

	// SHIP
	shipTexture = gdl::LoadImage("assets/spaceship.png", gdl::TextureFilterModes::Nearest);
	portTexture = gdl::LoadImage("assets/spaceport.png", gdl::TextureFilterModes::Nearest);
    gdl::FBXFile* shipFbx = new gdl::FBXFile();
    shipScene = shipFbx->LoadFile("assets/spaceship.fbx");
    shipScene->SetMaterialTexture("spaceship.png", shipTexture);
    shipScene->SetMaterialTexture("spaceport.png", portTexture);

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



    // Get the nodes that are moved with Rocket
    if (shipScene->GetRootNode() != nullptr)
    {
        shipNode = shipScene->GetNode("spaceship");
        gdl_assert_print(shipNode!=nullptr, "No ship");

        hangarDoorLeft = shipScene->GetNode("door_2");
        gdl_assert_print(hangarDoorLeft!=nullptr, "No left hangar door");
        //testmap
        hangarDoorLeft->transform.position.x = 0.0f;

        hangarDoorRight = shipScene->GetNode("door_3");
        gdl_assert_print(hangarDoorRight!=nullptr, "No right hangar door");

        elevatorDoorLeft = shipScene->GetNode("door");
        gdl_assert_print(elevatorDoorLeft!=nullptr, "No left elev door");

        elevatorDoorRight = shipScene->GetNode("door_1");
        gdl_assert_print(elevatorDoorRight!=nullptr, "No right elev door");

        elevatorPlatform = shipScene->GetNode("platform");
        gdl_assert_print(elevatorPlatform!=nullptr, "No elev platform");
    }
    else
    {
        printf("No root node set!\n");
    }




    delete terrainPNG;





    debugFont = gdl::LoadFont("assets/font8x16.png", 8, 16, ' ');

    camera = new Camera();
    camera->lerpSpeed = 1.0f;

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

        // Doors to hangar
        door_hangar = gdl::RocketSync::GetTrack("door:hangar");
        door_hangar_x = gdl::RocketSync::GetTrack("door:hangarX");

        // Runway elevator doors
        door_elevator = gdl::RocketSync::GetTrack("door:elevator");
        door_elevator_x = gdl::RocketSync::GetTrack("door:elevatorX");

        // Platform that moves the ship around
        platform_x = gdl::RocketSync::GetTrack("platform:x");
        platform_y = gdl::RocketSync::GetTrack("platform:y");
        platform_z = gdl::RocketSync::GetTrack("platform:z");
        platform_rotY = gdl::RocketSync::GetTrack("platform:rotY");

        // Ship itself
        ship_x = gdl::RocketSync::GetTrack("ship:x");
        ship_y = gdl::RocketSync::GetTrack("ship:y");
        ship_z = gdl::RocketSync::GetTrack("ship:z");
        ship_rotX = gdl::RocketSync::GetTrack("ship:rotX");
        ship_rotY = gdl::RocketSync::GetTrack("ship:rotY");
        ship_rotZ = gdl::RocketSync::GetTrack("ship:rotZ");
#endif

        gdl::RocketSync::StartSync();
    }
}

void Scene::Update()
{
    gdl::RocketSync::UpdateRow();


    shipNode->transform.position.x = gdl::RocketSync::GetFloat(ship_x);
    shipNode->transform.position.y = gdl::RocketSync::GetFloat(ship_y);
    shipNode->transform.position.z = gdl::RocketSync::GetFloat(ship_z);
    shipNode->transform.rotationDegrees.x = gdl::RocketSync::GetFloat(ship_rotX);
    shipNode->transform.rotationDegrees.y = gdl::RocketSync::GetFloat(ship_rotY);
    shipNode->transform.rotationDegrees.z = gdl::RocketSync::GetFloat(ship_rotZ);

    elevatorPlatform->transform.position.x = gdl::RocketSync::GetFloat(platform_x);
    elevatorPlatform->transform.position.y = gdl::RocketSync::GetFloat(platform_y);
    elevatorPlatform->transform.position.z = gdl::RocketSync::GetFloat(platform_z);
    elevatorPlatform->transform.rotationDegrees.y = gdl::RocketSync::GetFloat(platform_rotY);
    float hangar_open = gdl::RocketSync::GetFloat( door_hangar );
    float hangar_offset = gdl::RocketSync::GetFloat( door_hangar_x );
    float elevator_open = gdl::RocketSync::GetFloat(door_elevator);
    float elevator_offset =gdl::RocketSync::GetFloat(door_elevator_x);

    hangarDoorLeft->transform.position.x = hangar_open + hangar_offset;
    hangarDoorRight->transform.position.x = hangar_open * -1.0f + hangar_offset;
    elevatorDoorLeft->transform.position.x = elevator_open + elevator_offset;
    elevatorDoorRight->transform.position.x = elevator_open * -1.0f + elevator_offset;

    // NOTE: Update camera after updating target to avoid jitter
    if (gdl::RocketSync::GetBool(camera_orbit_on))
    {

        gdl::vec3 shipPos = shipScene->GetWorldPosition(shipNode);
        camera->target = shipPos;

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
    camera->Update(gdl::GetDeltaTime());

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
	gdl::InitPerspectiveProjection(75.0f, 0.1f, 1000.0f);

    camera->LookAt();

    glPushMatrix();

    shipScene->Draw();

    glPopMatrix();

    glDisable(GL_DEPTH_TEST);
    gdl::InitOrthoProjection();
    shipScene->DebugDraw(debugFont, 10, gdl::GetScreenHeight() - 10, gdl::Scene::DebugFlag::Position);
    camera->DebugDraw(gdl::GetScreenWidth()/2, gdl::GetScreenHeight()-gdl::GetScreenHeight()/2, debugFont);
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

