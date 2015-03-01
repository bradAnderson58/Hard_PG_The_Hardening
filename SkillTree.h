#ifndef __SkillTree_h_
#define __SkillTree_h_

#include "GameApplication.h"
#include "SkillNode.h"

#include <MyGUI.h>
#include <MyGUI_OgrePlatform.h>

#define NUMSKILLS 7

class SkillTree
{
public:
	SkillTree(MyGUI::Gui* mGUI, int left, int top);
	~SkillTree(void);

	void show(bool visible);	
	void reset();
private:
	SkillNode* skills[NUMSKILLS]; // 7 skills for now

protected:

};

#endif