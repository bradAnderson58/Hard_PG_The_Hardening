#ifndef __InventoryView_h_
#define __InventoryView_h_

#include "UsableItems.h"
#include "Player.h"
#include "GameApplication.h"

#include <MyGUI.h>
#include <MyGUI_OgrePlatform.h>

class InventoryView
{
public:
	InventoryView(MyGUI::Gui* mGUI, int left, int top);
	virtual ~InventoryView(void);

	void addItem(UsableItems* item);
	void open(bool visible);	// open the inventory window
	void update();				// update rendering of window
	
private:
	int mSelectedIndex;			// index of the selected item
	UsableItems* mSelectedItem;	// item currently selected in item box

	MyGUI::WindowPtr mWindow;	// pointer to the inventory window
	MyGUI::ButtonPtr backB;		// button to go back to menu
	MyGUI::ItemBox* mItemBox;	// box to hold things?

	// also an image of the character
	// did another view to display stats
	// and current equipment

	MyGUI::ItemBox* getItemBox(){ return mItemBox; }

	void updateInventory();		// update the contents of our inventory
	void updatePlayer();		// update player upon changing equipment

protected:

	//callbacks
	void buttonHit(MyGUI::WidgetPtr _sender);
};

#endif