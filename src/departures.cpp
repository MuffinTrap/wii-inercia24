#include "departures.h"
#include "draw3d.h"
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
    dotFont = gdl::LoadFontCustom("assets/led_counter50x64_v2.png", 25,32, 10, " -./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
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



