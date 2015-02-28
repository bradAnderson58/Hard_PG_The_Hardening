
#include "GUIController.h"
#include "GameApplication.h"
#include "GameController.h"
#include "InventoryView.h"


GUIController::GUIController(GameApplication* a){

	//some setup required
	app = a;
	mWindow = a->getWindow();
	app->getGameCont()->setGUICont(this);
	
	//make the base 
	mPlatform = new MyGUI::OgrePlatform();
	mPlatform->initialise(mWindow, a->getSceneMgr()); // mWindow is Ogre::RenderWindow*, mSceneManager is Ogre::SceneManager*
	mGUI = new MyGUI::Gui();
	mGUI->initialise();  //don't intialize until after resources have been loaded
	
	double wWidth, wHeight;
	int wMiddlish, hMiddlish;
	wWidth = mWindow->getWidth();
	wHeight = mWindow->getHeight();
	wMiddlish = wWidth/3+50;
	hMiddlish = wHeight/3+50;

	questWin = mGUI->createWidget<MyGUI::Window>("WindowC", 
		0, 0, 200, 100, MyGUI::Align::Default, "Main", "quest");
	questWin->setCaption("Quest/Debug Window?");

	// supposed to display ogre head, not working :/
	playerImage = mGUI->createWidget<MyGUI::ImageBox>("ImageBox", 
		0, wHeight-200, 200, 200, MyGUI::Align::Default, "Main", "face");
	playerImage->setImageTexture("thumb_cel.png"); // can't get player face here, player doesn't exist yet
	// this is assuming a set image size...
    playerImage->setImageCoord(MyGUI::IntCoord(0, 0, std::min(128, (int)wWidth), std::min(128, (int)wHeight)));
    playerImage->setImageTile(MyGUI::IntSize(std::min(128, (int)wWidth), std::min(128, (int)wHeight)));

	// progress bar to track health, update this in addtime
	// skin		pos			size		alignment			layer
	healthBar = mGUI->createWidget<MyGUI::ProgressBar>("ProgressBar", 
		200, wHeight-50, 200, 50, MyGUI::Align::Default, "Main", "health");
	manaBar = mGUI->createWidget<MyGUI::ProgressBar>("ProgressBar", 
		200, wHeight-100, 200, 30, MyGUI::Align::Default, "Main", "mana");
	
	// menu buttons
	inventoryB = mGUI->createWidget<MyGUI::Button>("Button", 
		wMiddlish, hMiddlish, 200, 50, MyGUI::Align::Default, "Main", "inventory");
	inventoryB->setCaption("Inventory");

	charRecordB = mGUI->createWidget<MyGUI::Button>("Button", 
		wMiddlish, hMiddlish+50, 200, 50, MyGUI::Align::Default, "Main", "records");
	charRecordB->setCaption("Character Records");

	exitB = mGUI->createWidget<MyGUI::Button>("Button", 
		wMiddlish, hMiddlish+100, 200, 50, MyGUI::Align::Default, "Main", "exit");
	exitB->setCaption("Exit Game");

	// inventory window
	inventory = new InventoryView(mGUI, wMiddlish, hMiddlish);
	
	// set callbacks
	inventoryB->eventMouseButtonClick += MyGUI::newDelegate(this, &GUIController::buttonHit); // CLASS_POINTER is pointer to instance of a CLASS_NAME (usually '''this''')
	charRecordB->eventMouseButtonClick += MyGUI::newDelegate(this, &GUIController::buttonHit);	
	exitB->eventMouseButtonClick += MyGUI::newDelegate(this, &GUIController::buttonHit);

	//inventoryB->attachToWidget(popMenu, popMenu->getWidgetStyle(), "Main");

	// hide guis to reveal later
	questWin->setVisible(false);
	playerImage->setVisible(false);
	healthBar->setVisible(false);
	manaBar->setVisible(false);
	inventoryB->setVisible(false);
	charRecordB->setVisible(false);
	exitB->setVisible(false);
	inventory->open(false);
	//popMenu->setVisible(false);
	mGUI->hidePointer();

}

GUIController::~GUIController(){
	mGUI->shutdown();
	delete mGUI;
	mGUI = 0;   
	mPlatform->shutdown();
	delete mPlatform;
	mPlatform = 0;
}

void GUIController::buttonHit(MyGUI::WidgetPtr _sender)
{
	std::cout << "I'm a MyGUI button!" << std::endl;
	if (_sender->getName() == "inventory")
		openInventory(true);
	else if(_sender->getName() == "records")
		std::cout << "Char records here" << std::endl;
	else if (_sender->getName() == "exit")
	{
		std::cout << "exit dis game now!" << std::endl;
		app->setShutDown(true);   //app shutdown
	}
}

//open in-game menu screen
void GUIController::openMenu(bool visible)
{
	//This will be replaced by GUI code
	//popMenu->setVisible(visible);
	inventoryB->setVisible(visible);
	charRecordB->setVisible(visible);
	exitB->setVisible(visible);
	if (visible)
	{
		std::cout << "This is the main Menu, do this things:" << std::endl;
		std::cout << "Enter Inventory" << std::endl;
		std::cout << "Enter Character Record" << std::endl;
		std::cout << "Return to Game" << std::endl;
	}
	else
	{
		std::cout << "close menu" << std::endl;
	}
}

//open inventory menu
void GUIController::openInventory(bool visible)
{
	openMenu(false);
	inventory->open(visible);
	if (visible)
	{
	}
	else
	{
		std::cout << "close inventory" << std::endl;
	}
}

void GUIController::openCharRecord(bool visible){
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

void GUIController::revealHUD(double health, double mana){

	// reveal HUD and set up gui things
	questWin->setVisible(true);
	playerImage->setVisible(true);

	healthBar->setProgressRange(health);
	healthBar->setProgressPosition(health);
	healthBar->setVisible(true);

	manaBar->setProgressRange(mana);
	manaBar->setProgressPosition(mana);
	manaBar->setVisible(true);
}