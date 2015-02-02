#include "GameApplication.h"
#include <fstream>
#include <sstream>
#include <map>

#define _USE_MATH_DEFINES 
#include <math.h>

//-------------------------------------------------------------------------------------
GameApplication::GameApplication(void): 
	mCurrentObject(0),
	bLMouseDown(false),
	bRMouseDown(false)
{
	agent = NULL; // Init member data
	gameState = MAINSCREEN;
	gameOver = false;
	level = 0;
	ghettoSelect = 0; //probably take this out later

}
//-------------------------------------------------------------------------------------
GameApplication::~GameApplication(void)
{
	if (agent != NULL)  // clean up memory
		delete agent; 
}

//-------------------------------------------------------------------------------------
void GameApplication::createScene(void)
{
    //delete or repurpose this?
	//mRayScnQuery = mSceneMgr->createRayQuery(Ogre::Ray());

}
//////////////////////////////////////////////////////////////////
// Returns a unique name for loaded objects and agents
std::string getNewName() // return a unique name 
{
	static int count = 0;	// keep counting the number of objects

	std::string s;
	std::stringstream out;	// a stream for outputing to a string
	out << count++;			// make the current count into a string
	s = out.str();

	return "object_" + s;	// append the current count onto the string
}

// load level from file
void // Load the buildings or ground plane, etc
GameApplication::loadEnv()
{
	using namespace Ogre;	// use both namespaces
	using namespace std;

	class readEntity // need a structure for holding entities
	{
	public:
		string filename;
		float y;
		float scale;
		float orient;
		bool agent;
	};

	//PlaySound(music.c_str(), NULL, SND_FILENAME|SND_ASYNC);  //Game sound
	
	GridNode *temp;

	ifstream inputfile;		// Holds a pointer into the file

	string path = __FILE__; //gets the current cpp file's path with the cpp file   --THIS NEEDS TO BE REDONE FOR RELEASE
	path = path.substr(0,1+path.find_last_of('\\')); //removes filename to leave path
	path+= "level001.txt"; //if txt file is in the same directory as cpp file
	inputfile.open(path);

	//inputfile.open("D:/CS425-2012/Lecture 8/GameEngine-loadLevel/level001.txt"); //explicit path!!!
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

	// create floor mesh using the dimension read
	MeshManager::getSingleton().createPlane("floor", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
		Plane(Vector3::UNIT_Y, 0), x*NODESIZE, z*NODESIZE, x, z, true, 1, x, z, Vector3::UNIT_Z);
	
	//create a floor entity, give it material, and place it at the origin
	Entity* floor = mSceneMgr->createEntity("Floor", "floor");
	floor->setMaterialName(matName);
	floor->setCastShadows(false);
	mSceneMgr->getRootSceneNode()->attachObject(floor);

	grid = new Grid(mSceneMgr, z, x); // Set up the grid. z is rows, x is columns
	
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
						agent = new Player(this->mSceneMgr, getNewName(), rent->filename, rent->y, rent->scale, this);
						playerPointer = (Player*) agent;  //you are a yoshimi
						agent->setPosition(grid->getPosition(i,j).x, 0, grid->getPosition(i,j).z);
						playerPointer->setInitPos(playerPointer->getPosition());
					}else if (c == 'f'){
						Rat* rat = new Rat(this->mSceneMgr, getNewName(), rent->filename, rent->y, rent->scale, this, 1, NPC::BAD, Rat::GUARD);
						rat->setPosition(grid->getPosition(i,j).x, 0, grid->getPosition(i,j).z);
						NPClist.push_back(rat);
						//enemy code will go here	
					}
					
				}
				else	// Load objects - non-agents will go in here
				{
					if (rent->filename == "tudorhouse.mesh"){
						String work = getNewName();
						//temp = grid->loadObject(work, rent->filename, i, rent->y, j, rent->scale);
						//housePointer = mSceneMgr->getSceneNode(work);

						Entity *ent = mSceneMgr->createEntity(work, rent->filename);
						
						//This house code is no longer needed, but will break stuff if taken out - DELETE
						housePointer = mSceneMgr->getRootSceneNode()->createChildSceneNode(work,
							grid->getPosition(i,j));
						housePointer->attachObject(ent);
						housePointer->setScale(rent->scale, rent->scale, rent->scale);
						housePointer->translate(0,rent->y,0);
						houseInitPos = housePointer->getPosition();
					}
					else {//The temp object holds a pointer to the barrel node, which we need for bounding box access
						temp = grid->loadObject(getNewName(), rent->filename, i, rent->y, j, rent->scale);
					}
				}
					
			}
			else // not an object or agent
			{
				if (c == 'w') // create a wall - this stays
				{
					Entity* ent = mSceneMgr->createEntity(getNewName(), Ogre::SceneManager::PT_CUBE);
					ent->setMaterialName("Examples/RustySteel");
					Ogre::SceneNode* mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
					mNode->attachObject(ent);
					mNode->scale(0.1f,0.2f,0.1f); // cube is 100 x 100
					grid->getNode(i,j)->setOccupied();  // indicate that agents can't pass through
					mNode->setPosition(grid->getPosition(i,j).x, 10.0f, grid->getPosition(i,j).z);
					wallList.push_back(mNode);
				}
				else if (c == 'i') // create a invisible wall
				{
					Entity* ent = mSceneMgr->createEntity(getNewName(), Ogre::SceneManager::PT_CUBE);
					ent->setMaterialName("Examples/RustySteel");
					Ogre::SceneNode* mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
					mNode->attachObject(ent);
					mNode->scale(0.1f,0.2f,0.1f); // cube is 100 x 100
					grid->getNode(i,j)->setOccupied();  // indicate that agents can't pass through
					mNode->setPosition(grid->getPosition(i,j).x, 10.0f, grid->getPosition(i,j).z);
					mNode->setVisible(false);
					wallList.push_back(mNode);
				}
				else if (c == 'b') // create borderwalls
				{
					Entity* ent = mSceneMgr->createEntity(getNewName(), Ogre::SceneManager::PT_CUBE);
					ent->setMaterialName("Examples/RustySteel");
					Ogre::SceneNode* mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
					mNode->attachObject(ent);
					mNode->scale(0.1f,0.2f,0.1f); // cube is 100 x 100
					grid->getNode(i,j)->setOccupied();  // indicate that agents can't pass through
					mNode->setPosition(grid->getPosition(i,j).x, 10.0f, grid->getPosition(i,j).z);
					mNode->setVisible(true);
					borderWalls.push_back(mNode);
				}
				else if (c == 'e')
				{
					ParticleSystem::setDefaultNonVisibleUpdateTimeout(5);  // set nonvisible timeout
					ParticleSystem* ps = mSceneMgr->createParticleSystem(getNewName(), "Examples/PurpleFountain");
					Ogre::SceneNode* mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
					mNode->attachObject(ps);
					mNode->setPosition(grid->getPosition(i,j).x, 0.0f, grid->getPosition(i,j).z);
				}
			}
		}
	
	// delete all of the readEntities in the objs map
	rent = objs["s"]; // just so we can see what is going on in memory (delete this later)
	
	std::map<string,readEntity*>::iterator it;
	for (it = objs.begin(); it != objs.end(); it++) // iterate through the map
	{
		delete (*it).second; // delete each readEntity
	}
	objs.clear(); // calls their destructors if there are any. (not good enough)
	
	inputfile.close();

	//set up world boundaries
	xMax = (grid->getRows() - 1) / 2;
	zMax = (grid->getCols() - 1) / 2;

	agent = NULL;	//this gets deleted by agent destructor
}

void // Set up lights, shadows, etc
GameApplication::setupEnv()
{
	using namespace Ogre;

	// set shadow properties
	mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);
	mSceneMgr->setShadowColour(ColourValue(0.5, 0.5, 0.5));
	mSceneMgr->setShadowTextureSize(1024);
	mSceneMgr->setShadowTextureCount(1);

	// disable default camera control so the character can do its own 
	mCameraMan->setStyle(OgreBites::CS_FREELOOK); // CS_FREELOOK, CS_ORBIT, CS_MANUAL

	// use small amount of ambient lighting
	mSceneMgr->setAmbientLight(ColourValue(0.5f, 0.5f, 0.5f));

	// add a bright light above the scene
	Ogre::Light* mLight = mSceneMgr->createLight();
	mLight->setType(Light::LT_POINT);
	mLight->setPosition(-10, 40, 20);
	mLight->setSpecularColour(ColourValue::White);
	mLight->setDiffuseColour(ColourValue::White);

	mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8); // pretty sky
}

void
GameApplication::addTime(Ogre::Real deltaTime)
{
	if (!gameOver){
		if (gameState == PLAYING) {
			playerPointer->update(deltaTime); //Yoshimi has a different update function
			for (NPC* guy : NPClist){
				guy->update(deltaTime);
			}
		}
	}
}

bool 
GameApplication::keyPressed( const OIS::KeyEvent &arg ) // Moved from BaseApplication
{
    if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up

    if (arg.key == OIS::KC_F)   // toggle visibility of advanced frame stats - leave for now
    {
        mTrayMgr->toggleAdvancedFrameStats();
    }
    else if (arg.key == OIS::KC_G)   // toggle visibility of even rarer debugging details - leave for now
    {
        if (mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
        {
            mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
            mDetailsPanel->show();
        }
        else
        {
            mTrayMgr->removeWidgetFromTray(mDetailsPanel);
            mDetailsPanel->hide();
        }
    }
    else if (arg.key == OIS::KC_T)   // cycle polygon rendering mode - DELETE?
    {
        Ogre::String newVal;
        Ogre::TextureFilterOptions tfo;
        unsigned int aniso;

        switch (mDetailsPanel->getParamValue(9).asUTF8()[0])
        {
        case 'B':
            newVal = "Trilinear";
            tfo = Ogre::TFO_TRILINEAR;
            aniso = 1;
            break;
        case 'T':
            newVal = "Anisotropic";
            tfo = Ogre::TFO_ANISOTROPIC;
            aniso = 8;
            break;
        case 'A':
            newVal = "None";
            tfo = Ogre::TFO_NONE;
            aniso = 1;
            break;
        default:
            newVal = "Bilinear";
            tfo = Ogre::TFO_BILINEAR;
            aniso = 1;
        }

        Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(tfo);
        Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(aniso);
        mDetailsPanel->setParamValue(9, newVal);
    }
    else if (arg.key == OIS::KC_R)   // cycle polygon rendering mode - DELETE?
    {
        Ogre::String newVal;
        Ogre::PolygonMode pm;

        switch (mCamera->getPolygonMode())
        {
        case Ogre::PM_SOLID:
            newVal = "Wireframe";
            pm = Ogre::PM_WIREFRAME;
            break;
        case Ogre::PM_WIREFRAME:
            newVal = "Points";
            pm = Ogre::PM_POINTS;
            break;
        default:
            newVal = "Solid";
            pm = Ogre::PM_SOLID;
        }

        mCamera->setPolygonMode(pm);
        mDetailsPanel->setParamValue(10, newVal);
    }
    else if(arg.key == OIS::KC_F5)   // refresh all textures
    {
        Ogre::TextureManager::getSingleton().reloadAll();
    }
    else if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
    {
        mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
    }
    else if (arg.key == OIS::KC_ESCAPE)
    {
        mShutDown = true;
    }
	else if (arg.key == OIS::KC_SPACE)
	{
		if(!playerPointer->doingStuff){
			playerPointer->changeSpeed(.6);  //jump should be slower
			playerPointer->buttonAnimation('j');
			playerPointer->doingStuff = true;
		}
	}
	else if (arg.key == OIS::KC_W) {
		
		if (gameState == PLAYING){
			playerPointer->playerRot(M_PI / 2);
			playerPointer->setMovement(true);
			playerPointer->setVelocity(.5);

		}
	}
	else if (arg.key == OIS::KC_A) {
		if (gameState == PLAYING){
			playerPointer->playerRot(M_PI);
			playerPointer->setMovement(true);
			playerPointer->setVelocity(.5);
		}
	}
	else if (arg.key == OIS::KC_D) {
		if (gameState == PLAYING){
			playerPointer->playerRot(0);
			playerPointer->setMovement(true);
			playerPointer->setVelocity(.5);
		}
	
	}
	else if (arg.key == OIS::KC_S) {
		if (gameState == PLAYING){
			playerPointer->playerRot(4.712);
			playerPointer->setMovement(true);
			playerPointer->setVelocity(.5);
		}
	}
	//Some wicked attacks - template for spellcasting possibly
	else if (arg.key == OIS::KC_Q){
		if(!playerPointer->doingStuff){
			playerPointer->buttonAnimation('t');
			playerPointer->doingStuff = true;
		}
	}
    return true;
}

bool GameApplication::keyReleased( const OIS::KeyEvent &arg )
{
	//Set the flag to false for whichever key is no longer pressed
	if (gameState == PLAYING){
		if (arg.key == OIS::KC_W || arg.key == OIS::KC_A || arg.key == OIS::KC_S || arg.key == OIS::KC_D){
			playerPointer->setMovement(false);
		}
	}
    return true;
}

bool GameApplication::mouseMoved( const OIS::MouseEvent &arg )
{
	if (gameState == PLAYING){
		//rotate the camera
		playerPointer->rotationCode(arg);
	}
	
    if (mTrayMgr->injectMouseMove(arg)) return true; //DELETE?
    return true;
}

bool GameApplication::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	// attack using left and right mouse buttons once the game starts
	if (gameState == PLAYING){
		if(id == OIS::MB_Left && !playerPointer->doingStuff){
			playerPointer->changeSpeed(1);
			playerPointer->buttonAnimation('s');
			playerPointer->doingStuff = true;
			playerPointer->checkHits('s'); //This may change
		}
		else if (id == OIS::MB_Right && !playerPointer->doingStuff){
			playerPointer->buttonAnimation('k');
			playerPointer->doingStuff = true;
			playerPointer->checkHits('k');
		}
	}
	
	//To prevent breakage
	if (id == OIS::MB_Right) bRMouseDown = false;
	else if (id == OIS::MB_Left) bLMouseDown = false;
    if (mTrayMgr->injectMouseDown(arg, id)) return true;
    return true;
}

bool GameApplication::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	//To prevent breakage
	if (id == OIS::MB_Right) bRMouseDown = false;
	else if (id == OIS::MB_Left) bLMouseDown = false;
	if (mTrayMgr->injectMouseUp(arg, id)) return true;
    return true;
}

//Xbox Controller button functions
bool GameApplication::buttonPressed( const OIS::JoyStickEvent &arg, int button ){
	std::cout << button << std::endl;

	if (gameState == PLAYING){
		//A button is 0
		if (button == 0){
			if (!playerPointer->doingStuff){
				playerPointer->changeSpeed(.8);
				playerPointer->buttonAnimation('j');
				playerPointer->doingStuff = true;
			}
		}
		//X button is 2
		else if (button == 2){
			if(!playerPointer->doingStuff){
				playerPointer->changeSpeed(1);
				playerPointer->buttonAnimation('s');
				playerPointer->doingStuff = true;
				playerPointer->checkHits('s');
			}
		}
		//Start button is 7
		else if (button == 7){
			gameState = MENUSCREEN;
			openMenu();
		}

	}
	//Pregame main menu screen
	else if (gameState == MAINSCREEN) {
		//A button
		if (button == 0){	
			mTrayMgr->hideCursor();
			mTrayMgr->destroyAllWidgetsInTray(OgreBites::TL_CENTER); //going to remove
			loadEnv();
			setupEnv();
			gameState = PLAYING;
		}
	}
	//in-game options menu
	else if (gameState == MENUSCREEN){
		//A button test
		if (button == 0){
			openInventory();
			gameState = INVENTORY;
		}
		if (button == 2){
			openCharRecord();
			gameState = CHAR_RECORD;
		}
		//Select is 6
		if (button == 6){
			gameState = PLAYING;
		}

	}
	//inventory screen
	else if (gameState == INVENTORY){
		//Select
		if (button == 6){
			gameState = MENUSCREEN;
			openMenu();
		}
	}
	//Character record screen
	else if (gameState == CHAR_RECORD){
		//Select is 6
		if (button == 6){
			gameState = MENUSCREEN;
			openMenu();
		}
	}
	return true;
}

bool GameApplication::buttonReleased( const OIS::JoyStickEvent &arg, int button ){
	//Some code will be here eventually
	return true;
}

bool GameApplication::axisMoved( const OIS::JoyStickEvent &arg, int axis){
	if (gameState == PLAYING){

		//first normalize
		//left joystick
		double xValLeft =  ((double)(arg.state.mAxes[1].abs)) / 32800.0; //x axis
		double yValLeft =  ((double)(arg.state.mAxes[0].abs)) / 32800.0; //y axis

		//right joystick
		double xValRight =  ((double)(arg.state.mAxes[3].abs)) / 32800.0; //x axis
		double yValRight =  ((double)(arg.state.mAxes[2].abs)) / 32800.0; //y axis

		//Now trig
		double radLeft = std::atan2(-yValLeft, xValLeft);		//left angle in radians
		double hypotLeft =	std::sqrt(std::pow(xValLeft, 2) + std::pow(yValLeft, 2));	//left hypotenuse

		double radRight = std::atan2(-yValRight, xValRight);	//right angle in radians
		double hypotRight =	std::sqrt(std::pow(xValRight, 2) + std::pow(yValRight, 2));	//right hypotenuse
			
		//Rotate player to direction of left stick
		if (hypotLeft >= .2){ //avoid hypersensitivity
			playerPointer->playerRot(radLeft);
			playerPointer->setMovement(true);
			playerPointer->setVelocity(hypotLeft * .23);
		}else{
			playerPointer->setMovement(false);
		}
		
		//Rotate camera with right stick
		if (hypotRight >= .3){
			if (xValRight > 0) playerPointer->cameraRot(-hypotRight);
			else if (xValRight < 0) playerPointer->cameraRot(hypotRight);
		}else{
			playerPointer->cameraRot(0);
		}
	}
	return true;
}

void GameApplication::createGUI(void)
{
	
	if (mTrayMgr == NULL) return;
	using namespace OgreBites; 
	
	//Set up our GUI buttons-- this will be replaced by CEGUI

	OgreBites::Label *title = mTrayMgr->createLabel(TL_CENTER, "Title", "Hard PG: The Hardening", 500.0f);
	mTrayMgr->createSeparator(TL_CENTER,"sep", 500.0f);
	
	cont = mTrayMgr->createButton(TL_CENTER, "ClickMe", "Press A", 200.0);
	mTrayMgr->buttonHit(cont);

	//////////////////////////////////////////////////////////////////////////////////
}

void GameApplication::buttonHit(OgreBites::Button* b)
{
	if (b->getName() == "ClickMe")
	{
		//Delete start GUI and start game
		if (gameState == MAINSCREEN){
			mTrayMgr->hideCursor();
			mTrayMgr->destroyAllWidgetsInTray(OgreBites::TL_CENTER); //going to remove
			loadEnv();
			setupEnv();
			gameState = PLAYING;
		}
	}
}

//open in-game menu screen
void GameApplication::openMenu(){
	//This will be replaced by GUI code
	std::cout << "This is the main Menu, do this things:" << std::endl;
	std::cout << "Enter Inventory" << std::endl;
	std::cout << "Enter Character Record" << std::endl;
	std::cout << "Return to Game" << std::endl;
}

//open inventory menu
void GameApplication::openInventory(){
	std::cout << "Helm: " << playerPointer->getHelm()->getName() << std::endl;
	std::cout << "Necklace: " << playerPointer->getNeck()->getName() << std::endl;
	std::cout << "Breastplate: " << playerPointer->getBoobs()->getName() << std::endl;
	std::cout << "Pants: " << playerPointer->getPants()->getName() << std::endl;
	std::cout << "Main Hand: " << playerPointer->getWpn()->getName() << std::endl;
	std::cout << "Off Hand: " << "NULL" << std::endl;
}

void GameApplication::openCharRecord(){
	std::cout << "Player Name: Mat Cauthon" << std::endl;
	std::cout << "stats: blah blah" << std::endl;
}

void GameApplication::endGame(char condition){
	//refurbish this

}

void GameApplication::destroyallChildren(Ogre::SceneNode* p){

	//WHat the fuck does all this do?

	Ogre::SceneNode::ObjectIterator it = p->getAttachedObjectIterator();
	while (it.hasMoreElements()){
		Ogre::MovableObject* o = static_cast<Ogre::MovableObject*>(it.getNext());
		p->getCreator()->destroyMovableObject(o);
	}

	Ogre::SceneNode::ChildNodeIterator itChild = p->getChildIterator();

   while ( itChild.hasMoreElements() )
   {
      Ogre::SceneNode* pChildNode = static_cast<Ogre::SceneNode*>(itChild.getNext());
      destroyallChildren( pChildNode );
   }

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
