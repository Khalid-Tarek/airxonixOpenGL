#ifndef GLUT_HELPER_H
#define GLUT_HELPER_H

#include "field.cpp"
#include "entity.cpp"

#ifndef INCLUDE_GLUT
#define INCLUDE_GLUT
#include <GL\glut.h>
#endif

#define CELL_SIZE 0.1

void addLighting();

/**
 *	Renders 3 simple lines signifying the 3 positive axis. Only used for debugging purposes
 */
void renderCoordinateSystem();

void renderCell(Field::Cell cell, double color[3]);

void renderField(Field field);

void renderPlayer(Player player);

void renderEnemies(vector<Enemy> enemies);

#endif