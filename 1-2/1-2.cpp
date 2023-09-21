#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
using namespace std;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);

float r[8], g[8], b[8];
float cx1[8], cx2[8], cy1[8], cy2[8];

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{
	srand(time(NULL));

	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(0, 0); // �������� ��ġ ����
	glutInitWindowSize(800, 600); // �������� ũ�� ����
	glutCreateWindow("Example"); // ������ ���� (������ �̸�)

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {    // glew �ʱ�ȭ
		cerr << "Unable to initialize GLEW" << endl;
		exit(EXIT_FAILURE);
	}
	else
		cout << "GLEW Initialized\n";
	for (int i = 0; i < 8; ++i) {
		int rand_r = rand() % 100, rand_g = rand() % 100, rand_b = rand() % 100;
		r[i] = 0.01f * rand_r, g[i] = 0.01f * rand_g, b[i] = 0.01f * rand_b;
	}
	cx1[0] = cx1[4] = -1.0f, cy1[0] = cy1[4] = 0.0f, cx2[0] = cx2[4] = 0.0f, cy2[0] = cy2[4] = 1.0f;
	cx1[1] = cx1[5] = 0.0f, cy1[1] = cy1[5] = 0.0f, cx2[1] = cx2[5] = 1.0f, cy2[1] = cy2[5] = 1.0f;
	cx1[2] = cx1[6] = -1.0f, cy1[2] = cy1[6] = -1.0f, cx2[2] = cx2[6] = 0.0f, cy2[2] = cy2[6] = 0.0f;
	cx1[3] = cx1[7] = 0.0f, cy1[3] = cy1[7] = -1.0f, cx2[3] = cx2[7] = 1.0f, cy2[3] = cy2[7] = 0.0f;
	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}

GLvoid drawScene() { //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < 8; ++i) {
		glColor3f(r[i], g[i], b[i]);
		glRectf(cx1[i], cy1[i], cx2[i], cy2[i]);
	}
	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) { //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'q':    // ���α׷� ����
		cout << "���α׷��� �����մϴ�." << endl;
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y) {
	float normalized_x, normalized_y;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		int rand_r = rand() % 100, rand_g = rand() % 100, rand_b = rand() % 100;
		normalized_x = (2.0 * x / 800) - 1.0;
		normalized_y = 1.0 - (2.0 * y / 600);
		for (int i = 0; i < 4; ++i) {   // �ٱ� �׸�
			if ((normalized_x > cx1[i] && normalized_x < cx2[i] && normalized_y > cy1[i] && normalized_y < cy2[i]) && !(normalized_x > cx1[i+4] && normalized_x < cx2[i+4] && normalized_y > cy1[i+4] && normalized_y < cy2[i+4]))
				r[i] = 0.01f * rand_r, g[i] = 0.01f * rand_g, b[i] = 0.01f * rand_b;
		}
		for (int i = 4; i < 8; ++i) {  // �ȿ� �׸�
			if (normalized_x > cx1[i] && normalized_x < cx2[i] && normalized_y > cy1[i] && normalized_y < cy2[i])
				r[i] = 0.01f * rand_r, g[i] = 0.01f * rand_g, b[i] = 0.01f * rand_b;
		}
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		normalized_x = (2.0 * x / 800) - 1.0;
		normalized_y = 1.0 - (2.0 * y / 600);
		for (int i = 0; i < 4; ++i) {
			if ((normalized_x > cx1[i] && normalized_x < cx2[i] && normalized_y > cy1[i] && normalized_y < cy2[i]) && !(normalized_x > cx1[i + 4] && normalized_x < cx2[i + 4] && normalized_y > cy1[i + 4] && normalized_y < cy2[i + 4])) {
				if (cx2[i+4] - cx1[i+4] != 1.0f) {
					cx1[i + 4] = cx1[i + 4] - 0.2f;
					cx2[i + 4] = cx2[i + 4] + 0.2f;
					cy1[i + 4] = cy1[i + 4] - 0.2f;
					cy2[i + 4] = cy2[i + 4] + 0.2f;
					return;
				}
			}
		}
		for (int i = 4; i < 8; ++i) {
			if (normalized_x > cx1[i] && normalized_x < cx2[i] && normalized_y > cy1[i] && normalized_y < cy2[i]) {
				if (cx2[i] - cx1[i] != 0.2f) {
					cx1[i] = cx1[i] + 0.2f;
					cx2[i] = cx2[i] - 0.2f;
					cy1[i] = cy1[i] + 0.2f;
					cy2[i] = cy2[i] - 0.2f;
				}
			}
		}
	}
	glutPostRedisplay();
}