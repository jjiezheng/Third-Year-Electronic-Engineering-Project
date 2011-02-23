#ifndef GAMESERVICES_H
#define GAMESERVICES_H

#include "IAudio.h"
#include "IGraphics.h"
#include "IResources.h"
#include "IHaptics.h"
#include "IFont.h"
//#include "IInput.h"

namespace de
{
	class Engine
	{
		public:
			static void Initialize();

			static IAudio& Audio()                       { return *audioService;     }
			static de::graphics::IGraphics& Graphics()   { return *graphicsService;  }
			static de::resources::IResources& Resources(){ return *resourcesService; }
			static de::haptics::IHaptics& Haptics()      { return *hapticsService; }
			static de::gui::IFont& Fonts()				 { return *fontsService; }
			//static de::input::IInput& Input()            { return *inputService; }

			static void Register( IAudio* _service );
			static void Register( de::gui::IFont* _service );
			static void Register( de::graphics::IGraphics* _service );
			static void Register( de::resources::IResources* _service );
			static void Register( de::haptics::IHaptics* _service );
		   // static void Register( de::input::IInput* _service );
			static void clear();
		private:
			static de::gui::IFont* fontsService;
			static de::gui::NullFont nullfontsService;

			static IAudio* audioService;
			static NullAudio nullAudioService;

			static de::graphics::IGraphics* graphicsService;
			static de::graphics::NullGraphics nullGraphicsService;

			static de::resources::IResources* resourcesService;
			static de::resources::NullResources nullResourcesService;

			static de::haptics::IHaptics* hapticsService;
			static de::haptics::NullHaptics nullHapticsService;
	/*
			static de::input::IInput* inputService;
			static de::input::NullInput nullInputService;*/
	};
};


#endif // GAMESERVICES_H
