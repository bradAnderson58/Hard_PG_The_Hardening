
#include "GUIController.h"
#include "GameApplication.h"
#include "GameController.h"
#include "InventoryView.h"
#include "CharacterRecord.h"
#include "DialogView.h"
#include "Event.h"


GUIController::GUIController(GameApplication* a)
{
	//
	// TODO: Base dimensions of GUI's based on window size 
	//

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

	//This is for the quest log - stylize later
	questWin = mGUI->createWidget<MyGUI::Window>("WindowC", 
		0, 0, 200, 100, MyGUI::Align::Default, "Main", "quest");
	questWin->setCaption("Quest/Debug Window?");

	// supposed to display ogre head, working
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

	//for xbox - set focus on top thing, we can switch these out using setCurrentActive
	inventoryB->_setMouseFocus(true);
	currentActive = INV_B;

	charRecordB = mGUI->createWidget<MyGUI::Button>("Button", 
		wMiddlish, hMiddlish+50, 200, 50, MyGUI::Align::Default, "Main", "records");
	charRecordB->setCaption("Character Records");

	exitB = mGUI->createWidget<MyGUI::Button>("Button", 
		wMiddlish, hMiddlish+100, 200, 50, MyGUI::Align::Default, "Main", "exit");
	exitB->setCaption("Exit Game");
	
	// character record window
	charRecord = new CharacterRecord(mGUI, wMiddlish, hMiddlish, this);
	
	// inventory window
	inventory = new InventoryView(mGUI, wMiddlish+100, hMiddlish-200, this);

	// dialog event view window
	dialog = new DialogView(mGUI, wMiddlish-125, 100, 600, 200, this);
	
	// set callbacks
	inventoryB->eventMouseButtonClick += MyGUI::newDelegate(this, &GUIController::buttonHit); // CLASS_POINTER is pointer to instance of a CLASS_NAME (usually '''this''')
	charRecordB->eventMouseButtonClick += MyGUI::newDelegate(this, &GUIController::buttonHit);
	exitB->eventMouseButtonClick += MyGUI::newDelegate(this, &GUIController::buttonHit);

	// hide guis to reveal later
	questWin->setVisible(false);
	playerImage->setVisible(false);
	healthBar->setVisible(false);
	manaBar->setVisible(false);
	inventoryB->setVisible(false);
	charRecordB->setVisible(false);
	exitB->setVisible(false);

	charRecord->open(false);
	inventory->show(false);
	dialog->show(false);
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
	if (_sender->getName() == "inventory")
		openInventory(true);
	else if(_sender->getName() == "records")
		openCharRecord(true);
	else if (_sender->getName() == "exit")
		app->setShutDown(true);   //app shutdown
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
	openMenu(!visible);
	inventory->show(visible);
	
}

//toggle character records
void GUIController::openCharRecord(bool visible){
	openMenu(!visible);
	charRecord->open(visible);
}

void
GUIController::openADialog(bool visible)
{
	dialog->show(visible);
}

void
GUIController::setDialogEvent(Event* e)
{
	 dialog->setEvent(e); 
}

void
GUIController::cycleDialog()
{
	dialog->setLineRead(true);
}

void
GUIController::updateDialog()
{
	dialog->update(app);
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

void GUIController::recordUpdator(){
	//charRecord->update(app->getPlayerPointer());
	inventory->update(app->getPlayerPointer());
	inventory->updateStatFields();
}

void GUIController::setCurrentActive(bool up){
	
	//set based on current active, and whether up or down
	if (currentActive == INV_B){
		//if we are on inventory we can only move down
		if (!up){
			currentActive = CHAR_B;
			inventoryB->_setMouseFocus(false);
			charRecordB->_setMouseFocus(true);
		}
	
	//charrecord in the middle, can move up or down
	}else if (currentActive == CHAR_B){
		charRecordB->_setMouseFocus(false);
		currentActive = (up) ? INV_B : EXIT_B;
		(up) ? inventoryB->_setMouseFocus(true) : exitB->_setMouseFocus(true);

	//if we are on Exit, we can only move up
	}else if (currentActive == EXIT_B){
		if(up){
			currentActive = CHAR_B;
			exitB->_setMouseFocus(false);
			charRecordB->_setMouseFocus(true);
		}
	}
	if (currentActive == INV_B){

	}
}

//manually call buttonhit on the active button
void GUIController::xBoxSelect(){

	if (inventory->mVisible){
		int index = inventory->swapWrapper();
		app->getPlayerPointer()->switchEquipment(index);
		recordUpdator();
	}
	else{
		(currentActive == INV_B) ? buttonHit(inventoryB) :
			(currentActive == CHAR_B) ? buttonHit(charRecordB) :
			(currentActive == EXIT_B) ? buttonHit(exitB) :
			std::cout << "Something wrong with xBoxSelect?" << std::endl;
	}
}