#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void TimerFunction(int value);
int random_check = 0;

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(0, 0); // �������� ��ġ ����
	glutInitWindowSize(800, 600); // �������� ũ�� ����
	glutCreateWindow("Example"); // ������ ���� (������ �̸�)

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {    // glew �ʱ�ȭ
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutKeyboardFunc(Keyboard);
	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}

float r = 1.0f;
float g = 1.0f;
float b = 1.0f;

GLvoid drawScene() { //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
	glClearColor(r, g, b, 1.0f); // �������� ��blue�� �� ����
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�
	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�.
	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) { //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
	glViewport(0, 0, w, h);
}

void TimerFunction(int value) {
	srand(time(NULL));
	int rand_r = rand() % 100, rand_g = rand() % 100, rand_b = rand() % 100;
	r = 0.01f * rand_r, g = 0.01f * rand_g, b = 0.01f * rand_b;
	glutPostRedisplay(); // ȭ�� �� ���
	if (random_check == 1)
		glutTimerFunc(200, TimerFunction, 1); // Ÿ�̸��Լ� �� ����
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	srand(time(NULL));
	int rand_r = rand() % 100, rand_g = rand() % 100, rand_b = rand() % 100;
	switch (key) {
	case 'c':    // û�ϻ�
		r = 0.0f, g = 0.3f, b = 0.4f;
		break;
	case 'm':    // ��ȫ��
		r = 1.0f, g = 0.0f, b = 1.0f;
		break;
	case 'y':    // �����
		r = 1.0f, g = 1.0f, b = 0.0f;
		break;
	case 'a':    // ������
		r = 0.01f * rand_r, g = 0.01f * rand_g, b = 0.01f * rand_b;
		break;
	case 'w':    // �Ͼ��
		r = 1.0f, g = 1.0f, b = 1.0f;
		break;
	case 'k':    // ������
		r = 0.0f, g = 0.0f, b = 0.0f;
		break;
	case 't':    // Ÿ�̸Ӹ� �����Ͽ� Ư�� �ð����� ���������� ����
		random_check = 1;
		glutTimerFunc(200, TimerFunction, 1); 
		break;
	case 's':    // Ÿ�̸� ����
		random_check = 0;
		break;
	case 'q':    // ���α׷� ����
		std::cout << "���α׷��� �����մϴ�." << std::endl;
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}