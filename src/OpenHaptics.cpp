#include "OpenHaptics.h"/*
#include <HDU/hduError.h>
#include <CoreEngine.h>
#include <HL/hl.h>
#include <HDU/hdu.h>
#include <HDU/hduError.h>
*/
/*
namespace
{
    HDCallbackCode HDCALLBACK gravityWellCallback(void *data)
    {
        static const double stiffness = 0.075, gravityWellInfluence = 40;
        static const glm::dvec3 wellPosition(0);
        glm::dvec3 position, force, vecToWell;
        HDErrorInfo error;

        HHD device = hdGetCurrentDevice();
        hdBeginFrame(device);

            hdGetDoublev(HD_CURRENT_POSITION, glm::value_ptr(position) );
            vecToWell = wellPosition-position;

            if( glm::length( vecToWell ) < gravityWellInfluence )
            {
                force.x = vecToWell.x*stiffness;
                force.y = vecToWell.y*stiffness;
                force.z = vecToWell.z*stiffness;
            }
            hdSetDoublev(HD_CURRENT_FORCE, glm::value_ptr(force) );

        hdEndFrame(device);

        if (HD_DEVICE_ERROR(error = hdGetError()))
        {
            //de::io::error<< "Error detected in gravityWellCallback.\n" << error;

            if (hduIsSchedulerError(&error))
            {
                return HD_CALLBACK_DONE;
            }
        }
        return HD_CALLBACK_CONTINUE;
    }

}
*/

namespace de
{
    namespace haptics
    {
        OpenHaptics::OpenHaptics() :active(false)
        {/*
            hapticsDevice = hdInitDevice(HD_DEFAULT_DEVICE);

            if ( !HD_DEVICE_ERROR(error = hdGetError()) )
            {
                active = true;
                de::io::log << "Haptic Device | " << hdGetString(HD_DEVICE_MODEL_TYPE) << " | " << hdGetString(HD_DEVICE_VENDOR) << " | " << hdGetString(HD_DEVICE_DRIVER_VERSION) << "\n";
            }
            else
            {
                de::io::error<< "Failed to initialize haptic device, printing error message\n" << error;
                //throw();
            }

            if(active)
            {
                hdStartScheduler();
                if (!HD_DEVICE_ERROR(error = hdGetError()))
                {
                    hdEnable(HD_FORCE_OUTPUT);
                    hdMakeCurrentDevice(hapticsDevice);
                    int lol(0);
                    void* aPointer = &lol;

                    hdScheduleSynchronous( gravityWellCallback, (void*)aPointer, HD_MAX_SCHEDULER_PRIORITY );
                    //light = hdScheduleAsynchronous( setLight, (void*)0, HD_DEFAULT_SCHEDULER_PRIORITY );

                }
                else
                {
                    de::io::error << "Failed to start scheduler, printing error message\n" << error;
                }
            }


*/
        }

        OpenHaptics::~OpenHaptics()
        {
            //hdStopScheduler();
            //hdUnschedule(hGravityWell);
            //hdUnschedule(light);
            //hdDisableDevice(hapticsDevice);
        }


        void OpenHaptics::Calibrate()
        {

        }

        unsigned int OpenHaptics::getDevice()
        {
            return 0;
        }
    }
}
