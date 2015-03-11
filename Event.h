#ifndef __Event_h_
#define __Event_h_

#include "GameApplication.h"
#include "loaderClass.h"			// might need to load in stuff for events

#include <vector>

class Event
{
public:
	Event(GameApplication* a);
	virtual ~Event(void);

	void addDialog(std::string);	// add a line of dialog

	void run();						// run the event

private:
	std::vector<std::string> dialogs;		// Dialogs to be printed by event

	bool repeat;					// repeat the script?

	void runScript(std::string);	// run a script file
	void printDialog(std::string);	// print dialog to dialog box

protected:

};

#endif