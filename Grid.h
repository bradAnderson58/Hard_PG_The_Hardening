////////////////////////////////////////////////////////
// Class to hold the grid layout of our environment
// Used for navigation and AI, not graphics

#ifndef GRID_H
#define GRID_H
#include <iostream>
#include <vector>
#include <assert.h>
#include "GameApplication.h"

#define NODESIZE 10.0

class GridNode {
private:
	int nodeID;			// identify for the node
	int rCoord;			// row coordinate
	int cCoord;			// column coordinate
	bool clear;			// is the node walkable?
			
	int cost;
	int dist;
	bool onPath;
	GridNode* parent;

public:

	enum listID
	{
		NONE,
		OPEN,
		CLOSED	
	};
	listID inList;

	Ogre::Entity *entity;	// a pointer to the entity in this node
	char contains;		// For printing... B = blocked, S = start, G = goal, numbers = path
	GridNode();			// default constructor
	GridNode(int nID, int row, int column, bool isC = true);	// create a node
	~GridNode();		// destroy a node

	void setID(int id);		// set the node id
	int getID(){return nodeID;};			// get the node ID
	void setRow(int r);		// set the row coordinate
	void setColumn(int c);		// set the column coordinate
	int getRow();				// get the row and column coordinate of the node
	int getColumn();
	Ogre::Vector3 getPosition(int rows, int cols);	// return the position of this node
	void setClear();		// set the node as walkable
	void setOccupied();		// set the node as occupied
	bool isClear();			// is the node walkable

	void setCost(int c){cost = c;}
	const int getCost(){return cost;}
	void setParent(GridNode* p){parent = p;}
	GridNode* getParent(){return parent;}
	void setDist(int d){dist = d;}
	const int getDist(){return dist;}
	void setInList(listID l){inList = l;}
	listID getInList(){return inList;}
	bool isOnPath(){return onPath;}
	void setOnPath(){onPath = !onPath;}

	bool operator<(const GridNode &a){return cost < a.cost;}
};

class GridRow {  // helper class
public:
	std::vector<GridNode> data;
	GridRow(int size) {data.resize(size);};
	~GridRow(){};
};

class Grid {
private:
	Ogre::SceneManager* mSceneMgr;		// pointer to scene graph
	std::vector<GridRow> data;  // actually hold the grid data
	int nRows;					// number of rows
	int nCols;					// number of columns
public:
	Grid(Ogre::SceneManager* mSceneMgr, int numRows, int numCols);	// create a grid
	~Grid();					// destroy a grid

	GridNode* getNode(int r, int c);  // get the node specified 

	GridNode* getNorthNode(GridNode* n);		  // get adjacent nodes;
	GridNode* getSouthNode(GridNode* n);
	GridNode* getEastNode(GridNode* n);
	GridNode* getWestNode(GridNode* n);
	GridNode* getNENode(GridNode* n);
	GridNode* getNWNode(GridNode* n);
	GridNode* getSENode(GridNode* n);
	GridNode* getSWNode(GridNode* n);
	std::list<GridNode*> getNeighbors(GridNode* n);

	int getDistance(GridNode* node1, GridNode* node2);  // get Manhattan distance between between two nodes
	
	void printToFile();				// Print a grid to a file.  Good for debugging

	//this node has a barrel on it
	GridNode* loadObject(std::string name, std::string filename, int row, int height, int col, float scale = 1); // load and place a model in a certain location.
	
	Ogre::Vector3 getPosition(int r, int c);	// return the position
	Ogre::Vector3 getMaxPos() { return this->getPosition(nRows, nCols); }
	Ogre::Vector3 getMinPos() { return this->getPosition(0	  , 0    ); }

	int getRows() { return nRows; }   //needed for world bounds
	int getCols() { return nCols; }

	std::list<GridNode*> aStar(GridNode* start, GridNode* goal);	//aStar algorithm for path finding and avoiding walls/objects

};

#endif