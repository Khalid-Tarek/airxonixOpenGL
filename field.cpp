#ifndef FIELD_CPP
#define FIELD_CPP

#include <vector>
#include "entity.cpp"

//Types of cell flags relating to fill checking (Filling Flags)
#define NO_ENTITY	'N'
#define CHECKED		'C'
#define QUEUED		'Q'

/**
 *	The field is a 2D representation of the board where the game takes place.
 *	The field class encompasses additional fields pretaining to game information
 *	
 *	@InnerClass Cell:			Representation of a singular cell (or element) in the board 2D array
 *
 *	@field dimension:			The size of the board. if the dimension is 5, the board is (5x5) large
 *	@field raisedColors:		An array representing the RGB color of the raised cells.
 *	@field board:				A 2D array of {@class: Cell}s. 
								It holds pointers instead of objects themselves, this is so no stack overflow occurs
 *	@field currentlyFilled:		Ranges from 0.0 to 1.0, representing the percentage of the cells filled in the board
 *	@field winCondition:		Ranges from 0.0 to 1.0. Get {@field: currentlyFilled} to this value to win the level
 *	@field timeLeft:			Represents the time left to pass this level. If it reaches 0, you lose the level
 *	@field enemies:				An array of {@class: Enemy}s. Holds the enemies of this particular level
 *	@field playerDrawnLine:		An array of {@class: Cell}s. Holds the cells that the player passes on when filling
 *	@field player:				A pointer to the player object. We need this here to be able to interact with the player
 */
class Field{
public:
	/**
	 *	A Cell is one element on the board.
	 *	
	 *	@field x:			x position of the cell on the board
	 *	@field z:			z position of the cell on the board
	 *	@field raised:		Ranges from 0.0 to 1.0. This was created to facilitate "filling animation"
	 *	@field cellFlag:	A character field used to mark this particular cell with either Entity::type or for filling flags
	 */
	class Cell {
	public:
		int x;
		int z;
		double raised;		
		char cellFlag;		

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
	double currentlyFilled;		
	double winCondition;		
	int timeLeft;				
	vector<Enemy> enemies;
	vector<Cell*> playerDrawnLine;
	Player *player;
	
	Field() {};
	/**
	 *	The field constructor constructs a field based on second parameter {@param: level}
	 *	A file must be named "{level}.txt" in a subdirectory called "levels" in the same folder as the executable
	 *
	 *	@param p:		A pointer to the player object
	 *  @param level:	The number of the level that will be created
	 *
	 *	The files structure should be as follows: (line & space sensitive)
	 *	{dimension}
	 *	{3 values (R G B) for raisedColors} (Range: 0.0 - 1.0)
	 *	{Number of prefilled squares on the board (n)}
	 *	n lines each consisting of: {x position of center of square} {z position of center of square} {dimension of the square}
	 *	{winCondition} (Range: 0.0 - 1.0)
	 *	{timeLeft}
	 *	{Number of enemies on the board (e)}
	 *	e lines each consisting of: {Enemy Type} {x position} {z position}
	 *
	 *	Example file:
	 *	100
	 *	0.8 0.0 0.8
	 *	2
	 *	20 30 9
	 *	82 76 11
	 *	0.75
	 *	60
	 *	4
	 *	R 20 60
	 *	F 20 30
	 *	F 22 28
	 *	R 10 10
	 */
	Field(Player &p, int level){
		
		player = &p;

		//Open file
		string filename = "levels/" + to_string((long double)level) + ".txt";
		freopen(filename.c_str(), "r", stdin);

		//Read in dimension
		cin >> dimension;

		//Create Board
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

		//Read in R G B
		cin >> raisedColors[0] >> raisedColors[1] >> raisedColors[2];

		//Read in number of prefilled squares
		int n;
		cin >> n;
		for(int i = 0; i < n; i++){
			int x, z, size;
			//Read in prefilled square info
			cin >> x >> z >> size;
			for(int i = max(0, x - (size - 1)/2); i <= min(dimension - 1, x + (size - 1)/2); i++){
				for(int j = max(0, z - (size - 1)/2); j <= min(dimension - 1, z + (size - 1)/2); j++){
					board[i][j]->raised = 1.0;
				}
			}
		}

		//Read in winCondition
		cin >> winCondition;
		updateCurrentlyFilled();

		//Read in timeLeft
		cin >> timeLeft;

		//Read in number of enemies
		int numOfEnemies;
		cin >> numOfEnemies;

		for(int i = 0; i < numOfEnemies; i++){
			char enemyType;
			int position[2];
			//Read in enemy info
			cin >> enemyType >> position[0] >> position[1];
			Enemy enemy(enemyType, position);
			enemies.push_back(enemy);
		}
	}

	//Moves the passed in entity. Different calculations happen based on entity type.
	//(Wasn't sure about polymorphism in C++ tbh, there has to be a better way to implement this)
	void move(Entity &entity){
		if(entity.type != PLAYER) //If the moving entity is not a player, check for collision
			collisionHandler(entity);
		else
			checkIfFilling(*player);

		//If the entity is a player is the player is currently filling, queue the current cell and add it to {@field: playerDrawnLine}
		if(entity.type == PLAYER && player->isFilling){
			board[entity.position[0]][entity.position[1]]->cellFlag = QUEUED;
			playerDrawnLine.push_back(board[player->position[0]][player->position[1]]);
		}
		else //Otherwise, set the cell you're moving from to have no entity
			board[entity.position[0]][entity.position[1]]->cellFlag = NO_ENTITY;

		entity.move(dimension);
		board[entity.position[0]][entity.position[1]]->cellFlag = entity.type;
		checkIfFilling(*player);
	}

	//Check if the player has entered a filling state (i.e. stepped on a non-raised cell)
	//Or has exited the filling state, then fire the filling process
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
	//Calculate the percentage of filled cells on the board
	void updateCurrentlyFilled(){
		double totalNumOfCells = dimension * dimension;
		double filledCells = 0;

		for(int i = 0; i < dimension; i++)
			for(int j = 0; j < dimension; j++)
				if(board[j][i]->raised) filledCells++;

		currentlyFilled = filledCells / totalNumOfCells;
	}

	//Starts the filling process. First raises the playerDrawnLine and then
	//fires {@method: actuateFilling} at the first cell of the {@field: playerDrawnLine}
	void fillPlayerDrawnLine(){
		for(int i = 0; i < playerDrawnLine.size(); i++){
			playerDrawnLine[i]->raised = 1;
			playerDrawnLine[i]->cellFlag = NO_ENTITY;
		}
		actuateFilling(playerDrawnLine[0]->x, playerDrawnLine[0]->z);
	}

	//Checks the four cells surrounding the passed in cell positions
	//There has to be a better way to actuate the filling
	void actuateFilling(int x, int z){
		if(x > 0)				fill(x - 1, z, isEmpty(x - 1, z));
		if(x < dimension - 1)	fill(x + 1, z, isEmpty(x + 1, z));
		if(z > 0)				fill(x, z - 1, isEmpty(x, z - 1));
		if(z < dimension - 1)	fill(x, z + 1, isEmpty(x, z + 1));
	}

	//Recursive method method that checks andd flags neighboring cells to see if they contain an enemy.
	bool isEmpty(int x, int z){
		if(board[x][z]->cellFlag == CHECKED)			return true;
		if(board[x][z]->raised == 1)					return true;
		if(board[x][z]->cellFlag == REGULAR_ENEMY)		return false;
		board[x][z]->cellFlag = CHECKED;
		return isEmpty(x - 1, z) && isEmpty(x + 1, z) && isEmpty(x, z - 1) && isEmpty(x, z + 1);
	}

	//The fill method iterates recursively much like {@method: isEmpty} over the checked cells
	//If {@param: isEmptyFlag} is true, raise the cell. 
	//This also works if an enemy in the area the cells aren't raised, but the cellFlag is cleared nonetheless
	void fill(int x, int z, bool isEmptyFlag){
		if(board[x][z]->cellFlag != CHECKED) return;
		if(isEmptyFlag) board[x][z]->raised = 1;
		board[x][z]->cellFlag = NO_ENTITY;
		fill(x - 1, z, isEmptyFlag);
		fill(x + 1, z, isEmptyFlag);
		fill(x, z - 1, isEmptyFlag);
		fill(x, z + 1, isEmptyFlag);
	}

	//Deals with the collision of enemies with the board, or with the player
	void collisionHandler(Entity &entity){

		//If the type of entity is regular, then it will collide against filled, if its filled, it will collide against empty or array bounds
		int collisionAgainst = (entity.type == REGULAR_ENEMY? 1 : 0);

		int newPos = entity.position[0] + 2 * entity.directions[0];
		if(newPos >= dimension || newPos < 0 || board[newPos][entity.position[1]]->raised == collisionAgainst){
			entity.directions[0] *= -1;
			return;
		}

		newPos = entity.position[1] + 2 * entity.directions[1];
		if(newPos >= dimension || newPos < 0 || board[entity.position[0]][newPos]->raised == collisionAgainst){
			entity.directions[1] *= -1;
			return;
		}

		//If an enemy has hit the player (if FILLED_ENEMY) or the draw line (if REGULAR_ENEMY), damage the player and clear the line
		collisionAgainst = (entity.type == REGULAR_ENEMY ? QUEUED : PLAYER);

		if(entity.type != PLAYER){
			if(board[entity.position[0] + 2 * entity.directions[0]][entity.position[1]]->cellFlag == collisionAgainst){
				entity.directions[0] *= -1;
				player->getHit();
				for(int i = 0; i < playerDrawnLine.size(); i++)
					playerDrawnLine[i]->cellFlag = NO_ENTITY;
				playerDrawnLine.clear();
			}

			if(board[entity.position[0]][entity.position[1] + 2 * entity.directions[1]]->cellFlag == collisionAgainst){
				entity.directions[1] *= -1;
				player->getHit();
				for(int i = 0; i < playerDrawnLine.size(); i++)
					playerDrawnLine[i]->cellFlag = NO_ENTITY;
				playerDrawnLine.clear();
			}
		}
	}
};

#endif