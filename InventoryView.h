#ifndef __InventoryView_h_
#define __InventoryView_h_

#include "UsableItems.h"
#include "Player.h"
#include "GameApplication.h"
#include "Cell.h"

#include <MyGUI.h>
#include <MyGUI_OgrePlatform.h>

class GUIController;

class InventoryView
{
public:
	InventoryView(MyGUI::Gui* mGUI, int left, int top, GUIController* gc);
	virtual ~InventoryView(void);

	void addItem(UsableItems* item);	// add item to next available slot
	void show(bool visible);			// open the inventory window
	void update(Player* p);	
	
private:
	int selectedRow;
	int selectedCol;

	GUIController* mainMenu;	//this is for turning control back over the the main menu at the end

	Cell* inventoryGrid[4][5];		// 20 items
	Cell* head;
	Cell* body;
	Cell* legs;
	Cell* weapon;
	Cell* shield;
	Cell* necklace;

	MyGUI::WindowPtr mWindow;	// window for background of inventory
	MyGUI::ButtonPtr backB;		// button to go back to menu

	void swap(Cell* a, Cell* b);		// swap items between two cells

	void updateInventory(Player* p);	// update the contents of our inventory
	void updateEquipment(Player* p);	// update the equipment slots

protected:

	//callbacks
	void buttonHit(MyGUI::WidgetPtr _sender);
};

#endif