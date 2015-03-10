#include "Cell.h"

Cell::Cell(MyGUI::Gui* mGUI, int left, int top, int size, std::string name)
{
	mItem = NULL;

	// create imgbox
	// initially ogre heads!!
	mImgBox = mGUI->createWidget<MyGUI::ImageBox>("ImageBox", 
		left, top, size, size, MyGUI::Align::Default, "Main", name);
	mImgBox->setImageTexture("emptySlot.png");
	// this is assuming a set image size...
    mImgBox->setImageCoord(MyGUI::IntCoord(0, 0, 128,128));
    mImgBox->setImageTile(MyGUI::IntSize(128, 128));
}

Cell::~Cell(void)
{

}

void
Cell::show(bool visible)
{ 
	mImgBox->setVisible(visible); 
}

// click this cell, making img box more visible
// and represent as selected
void
Cell::click()
{
	selected = true;
	mImgBox->setAlpha(1.00);
}

void
Cell::unClick()
{
	selected = false;
	mImgBox->setAlpha(0.70);
}

void
Cell::removeItem()
{
	free(mItem);	// do we need to free usableItems, or does ogre handle?
	mItem = NULL;
}

// assign new item and update img
void
Cell::setItem(UsableItems* item)
{
	if (item){

		std::cout << "Got an item " << item->getImgFile() << std::endl; 

		mItem = item;

		mImgBox->setImageTexture(item->getImgFile());
		// this is assuming a set image size...
		mImgBox->setImageCoord(MyGUI::IntCoord(0, 0, 128,128));
		mImgBox->setImageTile(MyGUI::IntSize(128, 128));
	}
	else{
		std::cout << "Didnt have an item" << std::endl;
		mImgBox->setImageTexture("emptySlot.png");
	}
}



