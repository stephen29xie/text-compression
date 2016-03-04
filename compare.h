#ifndef COMPARE_H
#define COMPARE_H

#include "node.h"
#include "rootnode.h"
#include "leafnode.h"

struct Compare {
	public:
		Compare();
		~Compare();
		bool operator() (Node* a, Node* b);
};

#endif