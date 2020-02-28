#ifndef NODE_H
#define NODE_H

#include <memory>
#include "lib/world.h"
#include "lib/pos.h"

class Node
{
public:
    Node();
    Node(int x, int y,float v, float g, float h, int loc);
    Node(int x, int y,float v, float g, float h, int loc, std::shared_ptr<Node> & prt);
    Node(int x, int y, float g, float h, std::shared_ptr<Node> & prt);
    Node(pos p);
    Node(Tile & t);
    Node(const Node &n);//copy ctor
    Node(const Node *n);//copy ctor2


    bool operator== (const Node & n) const;
    bool operator< (const Node & n) const;
    bool operator> (const Node & n) const;
    bool operator== (const pos p) const;

    void setParent(Node & parentNode);

    int getPosx() const;
    void setPosx(int value);

    int getPosy() const;
    void setPosy(int value);

    float getG() const;
    void setG(float value);

    float getH() const;
    void setH(float value);

    float getF() const;

    int getLocation() const;
    void setLocation(int value);

    float getValue() const;
    void setValue(float v);

private:

    friend class PathFinding;

    int posx;
    int posy;
    float value;
    std::shared_ptr<Node> parent;
    float g;
    float h;
    float f;
    int location;//0=hanging; 1=in open; 2=in closed.
};

struct CompareNode
{
    using is_transparent = void; // for example with void,
                                 // but could be int or struct CanSearchOnId;
    bool operator()(Node const& node1, Node const& node2) const
    {
        return node1.getF() < node2.getF();//since the priority_queue's top is the one with greatest element, here iso. less comp, greater comp is implemented
    }
    bool operator()(Node const& node, pos p) const
    {
        int scoreN = node.getPosy()*p.lengthX + node.getPosx();
        int scoreP = p.y*p.lengthX + p.x;
        return scoreN < scoreP;
    }
    bool operator()(pos p, Node const& node) const
    {
        int scoreN = node.getPosy()*p.lengthX + node.getPosx();
        int scoreP = p.y*p.lengthX + p.x;
        return scoreP < scoreN;
    }
};//using c++14 features!!

struct CompareNodeXY
{
    using is_transparent = void;
    bool operator()(Node const& node1, Node const& node2) const
    {
        if(node1.getPosy() < node2.getPosy()) return true;
        if(node1.getPosy() > node2.getPosy()) return false;

        if(node1.getPosx() < node2.getPosx()) return true;
        if(node1.getPosx() > node2.getPosx()) return false;
    }
};

struct CompareNodeG
{
    using is_transparent = void; // for example with void,
                                 // but could be int or struct CanSearchOnId;
    bool operator()(Node const& node1, Node const& node2) const
    {
        return node1.getG() > node2.getG();
    }
};

struct CompareNodePtrF
{
    using is_transparent = void;
    bool operator()(std::shared_ptr<Node> const& np1, std::shared_ptr<Node> const& np2) const
    {
        return (np2->getF() < np1->getF());
    }
};
struct CompareNodePtrG
{
    using is_transparent = void;
    bool operator()(std::shared_ptr<Node> const& np1, std::shared_ptr<Node> const& np2) const
    {
        return np1->getG() < np2->getG();
    }
};


#endif // NODE_H
