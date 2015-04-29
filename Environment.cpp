#include "Environment.h"
#include "GameApplication.h"
#include "GameController.h"
#include "GUIController.h"
#include "Player.h"
#include "Event.h"

Environment::Environment(Ogre::SceneManager* SceneManager, std::string name, std::string filename, float height, float scale,
						 GameApplication* a, EnvType tp)
{
	using namespace Ogre;
	
	mSceneMgr = SceneManager; // keep a pointer to where this agent will be
	mType = tp;
	correct = false;

	if (mSceneMgr == NULL)
	{
		std::cout << "ERROR: No valid scene manager in EnvironmentObject constructor" << std::endl;
		return;
	}

	this->height = height;
	this->scale = scale;
	this->app = a;  //this is the game app

	mBodyNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(); // create a new scene node
	mBodyEntity = mSceneMgr->createEntity(name, filename);
	mBodyNode->attachObject(mBodyEntity);

	mBodyNode->translate(0,height,0); // make the agent stand on the plane (almost)
	mBodyNode->scale(scale,scale,scale); // Scale the figure

	//Loot has an inner usable item to pass when picked up
	if (mType == LOCKED_DOOR){
		std::vector<std::string> pass;
		pass.push_back("Door is Locked!");
		popup = new Event(app, pass, true);
	}
	else if (mType == KEY){
		std::vector<std::string> pass;
		pass.push_back("Found the Key!");
		popup = new Event(app, pass, true);
	}
	animate = false;
	passable = false;
	amountDown = 0;
}

Environment::~Environment(){
	inner = NULL;
	//mBodyNode->removeAndDestroyAllChildren();
	mSceneMgr->destroyEntity(mBodyEntity);
	//delete mBodyEntity;
	//we need to look into deleting stuff!  TODO
}

void Environment::setPosition(float x, float y, float z){
	this->mBodyNode->setPosition(x, y + height, z);
}

//use this to check how close the player is.  If he is close enough, set interactable icon and change state
bool Environment::checkWithPlayer(){
	float distance = mBodyNode->getPosition().distance(app->getPlayerPointer()->getPosition());  //yup

	if (distance <= 10){
		app->getController()->setInteractible(this);
		return true;
	}
	return false;
}

//what should I do once the player has selected me?
Environment::EnvType Environment::handleInteraction(Player* pl){

	//based on type
	if (mType == LOOT){
		inner = new UsableItems(pl);
		pl->pushInventory(inner);
	}
	if (mType == DOOR){
		app->engine->play2D("../../media/door.wav");
		animate = true;
	}
	if (mType == LOCKED_DOOR){
		app->toggleState(GameApplication::DIALOG);
		app->getGUICont()->setDialogEvent(popup);
	}
	if (mType == MOVEABLE){
		if (!pl->isCarrying()){
			pl->carryMe(this);
			passable = true;
		}
		else{
			pl->dropMe();
			passable = false;
		}
	}
	if (mType == TRANSPORT){
		app->toggleState(GameApplication::LOADING);
	}
	if (mType == KEY){
		app->getLocked()->setType(Environment::DOOR);
		app->toggleState(GameApplication::DIALOG);
		app->getGUICont()->setDialogEvent(popup);
	}

	return mType;
}

void Environment::update(Ogre::Real dt){

	//passable false means the door is closed
	if(passable ==false){
		amountDown -= 0.1;

		Ogre::Vector3 currentPos = mBodyNode->getPosition();
		mBodyNode->setPosition(currentPos.x, currentPos.y - 0.1, currentPos.z);  //slowly move the door down

		if (amountDown <= -5.2){
			animate = false;
			passable = true;
		}

	//otherwise the door is open so it gets closed
	}else{
		amountDown += 0.1;

		Ogre::Vector3 currentPos = mBodyNode->getPosition();
		mBodyNode->setPosition(currentPos.x, currentPos.y + 0.1, currentPos.z);  //slowly move the door up

		if (amountDown >= 0){
			animate = false;
			passable = false;	//door is closed again
		}
	}

}

void Environment::checkPenguins(){
	
	bool thisCheck = false;
	//check each item to see if its on top of us
	for (Environment* peng : app->getEnvObj()){
		if (peng->getPosition().distance(mBodyNode->getPosition()) < 7.5){
			if (peng->mType == MOVEABLE){
				correct = true;
				thisCheck = true;
			}
		}
	}
	if (thisCheck == false) correct = false;
}