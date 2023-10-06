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
void Time_l(int value);
void Time_t(int value);
void Time_r(int value);
void Time_p(int value);
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
char* filetobuf(const char* file);

GLuint shaderProgramID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü
float r[18], g[18], b[18];
int check_shape[4], save_check_shape[4];   // ���� ������ ���� �������� Ȯ�� 1. �� / 2. �� / 3, �ﰢ�� / 4. �簢�� / 5. ������
BOOL check_all, check_l, check_t, check_r, check_p, check_d;
// 0 1 ���� / 2 ~ 5 1��и� / 6 ~ 9 2��и� / 10 ~ 13 3��и� / 14 ~17 4��и�
GLfloat Shape[5][36];    // ������ ��ǥ
GLfloat colors[5][36];  // ���� �������� ����
GLuint vao[5], vbo[10];

void reset() {
	check_all = check_l = check_t = check_r = check_p = check_d = false;
	for (int j = 0; j < 36; ++j) {
		Shape[4][j] = 0;
		colors[4][j] = 0;
	}
	Shape[4][0] = Shape[4][7] = 1.0;
	Shape[4][3] = Shape[4][10] = -1.0;

	for (int i = 0; i < 4; ++i) {
		r[i] = (rand() % 101) * 0.01, g[i] = (rand() % 101) * 0.01, b[i] = (rand() % 101) * 0.01;
		for (int j = 0; j < 36; ++j) {
			Shape[i][j] = 0;
			if (j % 3 == 0) colors[i][j] = r[i];
			else if (j % 3 == 1) colors[i][j] = g[i];
			else if (j % 3 == 2) colors[i][j] = b[i];
		}
	}
	save_check_shape[0] = check_shape[0] = 3;  // 1��и� �ﰢ��
	Shape[0][0] = Shape[0][1] = Shape[0][7] = 0.3;
	Shape[0][3] = 0.5;
	Shape[0][4] = Shape[0][6] = 0.7;

	save_check_shape[1] = check_shape[1] = 2;  // 2��и� ��
	Shape[1][0] = -0.7;
	Shape[1][1] = 0.3;
	Shape[1][3] = -0.3;
	Shape[1][4] = 0.7;

	save_check_shape[2] = check_shape[2] = 4;  // 3��и� �簢��
	Shape[2][0] = Shape[2][12] = Shape[2][21] = -0.5;
	Shape[2][1] = Shape[2][6] = Shape[2][10] = Shape[2][13] = Shape[2][18] = Shape[2][19] = Shape[2][22] = Shape[2][24] = -0.3;
	Shape[2][3] = Shape[2][4] = Shape[2][7] = Shape[2][9] = Shape[2][15] = Shape[2][16] = Shape[2][25] = -0.7;
	
	save_check_shape[3] = check_shape[3] = 5;  // 4��и� ������
	Shape[3][0] = Shape[3][12] = Shape[3][21] = Shape[3][33] = 0.5;
	Shape[3][1] = Shape[3][10] = Shape[3][13] = Shape[3][19] = Shape[3][22] = Shape[3][28] = Shape[3][31] = -0.5;
	Shape[3][3] = Shape[3][15] = 0.4;
	Shape[3][4] = Shape[3][7] = Shape[3][16] = Shape[3][25] = -0.7;
	Shape[3][6] = Shape[3][24] = 0.6;
	Shape[3][9] = Shape[3][27] = 0.3;
	Shape[3][18] = Shape[3][30] = 0.7;
	Shape[3][34] = -0.3;
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
	//glutTimerFunc(10, Time_l, 1);
	//glutTimerFunc(10, Time_t, 1);
	//glutTimerFunc(10, Time_r, 1);
	//glutTimerFunc(10, Time_p, 1);
	//glutTimerFunc(10, Time_d, 1);
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

	glBindVertexArray(vao[4]);
	glLineWidth(2);
	glDrawArrays(GL_LINES, 0, 4);

	for (int i = 0; i < 4; ++i) {
		if(check_shape[i] == 1) {
			glBindVertexArray(vao[i]);
			glPointSize(3);
			glDrawArrays(GL_POINTS, 0, 1);
		}
		else if (check_shape[i] == 2) {
			glBindVertexArray(vao[i]);
			glLineWidth(3);
			glDrawArrays(GL_LINES, 0, 2);
		}
		else if (check_shape[i] == 3) {
			glBindVertexArray(vao[i]);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		else if (check_shape[i] == 4) {
			glBindVertexArray(vao[i]);
			glDrawArrays(GL_TRIANGLES, 0, 9);
		}
		else if (check_shape[i] == 5) {
			glBindVertexArray(vao[i]);
			glDrawArrays(GL_TRIANGLES, 0, 12);
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
	glGenVertexArrays(5, vao);
	glGenBuffers(10, vbo);

	for (int i = 0; i < 5; ++i) {
		glBindVertexArray(vao[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i * 2]);
		glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(GLfloat), Shape[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[i * 2 + 1]);
		glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(GLfloat), colors[i], GL_STATIC_DRAW);
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

void Time_l(int value) {

	glutTimerFunc(10, Time_l, 1);
}

void Time_t(int value) {

	glutTimerFunc(10, Time_t, 1);
}

void Time_r(int value) {

	glutTimerFunc(10, Time_r, 1);
}

void Time_p(int value) {

	glutTimerFunc(10, Time_p, 1);
}

void Time_d(int value) {

	glutTimerFunc(10, Time_d, 1);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'l':   // ������ �ﰢ������
		if (check_all == false) {
			check_all = true;
			check_l = true;
			printf("l\n");
			for (int i = 0; i < 4; ++i) {
				if (check_shape[i] == 2)
					check_shape[i] = 3;
			}
		}
		break;
	case 't':   // �ﰢ������ �簢������
		if (check_all == false) {
			check_all = true;
			check_t = true;
			printf("t\n");
			for (int i = 0; i < 4; ++i) {
				if (check_shape[i] == 3)
					check_shape[i] = 4;
			}
		}
		break;
	case 'r':   // �簢������ ���������� 
		if (check_all == false) {
			check_all = true;
			check_r = true;
			printf("r\n");
			for (int i = 0; i < 4; ++i) {
				if (check_shape[i] == 4)
					check_shape[i] = 5;
			}
		}
		break;
	case 'p':   // ���������� ������
		if (check_all == false) {
			check_all = true;
			check_p = true;
			printf("p\n");
			for (int i = 0; i < 4; ++i) {
				if (check_shape[i] == 5)
					save_check_shape[i] = 1;
			}
		}
		break;
	case 'd':   // ������ ������
		if (check_all == false) {
			check_all = true;
			check_d = true;
			printf("d\n");
			for (int i = 0; i < 4; ++i) {
				if (check_shape[i] == 1)
					check_shape[i] = 2;
			}
		}
		break;
	case 'a':   // ��� ���� ����
		if (check_all == false) {
			check_all = true;
			printf("a\n");
			check_l = check_t = check_r = check_p = check_d = true;
			for (int i = 0; i < 4; ++i) {
				if (check_shape[i] != 5)
					check_shape[i] += 1;
				else
					save_check_shape[i] = 1;
			}
		}
		break;
	case 'c':   // ����
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