#include "InventoryView.h"
#include "GUIController.h"

#define INVCELLSIZE 50
#define EQPCELLSIZE 100

InventoryView::InventoryView(MyGUI::Gui* mGUI, int left, int top, GUIController* gc)
{
	mainMenu = gc;

	int mLeft = left;
	int mTop = top;
	int number = 0;

	selectedRow = 0;
	selectedCol = 0;

	mWindow = mGUI->createWidget<MyGUI::Window>("WindowC", 
		left-25, top-30, 350, 550, MyGUI::Align::Default, "Overlapped", "invWindow");
	mWindow->setCaption("Inventory");
	mWindow->setProperty("Text", "Test the Text?");
	mWindow->setMovable(false);	// don't want to drag it.

	backB = mGUI->createWidget<MyGUI::Button>("Button", 
		left, top, 75, 50, MyGUI::Align::Default, "Main", "back");
	backB->setCaption("<- Back");
	backB->eventMouseButtonClick += MyGUI::newDelegate(this, &InventoryView::buttonHit);

	// slots for equipment, also empty on construction
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
			// creates a bunch of empty cells, with ogre heads for now
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

	inventoryGrid[selectedRow][selectedCol]->click();  //start in top left

	updateAll();

}

InventoryView::~InventoryView(void)
{
	
}

void
InventoryView::addItem(UsableItems* item)
{

}

void InventoryView::updateAll(){
	//head->setItem(
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

	mWindow->setVisible(visible);
	backB->setVisible(visible);

	mVisible = visible;
}

// swap items between two cells
void
InventoryView::swap(Cell* a, Cell* b)
{
	UsableItems* itemA; 
	UsableItems* itemB;

	itemA = a->getItem();
	itemB = b->getItem();

	if (itemA && itemB)
	{
		UsableItems* temp = itemA;
		a->setItem(itemB);
		b->setItem(temp);
	}
	else if (!itemA && itemB)
	{
		a->setItem(itemB);
		b->removeItem();
	}
	else if (itemA && !itemB)
	{
		b->setItem(itemA);
		a->removeItem();
	}
	// else both are null, do nothing
}

// update rendering of window
void 
InventoryView::update(Player* p)
{
	updateInventory(p);
}

// update the contents of our inventory
void 
InventoryView::updateInventory(Player* p)
{
	std::vector<UsableItems*> tempvec = p->getInventory();
	int mSize = tempvec.size();
	int iter = 0;
	std::cout << mSize << " " << iter << std::endl;
	// update inventory view
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 5; j++){
			if (iter < mSize){
				inventoryGrid[i][j]->setItem(tempvec[iter]);
				iter++;
			}else{
				inventoryGrid[i][j]->setItem(NULL);
				std::cout << "Sent NULL " << std::endl;
			}
		}
	}
	// update equipment view
	head->setItem(p->getHelm());
	body->setItem(p->getBoobs());
	legs->setItem(p->getPants());
	weapon->setItem(p->getWpn());
	shield->setItem(p->getShield());
	necklace->setItem(p->getNeck());
}

// update player's equipment base on equipment view
void
InventoryView::updateEquipment(Player* p)
{
	p->setHelm(head->getItem());
	p->setBoobs(body->getItem());
	p->setPants(legs->getItem());
	p->setWpn(weapon->getItem());
	p->setShield(shield->getItem());
	p->setNeck(necklace->getItem());
	// not efficient, but was quick to write lol
}

// hide inventory if back button hit
void 
InventoryView::buttonHit(MyGUI::WidgetPtr _sender)
{
	if (_sender->getName() == "back")
	{
		show(false);
		mainMenu->openMenu(true);
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

//call this to buttonhit callback move thing yup
void InventoryView::switchSelected(int row, int col){
	int newRow = selectedRow + row;
	int newCol = selectedCol + col;

	if (newRow >= 0 && newRow < 4 && newCol >= 0 && newCol < 5){  //fix magic numbers?
		buttonHit(inventoryGrid[selectedRow+row][selectedCol+col]->getImageBox());
	}
}