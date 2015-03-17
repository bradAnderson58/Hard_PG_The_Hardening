#include "Event.h"

Event::Event(GameApplication* app, std::vector<std::string> dialog, bool repeatable)
{
	event_dialog = dialog;
	line = event_dialog.begin();
	repeat = repeatable;
	finish = false;
}

Event::~Event(void)
{
	event_dialog.clear();
}

void 
Event::addDialog(std::string text)
{
	event_dialog.push_back(text);
}

// get next string and move iterator,
// if that was the last line, finish the event and return null to signal end
std::string
Event::nextLine()
{
	if (finish) return NULL;
	std::string text = (*line);
	if (line == event_dialog.end())
		finish = true;
	else line++;
	return text;
}

// run the event!
void
Event::run()
{
	// run app->Dialog(Event* this) ?
	// or just use Event as storage and retrieval of dialogs
	// if a character is found holding a dialog event,
	// run it in dialog gui and pause game
}

void
Event::reset()
{
	line = event_dialog.begin();
	finish = false;
}