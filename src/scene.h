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

	gdl::Scene* shipScene;
	gdl::Image* shipTexture;
	gdl::Image* spacebg;
	gdl::Image* heightMap;
	gdl::Mesh* terrain;
	gdl::Font* debugFont;
	gdl::Sound* spaceMusic;
	Camera* camera;

	gdl::Node* shipNode;

};
