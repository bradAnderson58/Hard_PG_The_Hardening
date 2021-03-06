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

	bool mVisible;		//is the inventory currently visible?
	void switchSelected(int row, int col);	//switch with xbox controller

	int swapWrapper();  //return the index of the swapped item
	void updateStatFields();

private:
	int selectedRow;
	int selectedCol;

	Cell* selectCell;
	Cell* equippedCell;

	void updateAll();  //call this to update all the data in the inventory view
	

	GUIController* mainMenu;	//this is for turning control back over the the main menu at the end

	Cell* inventoryGrid[4][5];		// 20 items
	Cell* head;
	Cell* body;
	Cell* legs;
	Cell* weapon;
	Cell* shield;
	Cell* necklace;

	MyGUI::WindowPtr mWindow;	// window for background of inventory
	MyGUI::ListBox* equippedStats; //list guis for stats of inventory items
	MyGUI::ListBox* selectedStats;
	MyGUI::ButtonPtr backB;		// button to go back to menu

	void swap(Cell* a, Cell* b);		// swap items between two cells, return index of swapped inventory item

	void updateInventory(Player* p);	// update the contents of our inventory
	void updateEquipment(Player* p);	// update the equipment slots

protected:

	//callbacks
	void buttonHit(MyGUI::WidgetPtr _sender);
};

#endif