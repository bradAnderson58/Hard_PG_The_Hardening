#include "NPC.h"
#include "GameApplication.h"

NPC::NPC(Ogre::SceneManager* SceneManager, std::string name, std::string filename, float height, float scale, GameApplication* a):
	Agent(SceneManager, name, filename, height, scale, a)
{
	
}

NPC::~NPC(void)
{
	
}

void NPC::update(Ogre::Real deltaTime){
	
}

void NPC::updateLocomote(Ogre::Real deltaTime){
	
}

//Set movement flags based on a char to represent direction and boolean on or not
void NPC::setMovement(char dir, bool on){
	
}


void NPC::updateAnimations(Ogre::Real deltaTime){

}

void NPC::fadeAnimations(Ogre::Real deltaTime){

}

void NPC::setupAnimations(){
	
}

void NPC::setAnimation(AnimID id, bool reset){
	
}
