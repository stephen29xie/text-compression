#include "rootnode.h"

rootNode::~rootNode() {}

rootNode::rootNode(int count, Node* left, Node* right): Node(0), count(count), right(right), left(left) {}

rootNode::rootNode(int count, Node* left, Node* right, int timestamp): Node(timestamp), count(count), right(right), left(left) {}

int rootNode::getFrequency() const {
	return this->count;
}