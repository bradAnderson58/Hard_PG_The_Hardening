#include "NPC.h"
#include "GameApplication.h"

NPC::NPC(Ogre::SceneManager* SceneManager, std::string name, std::string filename, float height, float scale, GameApplication* a, int l, GoodBad t):
	Agent(SceneManager, name, filename, height, scale, a)
{
	type = t;
	level = l;
	health = level * 20;
	crit = .01 * level * 5;
	damage = log(level) * level + 5;
	defense = level * 10;  //need to reset based on every enemy
}

NPC::~NPC(void)
{
	
}

void NPC::update(Ogre::Real deltaTime){
	//will overwrite
}

void NPC::updateLocomote(Ogre::Real deltaTime){
	//will overwrite
}


void NPC::updateAnimations(Ogre::Real deltaTime){
	//will overwrite
}

void NPC::fadeAnimations(Ogre::Real deltaTime){
	//will overwrite
}

void NPC::setupAnimations(){
	//will overwrite
}

void NPC::setAnimation(AnimID id, bool reset){
	//will overwrite
}

void NPC::attackPlayer(Player* mainPlayer){
	//possibly change to something else
}

void NPC::attack(NPC* otherGuys){
	//possibly change to something else
}

void NPC::interact(){
	//possible interactions good or bad//// intro to boss sections
}

void NPC::dealDamage(NPC* guy){
	//hitting any NPC
}

void NPC::dealDamagePlayer(Player* player){
	//hitting the player
}

void NPC::checkHit(){
	//look through gameapp to see if something was hit
}

void NPC::getHurt(int d){
	if (d > defense){
		health -= (d - defense);
	}
}




bool 
NPC::nextLocation()
{
	if (mWalkList.empty()){
		return false;
	}
	return true;
}