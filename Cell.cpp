#include "Cell.h"

Cell::Cell(MyGUI::Gui* mGUI, int left, int top, int size, std::string name)
{
	mItem = NULL;

	// create imgbox
	// initially ogre heads!!
	mImgBox = mGUI->createWidget<MyGUI::ImageBox>("ImageBox", 
		left, top, size, size, MyGUI::Align::Default, "Main", name);
	mImgBox->setImageTexture("thumb_cel.png");
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

// assign new item and update img
void
Cell::setItem(UsableItems* item)
{
	mItem = item;

	mImgBox->setImageTexture(item->getImgFile());
	// this is assuming a set image size...
    mImgBox->setImageCoord(MyGUI::IntCoord(0, 0, 128,128));
    mImgBox->setImageTile(MyGUI::IntSize(128, 128));
}



