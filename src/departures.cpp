#include "departures.h"
#include <mgdl.h>
#include "../rocket/mgdl-rocket.h"

#include "../rocket_tracks.h"

#ifndef SYNC_PLAYER

    static ROCKET_TRACK departure_text;
    static ROCKET_TRACK departure_textScale;
    static ROCKET_TRACK departure_textX;
    static ROCKET_TRACK departure_textY;
    static ROCKET_TRACK departure_width;
    static ROCKET_TRACK departure_height;
    static ROCKET_TRACK departure_depth;


#endif

DeparturesScene::DeparturesScene()
{
    // dotFont = gdl::LoadFontCustom("assets/dot_font.png", 44,64, '1', 10);
    dotFont = gdl::LoadFontCustom("assets/led_counter50x64_v2.png", 50,64, 10, " -./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    gdl_assert_print(dotFont != nullptr, "Dot font failed to load");
#ifndef SYNC_PLAYER
        // Departures board
        departure_text = gdl::RocketSync::GetTrack("departure:text");
        departure_textX = gdl::RocketSync::GetTrack("departure:textX");
        departure_textY = gdl::RocketSync::GetTrack("departure:textY");
        departure_textScale = gdl::RocketSync::GetTrack("departure:textScale");
        // Board dimensions
        departure_width = gdl::RocketSync::GetTrack("departure:width");
        departure_height = gdl::RocketSync::GetTrack("departure:height");
        departure_depth = gdl::RocketSync::GetTrack("departure:depth");
#endif
}

void DeparturesScene::Update()
{
}

static float Display(gdl::Font* font, float x, float y, u32 color, float scale, const std::string& text)
{
    font->Print(color, x, y, scale, gdl::LJustify, gdl::LJustify, text.c_str());
    return scale * text.length();
}

static void Quad(glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d, const::glm::vec3& normal)
{
	// bottom
	glNormal3f(normal.x, normal.y, normal.z);
	glVertex3f(a.x, a.y, a.z);

	// top 1 and 2
	glNormal3f(normal.x, normal.y, normal.z);
	glVertex3f(b.x, b.y, b.z);
	glNormal3f(normal.x, normal.y, normal.z);
	glVertex3f(c.x, c.y, c.z);

	// bottom2
	glNormal3f(normal.x, normal.y, normal.z);
	glVertex3f(d.x, d.y, d.z);
}

static void SolidCube(float size)
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

void DeparturesScene::Draw(Camera* camera)
{
    glEnable(GL_DEPTH_TEST);
    camera->SetupFor3D();
    camera->LookAtTarget();

    glPushMatrix();
        // Draw departures display
        glColor3f(0.1f, 0.1f, 0.1f);
        glScalef(
            gdl::RocketSync::GetFloat(departure_width),
            gdl::RocketSync::GetFloat(departure_height),
            gdl::RocketSync::GetFloat(departure_depth)
        );
        SolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
        // Draw text on the display surface
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(
            gdl::RocketSync::GetFloat(departure_textX),
            gdl::RocketSync::GetFloat(departure_textY),
            gdl::RocketSync::GetFloat(departure_depth)/2.0 + 0.01f
        );

        static const std::string company = "MARMOT SPACELINES";
        static const std::string callsign = "MS 2024";
        static const std::string destination = "INERCIA ST./EUROPA";
        static const std::string gate = "A5";
        static const std::string boarding = "BOARDING";
        static const std::string closing = "GATE CLOSING";

        u32 red = gdl::Colors::Red;
        u32 green = gdl::Colors::LightGreen;
        u32 white = gdl::Colors::White;
        u32 yellow = gdl::Colors::Yellow;

        float scale = 1.0f + gdl::RocketSync::GetFloat(departure_textScale);
        // Draw company or callsign
        float x = 0;
        float y = 0;

        Display(dotFont, x, y, white, scale, "FLIGHT                DESTINATION            GATE STATUS");
        y -= scale;

        int showText = gdl::RocketSync::GetInt(departure_text);

        if (showText == 0)
        {
            Display(dotFont, x, y, yellow, scale, company);
        }
        else
        {
            Display(dotFont, x, y, yellow, scale, callsign);
        }

        float nameLength = (company.length()) * scale;
        x += nameLength;
        x += Display(dotFont, x, y, yellow, scale, destination );
        x += Display(dotFont, x, y, white, scale, gate ) + 2 * scale;
        if (showText == 1)
        {
            Display(dotFont, x, y, green, scale, boarding );
        }
        else
        {
            Display(dotFont, x, y, red, scale, closing );
        }
    glPopMatrix();
}



