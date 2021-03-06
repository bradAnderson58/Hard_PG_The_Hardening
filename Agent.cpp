#include "Agent.h"
#include "GameApplication.h"
#include "Grid.h"

#define _USE_MATH_DEFINES   //for some reason
#include <math.h>

Agent::Agent(Ogre::SceneManager* SceneManager, std::string name, std::string filename, float height, float scale, GameApplication* a,
			 Ogre::SceneNode* parent)
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

	if (parent == NULL){
		mSoulNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	}else{
		mSoulNode = parent->createChildSceneNode();
	}

	mBodyNode = mSoulNode->createChildSceneNode();

	//mModelNode is used to rotate model intependant of root node
	mModelNode = mBodyNode->createChildSceneNode();
	mModelEntity = mSceneMgr->createEntity(name, filename); // load the model
	mModelNode->attachObject(mModelEntity);	// attach the model to the scene node
	//mModelEntity->shad

	mBodyNode->translate(0,height,0); // make the agent stand on the plane (almost)
	mBodyNode->scale(scale,scale,scale); // Scale the figure

	//keep track of intial position - still needed?
	initPos = mBodyNode->getPosition();

	//If this is a projectile - may need for later
	projectile = false;

	//Particle code - may be needed for magic spells or something?
	ParticleSystem::setDefaultNonVisibleUpdateTimeout(5);  // set nonvisible timeout
	//ps = mSceneMgr->createParticleSystem("Fountain1", "Examples/PurpleFountain");
	Ogre::SceneNode* mnode = mBodyNode->createChildSceneNode();
	mnode->roll(Degree(180));
	//mnode->attachObject(ps);
	//ps->setVisible(false);

	// configure walking parameters - still needed?
	mWalkSpeed = 35.0f;	
	mDirection = Ogre::Vector3::ZERO;
}

Agent::~Agent(){
	// mSceneMgr->destroySceneNode(mBodyNode); // Note that OGRE does not recommend doing this. It prefers to use clear scene
	// mSceneMgr->destroyEntity(mBodyEntity);
	mSceneMgr->destroyEntity(mModelEntity);
}

void 
Agent::setPosition(float x, float y, float z)
{
	this->mBodyNode->setPosition(x, y + height, z);
}

// update is called at every frame from GameApplication::addTime
void
Agent::update(Ogre::Real deltaTime) 
{
	this->updateLocomote(deltaTime);	// Update Locomotion
	
	this->updateAnimations(deltaTime);	// Update animation playback
}


void 
Agent::updateLocomote(Ogre::Real deltaTime)
{
	//DELETE?
}

void 
Agent::updateAnimations(Ogre::Real deltaTime)
{
	//DELETE?
}

void 
Agent::fadeAnimations(Ogre::Real deltaTime)
{
	//DELETE?
}

void 
Agent::setupAnimations()
{
	//overwritten in subclasses
}


