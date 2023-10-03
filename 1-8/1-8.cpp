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
char* filetobuf(const char* file);

GLuint shaderProgramID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü
BOOL shape_mode = true;
float r[6], g[6], b[6];
GLfloat Shape[6][9];    // ������ ��ǥ
GLfloat colors[6][9];  // ���� �������� ����
GLuint vao[6], vbo[12];

void reset() {
	shape_mode = true;
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 9; ++j) {
			Shape[i][j] = 0;
			colors[i][j] = 0;
		}
	}
	Shape[0][0] = Shape[1][4] = 1.0;
	Shape[0][3] = Shape[1][1] = -1.0;

	for (int i = 2; i < 6; ++i) {
		r[i] = (rand() % 101) * 0.01, g[i] = (rand() % 101) * 0.01, b[i] = (rand() % 101) * 0.01;
		for (int j = 0; j < 9; ++j) {
			Shape[i][j] = 0;
			if (j % 3 == 0) colors[i][j] = r[i];
			else if (j % 3 == 1) colors[i][j] = g[i];
			else if (j % 3 == 2) colors[i][j] = b[i];
		}
	}
	// 1��и�
	Shape[2][0] = 0.5;
	Shape[2][1] = 0.5 + 0.2;
	Shape[2][3] = 0.5 - 0.1;
	Shape[2][4] = 0.5 - 0.1;
	Shape[2][6] = 0.5 + 0.1;
	Shape[2][7] = 0.5 - 0.1;
	// 2��и�
	Shape[3][0] = -0.5;
	Shape[3][1] = 0.5 + 0.2;
	Shape[3][3] = -0.5 - 0.1;
	Shape[3][4] = 0.5 - 0.1;
	Shape[3][6] = -0.5 + 0.1;
	Shape[3][7] = 0.5 - 0.1;
	// 3��и�
	Shape[4][0] = -0.5;
	Shape[4][1] = -0.5 + 0.2;
	Shape[4][3] = -0.5 - 0.1;
	Shape[4][4] = -0.5 - 0.1;
	Shape[4][6] = -0.5 + 0.1;
	Shape[4][7] = -0.5 - 0.1;
	// 4��и�
	Shape[5][0] = 0.5;
	Shape[5][1] = -0.5 + 0.2;
	Shape[5][3] = 0.5 - 0.1;
	Shape[5][4] = -0.5 - 0.1;
	Shape[5][6] = 0.5 + 0.1;
	Shape[5][7] = -0.5 - 0.1;
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

	for (int i = 0; i < 6; ++i) {
		glBindVertexArray(vao[i]);
		if (i < 2) {
			glLineWidth(2);
			glDrawArrays(GL_LINES, 0, 2);
		}
		else
			if(shape_mode == true)
				glDrawArrays(GL_TRIANGLES, 0, 3);
			else
				glDrawArrays(GL_LINE_LOOP, 0, 3);
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
	glGenVertexArrays(6, vao);
	glGenBuffers(12, vbo);

	for (int i = 0; i < 6; ++i) {
		glBindVertexArray(vao[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i * 2]);
		glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), Shape[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[i * 2 + 1]);
		glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors[i], GL_STATIC_DRAW);
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
	case 'a':    //  ���� ������ �׸���
		shape_mode = true;
		drawScene();
		break;
	case 'b':    // ���� ������ �׸���
		shape_mode = false;
		drawScene();
		break;
	case 'r':   // ��� ���� ����
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
		if (normalized_x >= 0 && normalized_y >= 0) {  // 1��и��϶�
			Shape[2][0] = normalized_x;
			Shape[2][1] = normalized_y + 0.2;
			Shape[2][3] = normalized_x - 0.1;
			Shape[2][4] = normalized_y - 0.1;
			Shape[2][6] = normalized_x + 0.1;
			Shape[2][7] = normalized_y - 0.1;
		}
		else if (normalized_x < 0 && normalized_y >= 0) {   // 2��и��϶�
			Shape[3][0] = normalized_x;
			Shape[3][1] = normalized_y + 0.2;
			Shape[3][3] = normalized_x - 0.1;
			Shape[3][4] = normalized_y - 0.1;
			Shape[3][6] = normalized_x + 0.1;
			Shape[3][7] = normalized_y - 0.1;
		}
		else if (normalized_x < 0 && normalized_y < 0) {   // 3��и��϶�
			Shape[4][0] = normalized_x;
			Shape[4][1] = normalized_y + 0.2;
			Shape[4][3] = normalized_x - 0.1;
			Shape[4][4] = normalized_y - 0.1;
			Shape[4][6] = normalized_x + 0.1;
			Shape[4][7] = normalized_y - 0.1;
		}
		else if (normalized_x >= 0 && normalized_y < 0) {   // 4��и��϶�
			Shape[5][0] = normalized_x;
			Shape[5][1] = normalized_y + 0.2;
			Shape[5][3] = normalized_x - 0.1;
			Shape[5][4] = normalized_y - 0.1;
			Shape[5][6] = normalized_x + 0.1;
			Shape[5][7] = normalized_y - 0.1;
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
	fptr = fopen(file, "rb"); // Open file for reading 
	if (!fptr) // Return NULL on failure 
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file 
	length = ftell(fptr); // Find out how many bytes into the file we are 
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator 
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file 
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer 
	fclose(fptr); // Close the file 
	buf[length] = 0; // Null terminator 
	return buf; // Return the buffer 
}