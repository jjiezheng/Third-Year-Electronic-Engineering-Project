#ifndef MENU_H
#define MENU_H


#include "CoreEngine.h"
#include "Coords.h"




namespace de
{
    namespace enums
    {
        enum MenuOptions
        {
            GAME,
            HIGHSCORES,
            OPTIONS,
            EXIT,
            NOTHING,
            NEW_GAME,
            LEVEL_EDITOR,
            DEVELOPER_TOOLS,
            LEVEL_SELECT,
            SET_KEYBOARD,
            SET_JOYSTICK,
            SET_GRAPHICS,
            SET_SOUND
        };


        enum GameModes
        {
        NONE,
        ALL
    };
    }
}

class MenuItem
{
    public:
        de::classes::Vector position;

        MenuItem( const de::classes::Vector &_position, const int &_name, const std::string &_text, const de::classes::Colour &_colour );
        ~MenuItem();

        int getWidth();
        int getHeight();
        int getName();
        void setGade();

        void reLoadTextures();
        void render();
    private:
        int width, height;
        int name;

        de::classes::SpriteString text;
        de::classes::Colour colour;
};


class BaseMenuList
{
protected:
    int Xgap, Ygap;
    de::classes::Vector Default;
    de::classes::Colour colour;
    bool itemSelected, moveBackward, moveForward, inSubMenu;
    std::string mode;


    std::vector<MenuItem>::iterator currentItem;
    std::vector<MenuItem> MenuItems;

public:
    de::classes::Poly curser;


    virtual ~BaseMenuList(){};

    bool isItemSelected();
    bool* getItemSelected();
    void setGade( int, int, std::string );
    void setSubMenu( BaseMenuList* );
    void add( const de::classes::Vector &_position, const int &name, const std::string &text, const de::classes::Colour &_colour );
    void add( int, std::string );
    void clearAll();


    void handleEvent( const SDL_Event & );
    int getName();
    int currentName();

    void reLoadTextures();
    void render();

    virtual int logic( Uint32 ) = 0;
};



class MenuList : public BaseMenuList
{
private:
public:
    MenuList();

    std::string name;

    ~MenuList(){};
    int logic( Uint32 );
};






class Menu
{
private:
    bool itemSelected;
    int mainCurser, subCurser;

    MenuList MainMenu, gameMenu, highScoreMenu,optionsMenu;
public:
    Menu();
    ~Menu();

    void handleEvent( const SDL_Event &event );
    int logic( Uint32 );
    void reLoadTextures();
    void render();


    std::vector<de::classes::Sprite>& getAllSprites();
};


#endif //MENU_H
