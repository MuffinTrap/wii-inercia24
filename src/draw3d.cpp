#include "draw3d.h"
#include <mgdl.h>

void Quad(glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d, const::glm::vec3& normal)
{
	// bottom
	glNormal3f(normal.x, normal.y, normal.z);
	glVertex3f(a.x, a.y, a.z);

	// bottom2
	glNormal3f(normal.x, normal.y, normal.z);
	glVertex3f(d.x, d.y, d.z);

	// top 1 and 2
	glNormal3f(normal.x, normal.y, normal.z);
	glVertex3f(c.x, c.y, c.z);

	glNormal3f(normal.x, normal.y, normal.z);
	glVertex3f(b.x, b.y, b.z);
}

void SolidCube(float size)
{
    float hs = size/2.0f;
	glm::vec3 bl = {-hs, -hs, -hs};
	glm::vec3 br = {hs, -hs, -hs};
	glm::vec3 fl = {-hs, -hs, hs};
	glm::vec3 fr = {hs, -hs, hs};

	glm::vec3 tbl = {-hs, hs, -hs};
	glm::vec3 tbr = {hs, hs, -hs};
	glm::vec3 tfl = {-hs, hs, hs};
	glm::vec3 tfr = {hs, hs, hs};

	glm::vec3 R = {1.0f, 0.0f, 0.0f};
	glm::vec3 U = {0.0f, 1.0f, 0.0f};
	glm::vec3 F = {0.0f, 0.0f, 0.0001f};

	glm::vec3 nR = R;
	glm::vec3 nL = -R;
	glm::vec3 nF =  {0.0f, 0.0f, 1.0f};
	glm::vec3 nB =  -F;

	glBegin(GL_QUADS);
		// left side
		Quad(bl, tbl, tfl, fl, nL);
		// front
		Quad(fl, tfl, tfr, fr, nF);
		// right
		Quad(fr, tfr, tbr, br, nR);
		// back
		Quad(br, tbr, tbl, bl, nB);

		// top
		Quad(tfl, tbl, tbr, tfr, U);

	glEnd();
}
