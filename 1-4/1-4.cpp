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
void Time_a(int value);
void Time_i(int value);
void Time_c(int value);
void Time_o(int value);

float r[5], g[5], b[5];
float cx[5], cy[5], save_x[5], save_y[5];
float size_x = 0.0f, size_y = 0.0f;
int cnt = 0;
int check_a = 0, check_i = 0, check_c = 0, check_o = 0;
int direction_a[5], direction_i[5], cnt_c = 0;
int save_location_cnt[5];

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
		r[i] = 0.01f * rand_r, g[i] = 0.01f * rand_g, b[i] = 0.01f * rand_b;
		direction_a[i] = direction_i[i] = 0;
	}

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape); 
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMainLoop();
}

GLvoid drawScene() {
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < cnt; ++i) {
		glColor3f(r[i], g[i], b[i]);
		glRectf(cx[i] - size_x - 0.1f, cy[i] - size_y - 0.1f, cx[i] + size_x + 0.1f, cy[i] + size_y + 0.1f);
	}
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	srand(time(NULL));
	switch (key) {
	case 'a':   // 대각선으로 이동
		if (check_a == 0) {
			check_a = 1;
			check_i = 0;
			for (int i = 0; i < 5; ++i) {
				direction_a[i] = rand() % 4;
				direction_i[i] = rand() % 4;
				save_location_cnt[i] = 0;
			}
			glutTimerFunc(10, Time_a, 1);
		}
		else
			check_a = 0;
		break;
	case 'i':    // 지그재그로 이동
		if (check_i == 0) {
			check_i = 1;
			check_a = 0;
			for (int i = 0; i < 5; ++i) {
				direction_a[i] = rand() % 4;
				direction_i[i] = rand() % 4;
				save_location_cnt[i] = 0;
			}
			glutTimerFunc(10, Time_i, 1);
		}
		else
			check_i = 0;
		break;
	case 'c':   // 크기변화
		if (check_c == 0) {
			check_c = 1;
			glutTimerFunc(30, Time_c, 1);
		}
		else
			check_c = 0;
		break;
	case 'o':   // 색깔변경
		if (check_o == 0) {
			check_o = 1;
			glutTimerFunc(100, Time_o, 1);
		}
		else
			check_o = 0;
		break;
	case 's':   // 모든 애니메이션 멈추기
		check_a = 0, check_i = 0, check_c = 0, check_o = 0;
		break;
	case 'm':   // 사각형의 위치를 초기 위치로 변경
		check_a = 0, check_i = 0, check_c = 0, check_o = 0, cnt_c = 0;;
		size_x = size_y = 0.0f;
		for (int i = 0; i < cnt; ++i) {
			cx[i] = save_x[i];
			cy[i] = save_y[i];
			save_location_cnt[i] = 0;
		}
		break;
	case 'r':   // 재시작
		cnt = 0;
		check_a = 0, check_i = 0, check_c = 0, check_o = 0, cnt_c = 0;
		size_x = size_y = 0.0f;
		for (int i = 0; i < 5; ++i) {
			int rand_r = rand() % 101, rand_g = rand() % 101, rand_b = rand() % 101;
			r[i] = 0.01f * rand_r, g[i] = 0.01f * rand_g, b[i] = 0.01f * rand_b;
			save_x[i] = cx[i] = save_y[i] = cy[i] = 0;
			direction_a[i] = rand() % 4;
			direction_i[i] = rand() % 4;
			save_location_cnt[i] = 0;
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
	float normalized_x, normalized_y;
	normalized_x = (2.0 * x / 600) - 1.0;
	normalized_y = 1.0 - (2.0 * y / 600);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (cnt < 5) {
			save_x[cnt] = cx[cnt] = normalized_x, save_y[cnt] = cy[cnt] = normalized_y;
			++cnt;
		}
	}
}

void Time_a(int value) {
	glutPostRedisplay(); // 화면 재 출력
	if (check_a == 1) {
		for (int i = 0; i < cnt; ++i) {
			if (direction_a[i] == 0) {  // 오른쪽 위로 이동
				cx[i] = cx[i] + 0.01f;
				cy[i] = cy[i] + 0.01f;
				if (cx[i] + size_x +0.1f >= 1.0f)
					direction_a[i] = 1;
				if (cy[i] + size_y + 0.1f >= 1.0f)
					direction_a[i] = 3;
			}
			else if(direction_a[i] == 1) {  // 왼쪽 위로 이동
				cx[i] = cx[i] - 0.01f;
				cy[i] = cy[i] + 0.01f;
				if (cx[i] - size_x - 0.1f <= -1.0f)
					direction_a[i] = 0;
				if (cy[i] + size_y + 0.1f >= 1.0f)
					direction_a[i] = 2;
			}
			else if(direction_a[i] == 2) {  // 왼쪽 아래로 이동
				cx[i] = cx[i] - 0.01f;
				cy[i] = cy[i] - 0.01f;
				if (cx[i] - size_x - 0.1f <= -1.0f)
					direction_a[i] = 3;
				if (cy[i] - size_y - 0.1f <= -1.0f)
					direction_a[i] = 1;
			}
			else if(direction_a[i] == 3) {  // 오른쪽 아래로 이동
				cx[i] = cx[i] + 0.01f;
				cy[i] = cy[i] - 0.01f;
				if (cx[i] + size_x + 0.1f >= 1.0f)
					direction_a[i] = 2;
				if (cy[i] - size_y - 0.1f <= -1.0f)
					direction_a[i] = 0;
			}
		}
		glutTimerFunc(10, Time_a, 1); // 타이머함수 재 설정
	}
}

void Time_i(int value) {
	glutPostRedisplay(); // 화면 재 출력
	if (check_i == 1) {
		for (int i = 0; i < cnt; ++i) {
			if (direction_i[i] == 0) {  // 오른쪽 이동 후 아래로 이동

				if (cx[i] + 0.1f < 1.0f) {
					cx[i] = cx[i] + 0.01f;
				}
				else {
					cy[i] = cy[i] - 0.01f;
					++save_location_cnt[i];
					if (save_location_cnt[i] == 20) {
						save_location_cnt[i] = 0;
						direction_i[i] = 1;
					}
					if (cy[i] - 0.1f <= -1.0f)
						direction_i[i] = 3;
				}
			}
			else if (direction_i[i] == 1) {  // 왼쪽 이동 후 아래로 이동
				if (cx[i] - 0.1f > -1.0f) {
					cx[i] = cx[i] - 0.01f;
				}
				else {
					cy[i] = cy[i] - 0.01f;
					++save_location_cnt[i];
					if (save_location_cnt[i] == 20) {
						save_location_cnt[i] = 0;
						direction_i[i] = 0;
					}
					if (cy[i] - 0.1f <= -1.0f)
						direction_i[i] = 2;
				}
			}
			else if (direction_i[i] == 2) {  // 오른쪽 이동 후 위로 이동
				if (cx[i] + 0.1f < 1.0f) {
					cx[i] = cx[i] + 0.01f;
				}
				else {
					cy[i] = cy[i] + 0.01f;
					++save_location_cnt[i];
					if (save_location_cnt[i] == 20) {
						save_location_cnt[i] = 0;
						direction_i[i] = 3;
					}
					if (cy[i] + 0.1f >= 1.0f)
						direction_i[i] = 1;
				}
			}
			else if (direction_i[i] == 3) {  // 왼쪽 이동 후 위로 이동
				if (cx[i] - 0.1f > -1.0f) {
					cx[i] = cx[i] - 0.01f;
				}
				else {
					cy[i] = cy[i] + 0.01f;
					++save_location_cnt[i];
					if (save_location_cnt[i] == 20) {
						save_location_cnt[i] = 0;
						direction_i[i] = 2;
					}
					if (cy[i] + 0.1f >= 1.0f)
						direction_i[i] = 0;
				}
			}
		}
		glutTimerFunc(10, Time_i, 1); // 타이머함수 재 설정
	}
}

void Time_c(int value) {
	if (check_c == 1) {
		if (cnt_c == 0) {
			size_x = size_x + 0.01f;
			size_y = size_y + 0.01f;
			if (size_x >= 0.2f)
				cnt_c = 1;
		}
		else if (cnt_c == 1) {
			size_x = size_x - 0.01f;
			size_y = size_y - 0.01f;
			if (size_x <= -0.05f)
				cnt_c = 0;
		}
		glutPostRedisplay();
		glutTimerFunc(30, Time_c, 1);
	}
}

void Time_o(int value) {
	srand(time(NULL));
	if (check_o == 1) {
		for (int i = 0; i < cnt; ++i) {
			int rand_r = rand() % 100, rand_g = rand() % 100, rand_b = rand() % 100;
			r[i] = 0.01f * rand_r, g[i] = 0.01f * rand_g, b[i] = 0.01f * rand_b;
		}
		glutPostRedisplay();
		glutTimerFunc(100, Time_o, 1);
	}
}