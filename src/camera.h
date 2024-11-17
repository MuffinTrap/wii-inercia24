
#pragma once
#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-font.h>

class Camera
{
public:
	gdl::vec3 position;
	gdl::vec3 target;

	Camera();
	void Orbit(float aroundY, float aroundX, float distance);
	void LookAt();
	void DebugDraw(short x, short y, gdl::Font* font);

private:
	gdl::vec3 up;



};
