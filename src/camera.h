
#pragma once
#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-font.h>

class Camera
{
public:
	gdl::vec3 position;
	gdl::vec3 target;
	gdl::vec3 direction;
	float lerpSpeed;

	Camera();

	void SetupFor3D();
	void Update(float delta, gdl::vec3 target);
	void Orbit(float aroundY, float aroundX, float distance);
	void LookAtTarget();
	void DebugDraw(short x, short y, gdl::Font* font);
	gdl::vec3 GetDirection();

	gdl::vec3 currentPosition; // Do not snap to position instantly
private:
	gdl::vec3 up;



};
