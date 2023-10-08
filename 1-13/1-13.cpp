#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

void make_vertexShaders();
void make_fragmentShaders();
void make_shaderProgram();
void InitBuffer();
void reset();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
char* filetobuf(const char* file);

GLuint shaderProgramID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü
int quadrant;
BOOL left_button;
GLfloat Shape[2][12];    // ������ ��ǥ
GLfloat colors[2][12];  // ���� �������� ����
GLuint vao[2], vbo[4];

void reset() {
	quadrant = 0;
	left_button = false;
	for (int i = 0; i < 12; ++i) {
		Shape[0][i] = Shape[1][i] = 0;
		colors[0][i] = 0;
		colors[1][i] = rand() % 101 * 0.01;
	}
	Shape[0][0] = Shape[0][7] = 1.0;
	Shape[0][3] = Shape[0][10] = -1.0;
	Shape[1][0] = Shape[1][9] = Shape[1][1] = Shape[1][4] = 0.5;
	Shape[1][3] = Shape[1][6] = Shape[1][7] = Shape[1][10] = -0.5;
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
	glutMotionFunc(Motion);
	glutMainLoop();
}

GLvoid drawScene() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position"); //	: 0  Shader�� 'layout (location = 0)' �κ�
	int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color");

	glBindVertexArray(vao[0]);
	glLineWidth(2);
	glDrawArrays(GL_LINES, 0, 4);

	glBindVertexArray(vao[1]);
	glLineWidth(3);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	
	glEnableVertexAttribArray(PosLocation);
	glEnableVertexAttribArray(ColorLocation);

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

void InitBuffer()
{
	glGenVertexArrays(2, vao);
	glGenBuffers(4, vbo);

	for (int i = 0; i < 2; ++i) {
		glBindVertexArray(vao[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i * 2]);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), Shape[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[i * 2 + 1]);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), colors[i], GL_STATIC_DRAW);
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

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'r':   // ����
		reset();
		InitBuffer();
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

	normalized_x = (2.0 * x / 800) - 1.0;
	normalized_y = 1.0 - (2.0 * y / 600);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (normalized_x > Shape[1][0] - 0.05 && normalized_x < Shape[1][0] + 0.05 && normalized_y > Shape[1][1] - 0.05 && normalized_y < Shape[1][1] + 0.05) {
			quadrant = 1;
			left_button = true;
		}
		else if (normalized_x > Shape[1][3] - 0.05 && normalized_x < Shape[1][3] + 0.05 && normalized_y > Shape[1][4] - 0.05 && normalized_y < Shape[1][4] + 0.05) {
			quadrant = 2;
			left_button = true;
		}
		else if (normalized_x > Shape[1][6] - 0.05 && normalized_x < Shape[1][6] + 0.05 && normalized_y > Shape[1][7] - 0.05 && normalized_y < Shape[1][7] + 0.05) {
			quadrant = 3;
			left_button = true;
		}
		else if (normalized_x > Shape[1][9] - 0.05 && normalized_x < Shape[1][9] + 0.05 && normalized_y > Shape[1][10] - 0.05 && normalized_y < Shape[1][10] + 0.05) {
			quadrant = 4;
			left_button = true;
		}
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		quadrant = 0;
	}
}

void Motion(int x, int y) {
	float normalized_x, normalized_y;

	normalized_x = (2.0 * x / 800) - 1.0;
	normalized_y = 1.0 - (2.0 * y / 600);
	if (left_button == true) {
		if (quadrant == 1) {
			if (normalized_x > 0 && normalized_y > 0) {
				Shape[1][0] = normalized_x;
				Shape[1][1] = normalized_y;
			}
			else {
				left_button = false;
				quadrant = 0;
			}
		}
		else if (quadrant == 2) {
			if (normalized_x < 0 && normalized_y > 0) {
				Shape[1][3] = normalized_x;
				Shape[1][4] = normalized_y;
			}
			else {
				left_button = false;
				quadrant = 0;
			}
		}
		else if (quadrant == 3) {
			if (normalized_x < 0 && normalized_y < 0) {
				Shape[1][6] = normalized_x;
				Shape[1][7] = normalized_y;
			}
			else {
				left_button = false;
				quadrant = 0;
			}
		}
		else if (quadrant == 4) {
			if (normalized_x > 0 && normalized_y < 0) {
				Shape[1][9] = normalized_x;
				Shape[1][10] = normalized_y;
			}
			else {
				left_button = false;
				quadrant = 0;
			}
		}
	}
	InitBuffer();
	drawScene();
}


char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb");
	if (!fptr)
		return NULL;
	fseek(fptr, 0, SEEK_END);
	length = ftell(fptr);
	buf = (char*)malloc(length + 1);
	fseek(fptr, 0, SEEK_SET);
	fread(buf, length, 1, fptr);
	fclose(fptr);
	buf[length] = 0;
	return buf;
}