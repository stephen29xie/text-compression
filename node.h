#ifndef NODE_H
#define NODE_H

class Node {
private:
	int timestamp;
public:
	Node(int timestamp);
	virtual ~Node() = 0;
	virtual int getFrequency() const = 0;
	int getTimestamp();
};

#endif