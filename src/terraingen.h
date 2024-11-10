#pragma once
#include <mgdl.h>

class TerrainGenerator
{
public:
	static gdl::Mesh* GenerateFromPNG(float unitsPerPixel, float heightMultiplier, gdl::PNGFile* png);

};

