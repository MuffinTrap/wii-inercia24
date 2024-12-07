#pragma once
#include <mgdl.h>
#include <glm/glm.hpp>
#include <stdio.h>
#include <string>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	gdl::vec3 position;
	gdl::vec3 target;
	gdl::vec3 direction;
	gdl::vec3 up;
	float lerpSpeed;

	Camera();

	void SetupFor3D();
	void Update(float delta, gdl::vec3 target);
	void Orbit(float aroundY, float aroundX, float distance);
	void LookAtTarget();
	void DebugDraw(short x, short y, gdl::Font* font);
	gdl::vec3 GetDirection();
	glm::mat4 GetViewMatrix();

	gdl::vec3 currentPosition; // Do not snap to position instantly
private:
	glm::vec3 convertToGLM(const gdl::vec3& gdlVec);
	glm::vec3 local_cross(const glm::vec3& v1, const glm::vec3& v2) ;
	float local_dot(const glm::vec3& side, const glm::vec3& position);



};
