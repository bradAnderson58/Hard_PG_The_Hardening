#include "GameApplication.h"
#include "GameController.h"
#include "loaderClass.h"
#include <fstream>
#include <sstream>
#include <map>

#define _USE_MATH_DEFINES 
#include <math.h>

//-------------------------------------------------------------------------------------
GameApplication::GameApplication(void): 
	mCurrentObject(0)
{
	gameState = MAINSCREEN;
	level = 0;
}
//-------------------------------------------------------------------------------------
GameApplication::~GameApplication(void)
{
	//just in case
}

//-------------------------------------------------------------------------------------
void GameApplication::createScene(void)
{
	//remove
}

//Use this to make the Game Controller and such
void GameApplication::createFrameListener(void)
{
    
	gameCont = new GameController(this);

    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, gameCont);

    // These were used to create the original mDetailsPanel - may want to recreate with MyGUI
	//don't actually need?
    Ogre::StringVector items;
    items.push_back("cam.pX");
    items.push_back("cam.pY");
    items.push_back("cam.pZ");
    items.push_back("");
    items.push_back("cam.oW");
    items.push_back("cam.oX");
    items.push_back("cam.oY");
    items.push_back("cam.oZ");
    items.push_back("");
    items.push_back("Filtering");
    items.push_back("Poly Mode");

    mRoot->addFrameListener(this);  //addFrameListener!
}

//update graphics on delta time
void
GameApplication::addTime(Ogre::Real deltaTime)
{
	//time updator
	if (gameState == PLAYING || gameState == DEAD_STATE) {

		playerPointer->update(deltaTime); //Yoshimi has a different update function
		healthBar->setProgressPosition(playerPointer->getHealthNow()); // update health bar
		for (NPC* guy : NPClist){
			guy->update(deltaTime);
		}
		healthBar->setProgressPosition(playerPointer->getHealthNow()); //American Association of Highway Officials, Litigators, and Engineers 
	}
}


// toggle to game state s
// handle initializations to each state here as well.
void
GameApplication::toggleState(GameState s)
{
	if (s == MAINSCREEN)	// start screen
	{
		gameState = s;
	}
	else if (s == SETUP)		// initialize actual game
	{
		gameState = s;
		
		// load enviroment and set up level
		loading = new LoaderClass(mSceneMgr, this);
		//loadEnv();
		//setupEnv();
		//use a loading class?

		// reveal HUD and set up gui things
		healthBar->setProgressRange(playerPointer->getHealthStat());
		healthBar->setProgressPosition(playerPointer->getHealthNow());
		healthBar->setVisible(true);

		toggleState(PLAYING);
	}
	else if(s == PLAYING)		// mode where player interacts with world
	{
		// hide menu and inventory
		openMenu(false);
		gameState = s;
	}
	else if (s == MENUSCREEN)	// pause, main menu
	{
		// show main menu
		// for testing, show all gui menus
		gameState = s;
		playerPointer->setMovement(false);	// movement locks up when you pause during movement
		openMenu(true);
	}
	else if (s == DEAD_STATE)
	{
		gameState = s;
		playerPointer->die();
	}
	else
		std::cout << "Not a valid state" << std::endl;
}

void GameApplication::createGUI(void)
{

	mPlatform = new MyGUI::OgrePlatform();
	mPlatform->initialise(mWindow, mSceneMgr); // mWindow is Ogre::RenderWindow*, mSceneManager is Ogre::SceneManager*
	mGUI = new MyGUI::Gui();
	mGUI->initialise();  //don't intialize until after resources have been loaded
	
	double wWidth, wHeight;
	wWidth = mWindow->getWidth();
	wHeight = mWindow->getHeight();

	// progress bar to track health, update this in addtime
													// skin				pos			size		alignment			layer
	healthBar = mGUI->createWidget<MyGUI::ProgressBar>("ProgressBar", 
			50, wHeight-50, 150, 50, MyGUI::Align::Default, "Main");
	
	// menu window to navigate to exit, inventory, etc
	//pauseMenu = mGUI->createWidget<MyGUI::Window>("Window",
	//		wWidth/3, wHeight/3, 300, 400, MyGUI::Align::Center, "Main", "pause");
	
	//menuCtrl = mGUI->createWidget<MyGUI::MenuControl>("MenuControl",
	//	wWidth/3, wHeight/3, 300, 400, MyGUI::Align::Center, "Main", "menu");

	popMenu = mGUI->createWidget<MyGUI::PopupMenu>("PopupMenu",
		wWidth/3, wHeight/3, 300, 400, MyGUI::Align::Center, "Main", "popmenu");

	// menu buttons
	inventoryB = mGUI->createWidget<MyGUI::Button>("Button", 
		wWidth/3+50, wHeight/3+50, 200, 50, MyGUI::Align::Default, "Main", "inventory");
	inventoryB->setCaption("Inventory");

	charRecordB = mGUI->createWidget<MyGUI::Button>("Button", 
		wWidth/3+50, wHeight/3+100, 200, 50, MyGUI::Align::Default, "Main", "records");
	charRecordB->setCaption("Character Records");

	exitB = mGUI->createWidget<MyGUI::Button>("Button", 
		wWidth/3+50, wHeight/3+150, 200, 50, MyGUI::Align::Default, "Main", "exit");
	exitB->setCaption("Exit Game");
	
	// set callbacks
	inventoryB->eventMouseButtonClick += MyGUI::newDelegate(this, &GameApplication::buttonHit); // CLASS_POINTER is pointer to instance of a CLASS_NAME (usually '''this''')
	charRecordB->eventMouseButtonClick += MyGUI::newDelegate(this, &GameApplication::buttonHit);	
	exitB->eventMouseButtonClick += MyGUI::newDelegate(this, &GameApplication::buttonHit);

	//inventoryB->attachToWidget(popMenu, popMenu->getWidgetStyle(), "Main");

	// hide certain guis
	inventoryB->setVisible(false);
	charRecordB->setVisible(false);
	exitB->setVisible(false);
	//popMenu->setVisible(false);
	mGUI->hidePointer();

	if (gameState == MAINSCREEN){
			toggleState(SETUP);
	}
	
}

void GameApplication::buttonHit(MyGUI::WidgetPtr _sender)
{
	std::cout << "I'm a MyGUI button!" << std::endl;
	if (_sender->getName() == "inventory")
		std::cout << "Open dat inventory!" << std::endl;
	else if(_sender->getName() == "records")
		std::cout << "Char records here" << std::endl;
	else if (_sender->getName() == "exit")
	{
		std::cout << "exit dis game now!" << std::endl;
		setShutDown(true);   //app shutdown
	}
}

//open in-game menu screen
void GameApplication::openMenu(bool visible){
	//This will be replaced by GUI code
	//popMenu->setVisible(visible);
	inventoryB->setVisible(visible);
	charRecordB->setVisible(visible);
	exitB->setVisible(visible);
	if (visible)
	{
		mGUI->showPointer();
		std::cout << "This is the main Menu, do this things:" << std::endl;
		std::cout << "Enter Inventory" << std::endl;
		std::cout << "Enter Character Record" << std::endl;
		std::cout << "Return to Game" << std::endl;
	}
	else
	{
		mGUI->hidePointer();
		std::cout << "close menu" << std::endl;
	}
}

//open inventory menu
void GameApplication::openInventory(bool visible){
	if (visible)
	{
		std::cout << "Helm: " << playerPointer->getHelm()->getName() << std::endl;
		std::cout << "Necklace: " << playerPointer->getNeck()->getName() << std::endl;
		std::cout << "Breastplate: " << playerPointer->getBoobs()->getName() << std::endl;
		std::cout << "Pants: " << playerPointer->getPants()->getName() << std::endl;
		std::cout << "Main Hand: " << playerPointer->getWpn()->getName() << std::endl;
		std::cout << "Off Hand: " << "NULL" << std::endl;
	}
	else
	{
		std::cout << "close inventory" << std::endl;
	}
}

void GameApplication::openCharRecord(bool visible){
	if (visible)
	{
		std::cout << "Player Name: Mat Cauthon" << std::endl;
		std::cout << "stats: blah blah" << std::endl;
	}
	else
	{
		std::cout << "close records" << std::endl;
	}
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

void GameApplication::setPlayer(Player* p){
	playerPointer = p;
	gameCont->setPlayer(p);
}
