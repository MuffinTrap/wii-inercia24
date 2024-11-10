#include <mgdl.h>
#include "src/scene.h"
#include <stdio.h>


static Scene scene;
//---------------------------------------------------------------------
void init()
{
    glViewport(0, 0, gdl::GetScreenWidth(), gdl::GetScreenHeight());
    gdl::InitOrthoProjection();

    glClearColor(247.0f/255.0f, 1.0f, 174.0f/255.0f, 0.0f);

    scene.Init();
}
// Rendering callback. glFlush etc.. is done automatically after it
void render()
{
    gdl::cross_glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    scene.Draw();
}


// Called before render()
void update()
{
    if (gdl::GetController(0).ButtonPress(gdl::WiiButtons::ButtonHome))
    {
        gdl::Platform& plat = gdl::Platform::GetPlatform();
        plat.DoProgramExit();
    }
}

int main()
{
    gdl::InitSystem(
        gdl::ScreenAspect::Screen4x3,
        init,
        update,  // std::function callbacks
        render
    );
}
