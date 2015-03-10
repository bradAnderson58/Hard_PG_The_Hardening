#ifndef __Cell_h_
#define __Cell_h_

#include "UsableItems.h"

#include <MyGUI.h>
#include <MyGUI_OgrePlatform.h>

class Cell
{
public:
	Cell(MyGUI::Gui* mGUI, int left, int top, int size, std::string name);
	virtual ~Cell(void);

	void show(bool visible);
	void click();
	void unClick();

	void removeItem();			// remove (and delete?) item from a cell

	UsableItems* getItem() { return mItem; }
	MyGUI::ImageBox* getImageBox() { return mImgBox; }

	void changeTexture(std::string fname){ mImgBox->setImageTexture(fname); }  //for changing the texture of a slot

	void setItem(UsableItems* item);
	bool isSelected() { return selected; }
private:
	UsableItems* mItem;			// my item!
	MyGUI::ImageBox* mImgBox;	// img to represent/click
	bool selected;				// is cell selected? who knows!

protected:

};

#endif