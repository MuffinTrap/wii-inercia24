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
    static ROCKET_TRACK departure_hangarX;
    static ROCKET_TRACK departure_hangarY;
    static ROCKET_TRACK departure_hangarZ;


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

        departure_hangarX = gdl::RocketSync::GetTrack("departure:hangarX");
        departure_hangarY = gdl::RocketSync::GetTrack("departure:hangarY");
        departure_hangarZ = gdl::RocketSync::GetTrack("departure:hangarZ");
#endif

    flights.push_back(CreateInfo("BEER SQUADRON", "BRUSSELS/TERRA", "A1", Future));
    flights.push_back(CreateInfo("SILJA LINE", "STOCKHOLM/TERRA", "A2", Future));
    flights.push_back(CreateInfo("LOVE ROCKET", "BYTE ST./VENUS", "A3", Boarding));
    flights.push_back(CreateInfo("FINNAIR", "JYVAESKYLAE/TERRA", "B4", Future));
    flights.push_back(CreateInfo("NOT THE BEES", "ZWOLLE/TERRA", "B5", Future));
}

void DeparturesScene::Update()
{
}

FlightInfo CreateInfo ( std::string cmp, std::string dest, std::string gateN, FlightStatus stat )
{
    FlightInfo info;
    info.company = cmp;
    info.destination = dest;
    info.gate = gateN;
    info.status = stat;
    return info;
}


static float Display(gdl::Font* font, float x, float y, u32 color, float scale, const std::string& text)
{
    font->Print(color, x, y, scale, gdl::LJustify, gdl::LJustify, text.c_str());
    return scale * text.length();
}

void DeparturesScene::DrawFlight ( const FlightInfo& info )
{
    Display(dotFont, linex, liney, yellow, linescale, info.company);
    Display(dotFont, destColumn, liney, yellow, linescale, info.destination );
    Display(dotFont, gateColumn, liney, white, linescale, info.gate );
    u32 col = white;
    switch(info.status)
    {
        case Future:
            Display(dotFont, statusColumn, liney, col, linescale, "IN FUTURE" );
            break;
        case Boarding:
            col = green;
            Display(dotFont, statusColumn, liney, col, linescale, "BOARDING" );
            break;
        case GateClosing:
            Display(dotFont, statusColumn, liney, col, linescale, "GATE CLOSING" );
        break;
    };
}


void DeparturesScene::Draw(Camera* camera, gdl::Scene* spacePortScene, gdl::Node* hangarNode)
{
    camera->SetupFor3D();
    camera->LookAtTarget();

    // Draw the hangar as a background
    glPushMatrix();
    glTranslatef(gdl::RocketSync::GetFloat(departure_hangarX),
                 gdl::RocketSync::GetFloat(departure_hangarY),
                 gdl::RocketSync::GetFloat(departure_hangarZ));
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        spacePortScene->DrawNode(hangarNode);
    glPopMatrix();


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


        float scale = 1.0f + gdl::RocketSync::GetFloat(departure_textScale);
        // Draw company or callsign
        float x = 0;
        float y = 0;

        Display(dotFont, x, y, white, scale, "FLIGHT                DESTINATION            GATE STATUS");
        y -= scale * 2.0f;

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
        destColumn = x;
        x += Display(dotFont, x, y, yellow, scale, destination );
        gateColumn = x;
        x += Display(dotFont, x, y, white, scale, gate ) + 2 * scale;

        statusColumn = x;
        if (showText == 1)
        {
            Display(dotFont, x, y, green, scale, boarding );
        }
        else
        {
            Display(dotFont, x, y, red, scale, closing );
        }
        linescale = scale;
        linex = 0;
        liney = y;
        for (size_t i = 0; i < flights.size(); i++)
        {

            liney -= scale + 0.5f * scale;
            DrawFlight(flights[i]);
        }
    glPopMatrix();

}



