#include "SkillTree.h"

#define NODESIZE 40

SkillTree::SkillTree(MyGUI::Gui* mGUI, int left, int top)
{
	int mLeft = left;
	int mTop = top;
	int mCenter = left + 100;

	SkillNode* absorbBox;
	SkillNode* bashBox;
	SkillNode* hamstringBox;
	SkillNode* massiveBlowBox;
	SkillNode* fireballBox;
	SkillNode* freezeBox;
	SkillNode* shockBox;

	absorbBox = new SkillNode(mGUI, mCenter, mTop, NODESIZE, 
		"absorb", Player::ABSORB, NULL);

	int leftBranch = mCenter - 50;
	bashBox = new SkillNode(mGUI, leftBranch, mTop+50, NODESIZE, 
		"bash", Player::BASH, absorbBox);
	hamstringBox = new SkillNode(mGUI, leftBranch, mTop+100, NODESIZE, 
		"hamstring", Player::HAMSTRING, bashBox);
	massiveBlowBox = new SkillNode(mGUI, leftBranch, mTop+150, NODESIZE, 
		"massiveblow", Player::MASSIVEBLOW, hamstringBox);

	fireballBox = new SkillNode(mGUI, mCenter+50, mTop+50, NODESIZE, 
		"fireball", Player::FIREBALL, absorbBox);
	freezeBox = new SkillNode(mGUI, mCenter+50, mTop+100, NODESIZE, 
		"freeze", Player::FREEZE, fireballBox);
	shockBox = new SkillNode(mGUI, mCenter+100, mTop+100, NODESIZE, 
		"shock", Player::SHOCK, fireballBox);

	skills[0] = absorbBox;
	skills[1] = bashBox;
	skills[2] = hamstringBox;
	skills[3] = massiveBlowBox;
	skills[4] = fireballBox;
	skills[5] = freezeBox;
	skills[6] = shockBox;

}

SkillTree::~SkillTree(void)
{

}

void
SkillTree::show(bool visible)
{
	for(int i = 0; i < NUMSKILLS; i++)
	{
		skills[i]->show(visible);
	}
}

void
SkillTree::reset()
{
	for(int i = 0; i < NUMSKILLS; i++)
	{
		skills[i]->unClick();
	}
}