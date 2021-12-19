#include "main.h"

//Current Game State
int gameState = PLAYING;

//The angles of view of the scene
double sceneRotateX = 30;
double sceneRotateY = 0;

Field board;
Player player;
vector<Enemy> enemies;

//Periodically Checks for change in the gameState variable
void checkGameState(){
	if(gameState != PLAYING) return;
	
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslated(-5, 0, -20);
	glRotated(sceneRotateX, 1, 0, 0);
	glRotated(sceneRotateY, 0, 1, 0);

	addLighting();

	renderCoordinateSystem();

	renderField(board);

	renderPlayer(player);

	renderEnemies(enemies);
	
	glutSwapBuffers();
}

void lockResizing(int width, int height){
	glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
	display();
}

void arrows(int key, int x, int y){
	switch(key){
		case GLUT_KEY_DOWN:
			sceneRotateX += 5;
			break;
		case GLUT_KEY_UP:
			sceneRotateX -= 5;
			break;
		case GLUT_KEY_LEFT:
			sceneRotateY += 5;
			break;
		case GLUT_KEY_RIGHT:
			sceneRotateY -= 5;
			break;
	}
	if(sceneRotateX >= 360)		sceneRotateX -= 360;
	if(sceneRotateX < 0)		sceneRotateX += 360;
	if(sceneRotateY >= 360)		sceneRotateY -= 360;
	if(sceneRotateY < 0)		sceneRotateY += 360;
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	if(key == 27) exit(0);
	if(gameState != PLAYING) return;
	switch(key){
	case 'w':
		player.directions[0] = 0;
		player.directions[1] = -1;
		break;
	case 's':
		player.directions[0] = 0;
		player.directions[1] = 1;
		break;
	case 'a':
		player.directions[0] = -1;
		player.directions[1] = 0;
		break;
	case 'd':
		player.directions[0] = 1;
		player.directions[1] = 0;
		break;
	}
	board.move(player);
	glutPostRedisplay();
}

void timer(int x){
	for(int i = 0; i < enemies.size(); i++)
		board.move(enemies[i]);
	glutTimerFunc(1, timer, 0);
	glutPostRedisplay();
}

int main(int argc, char* argv[]) {

	board = Field(1);

	int position[2] = {0, 0};
	player = Player(position);

	cout << position << endl;

	enemies = board.enemies;

	//GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

	//GLUT Window Initialization
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - WINDOW_WIDTH) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - WINDOW_HEIGHT) / 2);
	glutCreateWindow("AirXonix");
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	//Callback Attatchment
	glutDisplayFunc(display);
	glutReshapeFunc(lockResizing);
	glutSpecialFunc(arrows);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(1, timer, 0);

	//Perspective Projection Setup
	glClearColor(BACKGROUND_COLOR);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOV, WINDOW_WIDTH / WINDOW_HEIGHT, 2.0, 100.0);
	glMatrixMode(GL_MODELVIEW);

	//Depth and Multisampling Setup
	glEnable(GL_DEPTH_TEST);

	//Lighting and Shadows
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);			//Enables Lighting: Use current lighting parameters to compute color
	glEnable(GL_LIGHT0);			//Enables Light0
	glEnable(GL_LIGHT1);			//Enables Light1
	glEnable(GL_NORMALIZE);			//Normalizes color vectors to a unit length before lighting
	glShadeModel(GL_SMOOTH);

	//glutFullScreen();
	glutMainLoop();
}
