#include "InventoryView.h"

#define INVCELLSIZE 50
#define EQPCELLSIZE 100

InventoryView::InventoryView(MyGUI::Gui* mGUI, int left, int top)
{
	// set initial items to all be NULL
	int mLeft = left;
	int mTop = top;
	int number = 0;

	selectedRow = 0;
	selectedCol = 0;

	backB = mGUI->createWidget<MyGUI::Button>("Button", 
		left, top, 75, 50, MyGUI::Align::Default, "Main", "back");
	backB->setCaption("<- Back");
	backB->eventMouseButtonClick += MyGUI::newDelegate(this, &InventoryView::buttonHit);

	head = new Cell(mGUI, mLeft + 100	, mTop		, EQPCELLSIZE, "head");
	body = new Cell(mGUI, mLeft + 100	, mTop + 100, EQPCELLSIZE, "body");
	legs = new Cell(mGUI, mLeft + 100	, mTop + 200, EQPCELLSIZE, "legs");
	weapon = new Cell(mGUI, mLeft		, mTop + 100, EQPCELLSIZE, "weapon");
	shield = new Cell(mGUI, mLeft + 200	, mTop + 100, EQPCELLSIZE, "shield");
	necklace = new Cell(mGUI, mLeft + 200, mTop		, EQPCELLSIZE, "neck");

	mTop += 300;
	mLeft += 25;
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			std::string name = static_cast<std::ostringstream*>( &(std::ostringstream() << number) )->str();
			inventoryGrid[i][j] = new Cell(mGUI, mLeft, mTop, INVCELLSIZE, name);
			inventoryGrid[i][j]->getImageBox()->setAlpha(0.70);
			inventoryGrid[i][j]->getImageBox()->eventMouseButtonClick 
				+=  MyGUI::newDelegate(this, &InventoryView::buttonHit);
			mLeft += INVCELLSIZE;
			number++;
		}
		mLeft = left +25;
		mTop += INVCELLSIZE;
	}

}

InventoryView::~InventoryView(void)
{
	
}

void
InventoryView::addItem(UsableItems* item)
{

}

void 
InventoryView::show(bool visible)
{
	// show all inventory elements
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 5; j++)
			inventoryGrid[i][j]->show(visible);

	head->show(visible);
	body->show(visible);
	legs->show(visible);
	weapon->show(visible);
	shield->show(visible);
	necklace->show(visible);

	backB->setVisible(visible);
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

// get current equipment of player, and update equipment images
void
InventoryView::updateEquipment()
{

}


// hide inventory if back button hit
void 
InventoryView::buttonHit(MyGUI::WidgetPtr _sender)
{
	std::cout << "I'm a MyGUI button!" << std::endl;
	if (_sender->getName() == "back")
	{
		show(false);
	}
	// what inv item is selected
	else 
	{
		// select item, and up its alpha to show which is selected visually
		inventoryGrid[selectedRow][selectedCol]->unClick();
		int myInt = std::stoi(_sender->getName());
		selectedRow = myInt / 5;
		selectedCol = myInt % 5;
		inventoryGrid[selectedRow][selectedCol]->click();
	}

}