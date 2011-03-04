#ifndef IHAPTICS_H
#define IHAPTICS_H

namespace de
{
    namespace haptics
    {
        class IHaptics
        {
            public:
                /** Default constructor */
                IHaptics() {};
                /** Default destructor */
                virtual ~IHaptics() {};

                virtual unsigned int getDevice() = 0;
            protected:
            private:
        };


        class NullHaptics : public IHaptics
        {
            public:
                NullHaptics() {}
                virtual ~NullHaptics() {}
                //! Loads the Music from the filepath "_filePath"
                unsigned int getDevice() { return 0; }


            protected:
            private:
        };
    }
}

#endif // IHAPTICS_H
