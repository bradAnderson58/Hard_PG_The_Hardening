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
	InventoryView(MyGUI::Gui* mGUI);
	virtual ~InventoryView(void);

	void open();			// open the inventory window
	void update();			// update rendering of window
	

private:
	int mSelectedItem;			// index of the selected item

	MyGUI::WindowPtr inventW;	// pointer to the inventory window
	MyGUI::ButtonPtr backB;		// button to go back to menu
	MyGUI::ItemBox* mItemBox;	// box to hold things?

	void updateInventory();		// update the contents of our inventory
	void updatePlayer();		// update player upon changing equipment

protected:

	//callbacks
	void buttonHit(MyGUI::WidgetPtr _sender);
};

#endif