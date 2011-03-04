#ifndef UIBASEDATA_H
#define UIBASEDATA_H

#include "CoreEngine.h"
#include "Coords.h"

namespace de
{
    namespace enums
    {
        enum UIDataOptions
        {
            UI_None = 0,
            UI_WRITE = 1,
            UI_BOXSNUG = 1 << 1,
        };
    }
}

class textBox
{
    public:
        textBox();
        virtual ~textBox(){};

        bool handleEvent( const SDL_Event &_event, de::enums::fboTypes _fbo, bool _actAsButton = false );
        void reLoadTextures();
        void render( de::enums::fboTypes _fbo, de::enums::fboTypes _fbo2, bool _actAsButton = false );


        bool hoverEvent, hasFocus, formFittingBox;
        de::classes::Poly boxLocation;
        de::classes::SpriteString valueText;
    protected:
        de::classes::Vector mousePosition;

};





class UIInterface
{
    public:
        UIInterface() {}
        virtual ~UIInterface() {}

        virtual void handleEvent( const SDL_Event &_event ) = 0;
        virtual bool logic( const Uint32 &_deltaTicks ) = 0;
        virtual void reLoadTextures() = 0;
        virtual void render( de::enums::fboTypes _bottomLayer, de::enums::fboTypes _topLayer ) = 0;
};

template<class T>
class UIbaseData : public UIInterface
{
    public:
        UIbaseData() :
            pointerNotSet(true),
            changeEvent(false),
            write(false),
            fboLayer(de::enums::FBO_UI),
            offset(100,0)
        {}

        virtual ~UIbaseData(){};

        virtual void set( const std::string &_name,
                          T *_value,
                          const de::classes::Vector &_position,
                          const int &_priority,
                          de::enums::Align _align,
                          bool _write = true ) = 0;

    protected:
        bool pointerNotSet, changeEvent, write;

        de::enums::fboTypes fboLayer;
        de::classes::Vector offset;
        de::classes::SpriteString name;

        textBox box;

        T localValue;
        T *ValuePtr;
    private:
};


class UIEntry : public UIInterface
{
    public:
        UIEntry() :
            pointerNotSet(true),
            changeEvent(false),
            write(false),
            fboLayer(de::enums::FBO_UI),
            offset(100,0)
        {}

        virtual ~UIEntry(){};

        template<typename T>
        void set( const std::string &_name,
                  T *_value,
                  const de::classes::Vector &_position,
                  const int &_priority,
                  de::enums::Align _align,
                  bool _write = true )
        {

        }

    protected:
        bool pointerNotSet, changeEvent, write;

        de::enums::fboTypes fboLayer;
        de::classes::Vector offset;
        de::classes::SpriteString name;

        textBox box;

        boost::variant<bool, int, float> localValue;
        boost::variant<bool*, int*, float*> ValuePtr;
    private:
};




class UIDataBoolButton : public UIbaseData<bool>
{
    public:
        UIDataBoolButton() {}
        UIDataBoolButton( const std::string &_name,
                          bool *_value,
                          const de::classes::Vector &_position,
                          const int &_priority,
                          de::enums::Align _align,
                          bool _write = true);
        ~UIDataBoolButton() {}

        void set( const std::string &_name,
                  bool *_value,
                  const de::classes::Vector &_position,
                  const int &_priority,
                  de::enums::Align _align,
                  bool _write = true );


        void handleEvent( const SDL_Event &_event );
        bool logic( const Uint32 &_deltaTick  );
        void reLoadTextures();
        void render( de::enums::fboTypes _bottomLayer, de::enums::fboTypes _topLayer );

    private:
};

class UIDataEventButtonBF : public UIbaseData<int>
{
    public:
        UIDataEventButtonBF() {}
        UIDataEventButtonBF( const std::string &_name,
                             de::enums::events::SDL_EVENT_CODES _backButton,
                             de::enums::events::SDL_EVENT_CODES _forwardButton,
                             const de::classes::Vector &_position,
                             const int &_priority,
                             de::enums::Align _align );
        ~UIDataEventButtonBF() {}

        void set( const std::string &_name,
                  de::enums::events::SDL_EVENT_CODES _backButton,
                  de::enums::events::SDL_EVENT_CODES _forwardButton,
                  const de::classes::Vector &_position,
                  const int &_priority,
                  de::enums::Align _align );


        void set( const std::string &_name,
                  int *_value,
                  const de::classes::Vector &_position,
                  const int &_priority,
                  de::enums::Align _align,
                  bool _write = true );

        void handleEvent( const SDL_Event &_event );
        bool logic( const Uint32 &_deltaTicks );
        void reLoadTextures();
        void render( de::enums::fboTypes _bottomLayer, de::enums::fboTypes _topLayer );

    private:
        textBox otherBox;

        de::classes::SpriteString left, right;
        de::enums::events::SDL_EVENT_CODES backButtonEvent, forwardButtonEvent;
};

class UIDataBool : public UIbaseData<bool>
{
    public:
        UIDataBool() {}
        UIDataBool( const std::string &_name,
                    bool *_value,
                    const de::classes::Vector &_position,
                    const int &_priority,
                    de::enums::Align _align,
                    bool _write = true );
        ~UIDataBool() {}

        void set( const std::string &_name,
                  bool *_value,
                  const de::classes::Vector &_position,
                  const int &_priority,
                  de::enums::Align _align,
                  bool _write = true );

        void handleEvent( const SDL_Event &_event );
        bool logic( const Uint32 &_deltaTicks );
        void reLoadTextures();
        void render( de::enums::fboTypes _bottomLayer, de::enums::fboTypes _topLayer );

    private:
};

class UIDataInt : public UIbaseData<int>
{
    public:
        UIDataInt() {}
        UIDataInt( const std::string &_name,
                   int *_value,
                   const de::classes::Vector &_position,
                   const int &_priority,
                   de::enums::Align _align,
                   bool _write = true );
        ~UIDataInt() {}

        void set( const std::string &_name,
                  int *_value,
                  const de::classes::Vector &_position,
                  const int &_priority,
                  de::enums::Align _align,
                  bool _write = true );

        void handleEvent( const SDL_Event &_event );
        bool logic( const Uint32 &_deltaTicks );
        void reLoadTextures();
        void render( de::enums::fboTypes _bottomLayer, de::enums::fboTypes _topLayer );

    private:
};

class UIDataFloat : public UIbaseData<float>
{
    public:
        UIDataFloat();
        UIDataFloat( const std::string &_name,
                     float *_value,
                     const de::classes::Vector &_position,
                     const int &_priority,
                     de::enums::Align _align,
                     bool _write = true );
        ~UIDataFloat() {}

        void set( const std::string &_name,
                  float *_value,
                  const de::classes::Vector &_position,
                  const int &_priority,
                  de::enums::Align _align,
                  bool _write = true );


        void handleEvent( const SDL_Event &_event );
        bool logic( const Uint32 &_deltaTicks );
        void reLoadTextures();
        void render( de::enums::fboTypes _bottomLayer, de::enums::fboTypes _topLayer );

    private:
        bool  decimalPoint;
        int decimalPosition;
};

class UIDataVector : public UIbaseData<de::classes::Vector>
{
    public:
        UIDataVector();
        UIDataVector( const std::string &_name,
                      de::classes::Vector *_value,
                      const de::classes::Vector &_position,
                      const int &_priority,
                      de::enums::Align _align,
                      bool _write = true );
        ~UIDataVector() {}

        void set( const std::string &_name,
                  de::classes::Vector *_value,
                  const de::classes::Vector &_position,
                  const int &_priority,
                  de::enums::Align _align,
                  bool _write = true );


        void handleEvent( const SDL_Event &_event );
        bool logic( const Uint32 &_deltaTicks );
        void reLoadTextures();
        void render( de::enums::fboTypes _bottomLayer, de::enums::fboTypes _topLayer );

    private:
        bool  decimalPointX, decimalPointY;
        int decimalPositionX, decimalPositionY;

        textBox boxY;
};

class UIDataString : public UIbaseData<std::string>
{
    public:
        UIDataString();
        UIDataString( const std::string &_name,
                      std::string *_string,
                      const de::classes::Vector &_position,
                      const int &_priority,
                      de::enums::Align _align,
                      bool _write = true );

        UIDataString( const std::string &_name,
                      std::string *_string,
                      const std::vector<std::string> &_Options,
                      const std::vector<std::string> &_OptionsNames,
                      const de::classes::Vector &_position,
                      const int &_priority,
                      de::enums::Align _align,
                      bool _write = true );

        ~UIDataString() {}

        void set( const std::string &_name,
                  std::string *_string,
                  const de::classes::Vector &_position,
                  const int &_priority,
                  de::enums::Align _align,
                  bool _write = true );



        void set( const std::string &_name,
                  std::string *_string,
                  const std::vector<std::string> &_Options,
                  const std::vector<std::string> &_OptionsNames,
                  const de::classes::Vector &_position,
                  const int &_priority,
                  de::enums::Align _align,
                  bool _write = true );

        void handleEvent( const SDL_Event &_event );
        bool logic( const Uint32 &_deltaTicks );
        void reLoadTextures();
        void render( de::enums::fboTypes _bottomLayer, de::enums::fboTypes _topLayer );

    private:
        bool usingOptions;

        std::vector<std::string> Options;
        std::vector<std::string>::iterator iterOptions;
};



template<class T>
class UIbaseDataEnum : public UIInterface
{
    public:
        UIbaseDataEnum() :
            pointerNotSet(true),
            changeEvent(false),
            write(false),
            fboLayer(de::enums::FBO_UI),
            offset(100,0)
        {}

        virtual ~UIbaseDataEnum(){};

        virtual void set( const std::string &_name,
                          T *_string,
                          const std::vector<T> &_Options,
                          const std::vector<std::string> &_OptionsNames,
                          const de::classes::Vector &_position,
                          const int &_priority,
                          de::enums::Align _align,
                          bool _write = true ) = 0;

        virtual void handleEvent( const SDL_Event &_event ) = 0;
        virtual bool logic( const Uint32 &_deltaTick  ) = 0;
        virtual void reLoadTextures() = 0;
        virtual void render( de::enums::fboTypes _fbo, de::enums::fboTypes _fbo2 ) = 0;
    protected:
        bool pointerNotSet, changeEvent, write;

        de::enums::fboTypes fboLayer;
        de::classes::Vector offset;
        de::classes::SpriteString name;

        textBox box;

        T localValue;
        T *ValuePtr;
    private:
};



class UIDataEnum : public UIbaseDataEnum<int>
{
    public:
        UIDataEnum() {}
        UIDataEnum( const std::string &_name,
                    int *_value,
                    const std::vector<int> &_Options,
                    const std::vector<std::string> &_OptionsNames,
                    const de::classes::Vector &_position,
                    const int &_priority,
                    de::enums::Align _align,
                    bool _write = true );
        ~UIDataEnum() {}

        void set( const std::string &_name,
                  int *_value,
                  const std::vector<int> &_Options,
                  const std::vector<std::string> &_OptionsNames,
                  const de::classes::Vector &_position,
                  const int &_priority,
                  de::enums::Align _align,
                  bool _write = true );

        void handleEvent( const SDL_Event &_event );
        bool logic( const Uint32 &_deltaTicks );
        void reLoadTextures();
        void render( de::enums::fboTypes _fbo, de::enums::fboTypes _fbo2 );

    private:

        std::vector<int> Options;
        std::vector<int>::iterator iterOptions;

        std::vector<std::string> OptionsNames;
        std::vector<std::string>::iterator iterOptionsNames;
};







class UIContainer : public UIInterface
{
    public:
        UIContainer();
        virtual ~UIContainer();

        void add( UIInterface *_baseData );
        void clear();
        unsigned int size();

        void handleEvent( const SDL_Event &_event );
        bool logic( const Uint32 &_deltaTicks );
        void reLoadTextures();
        void render( de::enums::fboTypes _bottomLayer, de::enums::fboTypes _topLayer );
    private:

        typedef boost::ptr_vector<UIInterface> vectorObjects;
        vectorObjects::iterator iter, end;;
        vectorObjects Objects;
};



#endif // UIBASEDATA_H
