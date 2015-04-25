#include "GameApplication.h"
#include "GameController.h"
#include "loaderClass.h"
#include "Environment.h"
#include "GUIController.h"
#include "UsableItems.h"
#include <fstream>
#include <sstream>
#include <map>

#define _USE_MATH_DEFINES 
#include <math.h>

//-------------------------------------------------------------------------------------
GameApplication::GameApplication(void): 
	mCurrentObject(0)
{
	gameState = MAINSCREEN;
	level = 0;
	engine = irrklang::createIrrKlangDevice();

	//engine->play2D("../../media/getout.ogg", true);
	//engine->stopAllSounds();
}
//-------------------------------------------------------------------------------------
GameApplication::~GameApplication(void)
{
	//just in case
}

//-------------------------------------------------------------------------------------
void GameApplication::createScene(void)
{
	//remove
}

//Use this to make the Game Controller and such
void GameApplication::createFrameListener(void)
{
    
	gameCont = new GameController(this);

    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, gameCont);

    // These were used to create the original mDetailsPanel - may want to recreate with MyGUI
	//don't actually need?
    Ogre::StringVector items;
    items.push_back("cam.pX");
    items.push_back("cam.pY");
    items.push_back("cam.pZ");
    items.push_back("");
    items.push_back("cam.oW");
    items.push_back("cam.oX");
    items.push_back("cam.oY");
    items.push_back("cam.oZ");
    items.push_back("");
    items.push_back("Filtering");
    items.push_back("Poly Mode");

    mRoot->addFrameListener(this);  //addFrameListener!
}

//update graphics on delta time
void
GameApplication::addTime(Ogre::Real deltaTime)
{
	//time updator
	if (gameState == PLAYING || gameState == DEAD_STATE) 
	{
		playerPointer->update(deltaTime); //Yoshimi has a different update function
		for (NPC* guy : NPClist){
			guy->update(deltaTime);
		}
		if (!interactableObjs.empty()){

			bool something = false; //check if anything is close to the player

			for (Environment* env : interactableObjs){

				if (env->checkWithPlayer()) something = true;  //something is close to the player

				//animate doors opening
				if (env->isAnimating()){
					env->update(deltaTime);
				}
			}
			if (!something) gameCont->setInteractible(NULL);   //nothing closeby is interactible
		}

		//check our door puzzle stuff
		if (!placements.empty()){
			bool allGood = true;

			//if all the placements are correct, open the door
			for (Environment* puzz : placements){
				puzz->checkPenguins();

				if (!puzz->isCorrect()) allGood = false;
			}

			//if the puzzle is completed, locked becomes a regular door
			if (allGood){
				locked->setType(Environment::DOOR);
			}else{
				locked->setType(Environment::LOCKED_DOOR);
			}
		}
		mGUICont->setHealth(playerPointer->getHealthNow()); //healthBar->setProgressPosition(playerPointer->getHealthNow()); //American Association of Highway Officials, Litigators, and Engineers 
		mGUICont->setManaBar(playerPointer->getManaNow());  //manaBar->setProgressPosition(playerPointer->getManaNow());
		//mGUICont->recordUpdator();
	}
	else if (gameState == DIALOG)
	{
		//mGUICont->getDialog()->update();
		//mGUICont->cycleDialog();
		mGUICont->updateDialog();
	}
}


// toggle to game state s
// handle initializations to each state here as well.
void
GameApplication::toggleState(GameState s)
{
	if (s == MAINSCREEN)	// start screen
	{
		gameState = s;
	}
	else if (s == SETUP)		// initialize actual game
	{
		gameState = s;
		
		// load enviroment and set up level
		loading = new LoaderClass(mSceneMgr, this);
		//loadEnv();
		//setupEnv();
		//use a loading class?

		mGUICont->revealHUD(playerPointer->getHealthStat(), playerPointer->getManaStat());

		toggleState(PLAYING);
	}
	else if (s == PLAYING)		// mode where player interacts with world
	{
		// hide menu and inventory and dialog stuff
		mGUICont->hidePointer();
		mGUICont->openInventory(false);
		mGUICont->openMenu(false);
		mGUICont->openADialog(false);
		gameState = s;
	}
	else if (s == DIALOG)	// show dialog view and run through lines, than resume
	{
		mGUICont->openADialog(true);
		gameState = s;
	}
	else if (s == MENUSCREEN)	// pause, main menu
	{
		// show main menu
		// for testing, show all gui menus
		gameState = s;
		playerPointer->setMovement(false);	// movement locks up when you pause during movement
		mGUICont->showPointer();
		mGUICont->openMenu(true);
		for (UsableItems* thing : playerPointer->getInventory()){
			//std::cout << thing->getName() << " for testing - press Y to equip " << thing->getStat(UsableItems::DAMAGE) << std::endl;
		}
	}
	else if (s == DEAD_STATE)
	{
		gameState = s;
		playerPointer->die();
	}
	else if (s == LOADING)
	{
		gameState = s;
		destroyallChildren(mSceneMgr->getRootSceneNode());

		//clean up this memory
		for (NPC* npc : NPClist){
			delete npc;
		}
		NPClist.clear();  
		for (NPC* npc : goodNPCs){
			delete npc;
		}
		goodNPCs.clear();
		for (Environment* thing : interactableObjs){
			delete thing;
		}
		interactableObjs.clear();

		for (Environment* thing : placements){
			delete thing;
		}
		placements.clear();

		//delete playerPointer;
		this->stopSound();
		this->engine->play2D("../../media/music2.ogg", true);
		loading->loadEnv("demolevel.txt");

		//gameState = PLAYING;
	}
	else
		std::cout << "Not a valid state" << std::endl;
}

void GameApplication::createGUI(void)
{
	mGUICont = new GUIController(this);

	if (gameState == MAINSCREEN){
			toggleState(SETUP);
	}
	
}

void GameApplication::endGame(char condition){
	//refurbish this

}

void GameApplication::destroyallChildren(Ogre::SceneNode* p){

	mSceneMgr->destroyStaticGeometry("StaticTree");
	//mSceneMgr->destroyEntity("floor");

	//Weird singleton thing for floor
	Ogre::MeshManager::getSingleton().destroyAllResourcePools();  //try this?
	mSceneMgr->destroyEntity("Floor");

	//destroy everything in the scene node (this is why we pull the mSoulNode off the list)
	Ogre::SceneNode::ObjectIterator it = p->getAttachedObjectIterator();
	while (it.hasMoreElements()){
		Ogre::MovableObject* o = static_cast<Ogre::MovableObject*>(it.getNext());
		p->getCreator()->destroyMovableObject(o);
	}

   Ogre::SceneNode::ChildNodeIterator itChild = p->getChildIterator();

   p->removeChild(playerPointer->getName());
   p->removeAndDestroyAllChildren();

   //do lights
   for (Ogre::Light* deach : lightList){
	   delete deach;
   }
   lightList.clear();

   mSceneMgr->destroyAllLights();
}

void GameApplication::restartLevel(){
	
	//Keep for reference
	/*
	housePointer->setPosition(houseInitPos);
	yoshPointer->restart();
	houseHealth = 1.0;
	gameState = true;
	houseHealth = 1.0f;
	gameOver = false;
	mTrayMgr->destroyAllWidgetsInTray(OgreBites::TL_CENTER);
	houseHUD->setProgress(houseHealth);
	mTrayMgr->hideCursor();
	*/
}

void GameApplication::nextLevel(){

	//keep for reference

	/*
	Ogre::Vector3 housePos;
	if (level == 1){
		housePos = grid->getPosition(3, 11);
		housePos[1] = 27;
		housePointer->setPosition(housePos);
		Ogre::Vector3 yoshPos = grid->getPosition(5, 11);
		yoshPointer->setPosition(yoshPos[0], 0, yoshPos[2]);
	}
	else if (level == 2){
		housePos = grid->getPosition(11, 11);
		housePos[1] = 27;
		housePointer->setPosition(housePos);
		Ogre::Vector3 yoshPos = grid->getPosition(13, 11);
		yoshPointer->setPosition(yoshPos[0], 0, yoshPos[2]);
	}
	else{
		int row = rand()%18+1;
		int col = rand()%18+1;
		housePos = grid->getPosition(row, col);
		housePos[1] = 27;
		housePointer->setPosition(housePos);
		Ogre::Vector3 yoshPos = grid->getPosition(row+2, col);
		yoshPointer->setPosition(yoshPos[0], 0, yoshPos[2]);
	}
	Ogre::Vector3 robPos;
	
	houseHealth = 1.0;
	gameState = true;
	houseHealth = 1.0f;
	gameOver = false;
	mTrayMgr->destroyAllWidgetsInTray(OgreBites::TL_CENTER);
	houseHUD->setProgress(houseHealth);
	mTrayMgr->hideCursor();
	*/
}

void GameApplication::setPlayer(Player* p){
	playerPointer = p;
	gameCont->setPlayer(p);
}

void GameApplication::removeNulls(Environment* env){
	//pull this item off the list
	int pos = std::find(interactableObjs.begin(), interactableObjs.end(), env) - interactableObjs.begin();
	if (pos < interactableObjs.size()){
		interactableObjs.erase (interactableObjs.begin()+pos);  //erase this element from the list
		delete env;												//clean up memory
	}else{
		std::cout << "Object Not Found, something went wrong" << std::endl;
	}
	
	/*for (Environment* thing : interactableObjs){
		if (thing == env){

		}
	}*/
}
