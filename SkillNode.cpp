#include "SkillNode.h"

SkillNode::SkillNode(MyGUI::Gui* mGUI, int left, int top, int size, 
					 std::string name, Player::skillID skill, SkillNode* prev,
					 std::string img)
{
	mPrev = prev;

	mSkill = skill;

	if (img == "")
	{
		mIcon = mGUI->createWidget<MyGUI::ImageBox>("ImageBox", 
			left, top, size, size, MyGUI::Align::Default, "Main", name);
		mIcon->setImageTexture("thumb_cel.png");
		// this is assuming a set image size...
		mIcon->setImageCoord(MyGUI::IntCoord(0, 0, 128,128));
		mIcon->setImageTile(MyGUI::IntSize(128, 128));
	}
	else
	{
		mIcon = mGUI->createWidget<MyGUI::ImageBox>("ImageBox", 
			left, top, size, size, MyGUI::Align::Default, "Main", name);
		mIcon->setImageTexture(img);
		// this is assuming a set image size...
		mIcon->setImageCoord(MyGUI::IntCoord(0, 0, 56, 56));
		mIcon->setImageTile(MyGUI::IntSize(56, 56));
	}

	mIcon->eventMouseButtonClick += MyGUI::newDelegate(this, &SkillNode::buttonHit);

	selected = false;
}

SkillNode::~SkillNode(void)
{

}

void
SkillNode::show(bool visible)
{ 
	mIcon->setVisible(visible); 
}

// click this cell, making img box more visible
// and represent as selected
void
SkillNode::click()
{
	if (!mPrev || mPrev->isSelected())
	{
		selected = true;
		mIcon->setAlpha(1.00);
		std::cout << "Activate " << mIcon->getName() << std::endl;
	}
}

void
SkillNode::unClick()
{
	selected = false;
	mIcon->setAlpha(0.60);
}

void
SkillNode::buttonHit(MyGUI::WidgetPtr _sender)
{
	if (selected)
		unClick();
	else
		click();
}

