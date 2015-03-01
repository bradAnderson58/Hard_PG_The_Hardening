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

	UsableItems* getItem() { return mItem; } // maybe remove this and adjust scope
	MyGUI::ImageBox* getImageBox() { return mImgBox; }

	void setItem(UsableItems* item);
	bool isSelected() { return selected; }
private:
	UsableItems* mItem;			// my item!
	MyGUI::ImageBox* mImgBox;	// img to represent/click
	bool selected;				// is cell selected? who knows!

protected:

};

#endif