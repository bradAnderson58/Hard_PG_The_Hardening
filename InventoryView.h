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
	InventoryView();
	virtual ~InventoryView(void);

	void open();			// open the inventory window
	void update();			// update rendering of window
	void updatePlayer();	// update player upon changing equipment

private:
	int mSelectedItem;		// index of the selected item


protected:


}

#endif