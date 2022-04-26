////
// Author:		Nicholas Francisco
// School:		Everett Community College
// Class:		CS 233 Advance Data Structures and Intro to Algorithms 
// Quarter:		Spring 2021
// Assignment:  Program 3 SkipList
// Description: This program will implement a skip list data structure that  
//	has functions to insert, delete, and display the list.
////

#pragma once

#include <ctime>

#include "ComputerScientist.h"

using namespace std;

template <typename KeyComparable, typename Value >
class SkipList
{
private:
	class BinaryNode
	{
	public:
		KeyComparable key;
		Value value;

		int maxLevel = 0; 

		BinaryNode* next;
		BinaryNode* prev;
		BinaryNode* above; 
		BinaryNode* bellow;

		//Constructor Node w/Data 
		BinaryNode(KeyComparable& key, Value& value, BinaryNode* next = nullptr, BinaryNode* prev = nullptr, BinaryNode* above = nullptr, BinaryNode* bellow = nullptr)
			: key{ key }, value{ value }, next{ next }, prev{ prev }, above{ above }, bellow{ bellow }
		{
		}
	};

	BinaryNode* head = nullptr; 

	void increasHeadHeight(int nodesToAdd) {

		if (nodesToAdd == 0)
			return;

		//TopNodes
		int headNum = 0;
		ComputerScientist* headSci = new ComputerScientist("", "", "", headNum);
		BinaryNode* headTop = new BinaryNode(headNum, headSci, nullptr, nullptr, nullptr, head);

		//Settign new head start and updating maxLevel 
		//And connecting top 
		head->above = headTop;
		head = headTop;

		//Adding Nodes Recursively 
		increasHeadHeight(nodesToAdd - 1);
	}

	void addTopNode(KeyComparable key, BinaryNode* node, int numNodes) {

		//Original Node added
		BinaryNode* prevTop = node;
		BinaryNode* it = node->prev;

		//Prev and Next Connections 
		while (numNodes > 0) {

			//Checking if it can move up
			if (it->above != nullptr) {
				it = it->above;

				//Top node to add and connecting above and bottom
				ComputerScientist* topNodeSci = new ComputerScientist("", "", "", key);
				BinaryNode* topNode = new BinaryNode(key, topNodeSci, it->next, it, nullptr, prevTop);
				prevTop->above = topNode;

				prevTop = topNode;

				//Checking if next node is end of list
				if (it->next != nullptr) {
					it->next->prev = topNode;
					it->next = topNode;

					numNodes = numNodes - 1;
				}
				else {
					it->next = topNode;

					numNodes = numNodes - 1;
				}
			}
			//Moving left until head 
			else {
				if (it->prev != nullptr) {

					it = it->prev;
				}
			}
		}
	}

	int flipCoin() {

		bool coin = true;
		int num = 0;
		int denominator = 2;

		while (coin) {

			int rNum = rand() % 100;
			double checkNum = (1.0 / denominator) * 100.0;

			if (static_cast<double>(rNum) < checkNum) {
				num = num + 1;
				denominator = denominator * 2;
			}
			else
				coin = false;

		}

		return num;
	}

public:

	SkipList() {
		srand(time(0));
		int headNum = 0;
		ComputerScientist* headSci = new ComputerScientist("", "", "", headNum);
		BinaryNode* newHead = new BinaryNode(headNum, headSci, nullptr, nullptr, nullptr, nullptr);
		head = newHead;
	}

	bool insert(Value item, KeyComparable key) {

		//Number of topNodes to make
		int stack = flipCoin();
		bool spotFound = false;

		//Iterator 
		BinaryNode* it = head;

		//Finds node before inserting key
		while (key > it->key && spotFound != true) {
			//Checking next for nullptr
			//Not Nullptr and less than inserting key
			if (it->next != nullptr && key > it->next->key) {
				it = it->next;
			}
			//Checking if node bellow
			else if (it->bellow != nullptr) {
				it = it->bellow;
			}
			//Checking if next is equal to 
			//Already Contains
			else if (it->next != nullptr && key == it->next->key) {
				return false;
			}	
			//End of list
			else if (it->bellow == nullptr && it->next == nullptr) {
				spotFound = true;
			}
			//Bottom of list and next is grater than 
			else if (it->bellow == nullptr && key < it->next->key) {
				spotFound = true; 
			}	
		}

		//Node 
		BinaryNode* node = new BinaryNode(key, item, it->next, it, nullptr, nullptr);
		node->maxLevel = stack; 

		//Connections at Bottem Level
		//Checking if it is end of list
		if (it->next != nullptr) {
			it->next->prev = node;
			it->next = node;
		}
		else {
			it->next = node; 
		}

		//Checking if stack will be higher than 
		//Head level and adding if needed 
		int headLevelDifference = stack - head->maxLevel;
		if (headLevelDifference > 0) {
			increasHeadHeight(headLevelDifference);
			head->maxLevel = head->maxLevel + stack;
		}

		//Adding top node; 
		if (stack > 0) {
			addTopNode(key, node, stack);
		}

		return true; 
	}

	bool remove(KeyComparable key) {

		BinaryNode* it = head;
		bool found = false;

		//Finds node before inserting key
		while (found == false) {

			//Checking next for nullptr
			//Not Nullptr and less than inserting key
			if (it->next != nullptr && key > it->next->key) {
				it = it->next;
			}
			//Checking if next is equal to 
			else if (it->next != nullptr && key == it->next->key) {
				it = it->next;
				found = true;
			}
			//No more right moving down
			//Checking if node bellow
			else if (it->bellow != nullptr) {
				it = it->bellow;
			}
			//Not found - End of list 
			else if (it->bellow == nullptr && it->next == nullptr) {
				return false;
			}
			//Not Found - Next item small and at bottom level 
			else if (it->bellow == nullptr && key < it->next->key) {
				return false;
			}
		}

			while (it != nullptr) {

				BinaryNode* prev = it;

				//No node to the right 
				if (it->next == nullptr) {

					it->prev->next = nullptr;

					it = it->bellow;

					delete prev;	
				} 
				//Has node to the right
				else if (it->next != nullptr) {

					it->next->prev = it->prev;
					it->prev->next = it->next;

					it = it->bellow;

					delete prev;
				}
			}

			return true; 
	}

	void displayList() const {

		//Iterator 
		BinaryNode* it = head;
		//Stack Node 
		BinaryNode* headNode = head;

		//Counts Head Stack
		int stackLevel = head->maxLevel;

		//Checking for Empty list 
		//Checks if it next != nullptr 
		if (it->next == nullptr)
			return; 

		while (headNode != nullptr) {

			//Level Display 
			cout << "Level " << stackLevel << " : ";

			//Moves right and prints
			while (it->next != nullptr) {
				it = it->next;

				cout << it->key << " ";
			}

			cout << endl << endl;
			stackLevel = stackLevel - 1;

			//Moves down on head stack 
			headNode = headNode->bellow;
			it = headNode; 
		}
	}
};

