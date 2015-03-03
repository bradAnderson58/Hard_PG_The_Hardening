#include "CharacterRecord.h"
#include "GUIController.h"
#include "Player.h"
#include <string.h>

CharacterRecord::CharacterRecord(MyGUI::Gui* mGUI, int left, int top, GUIController* gc)
{
	mainMenu = gc;

	//This is the same size as the Inventory window
	mWindow = mGUI->createWidget<MyGUI::Window>("WindowC", 
		left - 100, top - 225, 450, 500, MyGUI::Align::Default, "Overlapped", "iWindow");
	mWindow->setCaption("Character Record");
	mWindow->setProperty("Text", "Test the Text?");
	mWindow->setMovable(false);

	//set up character face
	charFace = mGUI->createWidget<MyGUI::ImageBox>("ImageBox",
		left - 25, top - 200, 300, 250, MyGUI::Align::Default, "Main", "charface");
	charFace->setImageTexture("thumb_cel.png");
	charFace->setImageCoord(MyGUI::IntCoord(0, 0, 128, 128));
    charFace->setImageTile(MyGUI::IntSize(128, 128));
	
	//text boxes
	vStats = mGUI->createWidget<MyGUI::ListBox>("ListBox",
		left - 90, top + 50, 200, 200, MyGUI::Align::Default, "Main", "statdisplay");
	//vStats->setCaption("Player Stats\nStrength:\nDexterity:\nConstitution:\nIntelligence:\nEvil:");
	vStats->addItem("PlayerStats");

	/*
	vQuests = mGUI->createWidget<MyGUI::TextBox>("WindowC",
		left + 200, top / 2, 300, 200, MyGUI::Align::Default, "Main", "questdisplay");
	vQuests->setCaption("Quest Log");
	*/
	mSkillTree = new SkillTree(mGUI, left+100, top*1.2);
	mSkillTree->reset();

	backB = mGUI->createWidget<MyGUI::Button>("Button", 
		left + 75, top - 250, 100, 25, MyGUI::Align::Default, "Main", "back");
	backB->setCaption("<- Back");

	backB->eventMouseButtonClick += MyGUI::newDelegate(this, &CharacterRecord::backButtonCall);
}

CharacterRecord::~CharacterRecord(){

}

//toggle hide/show
void CharacterRecord::open(bool visible){

	// show all elements
	mWindow->setVisible(visible);
	backB->setVisible(visible);
	charFace->setVisible(visible);
	vStats->setVisible(visible);
	//vQuests->setVisible(visible);
	mSkillTree->show(visible);
}

//called for updating stats or quests?
void CharacterRecord::update(Player* pl){
	updateStats(pl);
	updateQuests();
}

//change stats on level up or what have you
void CharacterRecord::updateStats(Player* pl){
	//Clear out old stats
	vStats->removeAllItems();

	//append new stats
	vStats->addItem("PlayerStats");
	std::string test = std::to_string(5);
	vStats->addItem("Strength:      " + std::to_string(pl->getStrength()));
	vStats->addItem("Dexterity:     " + std::to_string(pl->getDex()));
	vStats->addItem("Constitution:  " + std::to_string(pl->getConstitution()));
	vStats->addItem("Intelligence:  " + std::to_string(pl->getIntel()));
	vStats->addItem("Evil:          " + std::to_string(pl->getEvil()));
}
void CharacterRecord::updateQuests(){}

void CharacterRecord::backButtonCall(MyGUI::WidgetPtr _sender){
	open(false);
	mainMenu->openMenu(true);
}