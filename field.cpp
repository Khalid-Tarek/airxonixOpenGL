#ifndef FIELD_CPP
#define FIELD_CPP

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <math.h>

#include "entity.cpp"

#define NO_ENTITY	'N'

using namespace std;

class Field{
public:

	class Cell {
	public:
		int x;
		int z;
		double raised;		//0 - 1 ------ 0 being totally unraised, 1 being totally raised
		char entityOnMe;	//Entity::type

		Cell() {};
		Cell(int x, int z, double raised) {
			this->x = x;
			this->z = z;
			this->raised = raised;
			this->entityOnMe = NO_ENTITY;
		}
	};

	int dimension;
	double raisedColors[3];
	vector<vector<Cell>> board;
	double currentlyFilled;		// 0 to 1
	double winCondition;		// 0 to 1
	int timeLeft;				// in seconds
	vector<Entity> enemies;
	
	Field() {};
	Field(int level){
		
		string filename = "levels/" + to_string((long double)level) + ".txt";
		freopen(filename.c_str(), "r", stdin);

		cin >> dimension;

		for(int i = 0; i < dimension; i++){
			vector<Cell> row;
			for(int j = 0; j < dimension; j++){
				Cell cell(j, i, 0.0);
				row.push_back(cell);
			}
			board.push_back(row);
		}

		cin >> raisedColors[0] >> raisedColors[1] >> raisedColors[2];

		int n; //Number of prefilled squares on the board
		cin >> n;
		for(int i = 0; i < n; i++){
			int x, z, size;
			cin >> x >> z >> size;

			for(int i = max(0, x - (size - 1)/2); i < min(dimension - 1, x + (size - 1)/2); i++){
				for(int j = max(0, z - (size - 1)/2); i < min(dimension - 1, z + (size - 1)/2); j++){
					board[j][i].raised = 1.0;
				}
			}
		}

		cin >> winCondition;
		updateCurrentlyFilled();

		int numOfEnemies;
		cin >> numOfEnemies;
		for(int i = 0; i < numOfEnemies; i++){
			char enemyType;
			int position[2];
		}
	}

	void updateCurrentlyFilled(){
		double totalNumOfCells = dimension * dimension;
		double filledCells = 0;

		for(int i = 0; i < dimension; i++)
			for(int j = 0; j < dimension; j++)
				if(board[j][i].raised) filledCells++;

		currentlyFilled = filledCells / totalNumOfCells;
	}
};

#endif