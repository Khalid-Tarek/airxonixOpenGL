#ifndef ENTITY_CPP
#define ENTITY_CPP

#include <vector>

#define SPEED_SLOW		0.5
#define SPEED_NORMAL	1.0
#define SPEED_FAST		2.0

using namespace std;

/**
 * Types of Enemies:
 *		* Y: 
 */
class Entity{
	vector<double> color;
	vector<int> position;
	char type;
	int speed;
	int movementDirection[2];

	//player only attributes
	int lives;

	//enemy only attributes
	int power;

	Entity() {};
};

#endif