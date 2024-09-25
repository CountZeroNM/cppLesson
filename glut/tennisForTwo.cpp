#include "glut.h"
#include "glm\glm.hpp"
#include "glm\gtc\constants.hpp"

#define WINDOW_WIDTH	(1280)
#define WINDOW_HEIGHT	(720)

#define GROUND_Y	(WINDOW_HEIGHT * 3 / 4)
#define BALL_POSITION_MAX	(60 * 3)

enum {
	PLAYER_1,
	PLAYER_2,
	PLAYER_NONE,
	PLAYER_MAX = PLAYER_NONE
};

typedef struct {
	glm::vec2 positions[BALL_POSITION_MAX];
	glm::vec2 velocity;
}BALL;

glm::vec2 mousePosition;
BALL ball;

glm::vec2 servePositions[PLAYER_MAX] = {
	{100,					GROUND_Y - 100},	//PLAYER1
	{WINDOW_WIDTH - 100,	GROUND_Y - 100}		//PLAYER2	
};

// youtube -37:09


void Display() {
	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(
		0, WINDOW_WIDTH,
		WINDOW_HEIGHT, 0
	);

	GLfloat range[2];
	glGetFloatv(GL_LINE_WIDTH_RANGE, range);
	glLineWidth(range[1]);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);

	glColor3ub(0x00, 0xff, 0xff);

	glBegin(GL_LINES);
	{
		glVertex2f(0, GROUND_Y);
		glVertex2f(WINDOW_WIDTH, GROUND_Y);
	}
	glEnd();

	glColor3ub(0xff, 0x00, 0x00);
	glBegin(GL_LINES);
	{
		glVertex2fv((GLfloat*)&mousePosition);
		glVertex2fv((GLfloat*)&ball.positions[0]);
	}
	glEnd();

	glPushMatrix();
	{
		glTranslatef(ball.positions[0].x, ball.positions[0].y, 0);
		glBegin(GL_TRIANGLE_FAN);
		{
			glColor3ub(0x00, 0xff, 0xff);
			glVertex2f(0, 0);
			int n = 64;
			glColor4ub(0x00, 0xff, 0xff, 0x00);
			for (int i = 0; i <= n; i++) {
				float r = glm::pi<float>() * 2 * i / n;
				glm::vec2 v = glm::vec2(cosf(r), sin(r)) * 20.0f;
				glVertex2fv((GLfloat*)&v);
			}
		}
		glEnd();
	}
	glPopMatrix();
	
	//glutWireTeapot(1);
	glutSwapBuffers();
}

void Idle() {
	ball.velocity.y += 0.02f;

	ball.positions[0] += ball.velocity;
	
	if (ball.positions[0].y > GROUND_Y) {
		ball.positions[0].y = GROUND_Y;
		ball.velocity.y *= -0.5f;
	}

	glutPostRedisplay();
}

void PassiveMotion(int _x, int _y) {
	mousePosition = { _x, _y };
}

void Mouse(int, int _mode, int, int) {
	if(_mode != GLUT_DOWN)
		return;

	glm::vec2 v = ball. positions[0] - mousePosition;
	v = glm::normalize(v);
	ball.velocity = v * 4.0f;

}

void Init() {
	for (int i = 0; i < BALL_POSITION_MAX; i++)
		ball.positions[i] = servePositions[PLAYER_1];
	ball.velocity = {};

}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GL_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("");
	glutDisplayFunc(Display);
	glutIdleFunc(Idle);
	glutPassiveMotionFunc(PassiveMotion);
	glutMouseFunc(Mouse);
	Init();
	glutMainLoop();
}
