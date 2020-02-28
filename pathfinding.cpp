#include "pathfinding.h"
#include <cmath>
#include <iostream>
#include <deque>
#include <queue>
#include <qdebug.h>
#include <algorithm>

#define D 1
#define D2 sqrt(2)

PathFinding::PathFinding(){ heuWeight = 1; }

std::deque<std::shared_ptr<Node>> PathFinding::Astar(int startx, int starty, int endx, int endy, std::vector<std::shared_ptr<Node>> & allNodes)
{
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, CompareNodePtrF> open;
    std::deque<std::shared_ptr<Node>> solution;

    restoreAllNodes(allNodes);

    //necessary info of the worldmap:
    std::vector<std::shared_ptr<Node>>::iterator it = allNodes.end();
    it--;
    int boundaryX = (*it)->posx;
    int boundaryY = (*it)->posy;
    int fullX = boundaryX +1;
    int fullY = boundaryY +1;


// 1),2), create the rootNode and push it into open:
    int startIdx = fullX*starty+startx;
    auto rootNode = allNodes.at(startIdx);
    rootNode->parent = nullptr;
    rootNode->setH(heuCalc(startx, starty, endx, endy));
    open.push(rootNode);
    rootNode->setLocation(1);

// 3), while open is not empty:
    while(!open.empty())
    {
// 3-1), pop the node with the best f and assign it to currentNode:
        int j = 0;
        //while(open.top()->getIsold()) open.pop();
        std::shared_ptr<Node> currentNode = open.top();
        open.pop();
        currentNode->setLocation(0);

        //getting info of the current tile:
        int currentX = currentNode->getPosx();
        int currentY = currentNode->getPosy();//(x,y)
        float currentNodeG = currentNode->getG();
        float currentNodeV = currentNode->getValue();

// 3-2), if currentNode is the goalPoint:
        if(currentX==endx && currentY==endy)
        {
            std::shared_ptr<Node> goal = currentNode;
            for(; goal->parent; goal=goal->parent)
            {
                solution.push_front(goal);
            }
            break;
        }

        j = 2;
 //3-3-2), for every nearbyPoint, check if a node of this point has been created before and operate correspondingly:
        int x; int y;
        //(x-1,y-1)
        x = currentX-1;
        y = currentY-1;
        if((x>=0) && (x<=boundaryX) && (y>=0) && (y<=boundaryY) && allNodes.at(fullX*y+x)->value<100)
        {
            allNodes.at(fullX*y+x)->setH(heuCalc(x,y,endx,endy));
            processNearbyPoint(true, currentNode, currentNodeG, currentNodeV, allNodes.at(fullX*y+x), open);
            j=0;
        }
        //(x,y-1)
        x = currentX;
        y = currentY-1;
        if((x>=0) && (x<=boundaryX) && (y>=0) && (y<=boundaryY) && allNodes.at(fullX*y+x)->value<100)
        {
            allNodes.at(fullX*y+x)->setH(heuCalc(x,y,endx,endy));
            processNearbyPoint(false, currentNode, currentNodeG, currentNodeV, allNodes.at(fullX*y+x), open);
            j=1;
        }
        //(x-1,y-1)
        x = currentX-1;
        y = currentY-1;
        if((x>=0) && (x<=boundaryX) && (y>=0) && (y<=boundaryY) && allNodes.at(fullX*y+x)->value<100)
        {
            allNodes.at(fullX*y+x)->setH(heuCalc(x,y,endx,endy));
            processNearbyPoint(false, currentNode, currentNodeG, currentNodeV, allNodes.at(fullX*y+x), open);
            j=1;
        }
        //(x-1,y)
        x = currentX-1;
        y = currentY;
        if((x>=0) && (x<=boundaryX) && (y>=0) && (y<=boundaryY) && allNodes.at(fullX*y+x)->value<100)
        {
            allNodes.at(fullX*y+x)->setH(heuCalc(x,y,endx,endy));
            processNearbyPoint(false, currentNode, currentNodeG, currentNodeV, allNodes.at(fullX*y+x), open);
            j=1;
        }
        //(x+1,y)
        x = currentX+1;
        y = currentY;
        if((x>=0) && (x<=boundaryX) && (y>=0) && (y<=boundaryY) && allNodes.at(fullX*y+x)->value<100)
        {
            allNodes.at(fullX*y+x)->setH(heuCalc(x,y,endx,endy));
            processNearbyPoint(false, currentNode, currentNodeG, currentNodeV, allNodes.at(fullX*y+x), open);
            j=1;
        }
        //(x-1,y+1)
        x = currentX-1;
        y = currentY+1;
        if((x>=0) && (x<=boundaryX) && (y>=0) && (y<=boundaryY) && allNodes.at(fullX*y+x)->value<100)
        {
            allNodes.at(fullX*y+x)->setH(heuCalc(x,y,endx,endy));
            processNearbyPoint(false, currentNode, currentNodeG, currentNodeV, allNodes.at(fullX*y+x), open);
            j=1;
        }
        //(x,y+1)
        x = currentX;
        y = currentY+1;
        if((x>=0) && (x<=boundaryX) && (y>=0) && (y<=boundaryY) && allNodes.at(fullX*y+x)->value<100)
        {
            allNodes.at(fullX*y+x)->setH(heuCalc(x,y,endx,endy));
            processNearbyPoint(false, currentNode, currentNodeG, currentNodeV, allNodes.at(fullX*y+x), open);
            j=1;
        }
        //(x+1,y+1)
        x = currentX+1;
        y = currentY+1;
        if((x>=0) && (x<=boundaryX) && (y>=0) && (y<=boundaryY) && allNodes.at(fullX*y+x)->value<100)
        {
            allNodes.at(fullX*y+x)->setH(heuCalc(x,y,endx,endy));
            processNearbyPoint(false, currentNode, currentNodeG, currentNodeV, allNodes.at(fullX*y+x), open);
            j=1;
        }
// 3-4), push the currentNode into closed:
        currentNode->setLocation(2);
        j=0;
    }


    return solution;
}

float PathFinding::heuCalc(int x, int y, int endx, int endy)
{
    float dx = std::abs(x - endx);
    float dy = std::abs(y - endy);
    float heu = D*(dx+dy) + (D2-D)*std::fmin(dx,dy);
    return (heu*heuWeight);
}

bool PathFinding::tileExists(int x, int y, int boundaryX, int boundaryY)
{
    return (x>=0) && (x<=boundaryX) && (y>=0) && (y<=boundaryY);
}


void PathFinding::processNearbyPoint(bool isDiagonal, std::shared_ptr<Node> &currentNode, float currentNodeG, float currentNodeValue, std::shared_ptr<Node> &nearbyPointNode, std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node> >, CompareNodePtrF> &open)
{
    int i = 0;

    float deltaValue = nearbyPointNode->getValue() - currentNodeValue;
    float oldG = currentNodeG;
    float nearbyPointG;

    if(deltaValue > 0) nearbyPointG = oldG + 20*deltaValue + float(isDiagonal? D2:D);
    else nearbyPointG = oldG + float(isDiagonal? D2:D);


    int nodeCreatedIn = nearbyPointNode->getLocation();
    if(nodeCreatedIn == 0)//the node for this position is in neither open or closed
    {
        //push it into open
        nearbyPointNode->setG(nearbyPointG);
        nearbyPointNode->parent = currentNode;
        nearbyPointNode->setLocation(1);
        open.push(nearbyPointNode);
    }
    else if(nearbyPointG<nearbyPointNode->getG() && nodeCreatedIn == 1)//if it would make a better node than oldNode
    {
        nearbyPointNode->setG(nearbyPointG);
        nearbyPointNode->parent = currentNode;
    }
    i = 3;
}

void PathFinding::performPath(std::deque<std::shared_ptr<Node> > &solution)
{
    for(auto i=solution.begin(); i!=solution.end(); ++i)
    {
        int x = (*i)->getPosx();
        int y = (*i)->getPosy();
        std::cout<<"next step: ("<< x << ", "<<y <<")"<<std::endl;
    }
    return;
}

void PathFinding::restoreAllNodes(std::vector<std::shared_ptr<Node> > &allNodes)
{
    for(auto i=allNodes.begin(); i!=allNodes.end(); i++)
    {
        (*i)->g = 0;
        (*i)->h = 0;
        (*i)->f = 0;
        (*i)->parent = nullptr;
        (*i)->location = 0;
    }
}

void PathFinding::setHeuWeight(int value)
{
    heuWeight = value;
}


















