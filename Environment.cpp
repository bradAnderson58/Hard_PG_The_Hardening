#include "Environment.h"
#include "GameApplication.h"
#include "GameController.h"

Environment::Environment(Ogre::SceneManager* SceneManager, std::string name, std::string filename, float height, float scale, GameApplication* a)
{
	using namespace Ogre;
	
	mSceneMgr = SceneManager; // keep a pointer to where this agent will be

	if (mSceneMgr == NULL)
	{
		std::cout << "ERROR: No valid scene manager in Agent constructor" << std::endl;
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

	inner = new UsableItems(UsableItems::SHIELD, 0, 5, 0, 0, 0, "Barrel Shield", 5); //this will not be hardcoded in the future
	inner->setImgFile("default.png");
}

Environment::~Environment(){
	inner = NULL;
	mBodyNode->removeAndDestroyAllChildren();
	mSceneMgr->destroySceneNode(mBodyNode);
	//delete mBodyEntity;
	//we need to look into deleting stuff!  TODO
}

void Environment::setPosition(float x, float y, float z){
	this->mBodyNode->setPosition(x, y + height, z);
}

//use this to check how close the player is.  If he is close enough, set interactable icon and change state
void Environment::checkWithPlayer(){
	float distance = mBodyNode->getPosition().distance(app->getPlayerPointer()->getPosition());  //yup

	if (distance <= 10){
		std::cout << "PICK ME UP!" << std::endl;
		app->getController()->setInteractible(this);
	}
	else{
		app->getController()->setInteractible(NULL);
	}
}