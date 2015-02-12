#ifndef __QuadTree_h_
#define __QuadTree_h_

class NPC;
class Objects;

class QuadTree{
private:
	int MAX_OBJECTS = 10;
	int MAX_LEVELS = 5;
 
	int level;
	std::list<NPC*> NPClist;
	std::list<Objects*> objects;
	Rectangle bounds;
	std::list<QuadTree*> nodes;
}

#endif