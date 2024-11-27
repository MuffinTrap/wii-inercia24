
#include "scene.h"
#include "terraingen.h"
#include "../rocket/mgdl-rocket.h"
#include "../tracks.h"
#include <glm/gtc/matrix_transform.hpp>

#ifndef SYNC_PLAYER

    static ROCKET_TRACK sceneNumber;
    static ROCKET_TRACK fade_out;
    static ROCKET_TRACK end_demo;

    static ROCKET_TRACK drawTerrain;
    static ROCKET_TRACK drawSpaceport;

    static ROCKET_TRACK departure_text;
    static ROCKET_TRACK departure_textScale;
    static ROCKET_TRACK departure_textX;
    static ROCKET_TRACK departure_textY;
    static ROCKET_TRACK departure_width;
    static ROCKET_TRACK departure_height;
    static ROCKET_TRACK departure_depth;


    static ROCKET_TRACK camera_x;
    static ROCKET_TRACK camera_y;
    static ROCKET_TRACK camera_z;
    static ROCKET_TRACK camera_orbit_on;
    static ROCKET_TRACK camera_orbit_x;
    static ROCKET_TRACK camera_orbit_y;
    static ROCKET_TRACK camera_orbit_dist;
    static ROCKET_TRACK camera_speed;
    static ROCKET_TRACK camera_interpolateOn;
    static ROCKET_TRACK camera_wiggle;
    static ROCKET_TRACK camera_fov;
    static ROCKET_TRACK camera_posX;
    static ROCKET_TRACK camera_posY;
    static ROCKET_TRACK camera_posZ;



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
    static ROCKET_TRACK ship_scale;

    // Terrain
    static ROCKET_TRACK terrain_x;
    static ROCKET_TRACK terrain_y;
    static ROCKET_TRACK terrain_z;
    static ROCKET_TRACK terrain_scale;
    static ROCKET_TRACK terrain_rotY;
#endif

void Scene::Init()
{

	// SHIP
	shipTexture = gdl::LoadImage("assets/spaceship.png", gdl::TextureFilterModes::Nearest);
	portTexture = gdl::LoadImage("assets/spaceport.png", gdl::TextureFilterModes::Nearest);

    gdl::FBXFile* spacePortFBX = new gdl::FBXFile();
    spaceportScene = spacePortFBX->LoadFile("assets/spaceport.fbx");
    spaceportScene->SetMaterialTexture("spaceport.png", portTexture);


    gdl::FBXFile* shipFBX = new gdl::FBXFile();
    shipScene = shipFBX->LoadFile("assets/ship.fbx");
    shipScene->SetMaterialTexture("standardSurface1", shipTexture);


	// BG
	spacebg = gdl::LoadImage("assets/spacebg.png", gdl::TextureFilterModes::Linear);
    earthbg = gdl::LoadImage("assets/earthbg.png", gdl::TextureFilterModes::Linear);

    // Terrain
    gdl::PNGFile* terrainPNG = new gdl::PNGFile();
    terrainPNG->ReadFile("assets/testmap.png");
    terrain = TerrainGenerator::GenerateFromPNG(2.0f, 1.0f, terrainPNG);

    moonsurface = gdl::LoadImage("assets/moonsurface.png", gdl::TextureFilterModes::Linear);
    matcap = gdl::LoadImage("assets/shinyorb.png", gdl::TextureFilterModes::Linear);

    terrainNode = new gdl::Node("terrain", terrain, new gdl::Material("terrain", moonsurface));
    terrainNode->transform.scale = 1.0f;


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

    shipNode = shipScene->GetNode("pCylinder5");
    gdl_assert_print(shipNode!=nullptr, "No ship");

    // Connect the ship to the platform
    spaceportScene->SetActiveParentNode(elevatorPlatform);
    spaceportScene->PushChildNode(shipNode);


    delete terrainPNG;



    debugFont = gdl::LoadFont("assets/font8x16.png", 8, 16, ' ');
    gdl_assert_print(debugFont != nullptr, "Debug font failed to load");

    // dotFont = gdl::LoadFontCustom("assets/dot_font.png", 44,64, '1', 10);
    dotFont = gdl::LoadFontCustom("assets/led_counter50x64_v2.png", 50,64, 10, " -./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    gdl_assert_print(dotFont != nullptr, "Dot font failed to load");

    camera = new Camera();


    DebugMenu = gdl::MenuCreator(debugFont, 1.0f, 1.0);

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
    bool rocketInit = gdl::RocketSync::InitRocket(spaceMusic, 68, 4);
    if (rocketInit)
    {
#ifndef SYNC_PLAYER

        sceneNumber = gdl::RocketSync::GetTrack("scene");
        fade_out = gdl::RocketSync::GetTrack("fade_out");
        end_demo = gdl::RocketSync::GetTrack("end_demo");

        // Render hacks
        drawTerrain = gdl::RocketSync::GetTrack("drawTerrain");
        drawSpaceport = gdl::RocketSync::GetTrack("drawSpaceport");

        // Departures board
        departure_text = gdl::RocketSync::GetTrack("departure:text");
        departure_textX = gdl::RocketSync::GetTrack("departure:textX");
        departure_textY = gdl::RocketSync::GetTrack("departure:textY");
        departure_textScale = gdl::RocketSync::GetTrack("departure:textScale");
        // Board dimensions
        departure_width = gdl::RocketSync::GetTrack("departure:width");
        departure_height = gdl::RocketSync::GetTrack("departure:height");
        departure_depth = gdl::RocketSync::GetTrack("departure:depth");

        camera_x = gdl::RocketSync::GetTrack("camera:x");
        camera_y = gdl::RocketSync::GetTrack("camera:y");
        camera_z = gdl::RocketSync::GetTrack("camera:z");
        camera_orbit_on = gdl::RocketSync::GetTrack("camera:orbitON");
        camera_orbit_x = gdl::RocketSync::GetTrack("camera:orbitX");
        camera_orbit_y = gdl::RocketSync::GetTrack("camera:orbitY");
        camera_orbit_dist = gdl::RocketSync::GetTrack("camera:orbitDistance");
        camera_speed = gdl::RocketSync::GetTrack("camera:speed");
        camera_interpolateOn = gdl::RocketSync::GetTrack("camera:interpolateOn");
        camera_wiggle = gdl::RocketSync::GetTrack("camera:wiggle");
        camera_fov = gdl::RocketSync::GetTrack("camera:fov");
        camera_posX = gdl::RocketSync::GetTrack("camera:posX");
        camera_posY = gdl::RocketSync::GetTrack("camera:posY");
        camera_posZ = gdl::RocketSync::GetTrack("camera:posZ");

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
        ship_scale = gdl::RocketSync::GetTrack("ship:scale");


        terrain_x = gdl::RocketSync::GetTrack("terrain:x");
        terrain_y = gdl::RocketSync::GetTrack("terrain:y");
        terrain_z = gdl::RocketSync::GetTrack("terrain:z");
        terrain_scale = gdl::RocketSync::GetTrack("terrain:scale");
        terrain_rotY = gdl::RocketSync::GetTrack("terrain:rotY");
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

    // After elevator has moved, set the ships position
    /*
    gdl::vec3 shipPos = spaceportScene->GetWorldPosition(shipNode);
    shipNode->transform.position = shipPos;
    */

    // Offset the ship position with rocket

    shipNode->transform.position.x = gdl::RocketSync::GetFloat(ship_x);
    shipNode->transform.position.y = gdl::RocketSync::GetFloat(ship_y);
    shipNode->transform.position.z = gdl::RocketSync::GetFloat(ship_z);
    shipNode->transform.rotationDegrees.x = gdl::RocketSync::GetFloat(ship_rotX);
    shipNode->transform.rotationDegrees.y = gdl::RocketSync::GetFloat(ship_rotY);
    shipNode->transform.rotationDegrees.z = gdl::RocketSync::GetFloat(ship_rotZ);

    gdl::vec3 tpos = gdl::vec3(gdl::RocketSync::GetFloat(terrain_x), gdl::RocketSync::GetFloat(terrain_y), gdl::RocketSync::GetFloat(terrain_z));
    terrainNode->transform.position = tpos;
    terrainNode->transform.scale = gdl::RocketSync::GetFloat(terrain_scale);
    terrainNode->transform.rotationDegrees.y = gdl::RocketSync::GetFloat(terrain_rotY);



    float hangar_open = gdl::RocketSync::GetFloat( door_hangar );
    float hangar_offset = gdl::RocketSync::GetFloat( door_hangar_x );
    float elevator_open = gdl::RocketSync::GetFloat(door_elevator);
    float elevator_offset =gdl::RocketSync::GetFloat(door_elevator_x);

    hangarDoorLeft->transform.position.x = hangar_open + hangar_offset;
    hangarDoorRight->transform.position.x = hangar_open * -1.0f + hangar_offset;
    elevatorDoorLeft->transform.position.x = elevator_open + elevator_offset;
    elevatorDoorRight->transform.position.x = elevator_open * -1.0f + elevator_offset;

    // Camera update
    // NOTE: Update camera after updating target to avoid jitter

    DrawScene scene = (DrawScene)gdl::RocketSync::GetInt(sceneNumber);
    switch(scene)
    {
        case Departures:
            camera->target = gdl::vec3(0.0f, 0.0f, 0.0f);
            break;

        default:
            gdl::vec3 shipPos = spaceportScene->GetWorldPosition(shipNode);
            camera->target = shipPos;
        break;
    }

        // Offsets to the target
        camera->target.x += gdl::RocketSync::GetFloat(camera_x);
        camera->target.y += gdl::RocketSync::GetFloat(camera_y);
        camera->target.z += gdl::RocketSync::GetFloat(camera_z);

    if (gdl::RocketSync::GetBool(camera_orbit_on))
    {
        camera->Orbit(gdl::RocketSync::GetFloat(camera_orbit_y),
                      gdl::RocketSync::GetFloat(camera_orbit_x),
                      gdl::RocketSync::GetFloat(camera_orbit_dist));
    }
    else
    {
        // Set position directly when not orbiting
        camera->position.x = gdl::RocketSync::GetFloat(camera_posX);
        camera->position.y = gdl::RocketSync::GetFloat(camera_posY);
        camera->position.z = gdl::RocketSync::GetFloat(camera_posZ);
    }
    bool doInterpolate = gdl::RocketSync::GetBool(camera_interpolateOn);
    float wiggle = gdl::RocketSync::GetFloat(camera_wiggle);
    float speed = gdl::RocketSync::GetFloat(camera_speed);
    camera->lerpSpeed = speed;
    camera->Update(gdl::GetDeltaTime(), doInterpolate, wiggle);


    // TerrainGenerator::CalculateMatcapFromCamera((TerrainMesh*)terrainNode->mesh, camera->GetDirection());
    TerrainGenerator::CalculateMatcapFromNormals((TerrainMesh*)terrainNode->mesh);


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
    DrawScene scene = (DrawScene)gdl::RocketSync::GetInt(sceneNumber);
    switch(scene)
    {
        case Spaceport:
            DrawSpaceportScene();
            break;

        case Departures:
            DrawDeparturesScene();
            break;


        case Terrain:

            break;

        case Earth:
            DrawEarthScene();

            break;

        case Gate:

            break;
    }
    gdl::InitOrthoProjection();
    DrawFadeOut();

#ifndef SYNC_PLAYER
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
    DebugDrawTiming();
#endif
}

static float Display(gdl::Font* font, float x, float y, u32 color, float scale, const std::string& text)
{
    font->Print(color, x, y, scale, gdl::LJustify, gdl::LJustify, text.c_str());
    return scale * text.length();
}

void Scene::DrawDeparturesScene()
{
    glEnable(GL_DEPTH_TEST);
    gdl::InitPerspectiveProjection(75.0f, 0.1f, 1000.0f);
    camera->LookAt();

    glPushMatrix();
        // Draw departures display
        glColor3f(0.1f, 0.1f, 0.1f);
        glScalef(
            gdl::RocketSync::GetFloat(departure_width),
            gdl::RocketSync::GetFloat(departure_height),
            gdl::RocketSync::GetFloat(departure_depth)
        );
        glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
        // Draw text on the display surface
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(
            gdl::RocketSync::GetFloat(departure_textX),
            gdl::RocketSync::GetFloat(departure_textY),
            gdl::RocketSync::GetFloat(departure_depth)/2.0 + 0.01f
        );

        static const std::string company = "MARMOT SPACELINES";
        static const std::string callsign = "MS 2024";
        static const std::string destination = "INERCIA ST./EUROPA";
        static const std::string gate = "A5";
        static const std::string boarding = "BOARDING";
        static const std::string closing = "GATE CLOSING";

        u32 red = gdl::Colors::Red;
        u32 green = gdl::Colors::LightGreen;
        u32 white = gdl::Colors::White;
        u32 yellow = gdl::Colors::Yellow;

        float scale = 1.0f + gdl::RocketSync::GetFloat(departure_textScale);
        // Draw company or callsign
        float x = 0;
        float y = 0;

        Display(dotFont, x, y, white, scale, "FLIGHT                DESTINATION            GATE STATUS");
        y -= scale;

        int showText = gdl::RocketSync::GetInt(departure_text);

        if (showText == 0)
        {
            Display(dotFont, x, y, yellow, scale, company);
        }
        else
        {
            Display(dotFont, x, y, yellow, scale, callsign);
        }

        float nameLength = (company.length()) * scale;
        x += nameLength;
        x += Display(dotFont, x, y, yellow, scale, destination );
        x += Display(dotFont, x, y, white, scale, gate ) + 2 * scale;
        if (showText == 1)
        {
            Display(dotFont, x, y, green, scale, boarding );
        }
        else
        {
            Display(dotFont, x, y, red, scale, closing );
        }
    glPopMatrix();
}

void Scene::DrawBG ( gdl::Image* bgImage, float scale)
{
	// Draw Full screen bg
	glDisable(GL_DEPTH_TEST);
	gdl::InitOrthoProjection();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    bgImage->Draw2DAligned(gdl::GetScreenWidth()/2, gdl::GetScreenHeight()/2, scale, gdl::Centered, gdl::Centered);
}

void Scene::DrawEarthScene()
{
    DrawBG(earthbg, 1.8f);
    glEnable(GL_DEPTH_TEST);
    gdl::InitPerspectiveProjection(75.0f, 0.1f, 1000.0f);
    camera->LookAt();
    glPushMatrix();
        shipScene->Draw();
    glPopMatrix();
    glDisable(GL_DEPTH_TEST);
}

void Scene::DrawSpaceportScene()
{
    DrawBG(spacebg, 20.0f);

    glEnable(GL_DEPTH_TEST);
    gdl::InitPerspectiveProjection(75.0f, 0.1f, 1000.0f);

    camera->LookAt();

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

    glDisable(GL_DEPTH_TEST);
}

void Scene::DrawTerrainNode ( gdl::Node* node )
{
	gdl::vec3 t = node->transform.position;
	glPushMatrix();
		glTranslatef(t.x, t.y, t.z);

		gdl::vec3 r = node->transform.rotationDegrees;
		glRotatef(r.x, 1.0f, 0.0f, 0.0f);
		glRotatef(r.y, 0.0f, 1.0f, 0.0f);
		glRotatef(r.z, 0.0f, 0.0f, 1.0f);

		float s = node->transform.scale;
		glScalef(s, s, s);

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

        glDisable(GL_DEPTH_TEST);
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
	}
}

void Scene::DebugDrawTiming()
{
    debugFont->Printf(gdl::Colors::White, 0, debugFont->GetCharacterHeight()*2, debugFont->GetCharacterHeight(), gdl::LJustify, gdl::LJustify, "Elapsed %.1f", spaceMusic->GetElapsedSeconds());
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

