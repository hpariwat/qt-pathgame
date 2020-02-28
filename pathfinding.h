#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <deque>
#include <queue>
#include <memory>
#include <set>
#include "node.h"

class PathFinding
{
public:
    PathFinding();
    std::deque<std::shared_ptr<Node> > Astar(int startx, int starty, int endx, int endy, std::vector<std::shared_ptr<Node>> & allNodes);
    float heuCalc(int x, int y, int endx, int endy);
    bool tileExists(int x, int y, int boundaryX, int boundaryY);
    void processNearbyPoint(bool isDiagonal,
                            std::shared_ptr<Node> & currentNode,
                            float currentNodeG,
                            float currentNodeValue,
                            std::shared_ptr<Node> & nearbyPointNode,
                            std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, CompareNodePtrF> & open);
    void performPath(std::deque<std::shared_ptr<Node>> & solution);
    void restoreAllNodes(std::vector<std::shared_ptr<Node>> & allNodes);

    void setHeuWeight(int value);

private:
    int heuWeight;



};


#endif // PATHFINDING_H
