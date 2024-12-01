#pragma once
#include "camera.h"
#include <vector>


enum FlightStatus
{
	Future,
	Boarding,
	GateClosing
};

struct FlightInfo
{
	std::string company;
	std::string destination;
	std::string gate;
	FlightStatus status;
};

FlightInfo CreateInfo(std::string cmp, std::string dest, std::string gateN, FlightStatus stat);

class DeparturesScene
{
public:
	DeparturesScene();
	void Update();
	void Draw(Camera* camera);
	void DrawFlight(const FlightInfo& info);
	gdl::Font* dotFont = nullptr;

	std::vector<FlightInfo> flights;

	float destColumn;
	float gateColumn;
	float statusColumn;
	float linex;
	float liney;
	float linescale;
	u32 red = gdl::Colors::Red;
	u32 green = gdl::Colors::LightGreen;
	u32 white = gdl::Colors::White;
	u32 yellow = gdl::Colors::Yellow;
};
