/*
	GUIController for taking care of all GUI things

*/
#ifndef __GUIController_h_
#define __GUIController_h_

#include <MyGUI.h>
#include <MyGUI_OgrePlatform.h>

class GameApplication;
class InventoryView;

class GUIController{
public:
	GUIController(GameApplication* a);  //constructor / destructor
	~GUIController();

	InventoryView* getInventory() { return inventory; }

	//set health or mana bar
	void setHealth(double health){ healthBar->setProgressPosition(health); }
	void setManaBar(double mana){ manaBar->setProgressPosition(mana); }

	//export some stuffs from game application
	void revealHUD(double health, double mana);
	void hidePointer(){ mGUI->hidePointer(); }		//hide show wrapper shits
	void showPointer(){ mGUI->showPointer(); }

	//menu code stuff - 
	void openMenu(bool visible);
	void openInventory(bool visible);
	void openCharRecord(bool visible);

private:

	MyGUI::OgrePlatform* mPlatform;	//this is the base platform for the mygui stuffs
	MyGUI::Gui* mGUI;
	GameApplication* app;
	Ogre::RenderWindow* mWindow;	//this is pointer to the ogre window

	// gui stuff decl's
	MyGUI::WindowPtr questWin;			// Window to contain quest text/debug info
	MyGUI::ImageBox* playerImage;		// image representing player status
	MyGUI::ProgressPtr healthBar;		// progress bar to track health
	MyGUI::ProgressPtr manaBar;			// progress bar to track mana
	MyGUI::ButtonPtr inventoryB;		// button to access inventory from pause menu
	MyGUI::ButtonPtr charRecordB;		// button to acces character records from pause menu
	MyGUI::ButtonPtr exitB;				// button to exit from pause menu
	InventoryView* inventory;			// inventory window for gear and items

	void buttonHit(MyGUI::WidgetPtr _sender);

};

#endif //End GUIController