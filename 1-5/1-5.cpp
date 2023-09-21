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
void add_food(int value);

float r[40], g[40], b[40];
float food_x[40], food_y[40];
float cr = 0.0f, cg = 0.0f, cb = 0.0f;
float cx, cy, size_c = 0;
int food_cnt;
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
	for (int i = 0; i < 40; ++i) {
		int rand_r = rand() % 101, rand_g = rand() % 101, rand_b = rand() % 101;
		r[i] = 0.01f * rand_r, g[i] = 0.01f * rand_g, b[i] = 0.01f * rand_b;    // 랜덤 색 지정
		int rand_x = rand() % 201 - 100, rand_y = rand() % 201 - 100;
		food_x[i] = 0.01f * rand_x, food_y[i] = 0.01f * rand_y;   // 랜덤 위치 지정
	}
	food_cnt = rand() % 20 + 20;

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutTimerFunc(150, add_food, 1);
	glutMainLoop();
}

GLvoid drawScene() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i <= food_cnt; ++i) {
		glColor3f(r[i], g[i], b[i]);
		glRectf(food_x[i] - 0.02f, food_y[i] - 0.02f, food_x[i] + 0.02f, food_y[i] + 0.02f);
	}
	if (left_button == true) {
		glColor3f(cr, cg, cb);
		glRectf(cx - size_c - 0.05f, cy - size_c - 0.05f, cx + size_c + 0.05f, cy + size_c + 0.05f);
	}
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	srand(time(NULL));
	switch (key) {
	case 'r':   // 재시작
		for (int i = 0; i < 40; ++i) {
			int rand_r = rand() % 101, rand_g = rand() % 101, rand_b = rand() % 101;
			r[i] = 0.01f * rand_r, g[i] = 0.01f * rand_g, b[i] = 0.01f * rand_b;    // 랜덤 색 지정
			int rand_x = rand() % 201 - 100, rand_y = rand() % 201 - 100;
			food_x[i] = 0.01f * rand_x, food_y[i] = 0.01f * rand_y;   // 랜덤 위치 지정
		}
		food_cnt = rand() % 20 + 20;
		left_button = false;
		glutTimerFunc(150, add_food, 1);
		break;
	case 'q':
		cout << "프로그램을 종료합니다." << endl;
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN)
			left_button = true;
		else if (state == GLUT_UP) {
			left_button = false;
			size_c = 0;
			cr = cg = cb = 0.0f;
		}
	}
}

void Motion(int x, int y) {
	float normalized_x, normalized_y;

	normalized_x = (2.0 * x / 600) - 1.0;
	normalized_y = 1.0 - (2.0 * y / 600);
	if (left_button == true) {
		cx = normalized_x;
		cy = normalized_y;

		for (int i = 0; i <= food_cnt; ++i) {
 			if ((food_x[i] - 0.02f < cx + size_c + 0.05f) && (food_y[i] + 0.02f > cy - size_c - 0.05f) && (food_x[i] + 0.02f > cx - size_c - 0.05f) && (food_y[i] - 0.02f < cy + size_c + 0.05f)) {
				cr = (cr + r[i]) / 2;
				cg = (cg + g[i]) / 2;
				cb = (cb + b[i]) / 2;
				for (int j = i; j < food_cnt; ++j) {
					r[j] = r[j + 1];
					g[j] = g[j + 1];
					b[j] = b[j + 1];
					food_x[j] = food_x[j + 1];
					food_y[j] = food_y[j + 1];
				}
				--food_cnt;
				size_c = size_c + 0.01f;
			}
		}
	}
	glutPostRedisplay();
}

void add_food(int value) {
	if (food_cnt < 39) {
		++food_cnt;
		int rand_r = rand() % 101, rand_g = rand() % 101, rand_b = rand() % 101;
		r[food_cnt] = 0.01f * rand_r, g[food_cnt] = 0.01f * rand_g, b[food_cnt] = 0.01f * rand_b;    // 랜덤 색 지정
		int rand_x = rand() % 201 - 100, rand_y = rand() % 201 - 100;
		food_x[food_cnt] = 0.01f * rand_x, food_y[food_cnt] = 0.01f * rand_y;   // 랜덤 위치 지정
		glutPostRedisplay();
	}
	glutTimerFunc(150, add_food, 1);
}
