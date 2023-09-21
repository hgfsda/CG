#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
using namespace std;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);

float r[5], g[5], b[5];
float cx[5], cy[5];
int cnt = 1, check_num = 5;
bool left_button = false;

void main(int argc, char** argv)
{
	srand(time(NULL));

	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(200, 30); // 윈도우의 위치 지정
	glutInitWindowSize(600, 600); // 윈도우의 크기 지정
	glutCreateWindow("Example"); // 윈도우 생성 (윈도우 이름)

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		cerr << "Unable to initialize GLEW" << endl;
		exit(EXIT_FAILURE);
	}
	else
		cout << "GLEW Initialized\n";
	for (int i = 0; i < 5; ++i) {
		int rand_r = rand() % 101, rand_g = rand() % 101, rand_b = rand() % 101;
		int rand_x = rand() % 201 - 100, rand_y = rand() % 201 - 100;
		r[i] = 0.01f * rand_r, g[i] = 0.01f * rand_g, b[i] = 0.01f * rand_b;
		cx[i] = 0.01f * rand_x, cy[i] = 0.01f * rand_y;
	}

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop();
}

GLvoid drawScene() {
	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < cnt; ++i) {
		glColor3f(r[i], g[i], b[i]);
		glRectf(cx[i] - 0.1f, cy[i] - 0.1f, cx[i] + 0.1f, cy[i] + 0.1f);
	}
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'a':
		if(cnt < 5)
			++cnt;
		break;
	case 'd':
		if (cnt > 0) {
			--cnt;
			int rand_r = rand() % 101, rand_g = rand() % 101, rand_b = rand() % 101;
			int rand_x = rand() % 201 - 100, rand_y = rand() % 201 - 100;
			r[cnt] = 0.01f * rand_r, g[cnt] = 0.01f * rand_g, b[cnt] = 0.01f * rand_b;
			cx[cnt] = 0.01f * rand_x, cy[cnt] = 0.01f * rand_y;
		}
		break;
	case 'r':
		for (int i = 0; i < 5; ++i) {
			int rand_r = rand() % 101, rand_g = rand() % 101, rand_b = rand() % 101;
			int rand_x = rand() % 201 - 100, rand_y = rand() % 201 - 100;
			r[i] = 0.01f * rand_r, g[i] = 0.01f * rand_g, b[i] = 0.01f * rand_b;
			cx[i] = 0.01f * rand_x, cy[i] = 0.01f * rand_y;
		}
		cnt = 1;
		break;
	case 'q':
		cout << "프로그램을 종료합니다." << endl;
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y) {
	float normalized_x, normalized_y;

	normalized_x = (2.0 * x / 600) - 1.0;
	normalized_y = 1.0 - (2.0 * y / 600);
	if (button == GLUT_LEFT_BUTTON)
		for (int i = cnt - 1; i >= 0; --i) {
			if (normalized_x > cx[i] - 0.1f && normalized_x < cx[i] + 0.1f && normalized_y > cy[i] - 0.1f && normalized_y < cy[i] + 0.1f) {
				left_button = true;
				check_num = i;
				return;
			}
			else
				check_num = 5;
		}
}

void Motion(int x, int y) {
	float normalized_x, normalized_y;

	normalized_x = (2.0 * x / 600) - 1.0;
	normalized_y = 1.0 - (2.0 * y / 600);
	if (left_button == true) {
		cx[check_num] = normalized_x;
		cy[check_num] = normalized_y;
	}
	glutPostRedisplay();
}
