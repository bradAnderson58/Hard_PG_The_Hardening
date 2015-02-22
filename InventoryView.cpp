#include "InventoryView.h"

InventoryView::InventoryView(MyGUI::Gui* mGUI, int left, int top)
{
	mWindow = mGUI->createWidget<MyGUI::Window>("WindowC", 
		left, top, 300, 400, MyGUI::Align::Default, "Main", "iWindow");
	mWindow->setCaption("Inventory Window");

	mItemBox = mGUI->createWidget<MyGUI::ItemBox>("WindowC",
		left, top+200, 300, 200, MyGUI::Align::Default, "Main", "itembox");

	backB = mGUI->createWidget<MyGUI::Button>("Button", 
		left, top-50, 100, 50, MyGUI::Align::Default, "Main", "back");
	backB->setCaption("<- Back");

	backB->eventMouseButtonClick += MyGUI::newDelegate(this, &InventoryView::buttonHit);
}

InventoryView::~InventoryView(void)
{
	
}

void
InventoryView::addItem(UsableItems* item)
{
	std::cout << mItemBox->getItemCount() << std::endl;
	mItemBox->addItem(item);
	std::cout << mItemBox->getItemCount() << std::endl;
	// gets added, but doesn't display it...
}

void 
InventoryView::open(bool visible)
{
	// show all elements
	mWindow->setVisible(visible);
	backB->setVisible(visible);
	mItemBox->setVisible(visible);
}

// update rendering of window
void 
InventoryView::update()
{
	updateInventory();
}

// update the contents of our inventory
void 
InventoryView::updateInventory()
{

}

// changes in inventory should update player stats
void 
InventoryView::updatePlayer()
{

}

// hide inventory if back button hit
void 
InventoryView::buttonHit(MyGUI::WidgetPtr _sender)
{
	std::cout << "inventory button hit." << std::endl;
	if (_sender->getName() == "back")
	{
		std::cout << "close inventory." << std::endl;
		open(false);
		
	}
}