#ifndef __GameApplication_h_
#define __GameApplication_h_

#include "BaseApplication.h"
#include "Agent.h"
#include "Player.h"
#include "Grid.h"
#include "UsableItems.h"
#include "NPC.h"
#include "Rat.h"

class Grid;  //okay whatever

class GameApplication : public BaseApplication
{
private:
	/*CEGUI::OgreRenderer* mRenderer;*/
	Agent* agent; // store a pointer to the character
	Ogre::Vector3 houseInitPos;

	std::list<NPC*> NPClist;

	Player* playerPointer;				//This is our heroic savior
	Ogre::SceneNode* housePointer;		//point to the location of the house COM
	bool gameOver;						//game is over

	std::list<Ogre::SceneNode*> wallList;
	std::list<Ogre::SceneNode*> borderWalls;
	Ogre::AxisAlignedBox boundBox;  //bounding box of the barrel - DELETE?
	int level;
	//Boundaries of the world
	Grid *grid;
	float xMax, zMax;
	void restartLevel();
	void nextLevel();

	//menu code stuff
	void openMenu();
	void openInventory();
	void openCharRecord();

	int ghettoSelect;

public:	

	enum GameState
	{
		MAINSCREEN,
		SETUP,
		PLAYING,
		MENUSCREEN,
		INVENTORY,
		CHAR_RECORD		
	};	//game states

    GameApplication(void);
    virtual ~GameApplication(void); 

	float getXmax() { return xMax; }
	float getZmax() { return zMax; }

	void loadEnv();			// Load the buildings or ground plane, etc.
	void setupEnv();		// Set up the lights, shadows, etc
	void loadObjects();		// Load other props or objects (e.g. furniture)
	void loadCharacters();	// Load actors, agents, characters
	void toggleState(GameState s);	// toggle to game state s

	Grid* getGrid(){ return grid; } //get it

	void addTime(Ogre::Real deltaTime);		// update the game state

	//////////////////////////////////////////////////////////////////////////
	// moved from base application
	// OIS::KeyListener
    bool keyPressed( const OIS::KeyEvent &arg );
    bool keyReleased( const OIS::KeyEvent &arg );
	void keyHandler(OIS::KeyCode keyd, bool down);
	bool keyW;
	bool keyA;
	bool keyS;
	bool keyD;

	//OIS::JoyStickListener
	bool buttonPressed( const OIS::JoyStickEvent &arg, int button);
	bool buttonReleased( const OIS::JoyStickEvent &arg, int button);
	bool axisMoved( const OIS::JoyStickEvent &arg, int axis);
    
	// OIS::MouseListener
    bool mouseMoved( const OIS::MouseEvent &arg );
    bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	////////////////////////////////////////////////////////////////////////////

	bool bLMouseDown, bRMouseDown;		//true if mouse buttons are held down
	Ogre::SceneNode *mCurrentObject;	//pointer to our currently selected object

	/////////////////////////////////////////////////////////////////////////////////

	Ogre::AxisAlignedBox getBox() { return boundBox; }  //return the bounding box of barrel

	Ogre::Camera* getCamera() { return mCamera; }  //why u no like?
	Player* getPlayerPointer(){return playerPointer;}
	std::list<Ogre::SceneNode*> getWallList(){return wallList;}
	Ogre::SceneNode* getHousePointer(){return housePointer;}

	void endGame(char condition);		//End the game in either victory or crushing defeat
	void destroyallChildren(Ogre::SceneNode* p);

protected:
    virtual void createScene(void);

	virtual void createGUI(void);  //for gui's

	void buttonHit(MyGUI::WidgetPtr _sender);

	GameState gameState;

	//GUI buttons and such
	OgreBites::Button *cont;

	//Strings for GUI
	
	//Path files for sounds

	OgreBites::ParamsPanel* mParamsPanel;

};

#endif // #ifndef __TutorialApplication_h_
