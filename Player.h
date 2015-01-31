#ifndef __Player_h_
#define __Player_h_

#include "Agent.h"

class GameApplication;
class NPC;
class UsableItems;

class Player : public Agent{

public:

	Player(Ogre::SceneManager* SceneManager, std::string name, std::string filename, float height, float scale, GameApplication* a);
	~Player();

	void update(Ogre::Real deltaTime);			// update hero!
	void updateLocomote(Ogre::Real deltaTime);
	void setMovement(bool on);				//set the movemnt
	void rotationCode(OIS::MouseEvent arg);  //Rotate the hero when we move the mouse

	void rotationCode(double arg);			//Rotate the hero
	void cameraRot(double arg);				//for rotating camera with RS joystick
	void playerRot(double arg);				//for rotating player with LS joystick
	void setVelocity(double arg);			//Set how fast to walk
	void buttonAnimation(char pressed);		//tell me what animation to do
	
	double mPlayerRot;
	double mPlayerVel;
	bool doingStuff;		//if Yoshimi is currently doing a non-looping animation
	void changeSpeed(double s) { speed = s; }	//change the speed for different animations

	void updateStats();

	void checkBump();
	Ogre::Vector3 getPosition(){return mBodyNode->getPosition();}
	void restart();
	void setInitPos(Ogre::Vector3 p){initPos = p;}

private:
	bool fForward; //how am I moving? Each flag indicates a direction

	bool fRot;
	double mRotator;
	double speed;	//how fast to move

	Ogre::SceneNode *mAttackNode;	//use this for bounding area for Yoshimi attacks
	Ogre::Entity *mAttackEntity;
	Ogre::SceneNode* mNullCamera;		//For rotating just the camera

	enum AnimID
	{
		ATTACK_ONE,
		ATTACK_TWO,
		ATTACK_THREE,
		BACKFLIP,
		BLOCK,
		CLIMB,
		CROUCH,
		DEATH_ONE,
		DEATH_TWO,
		HIGH_JUMP,
		IDLE_ONE,
		IDLE_TWO,
		IDLE_THREE,
		JUMP,
		JUMP_NO_HEIGHT,
		KICK,
		SIDE_KICK,
		SPIN,
		STEALTH,
		WALK,
		ANIM_NONE
	};

	AnimID playerAnim;
	void setupAnimations();									// load this character's animations
	void fadeAnimations(Ogre::Real deltaTime);				// blend from one animation to another
	void updateAnimations(Ogre::Real deltaTime);			// update the animation frame
	void setAnimation(AnimID id, bool reset = false);
	void collisionRobots();
	void collisionWalls();

	//Stats for player
	double criticalStat;	//chance to cause critical hit
	double damageStat;		//base damage
	double defenseStat;		//base defense
	double healthStat;		//base health
	double manaStat;		//base mana

	int level;				//your level

	//attributes for player
	int evilAtt;		//affects crit, mana
	int strengthAtt;	//affects dam, def
	int dexterityAtt;	//affects heal, crit
	int constitutionAtt;//affects def, heal
	int intelligenceAtt;//affects mana, dam
	
	//equipment for player
	UsableItems *equippedWpn;	//The weapon you are using
	UsableItems *equippedShield;
	UsableItems *equippedHelm;
	UsableItems *equippedBoobs;
	UsableItems *equippedPants;
	UsableItems *equippedNeck;

	bool isBlocking;  //whether the player is currently blocking
	
	void checkHits(char attack);  //for attacks - check if an enemy gets hit
	void dealDamage(NPC *enemy);
	void getHurt(int dam);		//code for when your getting hurt
	

};

#endif