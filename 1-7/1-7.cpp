#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include "filetobuf.h"

void make_vertexShaders();
void make_fragmentShaders();
void make_shaderProgram();
void InitBuffer();
void reset();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);

GLuint shaderProgramID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü
int cnt, save_draw_check[10];   // ���� �׷��� ���� ����, ������ ���� ����
float r[10], g[10], b[10];
GLfloat Shape[10][18];    // ������ ��ǥ
GLfloat colors[10][18];  // ���� �������� ����
GLuint vao[10], vbo[20];

void reset() {
	cnt = 0;
	for (int i = 0; i < 10; ++i) {
		save_draw_check[i] = 0;
		r[i] = (rand() % 101) * 0.01, g[i] = (rand() % 101) * 0.01, b[i] = (rand() % 101) * 0.01;
		for (int j = 0; j < 18; ++j) {
			Shape[i][j] = 0;
			if (j % 3 == 0) colors[i][j] = r[i];
			else if (j % 3 == 1) colors[i][j] = g[i];
			else if (j % 3 == 2) colors[i][j] = b[i];
		}
	}
}

void main(int argc, char** argv)
{
	srand(time(NULL));
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 30);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Example");
	glewExperimental = GL_TRUE;
	reset();
	glewInit();
	make_shaderProgram();
	InitBuffer();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMainLoop();
}

GLvoid drawScene() {
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position"); //	: 0  Shader�� 'layout (location = 0)' �κ�
	int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color");


	for (int i = 0; i < cnt; ++i) {
		glBindVertexArray(vao[i]);
		if (save_draw_check[i] == 1) {
			glPointSize(10);
			glDrawArrays(GL_POINTS, Shape[i][0], 1);
		}
		else if (save_draw_check[i] == 2) {
			glLineWidth(10);
			glDrawArrays(GL_LINES, Shape[i][0], 2);
		}
		else if (save_draw_check[i] == 3) {
			glDrawArrays(GL_TRIANGLES, Shape[i][0], 3);
		}
		else if (save_draw_check[i] == 4) {
			glDrawArrays(GL_TRIANGLES, Shape[i][0], 6);
		}
	}

	glEnableVertexAttribArray(PosLocation);
	glEnableVertexAttribArray(ColorLocation);

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

void InitBuffer()
{
	glGenVertexArrays(cnt, vao);
	glGenBuffers(cnt * 2, vbo);

	for (int i = 0; i < cnt; ++i) {
		glBindVertexArray(vao[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i * 2]);
		glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), Shape[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[i * 2 + 1]);
		glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), colors[i], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);
	}
}

void make_shaderProgram()
{
	make_vertexShaders(); //--- ���ؽ� ���̴� �����
	make_fragmentShaders(); //--- �����׸�Ʈ ���̴� �����
	//-- shader Program
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);
	//--- ���̴� �����ϱ�
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program ����ϱ�
	glUseProgram(shaderProgramID);
}

GLchar* vertexSource, * fragmentSource; //--- �ҽ��ڵ� ���� ����

void make_vertexShaders()
{
	vertexSource = filetobuf("vertex.glsl");
	//--- ���ؽ� ���̴� ��ü �����
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	//--- ���ؽ� ���̴� �������ϱ�
	glCompileShader(vertexShader);
	//--- �������� ����� ���� ���� ���: ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShaders()
{
	fragmentSource = filetobuf("fragment.glsl");
	//--- �����׸�Ʈ ���̴� ��ü �����
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	//--- �����׸�Ʈ ���̴� ������
	glCompileShader(fragmentShader);
	//--- �������� ����� ���� ���� ���: ������ ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

void move_shape(float dir_x, float dir_y) {
	for (int i = 0; i < cnt; ++i) {
		if (save_draw_check[i] == 1) {
			Shape[i][0] += dir_x;
			Shape[i][1] += dir_y;
		}
		else if (save_draw_check[i] == 2) {
			Shape[i][0] += dir_x;
			Shape[i][1] += dir_y;
			Shape[i][3] += dir_x;
			Shape[i][4] += dir_y;
		}
		else if (save_draw_check[i] == 3) {
			Shape[i][0] += dir_x;
			Shape[i][1] += dir_y;
			Shape[i][3] += dir_x;
			Shape[i][4] += dir_y;
			Shape[i][6] += dir_x;
			Shape[i][7] += dir_y;
		}
		else if (save_draw_check[i] == 4) {
			Shape[i][0] += dir_x;
			Shape[i][1] += dir_y;
			Shape[i][3] = Shape[i][9] += dir_x;
			Shape[i][4] = Shape[i][10] += dir_y;
			Shape[i][6] = Shape[i][12] += dir_x;
			Shape[i][7] = Shape[i][13] += dir_y;
			Shape[i][15] += dir_x;
			Shape[i][16] += dir_y;
		}
	}
	InitBuffer();
	drawScene();
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'p':    // �� �׸���
		save_draw_check[cnt] = 1;
		break;
	case 'l':    // �� �׸���
		save_draw_check[cnt] = 2;
		break;
	case 't':    // �ﰢ�� �׸���
		save_draw_check[cnt] = 3;
		break;
	case 'r':    // �׸� �׸���
		save_draw_check[cnt] = 4;
		break;
	case 'w':    // ���� �̵�
		move_shape(0, 0.05);
		break;
	case 'a':    // �������� �̵�
		move_shape(-0.05, 0);
		break;
	case 's':    // �Ʒ��� �̵�
		move_shape(0, -0.05);
		break;
	case 'd':    // ���������� �̵�
		move_shape(0.05, 0);
		break;
	case 'c':   // ��� ���� ����
		reset();
		drawScene();
		break;
	case 'q':   // ���α׷� ����
		std::cout << "���α׷��� �����մϴ�." << std::endl;
		glutLeaveMainLoop();
		break;
	}
}

void Mouse(int button, int state, int x, int y) {
	float normalized_x, normalized_y;
	float dis_x, dis_y;
	dis_x = (rand() % 100 + 200) * 0.001;  // 0.2~0.3����
	dis_y = (rand() % 600 - 300) * 0.001;  // -0.3 ~ 0.3

	normalized_x = (2.0 * x / 800) - 1.0;
	normalized_y = 1.0 - (2.0 * y / 600);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (save_draw_check[cnt] != 0 && cnt < 10) {
			if (save_draw_check[cnt] == 1) {
				Shape[cnt][0] = normalized_x;
				Shape[cnt][1] = normalized_y;
			}
			else if (save_draw_check[cnt] == 2) {
				Shape[cnt][0] = normalized_x - dis_x;
				Shape[cnt][1] = normalized_y - dis_y;
				Shape[cnt][3] = normalized_x + dis_x;
				Shape[cnt][4] = normalized_y + dis_y;
			}
			else if (save_draw_check[cnt] == 3) {
				Shape[cnt][0] = normalized_x;
				Shape[cnt][1] = normalized_y + 0.2;
				Shape[cnt][3] = normalized_x - 0.17;
				Shape[cnt][4] = normalized_y - 0.1;
				Shape[cnt][6] = normalized_x + 0.17;
				Shape[cnt][7] = normalized_y - 0.1;
			}
			else if (save_draw_check[cnt] == 4) {
				Shape[cnt][0] = normalized_x - 0.15;
				Shape[cnt][1] = normalized_y - 0.15;
				Shape[cnt][3] = Shape[cnt][9] = normalized_x - 0.15;
				Shape[cnt][4] = Shape[cnt][10] = normalized_y + 0.15;
				Shape[cnt][6] = Shape[cnt][12] = normalized_x + 0.15;
				Shape[cnt][7] = Shape[cnt][13] = normalized_y - 0.15;
				Shape[cnt][15] = normalized_x + 0.15;
				Shape[cnt][16] = normalized_y + 0.15;
			}
			cnt++;
			save_draw_check[cnt] = save_draw_check[cnt - 1];
			InitBuffer();
		}
	}
}