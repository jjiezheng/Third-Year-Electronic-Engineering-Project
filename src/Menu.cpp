#include "Menu.h"

using namespace de::classes;
using namespace de::graphics;
using namespace de::core;
using namespace de::enums;


MenuItem::MenuItem( const Vector &_position, const int &_name, const std::string &_text, const Colour &_colour )
{
    name = _name;
    colour = _colour;
    position = _position;
    text.set( _text, "GeoSansLight", _position, 11, ALIGN_CENTRE );
    text.setColour( colour );

    //Getting the width and height of the text
    std::vector<Sprite>::iterator iter, iterEnd;
    iter = text.getSprites().begin();
    iterEnd = text.getSprites().end() - 1;

    height = iter->coords.Vertices[3].y - iter->coords.Vertices[0].y;
    width = iterEnd->coords.Vertices[1].x - iter->coords.Vertices[0].x;
}

MenuItem::~MenuItem()
{

}

int MenuItem::getWidth()
{
    return width;
}

int MenuItem::getHeight()
{
    return height;
}

int MenuItem::getName()
{
    return name;
}

void MenuItem::reLoadTextures()
{
    text.reload();
}

void MenuItem::render()
{
    text.render( FBO_UI );
}






MenuList::MenuList()
{

    Xgap = Ygap = 0;
    mode = "vertical";
    itemSelected = moveBackward = moveForward = inSubMenu = false;

    Frect rect;
    rect.w = 0;
    rect.h = 8;

    curser.Setup( rect, 0 );
}

void BaseMenuList::setGade( int x, int y, std::string _mode )
{
    mode = _mode;
    Xgap = x;
    Ygap = y;
}

void BaseMenuList::add( const Vector &_position, const int &name, const std::string &text, const Colour &_colour )
{
    MenuItems.push_back( MenuItem( _position, name, text, _colour )  );

    Default = _position;
    colour = _colour;


    currentItem = MenuItems.begin();
    Vector pos( currentItem->position.x, 0 );
    curser.transform( -curser.centre + pos );
}

void BaseMenuList::add( int _name, std::string _text )
{
    Vector pos;

    if( mode == "horizontal" )
    {
        currentItem = MenuItems.end() - 1;
        MenuItems.push_back( MenuItem( Vector( currentItem->position.x + currentItem->getWidth() + Xgap, Default.y ), _name, _text, colour )  );

        currentItem = MenuItems.begin();
        pos = Vector( currentItem->position.x, currentItem->position.y + currentItem->getHeight()/2 );
    }

    else if( mode == "vertical" )
    {
        currentItem = MenuItems.end() - 1;
        MenuItems.push_back( MenuItem( Vector( Default.x, currentItem->position.y + currentItem->getHeight() + Ygap ), _name, _text, colour )  );

        currentItem = MenuItems.begin();
        pos = Vector( currentItem->position.x, currentItem->position.y + currentItem->getHeight()/2 );
    }

    curser.changeRect( currentItem->getWidth(), 4 );
    curser.transform( -curser.centre + pos );

}

void BaseMenuList::clearAll()
{
    int theEnd = MenuItems.end() - MenuItems.begin();
    int i = 0;

    for( i = 0; i < theEnd; i++ )
    {
        currentItem = MenuItems.begin();
        currentItem = MenuItems.erase( currentItem );
    }
}

void BaseMenuList::render()
{
    std::vector<MenuItem>::iterator iterMenuItems;
    for( iterMenuItems = MenuItems.begin(); iterMenuItems < MenuItems.end(); ++iterMenuItems )
    {
        iterMenuItems->render();
    }

    de::Engine::Graphics().add( curser, colour, 11, FBO_UI );
}

void BaseMenuList::handleEvent( const SDL_Event &event )
{
    itemSelected = moveForward = moveBackward = false;

    if ( event.type == SDL_KEYDOWN )
    {
        //Adjust the velocity
        if ( event.key.keysym.sym )
        {
            if( mode == "horizontal" )
            {
                if ( event.key.keysym.sym == SDLK_RIGHT )
                    moveForward = true;

                else if ( event.key.keysym.sym == SDLK_LEFT )
                    moveBackward = true;
            }

            if( mode == "vertical" )
            {
                if ( event.key.keysym.sym == SDLK_DOWN )
                    moveForward = true;

                else if ( event.key.keysym.sym == SDLK_UP )
                    moveBackward = true;
            }


            if ( event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER  )
                itemSelected = true;

            if ( event.key.keysym.sym == SDLK_ESCAPE )
                itemSelected = false;
        }
    }
}

bool BaseMenuList::isItemSelected()
{
    return itemSelected;
}

bool* BaseMenuList::getItemSelected()
{
    return &itemSelected;
}

int BaseMenuList::getName()
{
    if( itemSelected )
        return currentItem->getName();

    return 0;
}

int BaseMenuList::currentName()
{
    return currentItem->getName();
}


void BaseMenuList::reLoadTextures()
{
    std::vector<MenuItem>::iterator iter;
    for( iter = MenuItems.begin(); iter < MenuItems.end(); ++iter )
    {
        iter->reLoadTextures();
    }
}





int MenuList::logic( Uint32 deltaTicls )
{
    if( mode == "horizontal" )
    {
        if( moveForward && ( currentItem < MenuItems.end() - 1 ) )
        {
            moveForward = !moveForward;

            ++currentItem;
            curser.changeRect( currentItem->getWidth(), 4 );
            Vector pos( currentItem->position.x, currentItem->position.y + currentItem->getHeight()/2 );
            curser.transform( -curser.centre + pos );
        }

        if( moveBackward && ( currentItem > MenuItems.begin() ) )
        {
            moveBackward = !moveBackward;

            --currentItem;
            curser.changeRect( currentItem->getWidth(), 4 );
            Vector pos( currentItem->position.x, currentItem->position.y + currentItem->getHeight()/2 );
            curser.transform( -curser.centre + pos );
        }
    }

    else if( mode == "vertical" )
    {
        if( moveForward && ( currentItem < MenuItems.end() - 1 ) )
        {
            moveForward = !moveForward;
            ++currentItem;
            curser.changeRect( currentItem->getWidth(), 4 );
            Vector pos( currentItem->position.x, currentItem->position.y + currentItem->getHeight()/2 );
            curser.transform( -curser.centre + pos );
        }

        if( moveBackward && ( currentItem > MenuItems.begin() ) )
        {
            moveBackward = !moveBackward;
            --currentItem;
            curser.changeRect( currentItem->getWidth(), 4 );
            Vector pos( currentItem->position.x, currentItem->position.y + currentItem->getHeight()/2 );
            curser.transform( -curser.centre + pos );
        }
    }

    if( itemSelected )
        return currentItem->getName();


    return -1;
}






Menu::Menu()
{
    itemSelected = false;
    mainCurser = 0;

    Font::SelectFont( "GeoSansLight" );

    MainMenu.setGade( 100, 0, "horizontal" );
    MainMenu.add( Vector(280, 205), GAME, "Game", Colour(0,0,0,1) );
    MainMenu.add( HIGHSCORES, "HighScores" );
    MainMenu.add( OPTIONS, "Options" );
    MainMenu.add( EXIT, "Exit" );

    gameMenu.setGade( 0, 50, "vertical" );
    gameMenu.add( Vector(550, 300), NEW_GAME, "New Game", Colour(0,0,0,1) );
    gameMenu.add( LEVEL_EDITOR, "Level Editor" );

    highScoreMenu.setGade( 0, 50, "vertical" );
    highScoreMenu.add( Vector(550, 300), NOTHING, "Not made Yet", Colour(0,0,0,1) );

    optionsMenu.setGade( 0, 50, "vertical" );
    optionsMenu.add( Vector(550, 300), SET_KEYBOARD, "Set Keyboard", Colour(0,0,0,1) );
    optionsMenu.add( SET_JOYSTICK, "Set Joystick" );
}

Menu::~Menu()
{
}



void Menu::handleEvent( const SDL_Event &event )
{
    MainMenu.handleEvent( event );

    gameMenu.handleEvent( event );
    highScoreMenu.handleEvent( event );
    optionsMenu.handleEvent( event );
}

int Menu::logic( Uint32 deltaTicls )
{
    MainMenu.logic( deltaTicls );
    mainCurser = MainMenu.currentName();

    switch( mainCurser )
    {
        case GAME:
            return gameMenu.logic( deltaTicls );

        case HIGHSCORES:
            return highScoreMenu.logic( deltaTicls );

        case OPTIONS:
            return optionsMenu.logic( deltaTicls );

        case EXIT:
            if( MainMenu.isItemSelected() )
                return EXIT;
    }
    return 0;
}

void Menu::reLoadTextures()
{
    MainMenu.reLoadTextures();
    gameMenu.reLoadTextures();
    highScoreMenu.reLoadTextures();
    optionsMenu.reLoadTextures();
}

void Menu::render()
{
    MainMenu.render();

    switch( mainCurser )
    {
        case GAME:
            gameMenu.render();
            break;

        case HIGHSCORES:
            highScoreMenu.render();
            break;

        case OPTIONS:
            optionsMenu.render();
            break;
    }
}



