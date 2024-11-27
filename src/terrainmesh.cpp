#include "terrainmesh.h"

void TerrainMesh::DrawInColor()
{
	SetupColorArrays();
	// NOTE OpenGX does not impelement glDrawRangeElements()
	//glDrawRangeElements(GL_TRIANGLES, 0, vertexCount-1, indexCount, GL_UNSIGNED_SHORT, indices);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, indices);
}

void TerrainMesh::SetupColorArrays()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, positions);
	if (normals != nullptr)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, normals);
	}
	/*
	if (uvs != nullptr)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, uvs);
	}
	*/
	if (vertexColors != nullptr)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, 0, vertexColors);
	}
}


