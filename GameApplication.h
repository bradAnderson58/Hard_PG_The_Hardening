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
class Environment;

class GameApplication : public BaseApplication
{
private:

	std::list<NPC*> NPClist;

	Player* playerPointer;				//This is our heroic savior

	// gui stuff decl's
	MyGUI::WindowPtr questWin;			// Window to contain quest text/debug info
	MyGUI::ImageBox* playerImage;		// image representing player status
	MyGUI::ProgressPtr healthBar;		// progress bar to track health
	MyGUI::ProgressPtr manaBar;			// progress bar to track mana
	MyGUI::ButtonPtr inventoryB;		// button to access inventory from pause menu
	MyGUI::ButtonPtr charRecordB;		// button to acces character records from pause menu
	MyGUI::ButtonPtr exitB;				// button to exit from pause menu

	std::list<Ogre::SceneNode*> wallList;
	std::list<Ogre::SceneNode*> borderWalls;

	std::vector<Environment*> interactableObjs;   //these are objects that we can interact with in some way
	int level;

	//Boundaries of the world
	Grid *grid;
	float xMax, zMax;

	void restartLevel();
	void nextLevel();

	void addTime(Ogre::Real deltaTime);		// update the game state

	LoaderClass* loading;  //Do loading with this shits

public:	
	bool checkDebug;  //debugging take out
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

	//menu code stuff - 
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
	std::list<Ogre::SceneNode*> getWallList(){return wallList;}
	std::vector<Environment*> getEnvObj(){ return interactableObjs; }

	void pushNPCs(NPC* npc) { NPClist.push_back(npc); }
	void pushBorder(Ogre::SceneNode* wall){ borderWalls.push_back(wall); }
	void pushWalls(Ogre::SceneNode* wall){ wallList.push_back(wall); }
	void pushEnvObj(Environment* obj){ interactableObjs.push_back(obj); }

	void removeNulls(Environment* env);  //Use this to remove null objects from lists

	void toggleState(GameState s);	// toggle to game state s

	Grid* getGrid(){ return grid; } //get it
	GameController* getController(){ return gameCont; }

	void setShutDown(bool s){ mShutDown = s; }		//to shutdown the game
	GameState getGameState(){ return gameState; }	//To get the current GameState

	Ogre::SceneNode *mCurrentObject;	//pointer to our currently selected object - what is this for?

	/////////////////////////////////////////////////////////////////////////////////

	Ogre::Camera* getCamera() { return mCamera; }  //why u no like?
	Player* getPlayerPointer(){return playerPointer;}

	void endGame(char condition);		//End the game in either victory or crushing defeat
	void destroyallChildren(Ogre::SceneNode* p);

protected:
    virtual void createScene(void);

	virtual void createGUI(void);  //for gui's

	virtual void createFrameListener(void);  //for Controller

	void buttonHit(MyGUI::WidgetPtr _sender);

	GameState gameState;

};

#endif // #ifndef __TutorialApplication_h_
