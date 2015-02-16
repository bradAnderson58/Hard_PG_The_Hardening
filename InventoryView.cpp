#include "InventoryView.h"

InventoryView::InventoryView(MyGUI::Gui* mGUI)
{
	inventW = mGUI->createWidget<MyGUI::Window>("WindowC", 
			0, 0, 200, 100, MyGUI::Align::Default, "Main", "inventorywindow");
	inventW->setCaption("inventory goes here");

	backB = mGUI->createWidget<MyGUI::Button>("Button", 
		444, 555, 200, 50, MyGUI::Align::Default, "Main", "back");
	backB->setCaption("<- Back");

	mItemBox;
}

InventoryView::~InventoryView(void)
{

}

void InventoryView::open()
{
	// show all elements
	inventW->setVisible(true);
	backB->setVisible(true);
	mItemBox->setVisible(true);
}

void update();			// update rendering of window
void updateInventory();		// update the contents of our inventory
void updatePlayer();

void buttonHit(MyGUI::WidgetPtr _sender);