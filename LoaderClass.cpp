
#include "loaderClass.h"
#include "GameApplication.h"
#include "Player.h"
#include "Environment.h"
#include "Event.h"

LoaderClass::LoaderClass(Ogre::SceneManager* mgr, GameApplication* a){
	uSceneMgr = mgr;
	app = a;
	a->stopSound();
	a->engine->play2D("../../media/music1.ogg", true);
	playerLoaded = false;

	loadEnv("forestLevel.txt");
	setupEnv();
}

LoaderClass::~LoaderClass(void){
	if (agent != NULL)  // clean up memory
		delete agent; 
}

// Returns a unique name for loaded objects and agents
std::string LoaderClass::getNewName() // return a unique name 
{
	static int count = 0;	// keep counting the number of objects

	std::string s;
	std::stringstream out;	// a stream for outputing to a string
	out << count++;			// make the current count into a string
	s = out.str();

	return "object_" + s;	// append the current count onto the string
}

void LoaderClass::loadEnv(std::string envTxt){
	using namespace Ogre;	// use both namespaces
	using namespace std;

	StaticGeometry* sgNode;		//used to make walls static objects for faster rendering
	Ogre::SceneNode* mNode;			//
	Ogre::SceneNode* mTran;			//
	Ogre::SceneNode* mTree;			//
	//Ogre::SceneNode* nTree;


	class readEntity // need a structure for holding entities
	{
	public:
		string filename;
		float y;
		float scale;
		float orient;
		bool agent;
	};

	//clear all the old and default lights
	uSceneMgr->destroyAllLights();

	//PlaySound(music.c_str(), NULL, SND_FILENAME|SND_ASYNC);  //Game sound

	ifstream inputfile;		// Holds a pointer into the file

	string path = __FILE__; //gets the current cpp file's path with the cpp file   --THIS NEEDS TO BE REDONE FOR RELEASE
	path = path.substr(0,1+path.find_last_of('\\')); //removes filename to leave path
	path+= envTxt; //if txt file is in the same directory as cpp file
	inputfile.open(path);

	//inputfile.open("D:/CS425-2012/Lecture 8/GameEngine-loadLevel/level001.txt"); //explicit path in Release?
	if (!inputfile.is_open()) // oops. there was a problem opening the file
	{
		cout << "ERROR, FILE COULD NOT BE OPENED" << std::endl;	// Hmm. No output?
		return;
	}

	// the file is open
	int x,z;
	inputfile >> x >> z;	// read in the dimensions of the grid
	string matName;
	inputfile >> matName;	// read in the material name

	//MeshManager::getSingleton()
	// create floor mesh using the dimension read


	string planename = envTxt + "floor";
	MeshManager::getSingleton().createPlane(planename, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
		Plane(Vector3::UNIT_Y, 0), x*NODESIZE, z*NODESIZE, x, z, true, 1, x, z, Vector3::UNIT_Z);
	
	//create a floor entity, give it material, and place it at the origin
	Entity* floor = uSceneMgr->createEntity("Floor", planename);  
	floor->setMaterialName(matName);
	floor->setCastShadows(false);
	uSceneMgr->getRootSceneNode()->attachObject(floor);

	grid = new Grid(uSceneMgr, z, x); // Set up the grid. z is rows, x is columns

	app->setGrid(grid);
	
	string buf;
	inputfile >> buf;	// Start looking for the Objects section
	while  (buf != "Objects")
		inputfile >> buf;
	if (buf != "Objects")	// Oops, the file must not be formated correctly
	{
		cout << "ERROR: Level file error" << endl;
		return;
	}

	// read in the objects
	readEntity *rent = new readEntity();	// hold info for one object
	std::map<string,readEntity*> objs;		// hold all object and agent types;
	while (!inputfile.eof() && buf != "Characters") // read through until you find the Characters section
	{ 
		inputfile >> buf;			// read in the char
		if (buf != "Characters")
		{
			inputfile >> rent->filename >> rent->y >> rent->orient >> rent->scale;  // read the rest of the line
			rent->agent = false;		// these are objects
			objs[buf] = rent;			// store this object in the map
			rent = new readEntity();	// create a new instance to store the next object
		}
	}

	while  (buf != "Characters")	// get through any junk
		inputfile >> buf;
	
	// Read in the characters
	while (!inputfile.eof() && buf != "World") // Read through until the world section
	{
		inputfile >> buf;		// read in the char
		if (buf != "World")
		{
			inputfile >> rent->filename >> rent->y >> rent->scale; // read the rest of the line
			rent->agent = true;			// this is an agent
			objs[buf] = rent;			// store the agent in the map
			rent = new readEntity();	// create a new instance to store the next object
		}
	}
	delete rent; // we didn't need the last one

	mTree = uSceneMgr->getRootSceneNode()->createChildSceneNode("nTree"); // tree for static things
	// read through the placement map
	char c;
	for (int i = 0; i < z; i++)			// down (row)
		for (int j = 0; j < x; j++)		// across (column)
		{
			inputfile >> c;			// read one char at a time
			buf = c + '\0';			// convert char to string
			rent = objs[buf];		// find cooresponding object or agent
			if (rent != NULL){		// it might not be an agent or object
				if (rent->agent)	// if it is an agent...
				{
					// Use subclasses instead!
					if (c == 'n') {

						if (!playerLoaded){  //this is the first time loading, so we must make a new player

							agent = new Player(this->uSceneMgr, getNewName(), rent->filename, rent->y, rent->scale, app);
							app->setPlayer( (Player*) agent );		//access this player
							app->getPlayerPointer()->doUpdateGUI();
							playerLoaded = true;
							genPlayerLight((Player*)agent);
							agent->setPosition(grid->getPosition(i,j).x, 0, grid->getPosition(i,j).z);
							((Player*)agent)->setInitPos(((Player*)agent)->getPosition());		//casting magics
							
						}else{
							Player* p = app->getPlayerPointer();
							genPlayerLight(p);
							p->reloaded(grid->getPosition(i,j).x, 0.0, grid->getPosition(i,j).z);
						}

					}
					else if (c == 'f'){
						Rat* rat = new Rat(this->uSceneMgr, getNewName(), rent->filename, rent->y, rent->scale, app, app->getLoadInd() + 1, NPC::BAD, NPC::GUARD);
						rat->setPosition(grid->getPosition(i,j).x, 0, grid->getPosition(i,j).z);
						rat->setStartPos();
						app->pushNPCs(rat);
						//enemy code will go here	
					}
					else if (c == 'S'){
						Skelebro* skele = new Skelebro(this->uSceneMgr, getNewName(), rent->filename, rent->y, rent->scale, app, app->getLoadInd() + 1, NPC::BAD, NPC::GUARD);
						skele->setPosition(grid->getPosition(i,j).x, 0, grid->getPosition(i,j).z);
						skele->setStartPos();
						app->pushNPCs(skele);
					}
					// add a flying demon
					else if (c == 'D') 
					{
						// need each type of enemy to be a child of (<bad>NPC), to give their own animation states
						Diablous* flyingDemon = new Diablous(this->uSceneMgr, getNewName(), rent->filename, 
							rent->y, rent->scale, app, app->getLoadInd() + 1, NPC::BAD, NPC::GUARD);
						flyingDemon->setPosition(grid->getPosition(i,j).x, 0, grid->getPosition(i,j).z);
						flyingDemon->setStartPos();
						app->pushNPCs(flyingDemon);
					}
					else if (c == 'P')
					{
						Priestess* priest_chick = new Priestess(this->uSceneMgr, getNewName(), rent->filename, 
							rent->y, rent->scale, app, app->getLoadInd() + 1, NPC::BAD, NPC::GUARD);
						priest_chick->setPosition(grid->getPosition(i,j).x, 0, grid->getPosition(i,j).z);
						priest_chick->setStartPos();
						app->pushNPCs(priest_chick);
					}
					// added another Ninja to test dialog interaction
					else if (c == 'N')	
					{
						NPC* npcNinja = new NPC(this->uSceneMgr, getNewName(), rent->filename, rent->y, rent->scale, app, app->getLoadInd() + 1, NPC::GOOD, NPC::NONE);
						npcNinja->setPosition(grid->getPosition(i,j).x, 0, grid->getPosition(i,j).z);
						npcNinja->setStartPos();

						// set up a test event and give to ninja
						std::vector<std::string> someDialog;
						someDialog.push_back("A boats a boat,\n but a Mystery box could be anything. \nEven a Boat!!!");
						someDialog.push_back("You know how much we've wanted one of those.");
						someDialog.push_back("We'll take the box!");
						npcNinja->setEvent(new Event(app, someDialog, true));

						app->pushGoodGuy(npcNinja);
					}
				}
				else	// Load objects - non-agents will go in here
				{
					Environment* obj;
					bool reg = true;  //dont push placements
					if (c == 's'){  //this is a chest
						obj = new Environment(this->uSceneMgr, getNewName(), rent->filename, rent->y, rent->scale, app, Environment::LOOT);
					}
					else if (c == 'd'){  //I'm a door
						obj = new Environment(this->uSceneMgr, getNewName(), rent->filename, rent->y, rent->scale, app, Environment::DOOR);
					}
					else if (c == 'L'){
						obj = new Environment(this->uSceneMgr, getNewName(), rent->filename, rent->y, rent->scale, app, Environment::LOCKED_DOOR);
						app->setLocked(obj);
					}
					else if (c == 'p'){  //I'm a pushable object
						obj = new Environment(this->uSceneMgr, getNewName(), rent->filename, rent->y, rent->scale, app, Environment::MOVEABLE);
					}
					else if (c == 'P'){ //pad for penguin
						obj = new Environment(this->uSceneMgr, getNewName(), rent->filename, rent->y, rent->scale, app, Environment::PLACEMENT);
						app->pushPlacement(obj);
						reg = false;
						obj->setPosition(grid->getPosition(i, j).x, -5.2, grid->getPosition(i, j).z);
					}else if (c == 'a'){  //transport to next level
						obj = new Environment(this->uSceneMgr, getNewName(), rent->filename, rent->y, rent->scale, app, Environment::TRANSPORT);
					}
					else if (c == 'k'){  //key for locked door in maze
						obj = new Environment(this->uSceneMgr, getNewName(), rent->filename, rent->y, rent->scale, app, Environment::KEY);
					}

					//set position and put in list of interactable objects
					if (reg){
						obj->setPosition(grid->getPosition(i,j).x, 0, grid->getPosition(i,j).z);
						app->pushEnvObj(obj);
					}
				}
					
			}
			else // not an object or agent
			{
				if (c == 'w') // create a wall - this stays
				{
					Entity* ent = uSceneMgr->createEntity(getNewName(), Ogre::SceneManager::PT_CUBE);
					//ent->setMaterialName("ToonRockWall");
					ent->setMaterialName("Examples/Rockwall");
					//Ogre::SceneNode* mNode = uSceneMgr->getRootSceneNode()->createChildSceneNode();
					Ogre::SceneNode* mNode = uSceneMgr->getSceneNode("nTree")->createChildSceneNode(); // static thing, replace prev line with this
					mNode->attachObject(ent);
					mNode->scale(0.1f,0.2f,0.1f); // cube is 100 x 100
					grid->getNode(i,j)->setOccupied();  // indicate that agents can't pass through
					mNode->setPosition(grid->getPosition(i,j).x, 10.0f, grid->getPosition(i,j).z);
					app->pushWalls(mNode->getPosition());
					//mNode->showBoundingBox(true);
					//app->pushWallEntity(ent);
				}
				else if (c == 'i') // create a invisible wall
				{
					Entity* ent = uSceneMgr->createEntity(getNewName(), Ogre::SceneManager::PT_CUBE);
					ent->setMaterialName("Examples/RustySteel");
					Ogre::SceneNode* mNode = uSceneMgr->getRootSceneNode()->createChildSceneNode();
					mNode->attachObject(ent);
					mNode->scale(0.1f,0.2f,0.1f); // cube is 100 x 100
					grid->getNode(i,j)->setOccupied();  // indicate that agents can't pass through
					mNode->setPosition(grid->getPosition(i,j).x, 10.0f, grid->getPosition(i,j).z);
					mNode->setVisible(false);
					app->pushWalls(mNode->getPosition());
				}
				else if (c == 'b') // create borderwalls
				{
					Entity* ent = uSceneMgr->createEntity(getNewName(), Ogre::SceneManager::PT_CUBE);
					ent->setMaterialName("Examples/Rockwall");
					//Ogre::SceneNode* mNode = uSceneMgr->getRootSceneNode()->createChildSceneNode();
					Ogre::SceneNode* mNode = uSceneMgr->getSceneNode("nTree")->createChildSceneNode(); // static thing
					mNode->attachObject(ent);
					mNode->scale(0.1f,0.2f,0.1f); // cube is 100 x 100
					grid->getNode(i,j)->setOccupied();  // indicate that agents can't pass through
					mNode->setPosition(grid->getPosition(i,j).x, 10.0f, grid->getPosition(i,j).z);
					mNode->setVisible(true);
					app->pushBorder(mNode->getPosition());
				}
				else if (c == 'e')
				{
					ParticleSystem::setDefaultNonVisibleUpdateTimeout(5);  // set nonvisible timeout
					ParticleSystem* ps = uSceneMgr->createParticleSystem(getNewName(), "Examples/PurpleFountain");
					Ogre::SceneNode* mNode = uSceneMgr->getRootSceneNode()->createChildSceneNode();
					mNode->attachObject(ps);
					mNode->setPosition(grid->getPosition(i,j).x, 0.0f, grid->getPosition(i,j).z);
				}
				else if (c == 'T' ) // trees
				{
					Entity* ent = uSceneMgr->createEntity(getNewName(), "treebrown_002.mesh");
					//Ogre::SceneNode* mNode = uSceneMgr->getRootSceneNode()->createChildSceneNode();
					Ogre::SceneNode* mNode = uSceneMgr->getSceneNode("nTree")->createChildSceneNode(); // static thing, replace prev line with this
					mNode->attachObject(ent);
					mNode->scale(2.5f,3.5f,2.5f);
					grid->getNode(i,j)->setOccupied();  // indicate that agents can't pass through
					mNode->setPosition(grid->getPosition(i,j).x, 4.0f, grid->getPosition(i,j).z);
					app->pushWalls(mNode->getPosition()); // treat trees as walls
				}
				else if (c == 'A') 
				{
					Entity* ent = uSceneMgr->createEntity(getNewName(), "arbol_otono.mesh");
					//Ogre::SceneNode* mNode = uSceneMgr->getRootSceneNode()->createChildSceneNode();
					Ogre::SceneNode* mNode = uSceneMgr->getSceneNode("nTree")->createChildSceneNode(); // static thing, replace prev line with this
					mNode->attachObject(ent);
					mNode->scale(1.5f,2.5f,1.5f);
					grid->getNode(i,j)->setOccupied();  // indicate that agents can't pass through
					mNode->setPosition(grid->getPosition(i,j).x, 1.5f, grid->getPosition(i,j).z);
					app->pushWalls(mNode->getPosition()); // treat trees as walls
				}
				else if( c == 't'){
					Entity* ent = uSceneMgr->createEntity(getNewName(), "tudorhouse.mesh");
					//Ogre::SceneNode* mNode = uSceneMgr->getRootSceneNode()->createChildSceneNode();
					Ogre::SceneNode* mNode = uSceneMgr->getSceneNode("nTree")->createChildSceneNode(); // static thing, replace prev line with this
					mNode->attachObject(ent);
					mNode->scale(0.05, 0.05, 0.05);
					//mNode->translate(0, 27, 0);
					grid->getNode(i,j)->setOccupied();  // indicate that agents can't pass through
					mNode->setPosition(grid->getPosition(i,j).x, 27.0f, grid->getPosition(i,j).z);
					app->pushWalls(mNode->getPosition()); // treat trees as walls
				}
			}
		}

	//Static geomerty
	sgNode = uSceneMgr->createStaticGeometry("StaticTree");  
	sgNode->addSceneNode(uSceneMgr->getSceneNode("nTree"));
	sgNode->build();
	mTree->removeAndDestroyAllChildren();

	while  (!inputfile.eof() && buf != "Lights")	// read until light positions
		inputfile >> buf;

	// generate lights
	// 1 - point light, 2 - ambient, 3 - spotlight, 4 - directional
	std::list<Ogre::Light*> lights = app->getLightList();

	for (int i = 0; i < z; i++)			// down (row)
	{
		for (int j = 0; j < x; j++)		// across (column)
		{
			inputfile >> c;			// read one char at a time
			buf = c + '\0';			// convert char to string

			if (c == '1')	// point light
			{
				Ogre::Vector3 pos = Ogre::Vector3(	grid->getPosition(i,j).x, 
													20.0f, 
													grid->getPosition(i,j).z );

				Ogre::Light* mLight = uSceneMgr->createLight();
				mLight->setType(Light::LT_POINT);
				mLight->setPosition(pos);
				mLight->setSpecularColour(ColourValue::White);
				mLight->setDiffuseColour(ColourValue::White);
				// set light with a range of 100, and its power decreases 
				// farther away from it
				// http://www.ogre3d.org/tikiwiki/tiki-index.php?page=-Point+Light+Attenuation
				mLight->setAttenuation(200, 0.5, 0.022, 0.0019); 
				mLight->setCastShadows(false);
				lights.push_back(mLight);
			}
		}
	}

	// delete all of the readEntities in the objs map
	rent = objs["s"]; // just so we can see what is going on in memory
	
	std::map<string,readEntity*>::iterator it;
	for (it = objs.begin(); it != objs.end(); it++) // iterate through the map
	{
		delete (*it).second; // delete each readEntity
	}
	objs.clear(); // calls their destructors if there are any. (not good enough)
	
	inputfile.close();

	//set up world boundaries
	app->setMaxes((grid->getRows() - 1) /2 , (grid->getCols() - 1) / 2);

	agent = NULL;	//this gets deleted elsewhere (is a player or an NPC)
	app->toggleState(GameApplication::PLAYING);
}

void LoaderClass::setupEnv(){

	using namespace Ogre;

	// set shadow properties
	uSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);
	uSceneMgr->setShadowColour(ColourValue(0.5, 0.5, 0.5));
	uSceneMgr->setShadowTextureSize(1024);
	uSceneMgr->setShadowTextureCount(1);

	// disable default camera control so the character can do its own 
	//mCameraMan->setStyle(OgreBites::CS_FREELOOK); // CS_FREELOOK, CS_ORBIT, CS_MANUAL

	// use small amount of ambient lighting
	//uSceneMgr->setAmbientLight(ColourValue(0.2f, 0.1f, 0.1f));

	// add a bright light above the scene
	// want to create point lights from level editor...
	// if no other lights, make a default one
	//if(app->getLightList().empty())
	//{

	//	Ogre::Light* mLight = uSceneMgr->createLight();
	//	mLight->setType(Light::LT_POINT);
	//	//mLight->setPosition(-10, 80, 20);
	//	mLight->setPosition(0,20,0);
	//	mLight->setSpecularColour(ColourValue::White);
	//	mLight->setDiffuseColour(ColourValue::White);
	//	mLight->setCastShadows(false);
	//}

	// sky/background
	uSceneMgr->setSkyDome(true, "Examples/EveningSkyBox", 5, 8);
}

void LoaderClass::genPlayerLight(Player* p)
{
	using namespace Ogre;

	// give the player a small light
	Ogre::Light* mLight = uSceneMgr->createLight();
	mLight->setType(Light::LT_POINT);
	mLight->setSpecularColour(ColourValue::White);
	mLight->setDiffuseColour(ColourValue::White);
	mLight->setAttenuation(100, 0.5, 0.045, 0.0075); 
	mLight->setCastShadows(false);

	// attach light to character
	//Ogre::SceneNode* lantternNode = p->getMBodyNode()->createChildSceneNode();
	//lantternNode->attachObject(mLight);
	p->getMBodyNode()->attachObject(mLight);
}