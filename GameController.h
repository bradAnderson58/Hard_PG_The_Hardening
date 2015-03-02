#ifndef __GameController_h_
#define __GameController_h_

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OISJoyStick.h>
#include <SdkTrays.h>

#include "GameApplication.h"
class GameApplication;  //Why do I need to do this?
class Player;
class Environment;
class GUIController;

class GameController : public OIS::KeyListener, public OIS::MouseListener, public OIS::JoyStickListener, public Ogre::WindowEventListener
{
public:
	GameController(GameApplication* a);	//constructor
	virtual ~GameController(void);

	void captureAll();								//capture events from the listeners
	void removeSelf();								//on shutdown
	void setPlayer(Player* p){ player = p; }		//set player (done from gameApp)

	void setInteractible(Environment* i){ interactWith = i; }  //Make sure to do NULL checking

	void setGUICont(GUIController* g){ mGUICont = g; }

private:
	OgreBites::InputContext mInputContext;  //for Ogre 1.9
	OIS::InputManager* mInputManager;
    OIS::Mouse*    mMouse;
    OIS::Keyboard* mKeyboard;
	OIS::JoyStick* mJoy;
	Ogre::RenderWindow* uWindow;

	GameApplication* app;	//point to main app
	Player* player;			//point to main player
	GUIController* mGUICont;

	//Key Handler stuffs
	bool keyW;
	bool keyA;
	bool keyS;
	bool keyD;

	// OIS::KeyListener
    bool keyPressed( const OIS::KeyEvent &arg );
    bool keyReleased( const OIS::KeyEvent &arg );
	void keyHandler(OIS::KeyCode keyd, bool down);

	//OIS::JoyStickListener
	bool buttonPressed( const OIS::JoyStickEvent &arg, int button);
	bool buttonReleased( const OIS::JoyStickEvent &arg, int button);
	bool axisMoved( const OIS::JoyStickEvent &arg, int axis);

	// OIS::MouseListener
    bool mouseMoved( const OIS::MouseEvent &arg );
    bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	//handling window stuffs
	void GameController::windowResized(Ogre::RenderWindow* rw);
	void GameController::windowClosed(Ogre::RenderWindow* rw);

	Environment* interactWith;
};

#endif