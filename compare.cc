#include "compare.h"

Compare::Compare() {}

Compare::~Compare() {}

bool Compare::operator() (Node* a, Node* b) {
	if (a->getFrequency() != b->getFrequency()) {
		return a->getFrequency() > b->getFrequency();
	} else {
		return a->getTimestamp() > b->getTimestamp();
	}
}