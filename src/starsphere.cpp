#include "starsphere.h"
#include <math.h>
#include <cstdlib>
#include <glm/gtc/matrix_transform.hpp>

static void PutV(size_t i, gdl::Mesh* m, glm::vec3 v)
{
	m->positions[i + 0] = v.x;
	m->positions[i + 1] = v.y;
	m->positions[i + 2] = v.z;
}

static void PutN(size_t i, gdl::Mesh* m, glm::vec3 v)
{
	m->normals[i + 0] = v.x;
	m->normals[i + 1] = v.y;
	m->normals[i + 2] = v.z;
}

static void PutUV(size_t i, gdl::Mesh* m, glm::vec2 v)
{
	m->uvs[i + 0] = v.x;
	m->uvs[i + 1] = v.y;
}

static float RandF()
{
	static const float rm = (float)RAND_MAX;
	float r = (float)rand();
	return r/rm;
}

/*
static void PrintV(const glm::vec3& v)
{
	printf("%.1f, %.1f, %.1f\n", v.x, v.y, v.z);
}
*/

gdl::Mesh * StarSphere::CreateSphere(int starsAmount, float radius, float sizeMin, float sizeMax)
{
	gdl::Mesh* stars = new gdl::Mesh();
	stars->name = "starsphere";

	stars->vertexCount = starsAmount * 4;
	stars->positions = new GLfloat[starsAmount * 4 * 3];
	stars->normals = new GLfloat[starsAmount * 4 * 3];
	stars->uvs = new GLfloat[starsAmount * 4 * 2];
	stars->indices = new GLushort[starsAmount * 6];
	stars->indexCount = starsAmount * 6;

	size_t posI = 0;
	size_t nI = 0;
	size_t indI = 0;
	size_t uvI = 0;

			glm::vec4 worldRight4 = glm::vec4(1.0f, 0.0f, 0.0f,0.0f);
			glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
			float circle = glm::radians(360.0f);
			float hCircle = circle * 0.5f;
			int ind = 0;
			for (int i = 0; i < starsAmount; i++)
			{

				// Compass direction of the star
				float yAngle = RandF() * circle;
				// float starAngle = ((float)i / starsAmount) * circle;

				// Elevation of the star up or down
				float elevationAngle = (RandF() - 0.5f) * hCircle;

				// Get compass direction vector
				glm::mat4 headingRot = glm::mat4(1.0f);
				headingRot = glm::rotate(headingRot, yAngle, worldUp);
				glm::vec4 heading = headingRot * worldRight4;
				glm::vec3 right = glm::cross(glm::vec3(heading), worldUp);

				// Get elevation matrix and rotate compass direction with it
				glm::mat4 elevationRot = glm::mat4(1.0f);
				elevationRot = glm::rotate(elevationRot, elevationAngle, right);
				glm::vec4 direction = elevationRot * heading;

				direction = glm::normalize(direction);
				glm::vec3 normal = direction * -1.0f;
				glm::vec3 dir3 = glm::vec3(direction);
				glm::vec3 up = glm::cross(right, dir3);

				// Move star away from origo
				dir3 *= radius;
				float size = RandF() * (sizeMax - sizeMin) + sizeMin;
				right *= size;
				up *= size;



				glm::vec3 v1 = dir3 - right - up;
				glm::vec3 v2 = dir3 + right - up;
				glm::vec3 v3 = dir3 + right + up;
				glm::vec3 v4 = dir3 - right + up;
				/*
				printf("Star angle: %1.f / %.1f size: %.1f dir: %.1f, right: %.1f, up: %1.f\n",

					   glm::degrees(elevationAngle),
					   glm::degrees(yAngle),
					   size,
					   glm::length(dir3),
					   glm::length(right),
					   glm::length(up));
				PrintV(v1);
				PrintV(v2);
				PrintV(v3);
				PrintV(v4);
				*/

				PutV(posI, stars, v1);
				posI += 3;
				PutV(posI, stars, v2);
				posI += 3;
				PutV(posI, stars, v3);
				posI += 3;
				PutV(posI, stars, v4);
				posI += 3;

				PutN(nI, stars, normal);
				nI+=3;
				PutN(nI, stars, normal);
				nI+=3;
				PutN(nI, stars, normal);
				nI+=3;
				PutN(nI, stars, normal);
				nI+=3;

				stars->indices[indI + 0] = ind;
				stars->indices[indI + 1] = (ind + 1);
				stars->indices[indI + 2] = (ind + 2);
				stars->indices[indI + 3] = (ind);
				stars->indices[indI + 4] = (ind + 2);
				stars->indices[indI + 5] = (ind + 3);

				ind += 4;
				indI += 6;

				PutUV(uvI, stars, glm::vec2(0.0f, 0.0f));
				uvI += 2;
				PutUV(uvI, stars,glm::vec2(1.0f, 0.0f));
				uvI += 2;
				PutUV(uvI, stars,glm::vec2(1.0f, 1.0f));
				uvI += 2;
				PutUV(uvI, stars,glm::vec2(0.0f, 1.0f));
				uvI += 2;
			}

			return stars;
}

