
#ifndef __CharacterRecord_h_
#define __CharacterRecord_h_
#include "Player.h"
#include "GameApplication.h"
#include "SkillTree.h"

#include <MyGUI.h>
#include <MyGUI_OgrePlatform.h>
class GUIController;
class Player;

class CharacterRecord
{
public:
	CharacterRecord(MyGUI::Gui* mGUI, int left, int top, GUIController* gc);
	~CharacterRecord(void);

	void open(bool visible);	// open the character window
	void update(Player* pl);				// update rendering of window
	
private:

	MyGUI::WindowPtr mWindow;	// main background window
	MyGUI::ButtonPtr backB;		// button to go back to menu
	MyGUI::ImageBox* charFace;	// look at yourself
	MyGUI::ListBox* vStats;		//display the characters stats
	MyGUI::TextBox* vQuests;	//display the active quests
	
	GUIController* mainMenu;	//we want to be able to set the main menu back

	MyGUI::ImageBox* getItemBox(){ return charFace; }  //need this?
	SkillTree* mSkillTree;

	void updateStats(Player* pl);			// update the contents of our inventory
	void updateQuests();					// update player upon changing equipment

protected:

	//callbacks
	void backButtonCall(MyGUI::WidgetPtr _sender);
};




#endif //end character record