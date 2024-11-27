#pragma once
#include <mgdl.h>
#include "terrainmesh.h"

class TerrainGenerator
{
public:
	static TerrainMesh* GenerateFromPNG(float heightMultiplier, float uvRange, gdl::PNGFile* png);
	static TerrainMesh* CreateGridMesh(float width, float depth, float uvRange);
	static TerrainMesh* CreateNormals(TerrainMesh* mesh, float width, float depth);
	static TerrainMesh* SetHeightsFromPNG(TerrainMesh* mesh, gdl::PNGFile* png, float heightMultiplier, float width, float depth);

	static TerrainMesh* CalculateMatcapFromNormals(TerrainMesh* mesh);
	static TerrainMesh* CalculateMatcapFromCamera(TerrainMesh* mesh, gdl::vec3 cameraDirection);
};

