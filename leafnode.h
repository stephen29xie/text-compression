#ifndef LEAFNODE_H
#define LEAFNODE_H

#include "node.h"

class leafNode : public Node {
private:
	int frequency;
	char character;
public:
	leafNode(char character, int frequency);
	~leafNode();
	int getFrequency() const;
	char getChar() const;
};

#endif