#include "main.h"


//Current Game State
int gameState = PLAYING;

//The angles of view of the scene
double sceneRotateX = 35;
double sceneRotateY = 25;

//Periodically Checks for change in the gameState variable
void checkGameState(){
	if(gameState != PLAYING) return;
	
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslated( 0, 0, -10);
	glRotated(sceneRotateX, 1, 0, 0);
	glRotated(sceneRotateY, 0, 1, 0);

	renderCoordinateSystem();

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
	if(sceneRotateX >= 360)	sceneRotateX -= 360;
	if(sceneRotateX < 0)		sceneRotateX += 360;
	if(sceneRotateY >= 360)	sceneRotateY -= 360;
	if(sceneRotateY < 0)		sceneRotateY += 360;
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	if(key == 27) exit(0);
	if(gameState != PLAYING) return;
	switch(key){
	case 'w':
		
		break;
	case 's':
		
		break;
	case 'a':
		
		break;
	case 'd':
		
		break;
	}
	glutPostRedisplay();
}

void timer(int x){
	
	glutPostRedisplay();
}

int main(int argc, char* argv[]) {

	//GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

	//GLUT Window Initialization
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - WINDOW_WIDTH) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - WINDOW_HEIGHT) / 2);
	glutCreateWindow("Bloxorz");
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

	//glutFullScreen();
	glutMainLoop();
}
