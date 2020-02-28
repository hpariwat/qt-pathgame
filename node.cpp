#include "node.h"

Node::Node(){}

Node::Node(int x, int y, float v, float g, float h, int loc)
{
    posx = x;
    posy = y;
    value = v;
    this->g = g;
    this->h = h;
    f = g + h;
    location = loc;
    parent = nullptr;
}

Node::Node(int x, int y, float v, float g, float h, int loc, std::shared_ptr<Node> &prt)
{
    posx = x;
    posy = y;
    value = v;
    this->g = g;
    this->h = h;
    f = g + h;
    location = loc;
    parent = prt;
}

Node::Node(int x, int y, float g, float h, std::shared_ptr<Node> &prt)
{
    posx = x;
    posy = y;
    this->g = g;
    this->h = h;
    f = g + h;
    parent = prt;
}

Node::Node(pos p)
{
    posx = p.x;
    posy = p.y;
    g = 0;
    h = 0;
    g = 0;
}

Node::Node(Tile &t)
{
    posx = t.getXPos();
    posy = t.getYPos();
}

Node::Node(const Node &n)
{
    this->posx = n.posx;
    this->posy = n.posy;
    this->parent = n.parent;
    this->g = n.g;
    this->h = n.h;
    this->f = n.f;

}

Node::Node(const Node *n)
{
    this->posx = n->posx;
    this->posy = n->posy;
    this->parent = n->parent;
    this->g = n->g;
    this->h = n->h;
    this->f = n->f;

}

bool Node::operator==(const Node &n) const
{
    return (this->posx==n.posx)&&(this->posy==n.posy);
}

bool Node::operator<(const Node &n) const
{
    return (this->f < n.f);
}

bool Node::operator>(const Node &n) const
{
    return (this->f > n.f);
}

bool Node::operator==(const pos p) const
{
    return (this->posx==p.x)&&(this->posy==p.y);
}

void Node::setParent(Node & parentNode)
{
    parent = std::make_shared<Node> (parentNode);
}

int Node::getPosx() const
{
    return posx;
}

void Node::setPosx(int value)
{
    posx = value;
}

int Node::getPosy() const
{
    return posy;
}

void Node::setPosy(int value)
{
    posy = value;
}

float Node::getG() const
{
    return g;
}

void Node::setG(float value)
{
    g = value;
    f = h + g;
}

float Node::getH() const
{
    return h;
}

void Node::setH(float value)
{
    h = value;
    f = h + g;
}

float Node::getF() const
{
    return f;
}

int Node::getLocation() const
{
    return location;
}

void Node::setLocation(int value)
{
    location = value;
}



float Node::getValue() const
{
    return value;
}

void Node::setValue(float v)
{
    this->value = v;
}


