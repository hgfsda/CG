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
void motion_block(int value);

float r[5], g[5], b[5], sr[5], sg[5], sb[5];
float cx[5], cy[5], size_c[5];
float min_x[5][8], min_y[5][8], size_min[5];
int random_motion[5], check_click[5];

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
		sr[i] = r[i] = 0.01f * rand_r, sg[i] = g[i] = 0.01f * rand_g, sb[i] = b[i] = 0.01f * rand_b;    // 랜덤 색 지정
		int rand_x = rand() % 101 - 50, rand_y = rand() % 101 - 50;
		cx[i] = 0.01f * rand_x, cy[i] = 0.01f * rand_y;   // 랜덤 위치 지정
		size_min[i] = 0.1f;
		check_click[i] = random_motion[i] = 0;
		size_c[i] = 0.2f;
		random_motion[i] = rand() % 3 + 1;
		// 8개의 미니 사각형을 만든다 0. 오른쪽, 1. 위, 2. 왼쪽, 3. 아래, 4. 오른쪽위, 5. 왼쪽위, 6. 왼쪽아래, 7. 오른쪽아래
		min_x[i][0] = cx[i] + 0.1f, min_x[i][1] = cx[i], min_x[i][2] = cx[i] - 0.1f, min_x[i][3] = cx[i];
		min_x[i][4] = cx[i] + 0.1f, min_x[i][5] = cx[i] - 0.1f, min_x[i][6] = cx[i] - 0.1f, min_x[i][7] = cx[i] + 0.1f;
		min_y[i][0] = cy[i], min_y[i][1] = cy[i] + 0.1f, min_y[i][2] = cy[i], min_y[i][3] = cy[i] - 0.1f;
		min_y[i][4] = cy[i] + 0.1f, min_y[i][5] = cy[i] + 0.1f, min_y[i][6] = cy[i] - 0.1f, min_y[i][7] = cy[i] - 0.1f;
	}

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutTimerFunc(150, motion_block, 1);
	glutMainLoop();
}

GLvoid drawScene() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < 5; ++i) {
		glColor3f(sr[i], sg[i], sb[i]);
		if (random_motion[i] == 1) {
			for (int j = 0; j < 4; ++j) {
				glRectf(min_x[i][j] - size_min[i], min_y[i][j] - size_min[i], min_x[i][j] + size_min[i], min_y[i][j] + size_min[i]);
			}
		}
		else if (random_motion[i] == 2) {
			for (int j = 4; j < 8; ++j) {
				glRectf(min_x[i][j] - size_min[i], min_y[i][j] - size_min[i], min_x[i][j] + size_min[i], min_y[i][j] + size_min[i]);
			}
		}
		else if (random_motion[i] == 3) {
			for (int j = 0; j < 8; ++j) {
				glRectf(min_x[i][j] - size_min[i], min_y[i][j] - size_min[i], min_x[i][j] + size_min[i], min_y[i][j] + size_min[i]);
			}
		}
	}
	for (int i = 0; i < 5; ++i) {
		glColor3f(r[i], g[i], b[i]);
		glRectf(cx[i] - size_c[i], cy[i] - size_c[i], cx[i] + size_c[i], cy[i] + size_c[i]);
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
		for (int i = 0; i < 5; ++i) {
		int rand_r = rand() % 101, rand_g = rand() % 101, rand_b = rand() % 101;
		int rand_x = rand() % 201 - 100, rand_y = rand() % 201 - 100;
		sr[i] = r[i] = 0.01f * rand_r, sg[i] = g[i] = 0.01f * rand_g, sb[i] = b[i] = 0.01f * rand_b;
		cx[i] = 0.01f * rand_x, cy[i] = 0.01f * rand_y;
		size_c[i] = 0.2f;
		size_min[i] = 0.1f;
		check_click[i] = random_motion[i] = 0;
		min_x[i][0] = cx[i] + 0.1f, min_x[i][1] = cx[i], min_x[i][2] = cx[i] - 0.1f, min_x[i][3] = cx[i];
		min_x[i][4] = cx[i] + 0.1f, min_x[i][5] = cx[i] - 0.1f, min_x[i][6] = cx[i] - 0.1f, min_x[i][7] = cx[i] + 0.1f;
		min_y[i][0] = cy[i], min_y[i][1] = cy[i] + 0.1f, min_y[i][2] = cy[i], min_y[i][3] = cy[i] - 0.1f;
		min_y[i][4] = cy[i] + 0.1f, min_y[i][5] = cy[i] + 0.1f, min_y[i][6] = cy[i] - 0.1f, min_y[i][7] = cy[i] - 0.1f;
		random_motion[i] = rand() % 3 + 1;
		}
		break;
	case 'q':
		cout << "프로그램을 종료합니다." << endl;
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y) {
	srand(time(NULL));
	float normalized_x, normalized_y;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		normalized_x = (2.0 * x / 600) - 1.0;
		normalized_y = 1.0 - (2.0 * y / 600);
		for (int i = 4; i >= 0; --i) {
			if (normalized_x > cx[i] - size_c[i] && normalized_x < cx[i] + size_c[i] && normalized_y > cy[i] - size_c[i] && normalized_y < cy[i] + size_c[i]) {
				check_click[i] = 1;
				size_c[i] = 0;
				return;
			}
		}
	}
}

void motion_block(int value) {
	for (int i = 0; i < 5; ++i) {
		if (check_click[i] == 1) {
			if (size_min[i] > 0) {
				size_min[i] = size_min[i] - 0.005f;
				min_x[i][0] = min_x[i][0] + 0.03f;
				min_y[i][1] = min_y[i][1] + 0.03f;
				min_x[i][2] = min_x[i][2] - 0.03f;
				min_y[i][3] = min_y[i][3] - 0.03f;
				min_x[i][4] = min_x[i][4] + 0.03f;
				min_y[i][4] = min_y[i][4] + 0.03f;
				min_x[i][5] = min_x[i][5] - 0.03f;
				min_y[i][5] = min_y[i][5] + 0.03f;
				min_x[i][6] = min_x[i][6] - 0.03f;
				min_y[i][6] = min_y[i][6] - 0.03f;
				min_x[i][7] = min_x[i][7] + 0.03f;
				min_y[i][7] = min_y[i][7] - 0.03f;
			}
			else
				size_min[i] = 0;
		}
	}
	glutPostRedisplay();
	glutTimerFunc(150, motion_block, 1);
}
