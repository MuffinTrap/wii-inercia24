#include "camera.h"
#include "math.h"
#include <mgdl.h>
#include "../rocket/mgdl-rocket.h"
#include "../rocket_tracks.h"
#include "drawscene.h"

#ifndef SYNC_PLAYER
static ROCKET_TRACK camera_x;
static ROCKET_TRACK camera_y;
static ROCKET_TRACK camera_z;
static ROCKET_TRACK camera_orbitON;
static ROCKET_TRACK camera_orbitX;
static ROCKET_TRACK camera_orbitY;
static ROCKET_TRACK camera_orbitDistance;
static ROCKET_TRACK camera_speed;
static ROCKET_TRACK camera_interpolateOn;
static ROCKET_TRACK camera_wiggle;
static ROCKET_TRACK camera_fov;
static ROCKET_TRACK camera_posX;
static ROCKET_TRACK camera_posY;
static ROCKET_TRACK camera_posZ;
static ROCKET_TRACK camera_viewDistance;
static ROCKET_TRACK camera_targetOn;
#endif

Camera::Camera()
{
	position = gdl::vec3(0.0f, 0.0f, 1.0f);
	currentPosition = position;
	target = gdl::vec3(0.0f, 0.0f, 0.0f);
	up = gdl::vec3(0.0f, 1.0f, 0.0f);

#ifndef SYNC_PLAYER
	camera_x = gdl::RocketSync::GetTrack("camera:x");
	camera_y = gdl::RocketSync::GetTrack("camera:y");
	camera_z = gdl::RocketSync::GetTrack("camera:z");
	camera_orbitON = gdl::RocketSync::GetTrack("camera:orbitON");
	camera_orbitX = gdl::RocketSync::GetTrack("camera:orbitX");
	camera_orbitY = gdl::RocketSync::GetTrack("camera:orbitY");
	camera_orbitDistance = gdl::RocketSync::GetTrack("camera:orbitDistance");
	camera_speed = gdl::RocketSync::GetTrack("camera:speed");
	camera_interpolateOn = gdl::RocketSync::GetTrack("camera:interpolateOn");
	camera_wiggle = gdl::RocketSync::GetTrack("camera:wiggle");
	camera_fov = gdl::RocketSync::GetTrack("camera:fov");
	camera_posX = gdl::RocketSync::GetTrack("camera:posX");
	camera_posY = gdl::RocketSync::GetTrack("camera:posY");
	camera_posZ = gdl::RocketSync::GetTrack("camera:posZ");
	camera_viewDistance = gdl::RocketSync::GetTrack("camera:viewDistance");
	camera_targetOn = gdl::RocketSync::GetTrack("camera:targetOn");
#endif

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

void Camera::LookAtTarget()
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

void Camera::SetupFor3D()
{
    gdl::InitPerspectiveProjection(75.0f + gdl::RocketSync::GetFloat(camera_fov), 1.0f, gdl::RocketSync::GetFloat(camera_viewDistance));
}


void Camera::Update(float delta, gdl::vec3 target)
{
	if (gdl::RocketSync::GetBool(camera_targetOn))
	{
		this->target = target;

		// Offsets to the target
		this->target.x += gdl::RocketSync::GetFloat(camera_x);
		this->target.y += gdl::RocketSync::GetFloat(camera_y);
		this->target.z += gdl::RocketSync::GetFloat(camera_z);
	}

    if (gdl::RocketSync::GetBool(camera_orbitON))
    {
        Orbit(gdl::RocketSync::GetFloat(camera_orbitY),
                      gdl::RocketSync::GetFloat(camera_orbitX),
                      gdl::RocketSync::GetFloat(camera_orbitDistance));
    }
    else
    {
        // Set position directly when not orbiting
        position.x = gdl::RocketSync::GetFloat(camera_posX);
        position.y = gdl::RocketSync::GetFloat(camera_posY);
        position.z = gdl::RocketSync::GetFloat(camera_posZ);
    }

    bool doInterpolate = gdl::RocketSync::GetBool(camera_interpolateOn);
	// TODO: Implement shaky shaky
    // float wiggle = gdl::RocketSync::GetFloat(camera_wiggle);
    float speed = gdl::RocketSync::GetFloat(camera_speed);
    lerpSpeed = speed;

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
glm::vec3 Camera::convertToGLM(const gdl::vec3& gdlVec) {
    return glm::vec3(gdlVec.x, gdlVec.y, gdlVec.z);
}
glm::vec3 Camera::local_cross(const glm::vec3& v1, const glm::vec3& v2) {
    return glm::vec3(
        v1.y * v2.z - v1.z * v2.y,  // x component
        v1.z * v2.x - v1.x * v2.z,  // y component
        v1.x * v2.y - v1.y * v2.x   // z component
    );
}

float Camera::local_dot(const glm::vec3& side, const glm::vec3& position) {
    return side.x * position.x + side.y * position.y + side.z * position.z;
}

glm::mat4 Camera::GetViewMatrix() {
    glm::vec3 forward = {
	    target.x - position.x,
	    target.y - position.y,
	    target.z - position.z
    }; // Forward direction (camera looks at origin)
    forward = glm::normalize(forward);

    glm::vec3 side = glm::normalize(local_cross(convertToGLM(up), forward)); // Right vector
    glm::vec3 adjustedUp = local_cross(forward, side);         // Corrected Up vector

    glm::mat4 viewMatrix(1.0f); // Initialize as identity matrix

    // Set rotation (top-left 3x3 matrix)
    viewMatrix[0][0] = side.x;       viewMatrix[1][0] = side.y;       viewMatrix[2][0] = side.z;
    viewMatrix[0][1] = adjustedUp.x; viewMatrix[1][1] = adjustedUp.y; viewMatrix[2][1] = adjustedUp.z;
    viewMatrix[0][2] = -forward.x;   viewMatrix[1][2] = -forward.y;   viewMatrix[2][2] = -forward.z;

    // Set translation (last column)
    viewMatrix[3][0] = -local_dot(side, convertToGLM(position));
    viewMatrix[3][1] = -local_dot(adjustedUp, convertToGLM(position));
    viewMatrix[3][2] = local_dot(forward, convertToGLM(position));

    return viewMatrix;
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
