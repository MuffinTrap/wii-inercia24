#include "camera.h"
#include "math.h"
#include <mgdl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
{
	position = gdl::vec3(0.0f, 0.0f, 1.0f);
	currentPosition = position;
	target = gdl::vec3(0.0f, 0.0f, 0.0f);
	up = gdl::vec3(0.0f, 1.0f, 0.0f);
}


void Camera::Orbit ( float aroundY, float aroundX, float distance )
{
	aroundY = gdl::Deg2Rad(aroundY);
	aroundX = gdl::Deg2Rad(aroundX);
	// Limit around X so that camera does not flip around
	aroundX = gdl::clampF(aroundX, -70.0f, 70.0f);
	if (distance < 1.0f)
	{
		distance = 1.0f;
	}

	glm::mat4 orbit = glm::mat4(1.0f);
	glm::vec4 toTarget = glm::vec4(0.0f, 0.0f, -distance, 1.0f);

	// Rotate around the origo
	orbit = glm::rotate(orbit, aroundY, glm::vec3(0.0f, 1.0f, 0.0f));
	orbit = glm::rotate(orbit, aroundX, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::vec4 pos = orbit * toTarget;

	position = gdl::vec3(target.x + pos.x, target.y + pos.y, target.z + pos.z);
}

void Camera::LookAt()
{
	gdl::InitCamera(currentPosition, target, up);
}

gdl::vec3 Camera::GetDirection()
{
	gdl::vec3 dir = gdl::vec3(
		target.x-currentPosition.x,
		target.y-currentPosition.y,
		target.z-currentPosition.z);

	float len = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
	if (len > 0.0f)
	{
		return gdl::vec3(dir.x / len, dir.y / len, dir.z / len);
	}
	return dir;
}


void Camera::Update(float delta, bool doInterpolate, float wiggleSpeed)
{
	if (doInterpolate)
	{
		// Move towards position
		float mx = position.x-currentPosition.x;
		float my = position.y-currentPosition.y;
		float mz = position.z-currentPosition.z;
		currentPosition.x += mx * delta * lerpSpeed;
		currentPosition.y += my * delta * lerpSpeed;
		currentPosition.z += mz * delta * lerpSpeed;
	}
	else
	{
		currentPosition = position;
	}
}

void Camera::DebugDraw (short x, short y, gdl::Font* font )
{
	font->Printf(gdl::Colors::White,
				 x, y, 16.0f, gdl::LJustify, gdl::LJustify,
			  "POS %.2f %.1f, %.1f", position.x, position.y, position.z);
	font->Printf(gdl::Colors::White,
				 x, y - 16, 16.0f, gdl::LJustify, gdl::LJustify,
			  "TARGET %.2f %.1f, %.1f", target.x, target.y, target.z);
	gdl::vec3 dir = GetDirection();
	font->Printf(gdl::Colors::White,
				 x, y - 32, 16.0f, gdl::LJustify, gdl::LJustify,
			  "DIR %.2f %.1f, %.1f", dir.x, dir.y, dir.z);
}
