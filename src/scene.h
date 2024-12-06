#pragma once

#include <mgdl.h>
#include "camera.h"
#include "drawscene.h"
#include "departures.h"


class Scene
{
	// This has all the models and textures etc.
public:
	void Init();
	void Update();
	void Draw();
	void SaveTracks();
	void DrawFadeOut();
	bool ShouldQuit();

	// Scene drawing functions
	void DrawSpaceportScene();
	void DrawEarthScene();
	void DrawCredits();

	void DebugDraw3DSpace(float sideLength);
	void DebugDrawTiming();
	void DrawThirdsGuides();
	void DrawStars(const gdl::vec3& center);

	void DrawTerrainNode(gdl::Node* node);
	void DrawBG(gdl::Image* bgImage, float xoffset, float yoffset, float scale);

	// Scenes
	DeparturesScene* departures;

	// FBX files
	gdl::Scene* shipScene = nullptr;
	gdl::Scene* spaceportScene = nullptr;
	gdl::Scene* gateScene = nullptr;
	gdl::Scene* gearScene = nullptr;

	gdl::Image* shipTexture = nullptr;
	gdl::Image* portTexture = nullptr;
	gdl::Image* gateTexture = nullptr;
	gdl::Image* spacebg = nullptr;
	gdl::Image* earthbg = nullptr;
	gdl::Image* moonfg = nullptr;

	gdl::Image* heightMap = nullptr;
	gdl::Image* moonsurface = nullptr;
	gdl::Image* matcap = nullptr;

	gdl::Mesh* terrain = nullptr;
	gdl::Font* debugFont = nullptr;
	gdl::Sound* spaceMusic = nullptr;
	Camera* camera = nullptr;

	gdl::Node* shipNode = nullptr;
	gdl::Node* hangarDoorLeft = nullptr;
	gdl::Node* hangarDoorRight = nullptr;
	gdl::Node* elevatorDoorLeft = nullptr;
	gdl::Node* elevatorDoorRight = nullptr;
	gdl::Node* elevatorPlatform = nullptr;
	gdl::Node* terrainNode = nullptr;
	gdl::Node* tunnelNode = nullptr;
	gdl::Node* texturedShipNode = nullptr;
	gdl::Node* hangarNode = nullptr;
	gdl::Node* gearNode = nullptr;
	gdl::Node* gearNodeBackLeft = nullptr;
	gdl::Node* gearNodeBackRight = nullptr;
	gdl::Node* starSphereNode;

	std::vector<gdl::Node*> railingNodes;

	gdl::Node* gateRingNode = nullptr;

	gdl::PNGFile* heightMapPNG;

	gdl::MenuCreator DebugMenu;
	gdl::MenuCreator TimerMenu;
	gdl::vec3 shipPos;

	gdl::Font* creditFont = nullptr;

};
