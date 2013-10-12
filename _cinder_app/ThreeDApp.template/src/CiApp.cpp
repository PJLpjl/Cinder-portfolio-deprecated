#include "cinder/app/AppBasic.h"
#include "cinder/ImageIo.h"
#include "cinder/Camera.h"

#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Vbo.h"
#include "cinder/params/Params.h"

#include "../../../_common/MiniConfig.h"

using namespace ci;
using namespace ci::app;
using namespace std;

struct CiApp : public AppBasic 
{
    void prepareSettings(Settings *settings)
    {
        readConfig();
        
        settings->setWindowPos(0, 0);
        settings->setWindowSize(WIN_WIDTH, WIN_HEIGHT);
    }

    void setup()
    {
        mParams = params::InterfaceGl("params", Vec2i(300, getConfigUIHeight()));
        setupConfigUI(&mParams);
    }

    void keyUp(KeyEvent event)
    {
        if (event.getCode() == KeyEvent::KEY_ESCAPE)
        {
            quit();
        }
    }

    void update()
    {
    
    }

    void draw()
    {
        gl::clear(ColorA::black());

        mParams.draw();
    }

private:
    params::InterfaceGl mParams;
};

CINDER_APP_BASIC(CiApp, RendererGl)
