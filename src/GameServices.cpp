#include "GameServices.h"

IAudio* de::Engine::audioService = NULL;
NullAudio de::Engine::nullAudioService;

de::gui::IFont* de::Engine::fontsService = NULL;
de::gui::NullFont de::Engine::nullfontsService;


de::graphics::IGraphics* de::Engine::graphicsService = NULL;
de::graphics::NullGraphics de::Engine::nullGraphicsService;

de::resources::IResources* de::Engine::resourcesService = NULL;
de::resources::NullResources de::Engine::nullResourcesService;

de::haptics::IHaptics* de::Engine::hapticsService = NULL;
de::haptics::NullHaptics de::Engine::nullHapticsService;

namespace de
{
	void Engine::Initialize()
	{
		audioService = &nullAudioService;
		graphicsService = &nullGraphicsService;
		resourcesService = &nullResourcesService;
		fontsService = &nullfontsService;
	}

	void Engine::Register( IAudio* _service )
	{
		if( _service == NULL )
		{
			audioService = &nullAudioService;
		}
		else
		{
			if( audioService == NULL )
			{
				audioService = _service;
			}
			else
			{
				delete audioService;
				audioService = _service;
			}
		}
	}

	void Engine::Register( de::gui::IFont* _service )
	{
		if( _service == NULL )
		{
			fontsService = &nullfontsService;
		}
		else
		{
			if( audioService == NULL )
			{
				fontsService = _service;
			}
			else
			{
				delete fontsService;
				fontsService = _service;
			}
		}
	}

	void Engine::Register( de::graphics::IGraphics* _service )
	{
		if( _service == NULL )
		{
			graphicsService = &nullGraphicsService;
		}
		else
		{
			if( graphicsService == NULL )
			{
				graphicsService = _service;
			}
			else
			{
				delete graphicsService;
				graphicsService = _service;
			}
		}
	}

	void Engine::Register( de::resources::IResources* _service )
	{
		if( _service == NULL )
		{
			resourcesService = &nullResourcesService;
		}
		else
		{
			if( resourcesService == NULL )
			{
				resourcesService = _service;
			}
			else
			{
				delete resourcesService;
				resourcesService = _service;
			}
		}
	}

	void Engine::Register( de::haptics::IHaptics* _service )
	{
		if( _service == NULL )
		{
			hapticsService = &nullHapticsService;
		}
		else
		{
			if( hapticsService == NULL )
			{
				hapticsService = _service;
			}
			else
			{
				delete hapticsService;
				hapticsService = _service;
			}
		}
	}

	void Engine::clear()
	{
		delete resourcesService;
		resourcesService = NULL;

		delete fontsService;
		fontsService = NULL;

		delete audioService;
		audioService = NULL;

		delete graphicsService;
		graphicsService = NULL;

		delete hapticsService;
		hapticsService = NULL;
	}
}