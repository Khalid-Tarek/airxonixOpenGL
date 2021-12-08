#ifndef ENTITY_CPP
#define ENTITY_CPP

#define REGULAR_ENEMY	'R'
#define FILLED_ENEMY	'F'
#define PLAYER			'P'

class Entity{
public:
	char type;
	int position[2];
	double color[3];
	int directions[2];

	Entity() {};
	Entity(char type, int position[2]) {
		this->position[0] = position[0];
		this->position[1] = position[1];
		this->type = type;
		switch(type){
		case REGULAR_ENEMY: //Red
			this->color[0] = 1;
			this->color[1] = 1;
			this->color[2] = 0;
			break;
		case FILLED_ENEMY:	//Blue
			this->color[0] = 1;
			this->color[1] = 1;
			this->color[2] = 0;
			break;
		case PLAYER:		//Yellow
			this->color[0] = 1;
			this->color[1] = 1;
			this->color[2] = 0;
			break;
		}
		this->directions[0] = 1;
		this->directions[1] = 1;
	}
	Entity(char type, int position[2], int directions[2]) {
		Entity(type, position);
		this->directions[0] = directions[0];
		this->directions[1] = directions[1];
	}

	void move(){
		this->position[0] += this->directions[0];
		this->position[1] += this->directions[1];
	}
};

class Enemy: Entity {
public:
	int power;

	Enemy() {};
	Enemy(char type, int position[2]): Entity(type, position){
		this->power = 1;
	}
	Enemy(char type, int position[2], int directions[2]): Entity(type, position, directions){
		this->power = 1;
	}
	Enemy(char type, int position[2], int power): Entity(type, position){
		this->power = power;
	}
	Enemy(char type, int position[2], int directions[2], int power): Entity(type, position, directions){
		this->power = power;
	}

};

class Player: Entity {
public:
	int lives;

	Player() {};
	Player(char type, int position[2]): Entity(type, position){
		this->lives = 3;
	}
	Player(char type, int position[2], int directions[2]): Entity(type, position, directions){
		this->lives = 3;
	}
};

#endif