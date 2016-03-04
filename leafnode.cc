#include "leafnode.h"

leafNode::leafNode(char character, int frequency): Node(0), character(character), frequency(frequency) {}

leafNode::~leafNode() {}

char leafNode::getChar() const {
	return this->character;
}

int leafNode::getFrequency() const {
	return this->frequency;
}