#ifndef __Agent_h_
#define __Agent_h_

#include "BaseApplication.h"
#include <deque>

#pragma once
class GameApplication;
class GridNode;
class Grid;
class Agent
{
protected:
	Ogre::SceneManager* mSceneMgr;		// pointer to scene graph
	Ogre::SceneNode* mSoulNode;			// root node, use this in level loading
										// attach anything you don't want to move with the body to the soul
	Ogre::SceneNode* mBodyNode;			// node camera and entity
	Ogre::SceneNode* mModelNode;		// Node for model
	Ogre::Entity* mModelEntity;

	float height;						// height the character should be moved up
	float scale;						// scale of character from original model

	Ogre::Vector3 defaultPos;
	Ogre::Quaternion defaultOrient;		//reset the position and orientation to these values

	// may be necessary for later physics implementation?
	bool projectile; // is this agent going to be launched?
	Ogre::Vector3 initPos; // initial position
	Ogre::Vector3 vel; // velocity of agent
	Ogre::Vector3 gravity; 
	//Ogre::ParticleSystem* ps;  //I like the particles, I will keep them

	GameApplication *app;
	
	// all of the animations our character has, and a null ID
	// some of these affect separate body parts and will be blended together - possibly
	virtual enum AnimID;
	//Sinbad animations - in case we need them
	/*virtual enum AnimID
	{
		ANIM_IDLE_BASE,
		ANIM_IDLE_TOP,
		ANIM_RUN_BASE,
		ANIM_RUN_TOP,
		ANIM_HANDS_CLOSED,
		ANIM_HANDS_RELAXED,
		ANIM_DRAW_SWORDS,
		ANIM_SLICE_VERTICAL,
		ANIM_SLICE_HORIZONTAL,
		ANIM_DANCE,
		ANIM_JUMP_START,
		ANIM_JUMP_LOOP,
		ANIM_JUMP_END,
		ANIM_NONE
	};*/

	Ogre::AnimationState* mAnims[21];		// master animation list - CHANGE if need more than 21 animations
	//AnimID mBaseAnimID;						// current base (full- or lower-body) animation - if needed
	//AnimID mTopAnimID;						// current top (upper-body) animation
	bool mFadingIn[20];						// which animations are fading in
	bool mFadingOut[20];					// which animations are fading out
	Ogre::Real mTimer;						// general timer to see how long animations have been playing
	Ogre::Real mVerticalVelocity;			// for jumping

	virtual void setupAnimations();									// load this character's animations
	virtual void fadeAnimations(Ogre::Real deltaTime);				// blend from one animation to another
	virtual void updateAnimations(Ogre::Real deltaTime);			// update the animation frame

	// for locomotion
	Ogre::Real mDistance;					// The distance the agent has left to travel - possibly still needed?
	Ogre::Vector3 mDestination;				// The destination the object is moving towards
	std::deque<Ogre::Vector3> mWalkList;	// The list of points we are walking to (for A*)
	bool nextLocation();					// Is there another destination?

	Ogre::Vector3 mDirection;				// The direction the object is moving
	Ogre::Real mWalkSpeed;					// The speed at which the object is moving

	virtual void updateLocomote(Ogre::Real deltaTime);	// update the character's walking

	//////////////////////////////////////////////
	// DELETE?
	bool procedural;						// Is this character performing a procedural animation
    //////////////////////////////////////////////
public:
	Agent(Ogre::SceneManager* SceneManager, std::string name, std::string filename, float height, float scale, GameApplication* a,
		Ogre::SceneNode* parent = NULL);
	~Agent();
	void setPosition(float x, float y, float z);

	virtual void update(Ogre::Real deltaTime);		// update the agent
	
	//void setBaseAnimation(AnimID id, bool reset = false);	// choose animation to display
	//void setTopAnimation(AnimID id, bool reset = false);

	void setApp(GameApplication *a) { app = a; }  //I need to access some stuff in the gameApplication from the agent
};

#endif