#ifndef ENTITY_CPP
#define ENTITY_CPP

#define REGULAR_ENEMY	'R'
#define FILLED_ENEMY	'F'
#define PLAYER			'P'

class Entity{
public:
	int position[2];
	char type;
	double color[3];
	int directions[2];

	Entity() {};
	Entity(int position[2], char type) {
		this->position[0] = position[0];
		this->position[1] = position[1];
		this->type = type;
		switch(type){
		case REGULAR_ENEMY:
			break;
		case FILLED_ENEMY:
			break;
		case PLAYER:
			break;
		}
		this->directions[0] = 1;
		this->directions[1] = 1;
	}
	Entity(int position[2], char type, int directions[2]) {
		Entity(position, type);
		this->directions[0] = directions[0];
		this->directions[1] = directions[1];
	}

	void moveRight(){
		
	}
};

class Enemy: Entity {
public:
	int power;

	Enemy() {};

};

class Player: Entity {
public:
	int lives;

	Player() {};
};

#endif