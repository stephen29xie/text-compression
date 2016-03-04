#include "node.h"

Node::Node(int timestamp): timestamp(timestamp) {}

Node::~Node() {}

int Node::getTimestamp() {
	return this->timestamp;
}