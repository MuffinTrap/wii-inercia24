
#include "scene.h"
#include "terraingen.h"
#include "../rocket/mgdl-rocket.h"
#include "../tracks.h"
#include <glm/gtc/matrix_transform.hpp>
#include <mgdl/mgdl-draw2d.h>

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

    // Ship itself
    static ROCKET_TRACK ship_x;
    static ROCKET_TRACK ship_y;
    static ROCKET_TRACK ship_z;
    static ROCKET_TRACK ship_rotX;
    static ROCKET_TRACK ship_rotY;
    static ROCKET_TRACK ship_rotZ;
    static ROCKET_TRACK ship_scale;
    static ROCKET_TRACK ship_illumination;

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

#endif
    static float lastTerrainHeightMultiplier = 2.0f;
    static float lastTerrainUVrange = 1.0f;

void Scene::Init()
{

	// SHIP
	shipTexture = gdl::LoadImage("assets/spaceship.png", gdl::TextureFilterModes::Nearest);
	portTexture = gdl::LoadImage("assets/spaceport.png", gdl::TextureFilterModes::Nearest);
	gateTexture = gdl::LoadImage("assets/gate_texture.png", gdl::TextureFilterModes::Nearest);

    gdl::FBXFile* spacePortFBX = new gdl::FBXFile();
    spaceportScene = spacePortFBX->LoadFile("assets/spaceport.fbx");
    spaceportScene->SetMaterialTexture("spaceport.png", portTexture);


    gdl::FBXFile* shipFBX = new gdl::FBXFile();
    shipScene = shipFBX->LoadFile("assets/ship.fbx");
    shipScene->SetMaterialTexture("standardSurface1", shipTexture);

    gdl::FBXFile* gateFBX = new gdl::FBXFile();
    gateScene = gateFBX->LoadFile("assets/gate.fbx");
    gateScene->SetMaterialTexture("gate_texture.png", gateTexture);


	// BG
	spacebg = gdl::LoadImage("assets/spacebg.png", gdl::TextureFilterModes::Linear);
    earthbg = gdl::LoadImage("assets/earthbg.png", gdl::TextureFilterModes::Linear);
    moonfg = gdl::LoadImage("assets/moonfg.png", gdl::TextureFilterModes::Linear);

    // Terrain
    moonsurface = gdl::LoadImage("assets/moonsurface.png", gdl::TextureFilterModes::Linear);
    // Setup mirroring on the surface
    glBindTexture(GL_TEXTURE_2D, moonsurface->GetTextureId());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // NOTE No MIRRORED_REPEAT on WINDOWS
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

    matcap = gdl::LoadImage("assets/shinyorb.png", gdl::TextureFilterModes::Linear);

    heightMapPNG = new gdl::PNGFile();
    heightMapPNG->ReadFile("assets/testmap.png");
    heightMap = gdl::LoadImage(heightMapPNG, gdl::TextureFilterModes::Nearest);
    terrain = TerrainGenerator::GenerateFromPNG(lastTerrainHeightMultiplier, lastTerrainUVrange, heightMapPNG);
    terrainNode = new gdl::Node("terrain", terrain, new gdl::Material("terrain", moonsurface));
    terrainNode->transform.scale = gdl::vec3(1.0f, 1.0f, 1.0f);

#ifdef SYNC_PLAYER
    // Keep this around in PC version
    delete heightMapPNG;
    delete heightMap;
#endif


    // Get the nodes that are moved with Rocket
    if (spaceportScene->GetRootNode() != nullptr)
    {

        hangarDoorLeft = spaceportScene->GetNode("door_2");
        gdl_assert_print(hangarDoorLeft!=nullptr, "No left hangar door");
        //testmap
        hangarDoorLeft->transform.position.x = 0.0f;

        hangarDoorRight = spaceportScene->GetNode("door_3");
        gdl_assert_print(hangarDoorRight!=nullptr, "No right hangar door");

        elevatorDoorLeft = spaceportScene->GetNode("door");
        gdl_assert_print(elevatorDoorLeft!=nullptr, "No left elev door");

        elevatorDoorRight = spaceportScene->GetNode("door_1");
        gdl_assert_print(elevatorDoorRight!=nullptr, "No right elev door");

        elevatorPlatform = spaceportScene->GetNode("platform");
        gdl_assert_print(elevatorPlatform!=nullptr, "No elev platform");
    }
    else
    {
        printf("No root node set!\n");
    }

    gateRingNode = gateScene->GetNode("ring");

    shipNode = shipScene->GetNode("pCylinder5");
    gdl_assert_print(shipNode!=nullptr, "No ship");

    // Connect the ship to the platform
    spaceportScene->SetActiveParentNode(elevatorPlatform);
    spaceportScene->PushChildNode(shipNode);





    debugFont = gdl::LoadFont("assets/font8x16.png", 8, 16, ' ');
    gdl_assert_print(debugFont != nullptr, "Debug font failed to load");

    DebugMenu = gdl::MenuCreator(debugFont, 1.0f, 1.0);

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
    bool rocketInit = gdl::RocketSync::InitRocket(spaceMusic, 68, 4);
    if (rocketInit)
    {
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

        // Ship itself
        ship_x = gdl::RocketSync::GetTrack("ship:x");
        ship_y = gdl::RocketSync::GetTrack("ship:y");
        ship_z = gdl::RocketSync::GetTrack("ship:z");
        ship_rotX = gdl::RocketSync::GetTrack("ship:rotX");
        ship_rotY = gdl::RocketSync::GetTrack("ship:rotY");
        ship_rotZ = gdl::RocketSync::GetTrack("ship:rotZ");
        ship_scale = gdl::RocketSync::GetTrack("ship:scale");
        ship_illumination = gdl::RocketSync::GetTrack("ship:illumination");


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
#endif

        gdl::RocketSync::StartSync();
    }
}

void Scene::Update()
{
    gdl::RocketSync::UpdateRow();

    // Move the elevator first
    elevatorPlatform->transform.position.x = gdl::RocketSync::GetFloat(platform_x);
    elevatorPlatform->transform.position.y = gdl::RocketSync::GetFloat(platform_y);
    elevatorPlatform->transform.position.z = gdl::RocketSync::GetFloat(platform_z);
    elevatorPlatform->transform.rotationDegrees.y = gdl::RocketSync::GetFloat(platform_rotY);


    // Offset the ship position with rocket

    shipNode->transform.position.x = gdl::RocketSync::GetFloat(ship_x);
    shipNode->transform.position.y = gdl::RocketSync::GetFloat(ship_y);
    shipNode->transform.position.z = gdl::RocketSync::GetFloat(ship_z);
    shipNode->transform.rotationDegrees.x = gdl::RocketSync::GetFloat(ship_rotX);
    shipNode->transform.rotationDegrees.y = gdl::RocketSync::GetFloat(ship_rotY);
    shipNode->transform.rotationDegrees.z = gdl::RocketSync::GetFloat(ship_rotZ);
    shipNode->transform.SetScalef( gdl::RocketSync::GetFloat(ship_scale));

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
        gdl::vec3 shipPos = spaceportScene->GetWorldPosition(shipNode);
        camera->Update(gdl::GetDeltaTime(), shipPos );
        break;
    }


    // TerrainGenerator::CalculateMatcapFromCamera((TerrainMesh*)terrainNode->mesh, camera->GetDirection());
    // TerrainGenerator::CalculateMatcapFromNormals((TerrainMesh*)terrainNode->mesh);


#ifndef SYNC_PLAYER
    //NOTE Controller info valid only on update!
    {
        gdl::ControllerVec2 cp = gdl::GetController(0).GetCursorPosition();
        // flip
        float h = gdl::GetScreenHeight();
        cp.yAxis = h-cp.yAxis;
        bool press = gdl::GetController(0).ButtonPress(gdl::WiiButtons::ButtonA);
        DebugMenu.StartMenu(0, gdl::GetScreenHeight(), 100, cp.xAxis, cp.yAxis, press);
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
            departures->Draw(camera);
            break;


        case Terrain:
            camera->SetupFor3D();
            camera->LookAtTarget();
            spaceportScene->DrawNode(terrainNode);
            shipScene->Draw();
            break;

        case Earth:
            DrawEarthScene();

            break;

        case Gate:
            camera->SetupFor3D();
            camera->LookAtTarget();
            gateScene->Draw();
            shipScene->Draw();

            break;
    }
    gdl::InitOrthoProjection();
    DrawFadeOut();

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
            spaceportScene->DebugDraw(debugFont, 10, gdl::GetScreenHeight() - 10, gdl::Scene::DebugFlag::Position);
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


    // glEnable(GL_DEPTH_TEST);
    camera->SetupFor3D();
    camera->LookAtTarget();
    glPushMatrix();
        shipScene->Draw();
        glColor3f(1.0f, 1.0f, 1.0f);
    glPopMatrix();
    // glDisable(GL_DEPTH_TEST);
}

void Scene::DrawSpaceportScene()
{
    DrawBG(spacebg, 0.0f, 0.0f, 20.0f);

    // glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    camera->SetupFor3D();
    camera->LookAtTarget();

    glPushMatrix();

        if (gdl::RocketSync::GetBool(drawTerrain))
        {
            spaceportScene->DrawNode(terrainNode);
            // DrawTerrainNode(terrainNode);
        }

        if (gdl::RocketSync::GetBool(drawSpaceport))
        {
           spaceportScene->Draw();
        }
        //shipScene->DrawNode(shipNode);

    glPopMatrix();

    // glDisable(GL_DEPTH_TEST);
}

// This was to test the matcap by drawing rgb values
void Scene::DrawTerrainNode ( gdl::Node* node )
{
    glShadeModel(GL_SMOOTH);
	gdl::vec3 t = node->transform.position;
	glPushMatrix();
		glTranslatef(t.x, t.y, t.z);

		gdl::vec3 r = node->transform.rotationDegrees;
		glRotatef(r.x, 1.0f, 0.0f, 0.0f);
		glRotatef(r.y, 0.0f, 1.0f, 0.0f);
		glRotatef(r.z, 0.0f, 0.0f, 1.0f);

		gdl::vec3 s = node->transform.scale;
		glScalef(s.x, s.y, s.z);

		gdl::Mesh* m = node->mesh;
		if (m != nullptr)
		{
            TerrainMesh* tm = static_cast<TerrainMesh*>(m);
            if (tm != nullptr)
            {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                tm->DrawInColor();
                glDisable(GL_BLEND);
            }
		}
	glPopMatrix();
    glShadeModel(GL_FLAT);
}


void Scene::SaveTracks()
{
    gdl::RocketSync::SaveAllTracks();
}

bool Scene::ShouldQuit()
{
    return gdl::RocketSync::GetBool(end_demo);
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

