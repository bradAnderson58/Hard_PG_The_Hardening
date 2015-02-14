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
class LoaderClass;

class GameApplication : public BaseApplication
{
private:
	Agent* agent; // store a pointer to the character
	Ogre::Vector3 houseInitPos;

	std::list<NPC*> NPClist;

	Player* playerPointer;				//This is our heroic savior
	Ogre::SceneNode* housePointer;		//point to the location of the house COM
	bool gameOver;						//game is over

	// gui stuff decl's
	MyGUI::ProgressPtr healthBar;		// progress bar to track health
	MyGUI::WindowPtr pauseMenu;			// window to quit, check records, or inventory

	std::list<Ogre::SceneNode*> wallList;
	std::list<Ogre::SceneNode*> borderWalls;
	Ogre::AxisAlignedBox boundBox;  //bounding box of the barrel - DELETE?
	int level;

	//Boundaries of the world
	Grid *grid;
	float xMax, zMax;

	void restartLevel();
	void nextLevel();

	int ghettoSelect;	// what is this?

	LoaderClass* loading;  //Do loading with this shits

public:	

	enum GameState
	{
		MAINSCREEN,
		SETUP,
		PLAYING,
		MENUSCREEN,
		INVENTORY,
		CHAR_RECORD,
		DEAD_STATE
	};	//game states

    GameApplication(void);
    virtual ~GameApplication(void);

	//menu code stuff - accessed from GameController
	void openMenu(bool visible);
	void openInventory(bool visible);
	void openCharRecord(bool visible);

	//setter for player shits
	void setPlayer(Player* p);

	//For world bounds getters / setters
	float getXmax() { return xMax; }
	float getZmax() { return zMax; }
	void setMaxes(float x, float z){
		xMax = x;
		zMax = z;
	}

	//getters / setters for lists
	std::list<NPC*> getNPCs(){ return NPClist;}

	void pushNPCs(NPC* npc) { NPClist.push_back(npc); }
	void pushBorder(Ogre::SceneNode* wall){ borderWalls.push_back(wall); }
	void pushWalls(Ogre::SceneNode* wall){ wallList.push_back(wall); }

	void toggleState(GameState s);	// toggle to game state s

	Grid* getGrid(){ return grid; } //get it

	void addTime(Ogre::Real deltaTime);		// update the game state

	void setShutDown(bool s){ mShutDown = s; }		//to shutdown the game
	GameState getGameState(){ return gameState; }	//To get the current GameState

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

	virtual void createFrameListener(void);  //for Controller

	void buttonHit(MyGUI::WidgetPtr _sender);

	GameState gameState;

	//GUI buttons and such
	/*OgreBites::Button *cont;*/

	//Strings for GUI
	
	//Path files for sounds

	//OgreBites::ParamsPanel* mParamsPanel;

};

#endif // #ifndef __TutorialApplication_h_
