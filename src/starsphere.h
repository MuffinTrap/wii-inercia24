#pragma once
#include <mgdl.h>

// This class generates a mesh that
// is a sphere of quads that looks like stars

class StarSphere
{
public:

	gdl::Mesh* CreateSphere(int starAmount, float radius, float sizeMin, float sizeMax);


};
