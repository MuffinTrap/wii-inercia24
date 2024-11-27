#include "terraingen.h"
#include <glm/glm.hpp>
#include <stdio.h>
#include <string>

static const size_t X = 0;
static const size_t Y = 1;
static const size_t Z = 2;

TerrainMesh* TerrainGenerator::CreateGridMesh ( float width, float depth, float uvRange )
{

	TerrainMesh* mesh = new TerrainMesh();
	mesh->name = std::string("Terrain");

	{ // Allocate
		mesh->vertexCount = width * depth;
		mesh->positions = 	new GLfloat[mesh->vertexCount * 3];
		mesh->normals = 	new GLfloat[mesh->vertexCount * 3];
		mesh->uvs = 		new GLfloat[mesh->vertexCount * 2];
		mesh->vertexColors= new GLfloat[mesh->vertexCount * 3];

		// Clear memory to zeros
		for(size_t i = 0; i < mesh->vertexCount * 3; i++)
		{
			mesh->positions[i] = 0.0f;
			mesh->normals[i] = 0.0f;
			mesh->vertexColors[i] = 1.0f;
		}
		for(size_t i = 0; i < mesh->vertexCount * 2; i++)
		{
			mesh->uvs[i] = 0.0f;
		}

		mesh->indexCount = (width-1) * (depth-1) * 6;
		mesh->indices = 	new GLushort[mesh->indexCount];
		for (GLsizei i = 0; i < mesh->indexCount; i++)
		{
			mesh->indices[i] = 0;
		}
	}

	{ // Create vertices

		// Start here
		float sx = -0.5f - (float)(width/2) * 0.5f;
		float sz = -0.5f - (float)(depth/2) * 0.5f;

		size_t positionIndex = 0;
		const float uStep = uvRange/(float)(width);
		const float vStep = uvRange/(float)(depth);
		for (int d = 0; d < depth; d++)
		{
			for (int w = 0; w < width; w++)
			{
				size_t i = (d * width + w);
				size_t vertexi = i * 3;
				size_t uvi = i * 2;
				// DEBUG
				float px = sx + w;
				float py = 0.0f;
				float pz = sz + d;

				mesh->positions[vertexi + X] = px;
				mesh->positions[vertexi + Y] = py;
				mesh->positions[vertexi + Z] = pz;

				mesh->normals[vertexi + X] = 0.0f;
				mesh->normals[vertexi + Y] = 1.0f;
				mesh->normals[vertexi + Z] = 0.0f;

				mesh->uvs[uvi + X] = uStep * (float)w;
				mesh->uvs[uvi + Y] = vStep * (float)d;

				positionIndex++;
			}
		}

		// Set indices
		size_t ii = 0;
		size_t ti = 0;
		for (int d = 0; d < depth-1; d++)
		{
			for (int w = 0; w < width-1; w++)
			{
				GLushort corner = w + width * d;
				GLushort next = corner + 1;
				GLushort below = corner + width;
				GLushort across = below + 1;
				mesh->indices[ii + 0]=corner;
				mesh->indices[ii + 1]=below;
				mesh->indices[ii + 2]=next;
				// printf("T%lu (%d, %d, %d)\n", ti, corner, below, next);
				ti++;

				mesh->indices[ii + 3]=next;
				mesh->indices[ii + 4]=below;
				mesh->indices[ii + 5]=across;
				// printf("T%lu (%d, %d, %d)\n", ti, next, below, across);
				ii += 6;
				ti++;
			}
		}
	}
	return mesh;
}

TerrainMesh* TerrainGenerator::CreateNormals ( TerrainMesh* mesh, float width, float depth )
{
	// Generate normals
	// Go through all triangles
	// Calculate a normal for the
	// triangle
	GLsizei triangleCount = mesh->indexCount/3;
	// printf("Normals for %d triangles\n", triangleCount );
	glm::vec3* triangleNormals = new glm::vec3[triangleCount];
	if (triangleNormals == nullptr)
	{
		printf("Cannot allocate triangle normals\n");
		return mesh;
	}
	// Reset memory
	for (GLsizei ti = 0; ti < triangleCount; ti++)
	{
		triangleNormals[ti] = glm::vec3(0.0f, 1.0f, 0.0f);
	}

	// Calculate triangle normals
	{

		glm::vec3 a = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 b = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 c = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 N = glm::vec3(0.0f, 1.0f, 0.0f);
		for (GLsizei ti = 0; ti < triangleCount; ti++)
		{
			GLsizei draw = ti * 3;
			GLushort va = mesh->indices[draw + 0];
			GLushort vb = mesh->indices[draw + 1];
			GLushort vc = mesh->indices[draw + 2];

			GLushort index = va;
			// printf("TI: %d: %d %d %d\n", draw, va, vb, vc);
			a.x = mesh->positions[index*3 + 0];
			a.y = mesh->positions[index*3 + 1];
			a.z = mesh->positions[index*3 + 2];

			index = vb;
			b.x = mesh->positions[index*3 + 0];
			b.y = mesh->positions[index*3 + 1];
			b.z = mesh->positions[index*3 + 2];

			index = vc;
			c.x = mesh->positions[index*3 + 0];
			c.y = mesh->positions[index*3 + 1];
			c.z = mesh->positions[index*3 + 2];

			/*
			printf("T %d/A (%.1f, %.1f, %.1f)\n", ti, a.x, a.y, a.z);
			printf("T %d/B (%.1f, %.1f, %.1f)\n", ti, b.x, b.y, b.z);
			printf("T %d/C (%.1f, %.1f, %.1f)\n", ti, c.x, c.y, c.z);
			*/
			N = glm::normalize(glm::cross(c - a, b - a));
			N *= -1.0f; // flip flop
			// printf("N %d (%.1f, %.1f, %.1f)\n", ti, N.x, N.y, N.z);
			if (ti < triangleCount)
			{
				triangleNormals[ti] = N;
			}
			else
			{
				printf("Out of triangleNormals bounds\n");
			}
		}
	}
	// Go through all points and average the normals
	// Each point is shared by 6 triangles
	{
		int contributors[6] = {-1, -1, -1, -1, -1, -1};
		GLushort A;
		GLushort B;
		GLushort C;
		gdl::vec3 pointNormal;
		for (GLushort d = 0; d < depth; d++)
		{
			for (GLushort w = 0; w < width; w++)
			{
				GLushort point = w + width * d;

				// TODO this method is super slow
				// Contributing triangles
				// What are the triangles that use this point?
				size_t ci = 0;
				// Reset contributors
				for (int i = 0; i < 6; i++)
				{
					contributors[i] = -1;
				}

				// Min and max contributing triangle is based on row
				GLsizei row = d;
				// Each row has width-1 quads: double triangles
				GLsizei trisPerRow = (width-1) * 2;

				int minTri = (row-1) * trisPerRow;
				int maxTri = (row+1) * trisPerRow;
				if (minTri < 0) { minTri = 0;}
				if (maxTri >= triangleCount) { maxTri = triangleCount-1;};

				GLsizei firstTri = (GLsizei)minTri;
				GLsizei lastTri = (GLsizei)maxTri;

				for (GLsizei t = firstTri; t <= lastTri; t++)
				{
					if (mesh->GetTriangleIndices(t, A, B, C))
					{
						if (A == point || B == point || C == point)
						{
							contributors[ci] = (int)t;
							ci++;
						}
					}
				}
				// Now we have all triangles that use this point
				glm::vec3 normalSum = glm::vec3(0.0f,0.0f,0.0f);
				for (int i = 0; i < 6; i++)
				{
					int contri = contributors[i];
					if (contri != -1 && contri < triangleCount)
					{
						normalSum += triangleNormals[(size_t)contri];
						ci++;
					}
				}
				normalSum = glm::normalize(normalSum);
				pointNormal = gdl::vec3(normalSum.x, normalSum.y, normalSum.z);
				mesh->SetNormal(point, pointNormal);
				// printf("PN %d (%.1f, %.1f, %.1f)\n", point, pointNormal.x, pointNormal.y, pointNormal.z);
			}
		}
	}

	delete[] triangleNormals;
	return mesh;
}

TerrainMesh* TerrainGenerator::SetHeightsFromPNG ( TerrainMesh* mesh, gdl::PNGFile* png, float heightMultiplier, float width, float depth )
{
	for (int d = 0; d < depth; d++)
	{
		for (int w = 0; w < width; w++)
		{
			size_t i = (d * width + w);
			size_t vertexi = i * 3;
			mesh->positions[vertexi + Y] = png->GetGrayscale(w, d) * heightMultiplier;
		}
	}
	return mesh;
}



TerrainMesh* TerrainGenerator::GenerateFromPNG (float heightMultiplier, float uvRange, gdl::PNGFile* png )
{
	// The width and height are in pixels
	int depth = png->GetWidth();
	int width = png->GetHeight();



	TerrainMesh* mesh = CreateGridMesh(width, depth, uvRange);
	mesh = SetHeightsFromPNG(mesh, png, heightMultiplier, width, depth);
	mesh = CreateNormals(mesh, width, depth);

	return mesh;
}

TerrainMesh* TerrainGenerator::CalculateMatcapFromNormals( TerrainMesh* mesh)
{
	for(size_t i = 0; i < mesh->vertexCount; i++)
	{
		size_t vertexIndex = i * 3;
		glm::vec3 normal = glm::vec3(mesh->normals[vertexIndex + X], mesh->normals[vertexIndex + Y] , mesh->normals[vertexIndex + Z]);
		//normal = glm::normalize(normal);
		float u = (normal.x + 1.0f) * 0.5f;
		float v = (normal.y + 1.0f) * 0.5f;

		// size_t uvIndex = i * 2;
		// mesh->uvs[uvIndex + X] = u;
		// mesh->uvs[uvIndex + Y] = v;
		mesh->vertexColors[vertexIndex + X] = u;
		mesh->vertexColors[vertexIndex + Y] = v;
		mesh->vertexColors[vertexIndex + Z] = 0;
	}
	return mesh;
}

TerrainMesh* TerrainGenerator::CalculateMatcapFromCamera ( TerrainMesh* mesh, gdl::vec3 cameraDirection )
{
	glm::vec3 camDir = glm::vec3(cameraDirection.x, cameraDirection.y, cameraDirection.z);
	for(size_t i = 0; i < mesh->vertexCount; i++)
	{
		size_t vertexIndex = i * 3;
		glm::vec3 normal = glm::vec3(mesh->normals[vertexIndex + X],
								mesh->normals[vertexIndex + Y] ,
								mesh->normals[vertexIndex + Z]);

		glm::vec3 pos = glm::vec3(mesh->positions[vertexIndex + X],
							mesh->positions[vertexIndex + Y],
							mesh->positions[vertexIndex + Z]);

		glm::vec3 vertex_to_camera = camDir -  pos;

		vertex_to_camera = vertex_to_camera * (1.0f/ glm::length(vertex_to_camera));

		float dot = 2.0f * glm::dot(normal, vertex_to_camera);
		glm::vec3 reflection  = (normal * dot) - vertex_to_camera;

		float u = (0.5f) * 0.5f * reflection.x;
		float v = (0.5f) * 0.5f * reflection.y;

		// size_t uvIndex = i * 2;
		// mesh->uvs[uvIndex + X] = u;
		//mesh->uvs[uvIndex + Y] = v;
		mesh->vertexColors[vertexIndex + X] = u;
		mesh->vertexColors[vertexIndex + Y] = v;
		mesh->vertexColors[vertexIndex + Z] = 0;
	}
	return mesh;
}


