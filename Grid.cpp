#include "Grid.h"
#include <iostream>
#include <fstream>

////////////////////////////////////////////////////////////////
// create a node
GridNode::GridNode(int nID, int row, int column, bool isC)
{
	this->clear = isC;

	this->rCoord = row;
	this->cCoord = column;

	this->entity = NULL;

	if (isC)
		this->contains = '.';
	else
		this->contains = 'B';

	this->parent = NULL;
	this->cost = -1;
	this->dist = -1;
	this->onPath = false;

	this->inList = NONE;
}

// default constructor
GridNode::GridNode()
{
	nodeID = -999;			// mark these as currently invalid
	this->clear = true;
	this->contains = '.';
} 

////////////////////////////////////////////////////////////////
// destroy a node
GridNode::~GridNode()
{}  // doesn't contain any pointers, so it is just empty

////////////////////////////////////////////////////////////////
// set the node id
void 
GridNode::setID(int id)
{
	this->nodeID = id;
}

////////////////////////////////////////////////////////////////
// set the x coordinate
void 
GridNode::setRow(int r)
{
	this->rCoord = r;
}

////////////////////////////////////////////////////////////////
// set the y coordinate
void 
GridNode::setColumn(int c)
{
	this->cCoord = c;
}

////////////////////////////////////////////////////////////////
// get the x and y coordinate of the node
int 
GridNode::getRow()
{
	return rCoord;
}

int 
GridNode::getColumn()
{
	return cCoord;
}

// return the position of this node
Ogre::Vector3 
GridNode::getPosition(int rows, int cols)
{
	Ogre::Vector3 t;
	t.z = (rCoord * NODESIZE) - (rows * NODESIZE)/2.0 + (NODESIZE/2.0); 
	t.y = 0; 
	t.x = (cCoord * NODESIZE) - (cols * NODESIZE)/2.0 + (NODESIZE/2.0); 
	return t;
}

////////////////////////////////////////////////////////////////
// set the node as walkable
void 
GridNode::setClear()
{
	this->clear = true;
	this->contains = '.';
}

////////////////////////////////////////////////////////////////
// set the node as occupied
void 
GridNode::setOccupied()
{
	this->clear = false;
	this->contains = 'B';
}

////////////////////////////////////////////////////////////////
// is the node walkable
bool 
GridNode::isClear()
{
	return this->clear;
}


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// create a grid
Grid::Grid(Ogre::SceneManager* mSceneMgr, int numRows, int numCols)
{
	this->mSceneMgr = mSceneMgr; 

	assert(numRows > 0 && numCols > 0);
	this->nRows = numRows;
	this->nCols = numCols;

	data.resize(numCols, GridRow(numRows));
		
	// put the coordinates in each node
	int count = 0;
	for (int i = 0; i < numRows; i++)
		for (int j = 0; j < numCols; j++)
		{
			GridNode *n = this->getNode(i,j);
			n->setRow(i);
			n->setColumn(j);
			n->setID(count);
			count++;
		}
}

/////////////////////////////////////////
// destroy a grid
Grid::~Grid(){};  														

////////////////////////////////////////////////////////////////
// get the node specified 
GridNode* 
Grid::getNode(int r, int c)
{
	if (r >= nRows || c >= nCols || r < 0 || c < 0)
		return NULL;

	return &this->data[c].data[r];
}

////////////////////////////////////////////////////////////////
// get adjacent nodes;

//all the cardinal directions test for null and returns the right node
GridNode* 
Grid::getNorthNode(GridNode* n)
{
	if (n != NULL){
		GridNode* temp = getNode(n->getRow()-1, n->getColumn());
		if (temp != NULL && temp->isClear()){
			return temp;
		}
	}
	return NULL;
}

GridNode* 
Grid::getSouthNode(GridNode* n)
{
	if (n != NULL){
		GridNode* temp = getNode(n->getRow()+1, n->getColumn());
		if (temp != NULL && temp->isClear()){
			return temp;
		}
	}
	return NULL;
}

GridNode* 
Grid::getEastNode(GridNode* n)
{
	if (n != NULL){
		GridNode* temp = getNode(n->getRow(), n->getColumn()+1);
		if (temp != NULL && temp->isClear()){
			return temp;
		}
	}
	return NULL;
}

GridNode* 
Grid::getWestNode(GridNode* n)
{
	if (n != NULL){
		GridNode* temp = getNode(n->getRow(), n->getColumn()-1);
		if (temp != NULL && temp->isClear()){
			return temp;
		}
	}
	return NULL;
}


//the four diagonals use the previous cardinal directions to get the right neighbor
//and checks for valid diagonals to avoid wall clipping on corners.
GridNode* 
Grid::getNENode(GridNode* n)  
{
	if (getNorthNode(n) == NULL || getEastNode(n) == NULL){
		return NULL;
	}
	GridNode* temp = getNorthNode(getEastNode(n));
	if (temp != NULL && temp->isClear()){
		return temp;
	}
	return NULL;
}

GridNode* 
Grid::getNWNode(GridNode* n) 
{
	if (getNorthNode(n) == NULL || getWestNode(n) == NULL){
		return NULL;
	}
	GridNode* temp = getNorthNode(getWestNode(n));
	if (temp != NULL && temp->isClear()){
		return temp;
	}
	return NULL;
}

GridNode* 
Grid::getSENode(GridNode* n) 
{
	if (getSouthNode(n) == NULL || getEastNode(n) == NULL){
		return NULL;
	}
	GridNode* temp = getSouthNode(getEastNode(n));
	if (temp != NULL && temp->isClear()){
		return temp;
	}
	return NULL;
}

GridNode* 
Grid::getSWNode(GridNode* n) 
{
	if (getSouthNode(n) == NULL || getWestNode(n) == NULL){
		return NULL;
	}
	GridNode* temp = getSouthNode(getWestNode(n));
	if (temp != NULL && temp->isClear()){
		return temp;
	}
	return NULL;
}

//puts all the valid neighbors into a list for returns
std::list<GridNode*> Grid::getNeighbors(GridNode* n){
	std::list<GridNode*> neighbors;
	GridNode* temp = getNorthNode(n);
	if (temp != NULL){
		neighbors.push_back(temp);
	}
	temp = getSouthNode(n);
	if (temp != NULL){
		neighbors.push_back(temp);
	}
	temp = getEastNode(n);
	if (temp != NULL){
		neighbors.push_back(temp);
	}
	temp = getWestNode(n);
	if (temp != NULL){
		neighbors.push_back(temp);
	}
	temp = getNENode(n);
	if (temp != NULL){
		neighbors.push_back(temp);
	}
	temp = getNWNode(n);
	if (temp != NULL){
		neighbors.push_back(temp);
	}
	temp = getSENode(n);
	if (temp != NULL){
		neighbors.push_back(temp);
	}
	temp = getSWNode(n);
	if (temp != NULL){
		neighbors.push_back(temp);
	}
	return neighbors;
}
////////////////////////////////////////////////////////////////
//get distance between between two nodes
//Manhattan distance including diagonals

int 
Grid::getDistance(GridNode* node1, GridNode* node2)
{
	if (node1 == NULL || node2 == NULL){ //test for NULLs and return a default high value incase it hits in Astar
		return 99999;
	}
	//initialize variables for easier reading
	int dist = 0;
	int row1 = node1->getRow();
	int col1 = node1->getColumn();

	int row2 = node2->getRow();
	int col2 = node2->getColumn();

	if (row1 == row2 && col1 == col2){ return 0;}//check if node1 and node2 are the same

	if(abs(row1-row2) == abs(col1-col2)){return abs(row1-row2) *14;} //check if node1 is diagonal from node2

	if(abs(row1-row2) < abs(col1-col2)){//check for which difference is greater
		return (abs(row1-row2)*14 + (abs(col1-col2)-abs(row1-row2)) * 10); //calculate the distance including diagonals plus row or column
	}
	return (abs(col1-col2)*14 + (abs(row1-row2)-abs(col1-col2))*10);
}

///////////////////////////////////////////////////////////////////////////////
// Print out the grid in ASCII
void 
Grid::printToFile()
{
	std::string path = __FILE__; //gets the current cpp file's path with the cpp file
	path = path.substr(0,1+path.find_last_of('\\')); //removes filename to leave path
	path+= "Grid.txt"; //if txt file is in the same directory as cpp file
	std::ofstream outFile;
	outFile.open(path);

	if (!outFile.is_open()) // oops. there was a problem opening the file
	{
		std::cout << "ERROR, FILE COULD NOT BE OPENED" << std::endl;	
		return;
	}

	for (int i = 0; i < nRows; i++)
	{
		for (int j = 0; j < nCols; j++)
		{
			outFile << this->getNode(i, j)->contains << " ";
		}
		outFile << std::endl;
	}
	outFile.close();
}

GridNode* // load and place a model in a certain location.
Grid::loadObject(std::string name, std::string filename, int row, int height, int col, float scale)
{
	using namespace Ogre;

	if (row >= nRows || col >= nCols || row < 0 || col < 0)
		return NULL;

	Entity *ent = mSceneMgr->createEntity(name, filename);
	
    SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode(name,
        Ogre::Vector3(0.0f, 0.0f,  0.0f));
    node->attachObject(ent);
    node->setScale(scale, scale, scale);


	GridNode* gn = this->getNode(row, col);
	node->setPosition(getPosition(row, col)); 
	node->setPosition(getPosition(row, col).x, height, getPosition(row, col).z);
	gn->setOccupied();
	gn->entity = ent;

	//Ogre::AxisAlignedBox boundBox = ent->getWorldBoundingBox();  //bounding box for barrel
	//uncomment this to see barrel bounding box:
	//node->showBoundingBox(true);

	return gn;
}

////////////////////////////////////////////////////////////////////////////
// Added this method and changed GridNode version to account for varying floor 
// plane dimensions. Assumes each grid is centered at the origin.
// It returns the center of each square. 
Ogre::Vector3 
Grid::getPosition(int r, int c)	
{
	Ogre::Vector3 t;
	t.z = (r * NODESIZE) - (this->nRows * NODESIZE)/2.0 + NODESIZE/2.0; 
	t.y = 0; 
	t.x = (c * NODESIZE) - (this->nCols * NODESIZE)/2.0 + NODESIZE/2.0; 
	return t;
}

std::list<GridNode*>
Grid::aStar(GridNode* start, GridNode* goal){

	//initialize needed variables
	std::vector<GridNode*> open;
	
	std::list<GridNode*> neighbors;
	std::list<GridNode*>::iterator neighborIter;

	std::list<GridNode*> rList;

	int testDist;

	int minindex;
	GridNode* curr = NULL;
	GridNode* temp;

	//reset all the values of for each node
	for(int i = 0; i < this->getRows(); i++){
		for(int j = 0; j < this->getCols();j++){
			temp = this->getNode(i,j);
			temp->setCost(-1);
			temp->setDist(-1);
			temp->setInList(GridNode::NONE);
			temp->setParent(NULL);
		}
	}
	
	//set the distance of start
	start->setDist(0);
	start->setInList(GridNode::OPEN); //put start on the list
	open.push_back(start); //put start on the list

	while (open.size() != 0){//loop while open list is not empty
		for(int i = 0; i < open.size(); i++){//find min in the openlist
			if (curr == NULL){
				curr = open[i];
				minindex = i;
			}
			else if (curr->getDist() > open[i]->getDist()){
				curr = open[i];
				minindex = i;
			}
		}
		open.erase(open.begin() + minindex);//take out min node from openlist

		if (curr == goal){//check if current node is the goal
			while (curr->getParent() != NULL){//back track for the path
				rList.push_front(curr);
				curr = curr->getParent();
			}
			return rList;//return the path
		}
		neighbors = getNeighbors(getNode(curr->getRow(),curr->getColumn()));//get all the neighbors of current
		for (neighborIter = neighbors.begin(); neighborIter != neighbors.end(); neighborIter++){//loop through every neighbor
			if ((*neighborIter)->getInList() == GridNode::NONE){//if neighbor is in no list
				(*neighborIter)->setInList(GridNode::OPEN);//set it in the open list
				(*neighborIter)->setDist(curr->getDist() + getDistance(curr, (*neighborIter)));//calculate the distance
				(*neighborIter)->setCost((*neighborIter)->getDist() + getDistance((*neighborIter), goal));//add the heuristic for cost
				(*neighborIter)->setParent(curr);//put it in the openlist
				open.push_back((*neighborIter));
			}
			else if ((*neighborIter)->getInList() == GridNode::OPEN){//check if node is in openlist
				testDist = curr->getDist() + getDistance(curr, (*neighborIter));
				if (testDist < (*neighborIter)->getDist()){//test if new values are less than old
					//updating values
					(*neighborIter)->setDist(testDist);
					(*neighborIter)->setParent(curr);
					(*neighborIter)->setCost((*neighborIter)->getDist() + getDistance((*neighborIter), goal));
					open.push_back((*neighborIter));
				}
			}
		}
		curr->setInList(GridNode::CLOSED);//put current in the closed list
		curr = NULL;//set current to NULL
	}

	return rList; //return an empty list if no solution was found
	
}