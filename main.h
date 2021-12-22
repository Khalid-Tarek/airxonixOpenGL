#include "glutHelper.h"

//Window Properties
#define WINDOW_WIDTH		1024
#define WINDOW_HEIGHT		1024
#define FOV					60
#define BACKGROUND_COLOR	0, 0.3, 0.3, 1
#define FPS					75

//Game States
#define PLAYING				10
#define WON					20
#define LOST				30

//The steps of animating
#define ANIMATION_STEP		5

//Periodically Checks for change in the gameState variable
void checkGameState();

void display();

void lockResizing(int width, int height);

void arrows(int key, int x, int y);

void keyboard(unsigned char key, int x, int y);

void timer(int x);

int main(int argc, char* argv[]);