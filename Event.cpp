#include "Event.h"

Event::Event(GameApplication* a)
{

}

Event::~Event(void)
{

}

void 
Event::addDialog(std::string text)
{
	dialogs.push_back(text);
}

// run the event!
void
Event::run()
{

}


// print dialog to the dialog box
void 
Event::printDialog(std::string)
{

}