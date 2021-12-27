#include "glutHelper.h"

//Window Properties
#define WINDOW_WIDTH		1024
#define WINDOW_HEIGHT		1024
#define FOV					60
#define BACKGROUND_COLOR	0, 0.3, 0.3, 1
#define FPS					75

#define ENEMY_SLOW_RATE		4	//Lower is slower (Range: 2 - infinity) (1: 100% slow, 2: 50% slow, 3: 33% slow, 4: 25% slow, and so on)

//Game States
#define PLAYING				10
#define WON					20
#define LOST				30

void checkGameState();

void display();

void lockResizing(int width, int height);

void arrows(int key, int x, int y);

void keyboard(unsigned char key, int x, int y);

void timer(int x);

int main(int argc, char* argv[]);