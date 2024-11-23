#pragma once

#include <mgdl.h>
#include "camera.h"

class Scene
{
	// This has all the models and textures etc.
public:
	void Init();
	void Update();
	void Draw();
	void SaveTracks();

	void DebugDraw3DSpace(float sideLength);

	gdl::Scene* shipScene = nullptr;
	gdl::Image* shipTexture = nullptr;
	gdl::Image* portTexture = nullptr;
	gdl::Image* spacebg = nullptr;
	gdl::Image* heightMap = nullptr;
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


};
