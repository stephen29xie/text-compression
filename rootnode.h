#ifndef ROOTNODE_H
#define ROOTNODE_H

#include "node.h"

class rootNode : public Node {
private:
	int count;
public:
	rootNode(int count, Node* left, Node* right);
	rootNode(int count, Node* left, Node* right, int timestamp);
	~rootNode();
	Node* right;
	Node* left;
	int getFrequency() const;
};

#endif