
#ifndef __LoaderClass_h_
#define __LoaderClass_h_

#include <OgreSceneManager.h>

class Agent;
class GameApplication;
class Grid;

class LoaderClass {
public:
	LoaderClass(Ogre::SceneManager* mgr, GameApplication* a);
	~LoaderClass(void);

	void loadEnv(std::string envText);		//load environemnt
	void setupEnv();	//set up environment

	static std::string getNewName();
private:
	Agent* agent;
	GameApplication* app;
	Grid* grid;							//for reading in level datas
	Ogre::SceneManager* uSceneMgr;		//This will be nice for loading things

	//std::string getNewName();			//Kvothe
};

#endif