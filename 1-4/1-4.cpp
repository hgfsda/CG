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

	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(200, 30); // �������� ��ġ ����
	glutInitWindowSize(600, 600); // �������� ũ�� ����
	glutCreateWindow("Example"); // ������ ���� (������ �̸�)

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
	case 'a':   // �밢������ �̵�
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
	case 'i':    // ������׷� �̵�
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
	case 'c':   // ũ�⺯ȭ
		if (check_c == 0) {
			check_c = 1;
			glutTimerFunc(30, Time_c, 1);
		}
		else
			check_c = 0;
		break;
	case 'o':   // ���򺯰�
		if (check_o == 0) {
			check_o = 1;
			glutTimerFunc(100, Time_o, 1);
		}
		else
			check_o = 0;
		break;
	case 's':   // ��� �ִϸ��̼� ���߱�
		check_a = 0, check_i = 0, check_c = 0, check_o = 0;
		break;
	case 'm':   // �簢���� ��ġ�� �ʱ� ��ġ�� ����
		check_a = 0, check_i = 0, check_c = 0, check_o = 0, cnt_c = 0;;
		size_x = size_y = 0.0f;
		for (int i = 0; i < cnt; ++i) {
			cx[i] = save_x[i];
			cy[i] = save_y[i];
			save_location_cnt[i] = 0;
		}
		break;
	case 'r':   // �����
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
		cout << "���α׷��� �����մϴ�." << endl;
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
	glutPostRedisplay(); // ȭ�� �� ���
	if (check_a == 1) {
		for (int i = 0; i < cnt; ++i) {
			if (direction_a[i] == 0) {  // ������ ���� �̵�
				cx[i] = cx[i] + 0.01f;
				cy[i] = cy[i] + 0.01f;
				if (cx[i] + size_x +0.1f >= 1.0f)
					direction_a[i] = 1;
				if (cy[i] + size_y + 0.1f >= 1.0f)
					direction_a[i] = 3;
			}
			else if(direction_a[i] == 1) {  // ���� ���� �̵�
				cx[i] = cx[i] - 0.01f;
				cy[i] = cy[i] + 0.01f;
				if (cx[i] - size_x - 0.1f <= -1.0f)
					direction_a[i] = 0;
				if (cy[i] + size_y + 0.1f >= 1.0f)
					direction_a[i] = 2;
			}
			else if(direction_a[i] == 2) {  // ���� �Ʒ��� �̵�
				cx[i] = cx[i] - 0.01f;
				cy[i] = cy[i] - 0.01f;
				if (cx[i] - size_x - 0.1f <= -1.0f)
					direction_a[i] = 3;
				if (cy[i] - size_y - 0.1f <= -1.0f)
					direction_a[i] = 1;
			}
			else if(direction_a[i] == 3) {  // ������ �Ʒ��� �̵�
				cx[i] = cx[i] + 0.01f;
				cy[i] = cy[i] - 0.01f;
				if (cx[i] + size_x + 0.1f >= 1.0f)
					direction_a[i] = 2;
				if (cy[i] - size_y - 0.1f <= -1.0f)
					direction_a[i] = 0;
			}
		}
		glutTimerFunc(10, Time_a, 1); // Ÿ�̸��Լ� �� ����
	}
}

void Time_i(int value) {
	glutPostRedisplay(); // ȭ�� �� ���
	if (check_i == 1) {
		for (int i = 0; i < cnt; ++i) {
			if (direction_i[i] == 0) {  // ������ �̵� �� �Ʒ��� �̵�

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
			else if (direction_i[i] == 1) {  // ���� �̵� �� �Ʒ��� �̵�
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
			else if (direction_i[i] == 2) {  // ������ �̵� �� ���� �̵�
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
			else if (direction_i[i] == 3) {  // ���� �̵� �� ���� �̵�
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
		glutTimerFunc(10, Time_i, 1); // Ÿ�̸��Լ� �� ����
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