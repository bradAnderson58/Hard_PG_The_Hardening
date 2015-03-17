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
class GUIController;

class GameApplication : public BaseApplication
{
private:

	std::list<NPC*> NPClist;

	Player* playerPointer;				//This is our heroic savior
	GUIController* mGUICont;			//point to th controller

	std::list<Ogre::SceneNode*> wallList;
	std::list<Ogre::SceneNode*> borderWalls;
	std::list<Ogre::Entity*> wallEntities;

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
	enum GameState
	{
		MAINSCREEN,
		SETUP,
		PLAYING,
		DIALOG,
		MENUSCREEN,
		INVENTORY,
		CHAR_RECORD,
		DEAD_STATE
	};	//game states

    GameApplication(void);
    virtual ~GameApplication(void);

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

	std::list<Ogre::Entity*> getWallEntities(){return wallEntities;}
	void setGrid(Grid* g){grid = g;}

	void pushNPCs(NPC* npc) { NPClist.push_back(npc); }
	void pushBorder(Ogre::SceneNode* wall){ borderWalls.push_back(wall); }
	void pushWalls(Ogre::SceneNode* wall){ wallList.push_back(wall); }
	void pushEnvObj(Environment* obj){ interactableObjs.push_back(obj); }
	void pushWallEntity(Ogre::Entity* e) {wallEntities.push_back(e); }

	void removeNulls(Environment* env);  //Use this to remove null objects from lists

	void toggleState(GameState s);	// toggle to game state s

	Grid* getGrid(){ return grid; } //get it
	GameController* getController(){ return gameCont; }
	GUIController* getGUICont(){ return mGUICont; }

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
