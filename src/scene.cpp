
#include "scene.h"
#include "terraingen.h"
#include "draw3d.h"
#include "starsphere.h"
#include "../rocket/mgdl-rocket.h"
#include "../tracks.h"
#include <glm/gtc/matrix_transform.hpp>
#include <mgdl/mgdl-draw2d.h>
#include <mgdl/mgdl-util.h>
#include <cstring>

#ifndef SYNC_PLAYER

    static ROCKET_TRACK scene;
    static ROCKET_TRACK fade_out;
    static ROCKET_TRACK end_demo;

    static ROCKET_TRACK drawTerrain;
    static ROCKET_TRACK drawSpaceport;


    // Ship hangar doors
    static ROCKET_TRACK door_hangar;
    static ROCKET_TRACK door_hangarX;

    // Runway elevator doors
    static ROCKET_TRACK door_elevator;
    static ROCKET_TRACK door_elevatorX;

    // Platform that moves the ship around
    static ROCKET_TRACK platform_x;
    static ROCKET_TRACK platform_y;
    static ROCKET_TRACK platform_z;
    static ROCKET_TRACK platform_rotY;

    // Moves the danger zone railings in the tunnel up and down
    static ROCKET_TRACK platform_railingY;

    // Ship itself
    static ROCKET_TRACK ship_x;
    static ROCKET_TRACK ship_y;
    static ROCKET_TRACK ship_z;
    static ROCKET_TRACK ship_rotX;
    static ROCKET_TRACK ship_rotY;
    static ROCKET_TRACK ship_rotZ;
    static ROCKET_TRACK ship_scale;
    static ROCKET_TRACK ship_illumination;
    static ROCKET_TRACK ship_detailScale;
    static ROCKET_TRACK ship_gearRotX;
    static ROCKET_TRACK ship_gearRotY;
    static ROCKET_TRACK ship_gearScale;
    static ROCKET_TRACK ship_gearX;
    static ROCKET_TRACK ship_gearY;
    static ROCKET_TRACK ship_gearZ;
    static ROCKET_TRACK ship_gearBetween;
    static ROCKET_TRACK ship_gearFrontBack;


    // Terrain
    static ROCKET_TRACK terrain_x;
    static ROCKET_TRACK terrain_y;
    static ROCKET_TRACK terrain_z;
    static ROCKET_TRACK terrain_scale;
    static ROCKET_TRACK terrain_rotY;


    // The normals need to be recalculated when this changes
    static ROCKET_TRACK terrain_heightMultiplier;

    static ROCKET_TRACK terrain_uvRange;

    // Earth scene
    static ROCKET_TRACK earth_x;
    static ROCKET_TRACK earth_y;
    static ROCKET_TRACK earth_scale;
    static ROCKET_TRACK earth_moonX;
    static ROCKET_TRACK earth_moonY;
    static ROCKET_TRACK earth_moonScale;
    static ROCKET_TRACK earth_moonIllumination;

    // Gate scene
    static ROCKET_TRACK gate_x;
    static ROCKET_TRACK gate_y;
    static ROCKET_TRACK gate_z;
    static ROCKET_TRACK gate_scale;
    static ROCKET_TRACK gate_rotX;
    static ROCKET_TRACK gate_rotY;
    static ROCKET_TRACK gate_rotZ;
    static ROCKET_TRACK gate_ringRot;

    // Credits in the tunnel
    static ROCKET_TRACK credit_x;
    static ROCKET_TRACK credit_y;
    static ROCKET_TRACK credit_z;
    static ROCKET_TRACK credit_rotY;
    static ROCKET_TRACK credit_size;
    static ROCKET_TRACK credit_textSize;
    static ROCKET_TRACK credit_textX;
    static ROCKET_TRACK credit_textY;
    static ROCKET_TRACK credit_between;
    static ROCKET_TRACK credit_index;

#endif
    static float lastTerrainHeightMultiplier = 4.0f;
    static float lastTerrainUVrange = 8.0f;

void Scene::Init()
{

	// SHIP
	shipTexture = gdl::LoadImage("assets/spaceship.png", gdl::TextureFilterModes::Nearest);
	portTexture = gdl::LoadImage("assets/spaceport.png", gdl::TextureFilterModes::Nearest);
	gateTexture = gdl::LoadImage("assets/gate_texture.png", gdl::TextureFilterModes::Nearest);
    matcap = gdl::LoadImage("assets/shinyorb.png", gdl::TextureFilterModes::Linear);


    gdl::FBXFile* spacePortFBX = new gdl::FBXFile();
    spaceportScene = spacePortFBX->LoadFile("assets/spaceport.fbx");
    spaceportScene->SetMaterialTexture("spaceport.png", portTexture);

    gdl::FBXFile* shipFBX = new gdl::FBXFile();
    shipScene = shipFBX->LoadFile("assets/ship_with_uvs.fbx");
    shipScene->SetMaterialTexture("standardSurface1", matcap);
    shipScene->SetMaterialTexture("Material.002", matcap);

    gdl::FBXFile* gateFBX = new gdl::FBXFile();
    gateScene = gateFBX->LoadFile("assets/gate.fbx");
    gateScene->SetMaterialTexture("gate_texture.png", gateTexture);
    gateRingNode = gateScene->GetNode("ring");

    gdl::FBXFile* gearFBX = new gdl::FBXFile();
    gearScene = gearFBX->LoadFile("assets/gear.fbx");
    gearScene->SetMaterialTexture("spaceport.png", portTexture);
    gearNode = gearScene->GetNode("gear")->children[0];


	// BG
	spacebg = gdl::LoadImage("assets/spacebg.png", gdl::TextureFilterModes::Linear);
    earthbg = gdl::LoadImage("assets/earthbg.png", gdl::TextureFilterModes::Linear);
    moonfg = gdl::LoadImage("assets/moonfg.png", gdl::TextureFilterModes::Linear);

    // Terrain
    moonsurface = gdl::LoadImage("assets/moonsurface.png", gdl::TextureFilterModes::Linear);
    // Setup mirroring on the surface
    glBindTexture(GL_TEXTURE_2D, moonsurface->GetTextureId());
    GLint repeatMode = GL_REPEAT;
#ifndef MGDL_PLATFORM_WINDOWS
    // NOTE No MIRRORED_REPEAT on WINDOWS
    repeatMode = GL_MIRRORED_REPEAT;
#endif
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeatMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeatMode);
    glBindTexture(GL_TEXTURE_2D, 0);


    heightMapPNG = new gdl::PNGFile();
    heightMapPNG->ReadFile("assets/testmap.png");
    heightMap = gdl::LoadImage(heightMapPNG, gdl::TextureFilterModes::Nearest);
    terrain = TerrainGenerator::GenerateFromPNG(lastTerrainHeightMultiplier, lastTerrainUVrange, heightMapPNG);
    terrainNode = new gdl::Node("terrain", terrain, new gdl::Material("terrain", moonsurface));
    terrainNode->transform.scale = gdl::vec3(1.0f, 1.0f, 1.0f);

    // Create star sphere

    StarSphere sp;
    gdl::Mesh* stars =sp.CreateSphere(256, 1000.0f, 1.01f, 3.0f);
    gdl::Material* starMat = new gdl::Material("star", gdl::LoadImage("assets/star_texture.png", gdl::Nearest));
    starSphereNode = new gdl::Node("starspehere", stars, starMat);
    starSphereNode->transform.position = gdl::vec3(0.0f, 0.0f, 0.0f);

#ifdef SYNC_PLAYER
    // Delete these when running the demo
    delete heightMapPNG;
    delete heightMap;
#endif


    // Get the nodes that are moved with Rocket
    if (spaceportScene->GetRootNode() != nullptr)
    {

        hangarDoorLeft = spaceportScene->GetNode("door_t1");
        gdl_assert_print(hangarDoorLeft!=nullptr, "No left hangar door");

        hangarDoorRight = spaceportScene->GetNode("door_t2");
        gdl_assert_print(hangarDoorRight!=nullptr, "No right hangar door");

        elevatorDoorLeft = spaceportScene->GetNode("door_e1");
        gdl_assert_print(elevatorDoorLeft!=nullptr, "No left elev door");

        elevatorDoorRight = spaceportScene->GetNode("door_e2");
        gdl_assert_print(elevatorDoorRight!=nullptr, "No right elev door");

        elevatorPlatform = spaceportScene->GetNode("platform");
        gdl_assert_print(elevatorPlatform!=nullptr, "No elev platform");

        tunnelNode = spaceportScene->GetNode("tunnel");
        gdl_assert_print(tunnelNode!=nullptr, "No tunnel mesh");

        hangarNode = spaceportScene->GetNode("terminal");
        gdl_assert_print(hangarNode!=nullptr, "No terminal node");

        railingNodes.push_back(spaceportScene->GetNode("railingO"));
        railingNodes.push_back(spaceportScene->GetNode("railingTE"));
        railingNodes.push_back(spaceportScene->GetNode("railingOE"));
        railingNodes.push_back(spaceportScene->GetNode("railingTT"));
    }
    else
    {
        printf("No root node set!\n");
    }

    //spaceportScene->SetActiveParentNode(spaceportScene->GetRootNode());
    //spaceportScene->PushChildNode(starSphereNode);

    shipNode = shipScene->GetNode("pCylinder5");
    gdl_assert_print(shipNode!=nullptr, "No ship");
    // Connect the ship to the platform
    spaceportScene->SetActiveParentNode(elevatorPlatform);
    spaceportScene->PushChildNode(shipNode);

    // Duplicate ship node
    // ///////////////////////////////////////////////////


    // Duplicate mesh
    gdl::Mesh* shipDuplicate = new gdl::Mesh();

		shipDuplicate->indices = shipNode->mesh->indices;
		shipDuplicate->indexCount = shipNode->mesh->indexCount;
		shipDuplicate->vertexCount = shipNode->mesh->vertexCount;
		shipDuplicate->positions = shipNode->mesh->positions;
		shipDuplicate->normals = shipNode->mesh->normals;

        // Make copy of uvs because the matcap mesh constantly changes them
		shipDuplicate->uvs = new GLfloat[shipDuplicate->vertexCount * 2];
        std::memcpy(shipDuplicate->uvs, shipNode->mesh->uvs, sizeof(GLfloat) * shipDuplicate->vertexCount * 2);

		shipDuplicate->name = std::string("textureShip");
		shipDuplicate->uniqueId = shipNode->mesh->uniqueId;

    texturedShipNode = new gdl::Node();
    texturedShipNode->mesh = shipDuplicate;
    texturedShipNode->name = "texturedShip";
    texturedShipNode->light = nullptr;
    texturedShipNode->children = std::vector<gdl::Node*>();

    gdl::Material* shipMaterial = new gdl::Material();
    shipMaterial->name = std::string("ship");
    shipMaterial->texture = shipTexture;
    shipScene->AddMaterial(shipMaterial);
    texturedShipNode->material = shipMaterial;

    // Set scale
    texturedShipNode->transform = shipNode->transform;
    float small = 0.01f; // TODO: Track this
    texturedShipNode->transform.SetScalef(1.0f + small);

    spaceportScene->SetActiveParentNode(shipNode);
    spaceportScene->PushChildNode(texturedShipNode);


    // Duplicate landing gear
    gdl::Node* wheel = gearNode;
    spaceportScene->PushChildNode(wheel);

    gdl::Mesh* gearMesh = wheel->mesh;
    gearNodeBackLeft = new gdl::Node("leftGear", gearMesh, wheel->material);
    gearNodeBackRight = new gdl::Node("rightGear", gearMesh, wheel->material);
    gearNodeBackLeft->light = nullptr;
    gearNodeBackRight->light = nullptr;
    gearNodeBackLeft->transform.position = wheel->transform.position;
    gearNodeBackRight->transform.position = wheel->transform.position;
    gearNodeBackRight->transform.rotationDegrees.z = -90.0f;
    gearNodeBackLeft->transform.rotationDegrees.z = -90.0f;
    gearNodeBackRight->transform.rotationDegrees.y = -180.0f;
    gearNodeBackLeft->transform.rotationDegrees.y = -180.0f;
    gearNode->transform.rotationDegrees.y = -180.0f;
    spaceportScene->PushChildNode(gearNodeBackLeft);
    spaceportScene->PushChildNode(gearNodeBackRight);




    creditFont = gdl::LoadFontCustom("assets/heavydata_32.png", 16, 16, 8, " !ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    gdl_assert_print(creditFont != nullptr, "Credit font failed to load");



	// Init rendering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glShadeModel(GL_SMOOTH);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Init Rocket
#ifdef GEKKO
    spaceMusic = gdl::LoadOgg("assets/spacemusic.ogg");
#else
    spaceMusic = gdl::LoadSound("assets/spacemusic.wav");
#endif

#ifndef SYNC_PLAYER

    // DEBUG
    debugFont = gdl::LoadFont("assets/font8x16.png", 8, 16, ' ');
    gdl_assert_print(debugFont != nullptr, "Ibm font failed to load");

    DebugMenu = gdl::MenuCreator(debugFont, 1.0f, 1.0);
    TimerMenu = gdl::MenuCreator(debugFont, 1.0f, 1.0f);
#endif











    bool rocketInit = gdl::RocketSync::InitRocket(spaceMusic, 68, 4);
    gdl_assert_print(rocketInit, "Failed to init rocket");
        camera = new Camera();
        departures = new DeparturesScene();
#ifndef SYNC_PLAYER

        scene = gdl::RocketSync::GetTrack("scene");
        fade_out = gdl::RocketSync::GetTrack("fade_out");
        end_demo = gdl::RocketSync::GetTrack("end_demo");

        // Render hacks
        drawTerrain = gdl::RocketSync::GetTrack("drawTerrain");
        drawSpaceport = gdl::RocketSync::GetTrack("drawSpaceport");



        // Doors to hangar
        door_hangar = gdl::RocketSync::GetTrack("door:hangar");
        door_hangarX = gdl::RocketSync::GetTrack("door:hangarX");

        // Runway elevator doors
        door_elevator = gdl::RocketSync::GetTrack("door:elevator");
        door_elevatorX = gdl::RocketSync::GetTrack("door:elevatorX");

        // Platform that moves the ship around
        platform_x = gdl::RocketSync::GetTrack("platform:x");
        platform_y = gdl::RocketSync::GetTrack("platform:y");
        platform_z = gdl::RocketSync::GetTrack("platform:z");
        platform_rotY = gdl::RocketSync::GetTrack("platform:rotY");

        platform_railingY = gdl::RocketSync::GetTrack("platform:railingY");

        // Ship itself
        ship_x = gdl::RocketSync::GetTrack("ship:x");
        ship_y = gdl::RocketSync::GetTrack("ship:y");
        ship_z = gdl::RocketSync::GetTrack("ship:z");
        ship_rotX = gdl::RocketSync::GetTrack("ship:rotX");
        ship_rotY = gdl::RocketSync::GetTrack("ship:rotY");
        ship_rotZ = gdl::RocketSync::GetTrack("ship:rotZ");
        ship_scale = gdl::RocketSync::GetTrack("ship:scale");
        ship_illumination = gdl::RocketSync::GetTrack("ship:illumination");
        ship_detailScale = gdl::RocketSync::GetTrack("ship:detailScale");

        // Landing gear
        ship_gearRotX = gdl::RocketSync::GetTrack("ship:gearRotX");
        ship_gearRotY = gdl::RocketSync::GetTrack("ship:gearRotY");
        ship_gearScale = gdl::RocketSync::GetTrack("ship:gearScale");
        ship_gearX = gdl::RocketSync::GetTrack("ship:gearX");
        ship_gearY = gdl::RocketSync::GetTrack("ship:gearY");
        ship_gearZ = gdl::RocketSync::GetTrack("ship:gearZ");
        ship_gearBetween = gdl::RocketSync::GetTrack("ship:gearBetween");
        ship_gearFrontBack = gdl::RocketSync::GetTrack("ship:gearFrontBack");


        terrain_x = gdl::RocketSync::GetTrack("terrain:x");
        terrain_y = gdl::RocketSync::GetTrack("terrain:y");
        terrain_z = gdl::RocketSync::GetTrack("terrain:z");
        terrain_scale = gdl::RocketSync::GetTrack("terrain:scale");
        terrain_rotY = gdl::RocketSync::GetTrack("terrain:rotY");
        terrain_heightMultiplier = gdl::RocketSync::GetTrack("terrain:heightMult");
        terrain_uvRange = gdl::RocketSync::GetTrack("terrain:uvRange");


        earth_x = gdl::RocketSync::GetTrack("earth:x");
        earth_y = gdl::RocketSync::GetTrack("earth:y");
        earth_scale = gdl::RocketSync::GetTrack("earth:scale");
        earth_moonX = gdl::RocketSync::GetTrack("earth:moonX");
        earth_moonY = gdl::RocketSync::GetTrack("earth:moonY");
        earth_moonScale = gdl::RocketSync::GetTrack("earth:moonScale");
        earth_moonIllumination = gdl::RocketSync::GetTrack("earth:moonIllumination");


        gate_x = gdl::RocketSync::GetTrack("gate:x");
        gate_y = gdl::RocketSync::GetTrack("gate:y");
        gate_z = gdl::RocketSync::GetTrack("gate:z");
        gate_scale = gdl::RocketSync::GetTrack("gate:scale");
        gate_rotX = gdl::RocketSync::GetTrack("gate:rotX");
        gate_rotY = gdl::RocketSync::GetTrack("gate:rotY");
        gate_rotZ = gdl::RocketSync::GetTrack("gate:rotZ");
        gate_ringRot = gdl::RocketSync::GetTrack("gate:ringRot");

        credit_x = gdl::RocketSync::GetTrack("credit:x");
        credit_y = gdl::RocketSync::GetTrack("credit:y");
        credit_z = gdl::RocketSync::GetTrack("credit:z");
        credit_rotY = gdl::RocketSync::GetTrack("credit:rotY");
        credit_size = gdl::RocketSync::GetTrack("credit:size");
        credit_textSize = gdl::RocketSync::GetTrack("credit:textSize");
        credit_textX = gdl::RocketSync::GetTrack("credit:textX");
        credit_textY = gdl::RocketSync::GetTrack("credit:textY");
        credit_between = gdl::RocketSync::GetTrack("credit:between");
        credit_index = gdl::RocketSync::GetTrack("credit:index");
#endif

        gdl::RocketSync::StartSync();
}

void Scene::Update()
{
    gdl::RocketSync::UpdateRow();

    // Move the elevator first
    elevatorPlatform->transform.position.x = gdl::RocketSync::GetFloat(platform_x);
    elevatorPlatform->transform.position.y = gdl::RocketSync::GetFloat(platform_y);
    elevatorPlatform->transform.position.z = gdl::RocketSync::GetFloat(platform_z);
    elevatorPlatform->transform.rotationDegrees.y = gdl::RocketSync::GetFloat(platform_rotY);

    for(size_t i = 0; i < railingNodes.size(); i+=1)
    {
        railingNodes[i]->transform.position.y = gdl::RocketSync::GetFloat(platform_railingY);
    }


    // Offset the ship position with rocket

    shipNode->transform.position.x = gdl::RocketSync::GetFloat(ship_x);
    shipNode->transform.position.y = gdl::RocketSync::GetFloat(ship_y);
    shipNode->transform.position.z = gdl::RocketSync::GetFloat(ship_z);
    shipNode->transform.rotationDegrees.x = gdl::RocketSync::GetFloat(ship_rotX);
    shipNode->transform.rotationDegrees.y = gdl::RocketSync::GetFloat(ship_rotY);
    shipNode->transform.rotationDegrees.z = gdl::RocketSync::GetFloat(ship_rotZ);
    shipNode->transform.SetScalef( gdl::RocketSync::GetFloat(ship_scale));
    texturedShipNode->transform.SetScalef(1.0f + gdl::RocketSync::GetFloat(ship_detailScale));

    gearNode->transform.position.x = gdl::RocketSync::GetFloat(ship_gearX);
    gearNode->transform.position.y = gdl::RocketSync::GetFloat(ship_gearY);
    gearNode->transform.position.z = gdl::RocketSync::GetFloat(ship_gearZ);
    gearNode->transform.rotationDegrees.x = gdl::RocketSync::GetFloat(ship_gearRotX);
    gearNode->transform.rotationDegrees.y = gdl::RocketSync::GetFloat(ship_gearRotY);

    float gscale = 1.0f + gdl::RocketSync::GetFloat(ship_gearScale);
    gearNode->transform.SetScalef(gscale);

    gearNodeBackLeft->transform.position = gearNode->transform.position;
    gearNodeBackLeft->transform.SetScalef(gscale);
    gearNodeBackLeft->transform.position.z += gdl::RocketSync::GetFloat(ship_gearFrontBack);
    gearNodeBackLeft->transform.position.x += gdl::RocketSync::GetFloat(ship_gearBetween);

    gearNodeBackRight->transform.position = gearNode->transform.position;
    gearNodeBackRight->transform.SetScalef(gscale);
    gearNodeBackRight->transform.position.z += gdl::RocketSync::GetFloat(ship_gearFrontBack);
    gearNodeBackRight->transform.position.x -= gdl::RocketSync::GetFloat(ship_gearBetween);

    gdl::vec3 tpos = gdl::vec3(gdl::RocketSync::GetFloat(terrain_x), gdl::RocketSync::GetFloat(terrain_y), gdl::RocketSync::GetFloat(terrain_z));
    terrainNode->transform.position = tpos;
    terrainNode->transform.SetScalef(gdl::RocketSync::GetFloat(terrain_scale));
    terrainNode->transform.rotationDegrees.y = gdl::RocketSync::GetFloat(terrain_rotY);

#ifndef SYNC_PLAYER
    float newHeightMul = gdl::RocketSync::GetFloat(terrain_heightMultiplier);
    if (abs(newHeightMul - lastTerrainHeightMultiplier) > 1.0f)
    {
        float w = heightMap->GetWidth();
        float d = heightMap->GetHeight();
        TerrainGenerator::SetHeightsFromPNG((TerrainMesh*)terrain, heightMapPNG, newHeightMul, w, d);
        TerrainGenerator::CreateNormals((TerrainMesh*)terrain, w, d);
        lastTerrainHeightMultiplier = newHeightMul;
        printf("New terrain height multiplier is %.2f\n", newHeightMul);
    }
    float newUvRange = gdl::RocketSync::GetFloat(terrain_uvRange);
    if (abs(newUvRange - lastTerrainUVrange) > 1.0f)
    {
        float w = heightMap->GetWidth();
        float d = heightMap->GetHeight();
        TerrainGenerator::CreateUVs((TerrainMesh*)terrain, newUvRange, w, d);
        lastTerrainUVrange = newUvRange;
        printf("New terrain uv range is %.2f\n", newHeightMul);
    }
#endif



    float hangar_open = gdl::RocketSync::GetFloat( door_hangar );
    float hangar_offset = gdl::RocketSync::GetFloat( door_hangarX );
    float elevator_open = gdl::RocketSync::GetFloat(door_elevator);
    float elevator_offset =gdl::RocketSync::GetFloat(door_elevatorX);

    hangarDoorLeft->transform.position.x = hangar_open + hangar_offset;
    hangarDoorRight->transform.position.x = hangar_open * -1.0f + hangar_offset;
    elevatorDoorLeft->transform.position.x = elevator_open + elevator_offset;
    elevatorDoorRight->transform.position.x = elevator_open * -1.0f + elevator_offset;


    // Gate
    gdl::Node* gateRoot = gateScene->GetRootNode();
    gateRoot->transform.position = gdl::vec3(
        gdl::RocketSync::GetFloat(gate_x),
        gdl::RocketSync::GetFloat(gate_y),
        gdl::RocketSync::GetFloat(gate_z));
    gateRoot->transform.rotationDegrees = gdl::vec3(
        gdl::RocketSync::GetFloat(gate_rotX),
        gdl::RocketSync::GetFloat(gate_rotY),
        gdl::RocketSync::GetFloat(gate_rotZ));
    gateRoot->transform.SetScalef(gdl::RocketSync::GetFloat(gate_scale));

    DrawScene activeScene = (DrawScene)gdl::RocketSync::GetInt(scene);
    if (activeScene == DrawScene::Gate)
    {
        gateRingNode->transform.rotationDegrees.y += gdl::RocketSync::GetFloat(gate_ringRot) * gdl::GetDeltaTime();
    }



    // Camera update
    // NOTE: Update camera after updating target to avoid jitter
    switch (activeScene)
    {
        case Departures:
        camera->Update(gdl::GetDeltaTime(), gdl::vec3(0,0,0) );
            break;

        default:
            gdl_assert_print(spaceportScene != nullptr && shipNode != nullptr, "No ship or spaceport");
        shipPos = spaceportScene->GetWorldPosition(shipNode);

        camera->Update(gdl::GetDeltaTime(), shipPos );
        break;
    }


    // Update ship matcap

    // TerrainGenerator::CalculateMatcapFromCamera((TerrainMesh*)terrainNode->mesh, camera->GetDirection());
    // TerrainGenerator::CalculateMatcapFromNormals((TerrainMesh*)terrainNode->mesh);


    // DEBUG
#ifndef SYNC_PLAYER
    //NOTE Controller info valid only on update!
    {
        gdl::ControllerVec2 cp = gdl::GetController(0).GetCursorPosition();
        // flip
        float h = gdl::GetScreenHeight();
        cp.yAxis = h-cp.yAxis;
        bool press = gdl::GetController(0).ButtonPress(gdl::WiiButtons::ButtonA);
        DebugMenu.StartMenu(0, gdl::GetScreenHeight(), 100, cp.xAxis, cp.yAxis, press);
        TimerMenu.StartMenu(gdl::GetScreenWidth()-100, gdl::GetScreenHeight(), 100, cp.xAxis, cp.yAxis, press);
    }
#endif
}

void Scene::Draw()
{
    DrawScene activeScene = (DrawScene)gdl::RocketSync::GetInt(scene);
    switch(activeScene)
    {
        case Spaceport:
            DrawSpaceportScene();
            break;

        case Departures:
            departures->Draw(camera, spaceportScene, hangarNode);
            break;


        case Terrain:
            camera->SetupFor3D();
            camera->LookAtTarget();
            spaceportScene->DrawNode(terrainNode);
            shipScene->Draw();
            DrawStars(terrainNode->transform.position);
            break;

        case Earth:
            DrawEarthScene();

            break;

        case Gate:
            camera->SetupFor3D();
            camera->LookAtTarget();
            gateScene->Draw();
            shipScene->Draw();
            DrawStars(camera->currentPosition);

            break;
    }
    gdl::InitOrthoProjection();
    DrawFadeOut();

    // DEBUG
#ifndef SYNC_PLAYER
	glDisable(GL_DEPTH_TEST);
    {
        static bool showScene = false;
        if (DebugMenu.Button("Scenegraph"))
        {
            showScene = !showScene;
        }
        if (showScene)
        {
            spaceportScene->DebugDraw(debugFont, 10, gdl::GetScreenHeight() - 10, 0);
            shipScene->DebugDraw(debugFont, 200, gdl::GetScreenHeight() - 10, gdl::Scene::DebugFlag::Position);
        }
    }
    {
        static bool debugCamera = false;
        if (DebugMenu.Button("Camera"))
        {
            debugCamera = !debugCamera;
        }
        if (debugCamera)
        {
            camera->DebugDraw(gdl::GetScreenWidth()/2, gdl::GetScreenHeight()-gdl::GetScreenHeight()/2, debugFont);
            DrawThirdsGuides();
        }
    }
    {
        static bool debugTiming = false;
        if (DebugMenu.Button("Timing"))
        {
            debugTiming = !debugTiming;
        }
        if (debugTiming)
        {
            DebugDrawTiming();
        }
    }
	glEnable(GL_DEPTH_TEST);
#endif
}


void Scene::DrawBG ( gdl::Image* bgImage, float xoffset, float yoffset, float scale)
{
	// Draw Full screen bg
	glDisable(GL_DEPTH_TEST);
	gdl::InitOrthoProjection();
    bgImage->Draw2DAligned(gdl::GetScreenWidth()/2 + xoffset, gdl::GetScreenHeight()/2 + yoffset, scale, gdl::Centered, gdl::Centered);
    glEnable(GL_DEPTH_TEST);
}

void Scene::DrawEarthScene()
{
    camera->SetupFor3D();
    camera->LookAtTarget();
    DrawStars(camera->currentPosition);

    DrawBG(earthbg, gdl::RocketSync::GetFloat(earth_x),
           gdl::RocketSync::GetFloat(earth_y),
           gdl::RocketSync::GetFloat(earth_scale));

    float mi = gdl::RocketSync::GetFloat(earth_moonIllumination);
    moonfg->SetTint(mi, mi, mi);
    glEnable(GL_BLEND);
    DrawBG(moonfg, gdl::RocketSync::GetFloat(earth_moonX),
           gdl::RocketSync::GetFloat(earth_moonY),
           gdl::RocketSync::GetFloat(earth_moonScale));
    glDisable(GL_BLEND);


    camera->SetupFor3D();
    camera->LookAtTarget();
    glPushMatrix();
        shipScene->Draw();
        glColor3f(1.0f, 1.0f, 1.0f);
    glPopMatrix();
}

void Scene::DrawSpaceportScene()
{
    // DrawBG(spacebg, 0.0f, 0.0f, 20.0f);

    glDepthFunc(GL_LEQUAL);
    camera->SetupFor3D();
    camera->LookAtTarget();

    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();

        if (gdl::RocketSync::GetBool(drawTerrain))
        {
            spaceportScene->DrawNode(terrainNode);
        }

        if (gdl::RocketSync::GetBool(drawSpaceport))
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                spaceportScene->Draw();
            glDisable(GL_BLEND);
        }

    glPopMatrix();
    DrawCredits();
    DrawStars(camera->currentPosition);
}

void Scene::DrawStars(const gdl::vec3& center)
{
    gdl::Node* node = starSphereNode;
	glPushMatrix();

    glTranslatef(center.x, center.y, center.z);

    glColor3f(1.0f, 1.0f, 1.0f);
		gdl::Mesh* m = node->mesh;
		if (m != nullptr)
		{
			if (node->material != nullptr)
			{
				// TODO do not reapply same material
				node->material->Apply();
			}
			m->DrawElements();
		}
	glPopMatrix();
}

void Scene::DrawCredits()
{
    gdl::vec3 tunnelPos = spaceportScene->GetWorldPosition(tunnelNode);
    float xoff = gdl::RocketSync::GetFloat(credit_x);
    float yoff = gdl::RocketSync::GetFloat(credit_y);
    float zoff = gdl::RocketSync::GetFloat(credit_z);
    float rotY = gdl::RocketSync::GetFloat(credit_rotY);
    float sz = gdl::RocketSync::GetFloat(credit_size);
    float tsz = 1.0f + gdl::RocketSync::GetFloat(credit_textSize);
    float btw = gdl::RocketSync::GetFloat(credit_between);
    float tx = gdl::RocketSync::GetFloat(credit_textX);
    float ty = gdl::RocketSync::GetFloat(credit_textY);

    u32 custard = TO_RGBA(0xEC, 0xD5, 0x81, 0xFF);
    u32 rose = TO_RGBA(0xFF, 0x99, 0x88, 0xFF);
    u32 violet = TO_RGBA(0x7C, 0x7E, 0xC0, 0xFF);
    u32 green = TO_RGBA(0x1B, 0xBB, 0x9B, 0xFF);
    u32 fuchsia = TO_RGBA(0xC7, 0x23, 0x53, 0xFF);
    u32 blue = TO_RGBA(0x00, 0xe2, 0xff, 0xff);


    static const u32 colorsArray[] = { rose, violet, custard, green, fuchsia, blue};


    static const std::string creditsArray[] = {
        "WE EMPLOY\nMUFFINTRAP\nENGINEERING\nSOLUTIONS",   // 0
        "BUILD AT\nRACCOON\nVIOLET\nSHIP YARDS",        // 1
        "CONVINCING\nMATERIALS\nTRUST\nMUFFIN",                  // 2
        "ENJOY\nVURPO\nSOUND\nSCAPES",                         // 3
        "CALL\nTURUMORE\nASMR\nFOR SLEEP",              // 4
        "IJORO\nPAINTS ARE\nGOOD FOR\nENVIRONMENT"};    // 5

    static const size_t creditAmount = 6;

    size_t index = static_cast<size_t>(gdl::RocketSync::GetFloat(credit_index));
    if (index >= creditAmount)
    {
        index = 0;
    }

    glPushMatrix();

        glTranslatef(xoff, yoff, zoff);
        glTranslatef(tunnelPos.x, tunnelPos.y, tunnelPos.z);
        glRotatef(rotY, 0.0f, 1.0f, 0.0f);

        // Background quad for text???
        /*
        glPushMatrix();
            glColor3f(1.0f, 0.0f, 0.0f);
            glScalef(1.0f, 1.0f, 0.1f);
            for (size_t i = 0; i < 5; i++)
            {
                SolidCube(sz);
                glTranslatef(btw, 0.0f, 0.0f);
            }
        glPopMatrix();
        */

        glTranslatef(0.0f, 0.0f, sz* 0.25f);
        glPushMatrix();
            for (size_t i = 0; i < 10; i++)
            {
                const std::string& credit = creditsArray[index%creditAmount];
                // Text out of cube
                creditFont->Print(colorsArray[index%creditAmount], tx, ty, tsz, gdl::LJustify, gdl::LJustify, credit.c_str());

                glTranslatef(btw, 0.0f, 0.0f);
            }
        glPopMatrix();
    glPopMatrix();
}

void Scene::SaveTracks()
{
    gdl::RocketSync::SaveAllTracks();
}

bool Scene::ShouldQuit()
{
    bool quitquit = gdl::RocketSync::GetBool(end_demo);
    if (quitquit)
    {
        printf("Demo ends now!");
    }
    return quitquit;
}


void Scene::DrawFadeOut()
{
	float fade = gdl::RocketSync::GetFloat(fade_out);
	if (fade > 0.0f)
	{
		float W = gdl::GetScreenWidth();
		float H = gdl::GetScreenHeight();

        glDepthFunc(GL_ALWAYS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glColor4f(0.0f, 0.0f, 0.0f, fade);
		glBegin(GL_QUADS);
			glVertex2f(0, 0);
			glVertex2f(W, 0);
			glVertex2f(W, H);
			glVertex2f(0, H);
		glEnd();
		glDisable(GL_BLEND);
        glDepthFunc(GL_LEQUAL);
	}
}

void Scene::DrawThirdsGuides()
{

	float third_w = 640.0f/3.0f;
	float third_h = 480.0f/3.0f;

	glBegin(GL_LINES);
	glColor4f(0.7f, 0.7f, 0.7f, 0.5f);
	for (int w = 1; w <= 3; w++)
	{
		glVertex2f(w*third_w, gdl::GetScreenHeight());
		glVertex2f(w*third_w, 0.0f);
	}
	for (int h = 1; h <= 3; h++)
	{
		glVertex2f(0.0f, h*third_h);
		glVertex2f(gdl::GetScreenWidth(), h*third_h);
	}
	glEnd();
}

void Scene::DebugDrawTiming()
{
    int x = 0;
    int h = debugFont->GetCharacterHeight();
    int y = h;

    short sw = gdl::GetScreenWidth();
    float progress = spaceMusic->GetElapsedSeconds() / (274.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    //gdl::DrawBoxF(x, y, sw, h, gdl::Colors::Black);
    short p25 = x + sw/4;
    short p50 = x + sw/2;
    short p75 = x + +sw/2 + sw/4;
    gdl::DrawBoxF(p25,         y,                 p25+2, y-h, gdl::Colors::White);
    gdl::DrawBoxF(p50,         y,                 p50+2, y-h, gdl::Colors::White);
    gdl::DrawBoxF(p75,  y,                 p75+2, y-h, gdl::Colors::White);

    // Progress bar
    gdl::DrawBoxF(x,                y-2, sw * progress, y-h+2, gdl::Colors::White);
    debugFont->Printf(gdl::Colors::Green, x+4, y+h, debugFont->GetCharacterHeight(), gdl::LJustify, gdl::LJustify, "Elapsed %.1f", spaceMusic->GetElapsedSeconds());
    glColor3f(1.0f, 1.0f, 1.0f);

    // Allow changing the time
    char timeString[10];
    for (int i = 0; i < 27; i ++)
    {
        std::string time;
        snprintf(timeString, 10, "%d0s", i);
        if (TimerMenu.Button(timeString))
        {
            gdl::RocketSync::SetSeconds((float)i * 10.0f);
        }
    }
}

