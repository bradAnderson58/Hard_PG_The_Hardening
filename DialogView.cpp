#include "DialogView.h"

DialogView::DialogView(MyGUI::Gui* mGUI, int left, int top, 
					   int width, int height, GUIController* gc)
{
	int faceWidth, dbWidth, pos_face2;
	faceWidth = width * 1/4;
	dbWidth = width * 1/2;
	pos_face2 = left + width - faceWidth;

	// need to figure out getting height and width based on screen size
	face1 = mGUI->createWidget<MyGUI::ImageBox>("ImageBox", 
		left, top, faceWidth, height, MyGUI::Align::Default, "Main", "leftface");
	face2 = mGUI->createWidget<MyGUI::ImageBox>("ImageBox", 
		pos_face2, top, faceWidth, height, MyGUI::Align::Default, "Main", "rightface");
	mDialogBox = mGUI->createWidget<MyGUI::TextBox>("TextureBox",
		left+faceWidth, top, dbWidth, height, MyGUI::Align::Default, "Main", "dialogbox");
	mDialogWindow = mGUI->createWidget<MyGUI::Window>("Window",
		left, top, width, height, MyGUI::Align::Default, "Overlapped", "diaWindow");

	face1->setImageTexture("emptySlot.png");
	face2->setImageTexture("emptySlot.png");

	mDialogBox->setCaption("I can't do that dave.");
}

DialogView::~DialogView(void) {}

// call this when player hits a button to finish reading current text
// and at start of dialog
void
DialogView::update(Event* e)
{
	updateText(e->nextLine());
}

void
DialogView::show(bool visible)
{
	face1->setVisible(visible);
	face2->setVisible(visible);
	mDialogBox->setVisible(visible);
	mDialogWindow->setVisible(visible);
}

void
DialogView::updateText(std::string s)
{
	mDialogBox->setCaption(s);

	//add a transition animation for updating text?
}

void
DialogView::updateFace(std::string filename1, std::string filename2)
{
	face1->setImageTexture(filename1);
	// this is assuming a set image size...
    face1->setImageCoord(MyGUI::IntCoord(0, 0, 128,128));
    face1->setImageTile(MyGUI::IntSize(128, 128));

	face2->setImageTexture(filename1);
	// this is assuming a set image size...
    face2->setImageCoord(MyGUI::IntCoord(0, 0, 128,128));
    face2->setImageTile(MyGUI::IntSize(128, 128));
}