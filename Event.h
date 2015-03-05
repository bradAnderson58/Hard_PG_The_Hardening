#ifndef __Event_h_
#define __Event_h_

#include "GameApplication.h"
#include "loaderClass.h"			// might need to load in stuff for events

#include <vector>

class Event
{
public:
	Event(GameApplication* a);
	Event(GameApplication* a, std::vector<std::string> script);
	virtual ~Event(void);

	void addDialog(std::string);	// add a line of dialog
	void addNPC(NPC* npc);			// add a npc to event
	void addScript(std::string);	// add a script file to run (python file)

	void run();						// run the event

private:
	std::vector<std::string> dialogs;		// Dialogs to be printed by event
	std::vector<NPC*> freshNPCs;			// NPC's to be added by event
	std::vector<std::string> scriptFiles;	// script files to run. maybe python

	bool repeat;					// repeat the script?

	void runScript(std::string);	// run a script file
	void spawnNPC(NPC* npc);		// spawn a fresh and so clean clean NPC
	void printDialog(std::string);	// print dialog to dialog box

protected:

};

#endif