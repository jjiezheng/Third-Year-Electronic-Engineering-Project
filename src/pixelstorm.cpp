#include "pixelstorm.h"
#include "Font.h"

#include "Monitor.h"

#include "GameServices.h"
#include "Sound.h"
#include "LoggedAudio.h"
#include "Graphics.h"
#include "LoggedGraphics.h"
#include "ResourceManager.h"
#include "OpenHaptics.h"
#include "Font.h"


using namespace de::graphics;
using namespace de::classes;
using namespace de::core;
using namespace de::filesystem;
using namespace de::enums;
using namespace de::resources;
using namespace de::gui;


namespace Attrition
{
    DifferenceEngine *Game = NULL;


    DifferenceEngine::DifferenceEngine() :
        currentState(NULL),
        nextState(NULL),
        Delta( 60.0 ),
        Option( de::state::NOTHING ),
        failBit(false),
        exit(false)
    {
        initBase();
		deltaLog.open ("../deltaTimes.csv");
		deltaLog << "Delta\n";
        if( !failBit )
        {
            Roots = new Root();

            de::Engine::Register( new SDLSound() );
            de::Engine::Register( new Graphics() );
            de::Engine::Register( new de::haptics::OpenHaptics() );
            de::Engine::Register( new ResourceManager( de::Engine::Graphics().getVideoSettings() ) );
			de::Engine::Register( new FontManager() );

            de::Engine::Resources().load( SHADERS | 
										  TEXTURES | 
										  MESHES | 
										  MUSIC | 
										  SOUNDEFFECTS );

            overlay = new de::state::Overlay;
        }
    }

    DifferenceEngine::~DifferenceEngine()
    {
        if( !Sticks.empty() )
        {
            for( unsigned int i(0); i < Sticks.size(); ++i )
            {
                SDL_JoystickClose( Sticks[i] );
            }
        }

        delete currentState;
        de::Engine::clear();

		deltaLog.close();
        de::io::log << "Closing SDL... ";
        SDL_Quit();
        de::io::log << "Done\n";
    }

    void DifferenceEngine::initBase()
    {
        de::io::log << "Staring PixelStorm.\nStarting SDL... ";
        if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK  ) == -1 )
        //if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
        {
            de::io::error << "Failure to start SDL\n";
            lastError = "Failure to start SDL\n";
            failBit = true;
        }
        else
        {
            de::io::log << "Done\n";

            SDL_EnableUNICODE(SDL_ENABLE);
            SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
			SDL_WM_SetCaption( "Difference Engine", NULL );

            //SDL_JoystickOpen( 0 );
            for( int i(0); i < SDL_NumJoysticks(); ++i )
            {
                Sticks.push_back( SDL_JoystickOpen( i ) );
                de::io::log << "opening joystick " << i << "\n";
            }
        }
    }

    void DifferenceEngine::setNextState( de::state::State *_nextState )
    {
        nextState = _nextState;
    }

    void DifferenceEngine::changeState()
    {
        if( Option == de::state::EXIT )
        {
            exit = true;
        }
        else
        {
            if( nextState != NULL )
            {
                delete currentState;
                currentState = nextState;
                nextState = NULL;
                //de::Engine::Audio().stopMusic();
            }
        }
    }

    bool DifferenceEngine::running()
    {
        return (!exit);

    }

    void DifferenceEngine::runCurrentState()
    {
        if( !exit )
        {
			//Delta.start();
            handleEvents();
            logic();
            render();

            overlay->graph( perfTimer.stopstartAndSample()/*, GRAPH_MAIN_IDLETIME*/ );
            overlay->endOfFrame();

			Delta.frame_cap();
            changeState();
        }
    }

    void DifferenceEngine::handleEvents()
    {
        perfTimer.start();

        SDL_PumpEvents();
        while( SDL_PollEvent( &event ) )
        {
            localHandleEvent( event );

            if( overlay->handleEvents( event ) )
            {
                currentState->handleEvents( event );
            }

            if( event.type == SDL_KEYDOWN )
            {
                //Adjust the velocity
                if( event.key.keysym.sym )
                {
                    if( event.key.keysym.sym == SDLK_ESCAPE )
                    {
                        Option = de::state::EXIT;
                        changeState();
                    }
                }
            }

            if( event.type == SDL_QUIT )
            {
                Option = de::state::EXIT;
                changeState();
            }
        }


        overlay->graph( perfTimer.stopstartAndSample()/*, GRAPH_MAIN_CONTROL*/ );
    }

    void DifferenceEngine::localHandleEvent( const SDL_Event &_event )
    {
        if( _event.type == SDL_KEYDOWN )
        {
            Uint8 *keystate = SDL_GetKeyState(NULL);

            if( keystate[SDLK_LALT] && ( keystate[SDLK_RETURN] || keystate[SDLK_KP_ENTER] )  )
            {
				
				bool fullscreen = de::Engine::Graphics().getVideoSettings().fullScreen;
                de::Engine::Resources().free( TEXTURES | SHADERS | MESHES );
                de::Engine::Graphics().fullscreen( !fullscreen );
            }

            if( keystate[SDLK_LALT] && ( keystate[SDLK_RETURN] || keystate[SDLK_BACKSPACE] )  )
            {
                de::Engine::Resources().free( SHADERS | TEXTURES | MESHES | MUSIC | SOUNDEFFECTS );

                de::Engine::clear();
				de::Engine::Register( new SDLSound() );
				de::Engine::Register( new Graphics() );
				de::Engine::Register( new de::haptics::OpenHaptics() );
				de::Engine::Register( new ResourceManager( de::Engine::Graphics().getVideoSettings() ) );
				de::Engine::Register( new FontManager() );

				de::Engine::Resources().load( MUSIC | SOUNDEFFECTS );
                de::events::pushEvent( de::enums::events::OPENGL_RELOAD );
            }
        }

        else if( _event.type == SDL_VIDEORESIZE )
        {
            int screenWidth(0), screenHeight(0);

            if( _event.resize.w )
            {
                screenWidth = _event.resize.w;
                screenHeight = (int)(_event.resize.w/1.6);

                de::Engine::Resources().free( TEXTURES | MESHES | SHADERS );
                de::Engine::Graphics().resize( screenWidth, screenHeight );
            }
            else if( _event.resize.h )
            {
                screenWidth = (int)(_event.resize.w*1.6);
                screenHeight = _event.resize.h;

                de::Engine::Resources().free( TEXTURES | MESHES | SHADERS );
                de::Engine::Graphics().resize( screenWidth, screenHeight );
            }
        }

		else if( _event.type == SDL_USEREVENT )
		{
			if( _event.user.code == de::enums::events::OPENGL_RELOAD )
			{
				de::Engine::Resources().load( TEXTURES | SHADERS | MESHES );
				de::Engine::Fonts().reload();
			}
		}
    }

    void DifferenceEngine::logic()
    {
        if( overlay->logic( Delta.getTime(), nextState, Option ) )
        {
            currentState->logic( Delta.getTime(), nextState, Option );
        }

        de::sys::getFrameRate( Delta.getFrameRate() );

		deltaLog << Delta.getTime() << "\n";
        overlay->graph( perfTimer.stopstartAndSample()/*, GRAPH_MAIN_LOGIC*/ );
    }

    void DifferenceEngine::render()
    {
        currentState->render();
		overlay->render();

        overlay->graph( perfTimer.stopstartAndSample()/*, GRAPH_MAIN_RENDER*/ );


        de::sys::getFrameDetails( de::Engine::Graphics().render() );
        overlay->graph( perfTimer.stopstartAndSample()/*, GRAPH_MAIN_RENDERING*/ );
    }

}
