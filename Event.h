#ifndef __Event_h_
#define __Event_h_

#include "GameApplication.h"

#include <vector>


// Event class, for now, will pass dialogs when promped 
class Event
{
public:
	Event(GameApplication* app, std::vector<std::string> dialog);
	virtual ~Event(void);

	void addDialog(std::string text);			// add a line of dialog
	std::string nextLine();						// get next line of dialog

	void run();									// run the event
	void reset();								// reset the dialog if repeatable

private:
	std::vector<std::string> event_dialog;		// Dialogs to be printed by event
	std::vector<std::string>::iterator line;	// next line of dialog

	bool repeat;								// if repeat is false, git rid of once ran
	bool finish;

};

#endif