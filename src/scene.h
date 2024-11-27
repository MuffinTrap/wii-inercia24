#pragma once

#include <mgdl.h>
#include "camera.h"

enum DrawScene : int
{
	Spaceport = 0,
	Departures  = 1,
	Terrain = 2,
	Earth = 3,
	Gate = 4
};

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
	void DrawDeparturesScene();
	void DrawSpaceportScene();
	void DrawEarthScene();

	void DebugDraw3DSpace(float sideLength);
	void DebugDrawTiming();

	void DrawTerrainNode(gdl::Node* node);
	void DrawBG(gdl::Image* bgImage, float scale);

	gdl::Scene* shipScene = nullptr;
	gdl::Scene* spaceportScene = nullptr;
	gdl::Scene* gateScene = nullptr;

	gdl::Image* shipTexture = nullptr;
	gdl::Image* portTexture = nullptr;
	gdl::Image* spacebg = nullptr;
	gdl::Image* earthbg = nullptr;

	gdl::Image* heightMap = nullptr;
	gdl::Image* moonsurface = nullptr;
	gdl::Image* matcap = nullptr;

	gdl::Mesh* terrain = nullptr;
	gdl::Font* debugFont = nullptr;
	gdl::Sound* spaceMusic = nullptr;
	gdl::Font* dotFont = nullptr;
	Camera* camera = nullptr;

	gdl::Node* shipNode = nullptr;
	gdl::Node* hangarDoorLeft = nullptr;
	gdl::Node* hangarDoorRight = nullptr;
	gdl::Node* elevatorDoorLeft = nullptr;
	gdl::Node* elevatorDoorRight = nullptr;
	gdl::Node* elevatorPlatform = nullptr;
	gdl::Node* terrainNode = nullptr;

	gdl::MenuCreator DebugMenu;

};
