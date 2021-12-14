#ifndef FIELD_CPP
#define FIELD_CPP

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <math.h>

#include "entity.cpp"

#define NO_ENTITY	'N'
#define CHECKED		'C'

using namespace std;

class Field{
public:
	class Cell {
	public:
		int x;
		int z;
		double raised;		//0 - 1 ------ 0 being totally unraised, 1 being totally raised
		char cellFlag;	//Entity::type

		Cell() {};
		Cell(int x, int z, double raised) {
			this->x = x;
			this->z = z;
			this->raised = raised;
			this->cellFlag = NO_ENTITY;
		}
	};

	int dimension;
	double raisedColors[3];
	vector<vector<Cell>> board;
	double currentlyFilled;		// 0 to 1
	double winCondition;		// 0 to 1
	int timeLeft;				// in seconds
	vector<Enemy> enemies;
	
	Field() {};
	Field(int level){
		
		string filename = "levels/" + to_string((long double)level) + ".txt";
		freopen(filename.c_str(), "r", stdin);

		cin >> dimension;

		for(int i = 0; i < dimension; i++){
			vector<Cell> row;
			for(int j = 0; j < dimension; j++){
				Cell cell(j, i, 0.0);
				if(i == 0 || j == 0 || i == dimension - 1 || j == dimension - 1)
					cell.raised = 1;
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
			for(int i = max(0, x - (size - 1)/2); i <= min(dimension - 1, x + (size - 1)/2); i++){
				for(int j = max(0, z - (size - 1)/2); j <= min(dimension - 1, z + (size - 1)/2); j++){
					board[j][i].raised = 1.0;
				}
			}
		}

		cin >> winCondition;
		updateCurrentlyFilled();

		cin >> timeLeft;

		int numOfEnemies;
		cin >> numOfEnemies;

		for(int i = 0; i < numOfEnemies; i++){
			char enemyType;
			int position[2];
			cin >> enemyType >> position[0] >> position[1];
			Enemy enemy(enemyType, position);
			enemies.push_back(enemy);
		}
	}

	void actuateFilling(int x, int y){
		if(x > 0)				fill(x - 1, y, isEmpty(x - 1, y));
		if(x < dimension - 1)	fill(x + 1, y, isEmpty(x + 1, y));
		if(y > 0)				fill(x, y - 1, isEmpty(x, y - 1));
		if(y < dimension - 1)	fill(x, y + 1, isEmpty(x, y + 1));
	}

private:
	void updateCurrentlyFilled(){
		double totalNumOfCells = dimension * dimension;
		double filledCells = 0;

		for(int i = 0; i < dimension; i++)
			for(int j = 0; j < dimension; j++)
				if(board[j][i].raised) filledCells++;

		currentlyFilled = filledCells / totalNumOfCells;
	}

	bool isEmpty(int x, int y){
		if(board[x][y].cellFlag == CHECKED)			return true;
		if(board[x][y].raised == 1)					return true;
		if(board[x][y].cellFlag == REGULAR_ENEMY)	return false;
		board[x][y].cellFlag = CHECKED;
		return isEmpty(x - 1, y) && isEmpty(x + 1, y) && (x, y - 1) && isEmpty(x, y + 1);
	}

	void fill(int x, int y, bool isEmptyFlag){
		if(board[x][y].cellFlag != CHECKED) return;
		if(isEmptyFlag) board[x][y].raised = 1;
		board[x][y].cellFlag = NO_ENTITY;
		fill(x - 1, y, isEmptyFlag);
		fill(x + 1, y, isEmptyFlag);
		fill(x, y - 1, isEmptyFlag);
		fill(x, y + 1, isEmptyFlag);
	}
};

#endif