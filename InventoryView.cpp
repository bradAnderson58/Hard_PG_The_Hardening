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

	//this is the cooresponding equipped item to the selected item
	equippedStats = mGUI->createWidget<MyGUI::ListBox>("ListBox",
						mLeft - 265, mTop + 75, 240, 100, MyGUI::Align::Default, "Main", "currentStats");
	//equippedStats->addItem("Currently Equipped: ");

	//this is the currently highlighted item
	selectedStats = mGUI->createWidget<MyGUI::ListBox>("ListBox",
						mLeft - 265, mTop + 250, 240, 100, MyGUI::Align::Default, "Main", "selectedStats");
	//selectedStats->addItem("Selected Item: ");

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

	updateStatFields();

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

	equippedStats->setVisible(visible);
	selectedStats->setVisible(visible);

	mVisible = visible;
}

int InventoryView::swapWrapper(){
	
	swap(selectCell, equippedCell);
	return (selectedRow * 5) + selectedCol;
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
		b->setItem(NULL);
	}
	else if (itemA && !itemB)
	{
		b->setItem(itemA);
		a->setItem(NULL);
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
	// update inventory view
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 5; j++){
			if (iter < mSize){
				inventoryGrid[i][j]->setItem(tempvec[iter]);
				iter++;
			}else{
				inventoryGrid[i][j]->setItem(NULL);
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
		updateStatFields();
	}

}

void InventoryView::updateStatFields(){
	selectCell = inventoryGrid[selectedRow][selectedCol];
	UsableItems *newItem, *equippedItem;
	newItem = selectCell->getItem();
	equippedStats->removeAllItems();
	selectedStats->removeAllItems();

	//if we've selected an empty slot
	if (newItem == NULL) {
		selectedStats->addItem("Currently Selected: ");
		selectedStats->addItem("Empty");
		equippedStats->addItem("Currently Equipped:");
		return;
	}

	selectedStats->addItem("Currently Selected: " );
	selectedStats->addItem(newItem->getName());

	//selected Item is a weapon
	if (newItem->getType() == UsableItems::WEAPON){

		equippedCell = weapon;
		selectedStats->addItem("Damage Rating: " + std::to_string(newItem->getStat(UsableItems::DAMAGE)));
		equippedItem = weapon->getItem();

		//Null check before getting name of the currently equipped item.  (There may be no item currently equipped)
		if (equippedItem != NULL){
			equippedStats->addItem("Currently Equipped: " );
			equippedStats->addItem(equippedItem->getName());
			equippedStats->addItem("Damage Rating: " + std::to_string(equippedItem->getStat(UsableItems::DAMAGE)));
		}else{
			equippedStats->addItem("Currently Equipped: \nNone");
		}

	//This is the case where selected item is a potion (Which we dont have yet)
	}else if (newItem->getType() == UsableItems::POTION){

		equippedCell = NULL;
		selectedStats->addItem("Heal Thyself");
		equippedStats->addItem("Currently Equipped: ");

	//should be either helm, breastplate, pants, shield or necklace
	}else{
		switch (newItem->getType()){
			case UsableItems::HELM :
				equippedCell = head;
				break;
			case UsableItems::BOOBPLATE :
				equippedCell = body;
				break;
			case UsableItems::PANTS :
				equippedCell = legs;
				break;
			case UsableItems::SHIELD :
				equippedCell = shield;
				break;
			case UsableItems::NECKLACE :
				equippedCell = necklace;
				break;
		}

		equippedItem = equippedCell->getItem();

		selectedStats->addItem("Armor Rating: " + std::to_string(newItem->getStat(UsableItems::DEFENSE)));
		if (equippedItem != NULL){
			equippedStats->addItem("Currently Equipped:");
			equippedStats->addItem(equippedItem->getName());
			equippedStats->addItem("Armor Rating: " + std::to_string(equippedItem->getStat(UsableItems::DEFENSE)));
		}else{
			equippedStats->addItem("Currently Equipped:");
			equippedStats->addItem("None");
		}
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