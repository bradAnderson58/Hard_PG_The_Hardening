#ifndef __SkillNode_h_
#define __SkillNode_h_

#include "Player.h"
#include <vector>
#include <MyGUI.h>
#include <MyGUI_OgrePlatform.h>

class SkillNode
{
public:
	SkillNode(MyGUI::Gui* mGUI, int left, int top, int size, 
		std::string name, Player::skillID skill, SkillNode* prev);
	virtual ~SkillNode(void);

	void show(bool visible);
	void click();
	void unClick();

	bool isSelected() { return selected; }

	SkillNode* getPrev() { return mPrev; }

	MyGUI::ImageBox* getIcon() { return mIcon; }
	
private:
	SkillNode* mPrev;

	Player::skillID mSkill;
	MyGUI::ImageBox* mIcon;
	bool selected;

protected:

	void buttonHit(MyGUI::WidgetPtr _sender);
};

#endif