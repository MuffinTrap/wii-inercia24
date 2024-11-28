#pragma once
#include "camera.h"

class DeparturesScene
{
public:
	DeparturesScene();
	void Update();
	void Draw(Camera* camera);
	gdl::Font* dotFont = nullptr;
};
