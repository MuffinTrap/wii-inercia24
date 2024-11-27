#pragma once

#include <mgdl.h>

class TerrainMesh : public gdl::Mesh
{
public:
	GLfloat* vertexColors;

	void SetupColorArrays();
	void DrawInColor();
};
