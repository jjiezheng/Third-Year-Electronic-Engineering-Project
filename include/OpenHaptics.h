#ifndef OPENHAPTICS_H
#define OPENHAPTICS_H

#include "IHaptics.h"
//#include <HD/hd.h>


namespace de
{
    namespace haptics
    {
        class OpenHaptics : public IHaptics
        {
            public:
                /** Default constructor */
                OpenHaptics();
                /** Default destructor */
                virtual ~OpenHaptics();

                unsigned int getDevice();
            protected:
            private:
                void Calibrate();

                bool active;
                /*HHD hapticsDevice;
                HDErrorInfo error;
                HDSchedulerHandle hGravityWell, light;*/


        };
    }
}

#endif // OPENHAPTICS_H
