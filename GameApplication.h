#ifndef __GameApplication_h_
#define __GameApplication_h_

#include "BaseApplication.h"
#include "Agent.h"
#include "Player.h"
#include "Grid.h"
#include "UsableItems.h"
#include "NPC.h"
#include "Rat.h"
#include "Diablous.h"
#include "Priestess.h"
#include "Skelebro.h"
#include <irrKlang.h>

class Grid;  //okay whatever
class LoaderClass;
class Environment;
class GUIController;

class GameApplication : public BaseApplication
{
private:

	std::list<NPC*> NPClist;
	std::list<NPC*> goodNPCs;

	Player* playerPointer;				//This is our heroic savior
	GUIController* mGUICont;			//point to th controller

	std::list<Ogre::Vector3> wallList;
	std::list<Ogre::Vector3> borderWalls;
	//std::list<Ogre::Entity*> wallEntities;

	std::list<Ogre::Light*> lightList;	// lights for the level

	std::vector<Environment*> interactableObjs;   //these are objects that we can interact with in some way
	std::vector<Environment*> placements;	//for unlocking door
	Environment* locked;
	int level;

	//Boundaries of the world
	Grid *grid;
	float xMax, zMax;

	void restartLevel();
	void nextLevel();

	void addTime(Ogre::Real deltaTime);		// update the game state

	std::string levels[5];  //hold level names for loading
	int loadInd;

public:	

	LoaderClass* loading;  //Do loading with this shits
	void destroyAllThings();  //for level loading sirs

	void restartGame();

	void setLocked(Environment* l){ locked = l; }
	Environment* getLocked(){ return locked; }

	enum GameState
	{
		MAINSCREEN,
		SETUP,
		PLAYING,
		DIALOG,
		MENUSCREEN,
		INVENTORY,
		CHAR_RECORD,
		DEAD_STATE,
		LOADING,
		ENDIT
	};	//game states

    GameApplication(void);
    virtual ~GameApplication(void);

	//setter for player shits
	void setPlayer(Player* p);

	int getLoadInd(){return loadInd;}
	//For world bounds getters / setters
	float getXmax() { return xMax; }
	float getZmax() { return zMax; }
	void setMaxes(float x, float z){
		xMax = x;
		zMax = z;
	}

	//getters / setters for lists
	std::list<NPC*> getNPCs(){ return NPClist;}
	std::list<NPC*> getGoodGuys(){ return goodNPCs; }
	std::list<Ogre::Vector3> getWallList(){return wallList;}
	std::list<Ogre::Light*> getLightList(){ return lightList; }
	std::vector<Environment*> getEnvObj(){ return interactableObjs; }

	//std::list<Ogre::Entity*> getWallEntities(){return wallEntities;}
	void setGrid(Grid* g){grid = g;}

	void pushNPCs(NPC* npc) { NPClist.push_back(npc); }
	void pushGoodGuy(NPC* npc) { goodNPCs.push_back(npc); }

	void pushBorder(Ogre::Vector3 wall){ borderWalls.push_back(wall); }
	void pushWalls(Ogre::Vector3 wall){ wallList.push_back(wall); }
	void pushEnvObj(Environment* obj){ interactableObjs.push_back(obj); }
	void pushPlacement(Environment* thing){ placements.push_back(thing); }
	//void pushWallEntity(Ogre::Entity* e) {wallEntities.push_back(e); }

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

	void playSound(char* x){engine->play2D(x);}
	void stopSound(){engine->stopAllSounds();}
	void playSong(char* x){engine->play2D(x,true);}
	irrklang::ISoundEngine* engine;

protected:
    virtual void createScene(void);
	virtual void createGUI(void);  //for gui's
	virtual void createFrameListener(void);  //for Controller

	void buttonHit(MyGUI::WidgetPtr _sender);

	GameState gameState;

};

#endif // #ifndef __TutorialApplication_h_
