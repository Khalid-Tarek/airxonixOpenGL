#ifndef ENTITY_CPP
#define ENTITY_CPP

#include <iostream>
#include <string>

using namespace std;

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
			this->color[1] = 0;
			this->color[2] = 0;
			break;
		case FILLED_ENEMY:	//Blue
			this->color[0] = 0;
			this->color[1] = 0;
			this->color[2] = 1;
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

	void move(int limit){
		int newX = this->position[0] + this->directions[0];
		int newZ = this->position[1] + this->directions[1];

		if(newX >= limit || newX < 0 || newZ >= limit || newZ < 0) return;

		this->position[0] = newX;
		this->position[1] = newZ;
	}

	string toString(){
		string entity = "";

		entity.push_back(type);
		entity.push_back(' ');

		entity +=	"(" + std::to_string((long long)position[0]) + "," + 
					std::to_string((long long)position[1]) + ") ";
		entity +=	"(" + std::to_string((long long)color[0]) + "," + 
					std::to_string((long long)color[1]) + "," + 
					std::to_string((long long)color[2]) + ") ";
		entity +=	"(" + std::to_string((long long)directions[0]) + "," + 
					std::to_string((long long)directions[1]) + ")";

		return entity;
	}
};

class Enemy: public Entity {
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

	string toString(){
		string enemy = Entity::toString();

		enemy += " " + std::to_string((long long)power);

		return enemy;
	}

};

class Player: public Entity {
public:
	int lives;
	bool isFilling;

	Player() {};
	Player(int position[2]): Entity(PLAYER, position){
		this->lives = 3;
		this->isFilling = false;
	}

	void getHit(){
		lives--;
		position[0] = 0;
		position[1] = 0;
		isFilling = false;
	}

	string toString(){
		string player = Entity::toString();

		player += " " + std::to_string((long long)lives);
		player += isFilling? " true" : " false";

		return player;
	}
};

#endif