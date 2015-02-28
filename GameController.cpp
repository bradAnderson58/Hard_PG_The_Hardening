
#include "GameController.h"
#include "Environment.h"
#include "GUIController.h"

//Contructor requires use of GameApps mWindow for binding
GameController::GameController(GameApplication* a)
		: mInputManager(0),
		mMouse(0),
		mKeyboard(0),
		mJoy(0) {

	app = a;
	uWindow = a->getWindow();		//It will be nice to hav a pointer to the main window
	interactWith = NULL;

	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    uWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    mInputManager = OIS::InputManager::createInputSystem( pl );

	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

	//If the controller is not plugged in, mJoy will be set to 0
	try {
		mJoy = static_cast<OIS::JoyStick*>(mInputManager->createInputObject( OIS::OISJoyStick, true ));
		mJoy->setEventCallback(this);
	}
	catch(...) {
		mJoy = 0;
	}

	//Fix for 1.9
	mInputContext.mKeyboard = mKeyboard;
    mInputContext.mMouse = mMouse;

    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);

    //Set initial mouse clipping size
    windowResized(uWindow);

    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(uWindow, this);

	//keep track of mouse handlings
	keyW = keyA = keyS = keyD = 0;
	
}

GameController::~GameController(void){

}

//Adjust mouse clipping area  - this is necessary for the mouse to be right
void GameController::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

//Unattach OIS before window shutdown (very important under Linux)
void GameController::windowClosed(Ogre::RenderWindow* rw)
{
    //Only close for window that created OIS (the main window in these demos)
    if( rw == uWindow )
    {
        if( mInputManager )
        {
            mInputManager->destroyInputObject( mMouse );
            mInputManager->destroyInputObject( mKeyboard );
			mInputManager->destroyInputObject( mJoy );

            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}

void GameController::captureAll(){
	mKeyboard->capture();
    mMouse->capture();
	if (mJoy != 0) mJoy->capture();
}

void GameController::removeSelf(){
	Ogre::WindowEventUtilities::removeWindowEventListener(uWindow, this);
    windowClosed(uWindow);
}

//Key Press handler
bool GameController::keyPressed( const OIS::KeyEvent &arg ) 
{
    if(arg.key == OIS::KC_F5)   // refresh all textures
    {
        Ogre::TextureManager::getSingleton().reloadAll();
    }
    else if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
    {
        uWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
    }
    else if (arg.key == OIS::KC_ESCAPE)
    {
        app->setShutDown(true);   //app shutdown
    }

	else if (arg.key == OIS::KC_SPACE)
	{
		if (interactWith == NULL){
			if(!player->doingStuff){
				player->changeSpeed(.6);  //jump should be slower
				player->buttonAnimation(arg.key, true);
				player->doingStuff = true;
			}
		}else{
			//interact with
			player->pushInventory(interactWith->getItem());

			// add to inventory gui
			//app->getInventory()->addItem(interactWith->getItem());
			
			app->removeNulls(interactWith);
			interactWith = NULL;
		}
	}
	else if (arg.key == OIS::KC_F)
	{
		std::cout << "fire?" << std::endl;
		player->shoot();
	}
	else if (arg.key == OIS::KC_W || arg.key == OIS::KC_A || arg.key == OIS::KC_S || OIS::KC_D) {
		
		if (app->getGameState() == GameApplication::PLAYING){
			keyHandler(arg.key, true);
		}
	
	}
	//Some wicked attacks - template for spellcasting possibly
	else if (arg.key == OIS::KC_Q){
		if(!player->doingStuff){
			player->buttonAnimation(arg.key, true);
			player->doingStuff = true;
				
		}
	}

	MyGUI::InputManager::getInstance().injectKeyPress(MyGUI::KeyCode::Enum(arg.key), arg.text);

    return true;
}

bool GameController::keyReleased( const OIS::KeyEvent &arg )
{
	//Set the flag to false for whichever key is no longer pressed
	if (app->getGameState() == GameApplication::PLAYING){
		if (arg.key == OIS::KC_W || arg.key == OIS::KC_A || arg.key == OIS::KC_S || arg.key == OIS::KC_D){
			keyHandler(arg.key, false);
		}

		// keys for testing menus without xbox controller ==========
		else if (arg.key == OIS::KC_F1)
		{
			app->toggleState(GameApplication::MENUSCREEN);
		}
		// ========================================================
	}
	else if (app->getGameState() == GameApplication::MENUSCREEN)
	{
		if (arg.key == OIS::KC_F1)
		{
			app->toggleState(GameApplication::PLAYING);
		}
	}

	//myGUI
	MyGUI::InputManager::getInstance().injectKeyRelease(MyGUI::KeyCode::Enum(arg.key));

    return true;
}

void GameController::keyHandler(OIS::KeyCode keyd, bool down){
	double vx, vy, rots;
	vx = vy = rots = 0;

	//update flags based on which button was just pressed
	if (keyd == OIS::KC_W) keyW = (down) ? true : false;
	else if (keyd == OIS::KC_S) keyS = (down) ? true : false;
	else if (keyd == OIS::KC_A) keyA = (down) ? true : false;
	else if (keyd == OIS::KC_D) keyD = (down) ? true : false;

	//if no keys are down, dont move the player
	if (!keyW && !keyA && !keyS && !keyD) player->setMovement(false);
	
	//set player rotation based on keys.  Compatible with Xbox code
	else{

		if (keyW) vy = 1.0;
		if (keyA) vx = -1.0;
		if (keyS) vy = -1.0;
		if (keyD) vx = 1.0;

		rots = std::atan2(vy, vx);
		player->playerRot(rots);
		player->setMovement(true);
		player->setVelocity(.3);
	}
}

bool GameController::mouseMoved( const OIS::MouseEvent &arg )
{
	if (app->getGameState() == GameApplication::PLAYING){
		//rotate the camera
		player->rotationCode(arg);
	}
	
	MyGUI::InputManager::getInstance().injectMouseMove(arg.state.X.abs, arg.state.Y.abs, arg.state.Z.abs);
	
	return true;
}

bool GameController::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	// attack using left and right mouse buttons once the game starts
	if (app->getGameState() == GameApplication::PLAYING){
		if(id == OIS::MB_Left && !player->doingStuff){
			player->changeSpeed(1);
			player->buttonAnimation(id, true);
			player->doingStuff = true;
			player->checkHits(); //This may change
		}
		else if (id == OIS::MB_Right && !player->doingStuff){
			player->buttonAnimation(id, true);
			player->doingStuff = true;
			player->setBlocking(true);  //make
		}
	}
	
	//MyGUI
	MyGUI::InputManager::getInstance().injectMousePress(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));

	//To prevent breakage
	//if (id == OIS::MB_Right) bRMouseDown = false;
	//else if (id == OIS::MB_Left) bLMouseDown = false;
    return true;
}

bool GameController::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if (id == OIS::MB_Right){
		player->doingStuff = false;
		player->setBlocking(false);
		player->buttonAnimation(id, false);
	}
	//MyGUI
	MyGUI::InputManager::getInstance().injectMouseRelease(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
	
	//To prevent breakage
	//if (id == OIS::MB_Right) bRMouseDown = false;
	//else if (id == OIS::MB_Left) bLMouseDown = false;
    return true;
}

//Xbox Controller button functions
bool GameController::buttonPressed( const OIS::JoyStickEvent &arg, int button ){

	if (app->getGameState() == GameApplication::PLAYING){
		//A button is 0
		if (button == 0){
			if (interactWith == NULL){
				if(!player->doingStuff){
					player->changeSpeed(.6);  //jump should be slower
					player->buttonAnimation(OIS::KC_SPACE, true);
					player->doingStuff = true;
				}
			}else{
				//interact with
				player->pushInventory(interactWith->getItem());
				
				app->removeNulls(interactWith);
				interactWith = NULL;
			}
		}
		//X button is 2
		else if (button == 2){
			if(!player->doingStuff){
				player->changeSpeed(1);
				player->buttonAnimation(OIS::MB_Left, true);
				player->doingStuff = true;
				player->checkHits();
			}
		}
		//B button is 1
		else if (button == 1){
			if(!player->doingStuff){
				player->buttonAnimation(OIS::MB_Right, true);
				player->doingStuff = true;
				player->setBlocking(true);
			}
		}
		//Y button is 3
		else if (button == 3){
			//This is just Hari-Kari code for testing purposes
			player->getHurt(5);
		}
		//Start button is 7
		else if (button == 7){
			app->toggleState(GameApplication::MENUSCREEN);
		}

	}
	//Pregame main menu screen
	else if (app->getGameState() == GameApplication::MAINSCREEN) {
		//A button
		if (button == 0){	
			app->toggleState(GameApplication::SETUP);
		}
	}
	//in-game options menu
	else if (app->getGameState() == GameApplication::MENUSCREEN){
		//A button test
		if (button == 0){
			//toggleState(INVENTORY);
			//just show inventory and hide others
			mGUICont->openInventory(true);
		}
		else if (button == 2){
			//toggleState(CHAR_RECORD);
			//show record and hide others
			mGUICont->openCharRecord(true);
		}
		// need buttons to toggle back to menu
		// need buttons prompt exit game window
		//Select is 6
		else if (button == 6){
			app->toggleState(GameApplication::PLAYING);
		}
		//Y button is 3
		else if (button == 3){
			player->switchEquipment(0);  //testing purposes - this will be expanded later
			player->switchEquipment(0);
		}

	}
	return true;
}

bool GameController::buttonReleased( const OIS::JoyStickEvent &arg, int button ){
	//Blocking codes
	if (button == 1){
		player->doingStuff = false;
		player->setBlocking(false);
		player->buttonAnimation(OIS::MB_Right, false);
	}
	return true;
}

bool GameController::axisMoved( const OIS::JoyStickEvent &arg, int axis){
	if (app->getGameState() == GameApplication::PLAYING){

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
			player->playerRot(radLeft);
			player->setMovement(true);
			player->setVelocity(hypotLeft * .23);
		}else{
			player->setMovement(false);
		}
		
		//Rotate camera with right stick
		if (hypotRight >= .3){
			if (xValRight > 0) player->cameraRot(-hypotRight);
			else if (xValRight < 0) player->cameraRot(hypotRight);
		}else{
			player->cameraRot(0);
		}
	}
	return true;
}

