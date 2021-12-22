#ifndef FIELD_CPP
#define FIELD_CPP

#include <vector>
#include <math.h>

#include "entity.cpp"

#define NO_ENTITY	'N'
#define CHECKED		'C'
#define QUEUED		'Q'

class Field{
public:
	class Cell {
	public:
		int x;
		int z;
		double raised;		//0 - 1 ------ 0 being totally unraised, 1 being totally raised
		char cellFlag;		//Entity::type

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
	vector<vector<Cell*>> board;
	double currentlyFilled;		// 0 to 1
	double winCondition;		// 0 to 1
	int timeLeft;				// in seconds
	vector<Enemy> enemies;
	vector<Cell*> playerDrawnLine;
	Player *player;
	
	Field() {};
	Field(Player &p, int level){
		
		player = &p;

		string filename = "levels/" + to_string((long double)level) + ".txt";
		freopen(filename.c_str(), "r", stdin);

		cin >> dimension;

		for(int i = 0; i < dimension; i++){
			vector<Cell*> row;
			for(int j = 0; j < dimension; j++){
				Cell* cell = new Cell(i, j, 0.0);
				if(i == 0 || j == 0 || i == dimension - 1 || j == dimension - 1)
					cell->raised = 1;
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
					board[j][i]->raised = 1.0;
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

	void move(Entity &entity){
		if(entity.type != PLAYER) 
			collisionHandler(entity);
		else
			checkIfFilling(*player);

		if(entity.type == PLAYER && player->isFilling){
			board[entity.position[0]][entity.position[1]]->cellFlag = QUEUED;
			playerDrawnLine.push_back(board[player->position[0]][player->position[1]]);
		}
		else
			board[entity.position[0]][entity.position[1]]->cellFlag = NO_ENTITY;

		entity.move(dimension);
		board[entity.position[0]][entity.position[1]]->cellFlag = entity.type;
		checkIfFilling(*player);
	}

	void checkIfFilling(Player &player){
		if(player.isFilling	== false && board[player.position[0]][player.position[1]]->raised == 0){
			player.isFilling = true;
		}
		else if(player.isFilling == true && board[player.position[0]][player.position[1]]->raised == 1){
			player.isFilling = false;
			fillPlayerDrawnLine();
			playerDrawnLine.clear();
			updateCurrentlyFilled();
		}
		
	}

private:
	void updateCurrentlyFilled(){
		double totalNumOfCells = dimension * dimension;
		double filledCells = 0;

		for(int i = 0; i < dimension; i++)
			for(int j = 0; j < dimension; j++)
				if(board[j][i]->raised) filledCells++;

		currentlyFilled = filledCells / totalNumOfCells;
	}

	void fillPlayerDrawnLine(){
		for(int i = 0; i < playerDrawnLine.size(); i++){
			playerDrawnLine[i]->raised = 1;
			playerDrawnLine[i]->cellFlag = NO_ENTITY;
		}
		actuateFilling(playerDrawnLine[0]->x, playerDrawnLine[0]->z);
	}

	void actuateFilling(int x, int z){
		if(x > 0)				fill(x - 1, z, isEmpty(x - 1, z));
		if(x < dimension - 1)	fill(x + 1, z, isEmpty(x + 1, z));
		if(z > 0)				fill(x, z - 1, isEmpty(x, z - 1));
		if(z < dimension - 1)	fill(x, z + 1, isEmpty(x, z + 1));
	}

	bool isEmpty(int x, int z){
			if(board[x][z]->cellFlag == CHECKED)		return true;
		if(board[x][z]->raised == 1)					return true;
		if(board[x][z]->cellFlag == REGULAR_ENEMY)	{
			removeChecked(x, z);
			return false;
		}
		board[x][z]->cellFlag = CHECKED;
		return isEmpty(x - 1, z) && isEmpty(x + 1, z) && isEmpty(x, z - 1) && isEmpty(x, z + 1);
	}

	void removeChecked(int x, int z){
		if(board[x][z]->cellFlag != CHECKED) return;
		board[x][z]->cellFlag = NO_ENTITY;
		removeChecked(x - 1, z);
		removeChecked(x + 1, z);
		removeChecked(x, z - 1);
		removeChecked(x, z + 1);
	}

	void fill(int x, int z, bool isEmptyFlag){
		if(board[x][z]->cellFlag != CHECKED) return;
		if(isEmptyFlag) board[x][z]->raised = 1;
		board[x][z]->cellFlag = NO_ENTITY;
		fill(x - 1, z, isEmptyFlag);
		fill(x + 1, z, isEmptyFlag);
		fill(x, z - 1, isEmptyFlag);
		fill(x, z + 1, isEmptyFlag);
	}

	void collisionHandler(Entity &entity){
		//Check if a regular enemy hits a queued cell, and if it does, clear the queued cells and hit the player
		if(entity.type == REGULAR_ENEMY){
			if(board[entity.position[0] + 2 * entity.directions[0]][entity.position[1]]->cellFlag == QUEUED){
				entity.directions[0] *= -1;
				player->getHit();
				for(int i = 0; i < playerDrawnLine.size(); i++)
					playerDrawnLine[i]->cellFlag = NO_ENTITY;
				playerDrawnLine.clear();
			}
			if(board[entity.position[0]][entity.position[1] + 2 * entity.directions[1]]->cellFlag == QUEUED){
				entity.directions[1] *= -1;
				player->getHit();
				for(int i = 0; i < playerDrawnLine.size(); i++)
					playerDrawnLine[i]->cellFlag = NO_ENTITY;
				playerDrawnLine.clear();
			}
		}
		//If the type of entity is regular, then it will collide against filled, if its filled, it will collide against empty
		int collisionAgainst = (entity.type == REGULAR_ENEMY? 1 : 0);
		if(board[entity.position[0] + 2 * entity.directions[0]][entity.position[1]]->raised == collisionAgainst)
			entity.directions[0] *= -1;
		if(board[entity.position[0]][entity.position[1] + 2 * entity.directions[1]]->raised == collisionAgainst)
			entity.directions[1] *= -1;
	}
};

#endif