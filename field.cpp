#ifndef FIELD_CPP
#define FIELD_CPP

#include <iostream>
#include <fstream>
#include <vector>

#include "entity.cpp"

using namespace std;

class Field{
public:

	class Cell {
	public:
		int x;
		int z;
		bool raised;
		char entityOnMe;	//Entity::type

		Cell() {};
		Cell(int x, int z, bool raised, char entityOnMe){
			this->x = x;
			this->z = z;
			this->raised = raised;
			this->entityOnMe = entityOnMe;
		}
	};

	int dimension;
	double raisedColors[3];
	vector<vector<Cell>> board;
	double currentlyFilled;		// 0 to 1
	double winCondition;		// 0 to 1
	int timeLeft;				// in seconds
	//vector<Entity> entity;
	
	Field() {};
	Field(int d) {dimension = d;};
	/*
	Field(int level){
		ifstream in("levels/" + to_string((long double)level) + ".txt");

		string s;
		getline(in, s);

		dimension = stoi(s);


	}
	*/
};

#endif