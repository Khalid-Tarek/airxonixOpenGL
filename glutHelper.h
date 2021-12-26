#ifndef GLUT_HELPER_H
#define GLUT_HELPER_H

#include "field.cpp"
#include "entity.cpp"

#ifndef INCLUDE_GLUT
#define INCLUDE_GLUT
#include <GL\glut.h>
#endif

#define Z_DISTANCE	20	//Distance of which the GUI should be away from the origin

#define CELL_SIZE	0.1	//The dimension of a single cell on the board

void addLighting();

void renderGUI(int lives, double percentageFilled, double goalFilled, int timeLeft);

void renderChar(double x, double y, char c);

void renderCoordinateSystem();

void renderCell(Field::Cell cell, double color[3]);

void renderField(Field field);

void renderPlayer(Player player);

void renderEnemies(vector<Enemy> enemies);

#endif