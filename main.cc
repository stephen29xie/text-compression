#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <map>
using namespace std;
#include "node.h"
#include "leafnode.h"
#include "rootnode.h"
#include "compare.h"


// traverseTree(Node* root, string code) traverses the Huffman tree for the leafnodes and adds the 
// char-string pairs to the map, where the string is the route from the root to the leaf
void traverseTree(Node* node, string code, map<char,string>& codeList) {
	if (dynamic_cast<leafNode*>(node)) { // if node is a leafNode
		codeList[dynamic_cast<leafNode*>(node)->getChar()] = code;
	}
	if (dynamic_cast<rootNode*>(node)) { // if node is a rootNode
		traverseTree(dynamic_cast<rootNode*>(node)->left, code + "0", codeList);
		traverseTree(dynamic_cast<rootNode*>(node)->right, code + "1", codeList);
	}
}


// createTree takes a priority queue and returns a Huffman Tree made from the elements of the queue
Node* createTree(priority_queue<Node*, vector<Node*>, Compare> pq){
	while (pq.size() >= 2) {
		int freq1 = pq.top()->getFrequency();
		Node* l = pq.top();
		pq.pop();
		int freq2 = pq.top()->getFrequency();
		Node* r = pq.top();
		pq.pop();
		int depth;
		if (dynamic_cast<rootNode*>(l) && dynamic_cast<rootNode*>(r)) {
			if (l->getTimestamp() > r->getTimestamp()) {
				depth = l->getTimestamp();
			} else {
				depth = r->getTimestamp();
			}
		} else if (dynamic_cast<rootNode*>(r)) {
			depth = r->getTimestamp();
		} else if (dynamic_cast<rootNode*>(l)) {
			depth = l->getTimestamp();
		} else {
			depth = 0;
		}
		Node* node = new rootNode(freq1+freq2, l, r, depth + 1);
		pq.push(node);
	}
	return pq.top();
}


// packBytes takes a stream of bits (0's and 1's) and packs them in groups of 8 into unsigned chars (a byte) 
// and returns the vector of the bytes. The bytes are written to the ofstream output file.
vector<unsigned char> packBytes(vector<bool> bitstream, ofstream& compressedfile) {
	vector<unsigned char> packedbytes;
	for (int j = 0; j < bitstream.size(); j = j + 8) {
		unsigned char byte = 0;
		if (bitstream.size() - j >= 8) { 
			for (int k = 0; k < 8; ++k) {
				if (bitstream[j+k] == true) {
					byte |= 1 << k;
				} else {
					byte |= 0 << k;
				}
			}
			packedbytes.push_back(byte);
			compressedfile << byte;
		} else { // if less than 8 bits left
			int rem = bitstream.size() - j;
			int bitsneeded = 8 - rem;
			for (int k = 0; k < rem; ++k) {
				if (bitstream[j+k] == true) {
					byte |= 1 << k;
				} else {
					byte |= 0 << k;
				}
			}
			for (int n = rem; n < 8; ++n) {
				if (bitstream[j+n] == true) {
					byte |= 1 << n;
				} else {
					byte |= 0 << n;
				}
			}
			packedbytes.push_back(byte);
			compressedfile << byte;
		}
	}
	return packedbytes;
}


// createPriorityQueue takes an int array and returns a priority queue of the non zero entries of the array.
// The elements of the queue are Nodes, and the priority is by Node frequency and first-in-first-out convention.
priority_queue<Node*, vector<Node*>, Compare> createPriorityQueue(int* freqVector) {
	priority_queue<Node*, vector<Node*>, Compare> pq;
	for (int i = 0; i < 256; ++i) {
		if (freqVector[i] != 0) {
			char ch = i;
			Node* newnode = new leafNode(i, freqVector[i]);
			pq.push(newnode);
		}
	}
	return pq;
}


// unpackByte takes in an unsigned char (byte) and pushes its 8 bit composition of 0's and 1's into a bool vector
void unpackByte(unsigned char character, vector<bool>& bitstream) {
	for(int k=0; k < 8; k++) {
		int thebit = (character & (1 << k)) >> k;
    	bitstream.push_back(thebit);
    }
}


// decode takes in a bool vector and decodes the variable-length bit sequences with a Huffman tree. 
// The corresponding characters are pushed to an ofstream output file.
void decode(Node* root, ofstream& o, vector<bool> decodedBitstream, int charactersToDecode) {
	Node* current = root;
	int charactersDecoded = 0;
	for (int j = 0; j < decodedBitstream.size(); ++j) { // iterate through all the bits
		if (decodedBitstream[j] == 0) { // if bit is a 0
			current = dynamic_cast<rootNode*>(current)->left;
			if (dynamic_cast<leafNode*>(current)) { // if a leafNode is reached
				o << dynamic_cast<leafNode*>(current)->getChar();
				++charactersDecoded;
				current = root; // current Node points at root Node again
				if (charactersDecoded == charactersToDecode) {
					return;
				}
			}
		} else { // if bit is a 1
			current = dynamic_cast<rootNode*>(current)->right;
			if (dynamic_cast<leafNode*>(current)) { // if a leafNode is reached
				o << dynamic_cast<leafNode*>(current)->getChar();
				++charactersDecoded;
				current = root; // current Node points at root Node again
				if (charactersDecoded == charactersToDecode) {
					return;
				}
			}
		}
	}
}



/*************************   MAIN   *************************/

// ./exe [file] [-c/-d] [outputfile name]
// -c to compress, -d to decompress
int main(int argc, char* argv[]) {
	if (argc != 4) { // check for correct number of arguments
		cout << "Invalid number of command line arguments" << endl;
		return 0;
	}
	
	// COMPRESSION
	if (string(argv[2]) == "-c") { 

		// open file to compress
		ifstream textfile;
		textfile.open(argv[1]);

		// open empty output file
		ofstream compressedfile;
		compressedfile.open(string(argv[3]) + ".txt");

		int freqVector[256] = {0};
		int numberofuniquechars = 0;
		int numberofchars = 0;
		char c;
		
		// count frequency of ASCII characters
		while (textfile >> noskipws >> c) {
			freqVector[c]++;
			++numberofchars;
		}

		// if empty file, return empty file
		if (numberofchars == 0) {
			return 0;
		}

		compressedfile << numberofchars << endl;

		// count number of unique ASCII characters
		for (int i = 0; i < 256; ++i) {
			if (freqVector[i] != 0) {
				++numberofuniquechars;
			}
		}
		compressedfile << numberofuniquechars << endl;

		// store character and frequency in output file
		for (int i = 0; i < 256; ++i) {
			if (freqVector[i] != 0) {
				char c = i;
				compressedfile << c << " " << freqVector[i] << endl;
			}
		}

		// insert freqVector in to a priority queue, with low frequency as high priority
		priority_queue<Node*, vector<Node*>, Compare> pq = createPriorityQueue(freqVector);
		
		// priority queue is passed to createTree function to create Huffman tree
		Node* head = createTree(pq);

		// traverse Huffman tree to find bit sequences for every unique character. Store sequences in a map
		map<char, string> codeList;
		traverseTree(head, "", codeList);

		// reset textfile to be read in again
		textfile.clear();                
		textfile.seekg(0, textfile.beg); 

		// reread characters as their bit sequence into bool vector
		vector<bool> bitstream; 
		while (textfile >> noskipws >> c) {
			for (int i = 0; i < codeList[c].length(); ++i) {
				if (codeList[c][i] == '0') {
					bitstream.push_back(false);
				} else {
					bitstream.push_back(true);
				}
			}
		}

		// pack bit sequence into bytes (8 bits per byte) and write to output file
		vector<unsigned char> packedbytes =  packBytes(bitstream, compressedfile);
		
		return 0; 
	}


	// DECOMPRESSION
	else if (string(argv[2]) == "-d") {
		int charactersToDecode;
		int uniqueCharacters;
		
		// open file to decompress
		ifstream textfile; 
		textfile.open(argv[1]);

		// open empty output file
		ofstream compressedfile;
		compressedfile.open(string(argv[3]) + ".txt");

		textfile >> charactersToDecode >> uniqueCharacters;
		textfile.ignore(1, '\n'); // ignore newline character after end of character-frequency lines

		int frequencyVector[256] = {0};
		// read in character frequencies
		for (int i = 0; i < uniqueCharacters; ++i) {
			char character;
			int frequency;
			
			textfile >> noskipws >> character;
			textfile.ignore(1, ' ');
			textfile >> noskipws >> frequency;
			textfile.ignore(1, '\n');
			
			int index = character;
			frequencyVector[index] = frequency;
		}
		
		// create priority queue of Node* containing frequencies of characters
		priority_queue<Node*, vector<Node*>, Compare> pq = createPriorityQueue(frequencyVector);
		
		// create Huffman tree from priority queue
		Node* root = createTree(pq);
		
		vector<bool> decodedBitstream;
		unsigned char packedbyte;

		// read in characters from textfile and unpack them into their 8 bit compositions.
		// read the bits into a bool vector
		while (textfile >> noskipws >> packedbyte) {
			unpackByte(packedbyte, decodedBitstream);
		}

		// decode the variable-length bit sequences to original uncompressed text and write to output file
		decode(root, compressedfile, decodedBitstream, charactersToDecode);

		return 0;
	}
} // main