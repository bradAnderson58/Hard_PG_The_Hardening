#ifndef __DialogView_h_
#define __DialogView_h_

#include "GameApplication.h"

#include <MyGUI.h>
#include <MyGUI_OgrePlatform.h>

class Event;

// Dialog view is a gui element to view text based conversations between characters
// conversatns will each be represented by a image of their face
// dialog will appear in a TextBox, and update on a key press of some kind.

class DialogView
{
private:
	MyGUI::ImageBox* face1;				// img of conversant 1
	MyGUI::ImageBox* face2;				// img of conversant 2
	MyGUI::TextBox* mDialogBox;			// text goes in here
	MyGUI::WindowPtr mDialogWindow;		// window for background

	Event* mEvent;
	bool lineRead;						// has the current line been read

	void updateText(std::string s);
	void updateFace(std::string filename1, std::string filename2);

public:
	DialogView(MyGUI::Gui* mGUI, int left, int top, 
				int width, int height, GUIController* gc);
	virtual ~DialogView(void);

	void update(GameApplication* app);
	void show(bool visible);
	void setEvent(Event* e) { mEvent = e; }
	void setLineRead(bool b) { lineRead = b; } 
};

#endif