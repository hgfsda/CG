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
void setting_point(int check_shape, int i);
void Time(int value);
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
char* filetobuf(const char* file);

GLuint shaderProgramID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü
float r[15], g[15], b[15];
int check_shape[15];     // ������ ������ Ȯ��   0. ���� / 1. �� / 2. ���� / 3. �ﰢ�� / 4. �簢�� / 5. ������ / 6. ������
int move_check[15];      // ������ �����̴� ���� Ȯ��  0. �ȿ����� / 1. �밢�� / 2. �������
int check_num;           // ���� ���õ� ����
int dir[15];
float sx[15], sy[15];    // ������ ��ǥ ����
BOOL left_button;
GLfloat Shape[15][36];    // ������ ��ǥ
GLfloat colors[15][36];  // ���� �������� ����
GLuint vao[15], vbo[30];

void reset() {
	int set_cnt_shape = 0;
	left_button = false;
	check_num = 15;
	for (int i = 0; i < 15; ++i) {
		if (i % 3 == 0)
			++set_cnt_shape;
		check_shape[i] = set_cnt_shape;
		move_check[i] = dir[i] = 0;
		sx[i] = (rand() % 201 - 100) * 0.01;
		sy[i] = (rand() % 201 - 100) * 0.01;

		r[i] = (rand() % 101) * 0.01, g[i] = (rand() % 101) * 0.01, b[i] = (rand() % 101) * 0.01;
		for (int j = 0; j < 36; ++j) {
			Shape[i][j] = 0;
			if (j % 3 == 0) colors[i][j] = r[i];
			else if (j % 3 == 1) colors[i][j] = g[i];
			else if (j % 3 == 2) colors[i][j] = b[i];
		}
		setting_point(check_shape[i], i);
	} 
}

void setting_point(int check_shape, int i) {
	if (check_shape == 1) {
		Shape[i][0] = sx[i];
		Shape[i][1] = sy[i];
	}
	else if (check_shape == 2) {
		Shape[i][0] = sx[i] + 0.1;
		Shape[i][1] = sy[i] + 0.1;
		Shape[i][3] = sx[i] - 0.1;
		Shape[i][4] = sy[i] - 0.1;
	}
	else if (check_shape == 3) {
		Shape[i][0] = sx[i];
		Shape[i][1] = sy[i] + 0.1;
		Shape[i][3] = sx[i] - 0.1;
		Shape[i][4] = sy[i] - 0.1;
		Shape[i][6] = sx[i] + 0.1;
		Shape[i][7] = sy[i] - 0.1;
	}
	else if (check_shape == 4) {
		Shape[i][0] = sx[i] - 0.1;
		Shape[i][1] = sy[i] + 0.1;
		Shape[i][3] = Shape[i][12] = sx[i] - 0.1;
		Shape[i][4] = Shape[i][13] = sy[i] - 0.1;
		Shape[i][6] = Shape[i][15] = sx[i] + 0.1;
		Shape[i][7] = Shape[i][16] = sy[i] + 0.1;
		Shape[i][9] = sx[i] + 0.1;
		Shape[i][10] = sy[i] - 0.1;
	}
	else if (check_shape == 5) {
		Shape[i][0] = Shape[i][15] = Shape[i][24] = sx[i];
		Shape[i][1] = Shape[i][16] = Shape[i][25] = sy[i] + 0.1;
		Shape[i][3] = Shape[i][12] = sx[i] - 0.05;
		Shape[i][4] = Shape[i][13] = sy[i] - 0.1;
		Shape[i][6] = Shape[i][21] = sx[i] + 0.05;
		Shape[i][7] = Shape[i][22] = sy[i] - 0.1;
		Shape[i][9] = sx[i] - 0.1;
		Shape[i][18] = sx[i] + 0.1;
		Shape[i][10] = Shape[i][19] = sy[i];
	}
	else if (check_shape == 6) {
		Shape[i][0] = Shape[i][9] = Shape[i][18] = Shape[i][27] = Shape[i][15] = Shape[i][21] = sx[i];
		Shape[i][1] = Shape[i][10] = Shape[i][19] = Shape[i][28] = sy[i] + 0.1;
		Shape[i][3] = Shape[i][6] = Shape[i][12] = sx[i] - 0.1;
		Shape[i][4] = Shape[i][13] = Shape[i][25] = Shape[i][31] = sy[i] - 0.05;
		Shape[i][7] = Shape[i][34] = sy[i] + 0.05;
		Shape[i][16] = Shape[i][22] = sy[i] - 0.1;
		Shape[i][24] = Shape[i][30] = Shape[i][33] = sx[i] + 0.1;
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
	glutMotionFunc(Motion);
	glutTimerFunc(10, Time, 1);
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

	for (int i = 0; i < 15; ++i) {
		if (check_shape[i] == 1) {
			glBindVertexArray(vao[i]);
			glPointSize(5);
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
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		else if (check_shape[i] == 5) {
			glBindVertexArray(vao[i]);
			glDrawArrays(GL_TRIANGLES, 0, 9);
		}
		else if (check_shape[i] == 6) {
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
	glGenVertexArrays(15, vao);
	glGenBuffers(30, vbo);

	for (int i = 0; i < 15; ++i) {
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

void change_shape(int check_num, int i) {
	check_shape[check_num] += check_shape[i];
	if (check_shape[check_num] > 6)
		check_shape[check_num] -= 6;
	check_shape[i] = 0;
	r[check_num] = (r[check_num] + r[i]) / 2;
	g[check_num] = (g[check_num] + g[i]) / 2;
	b[check_num] = (b[check_num] + b[i]) / 2;
	for (int j = 0; j < 36; ++j) {
		if (j % 3 == 0) colors[check_num][j] = r[i];
		else if (j % 3 == 1) colors[check_num][j] = g[i];
		else if (j % 3 == 2) colors[check_num][j] = b[i];
	}
}

void Time(int value) {
	for (int i = 0; i < 15; ++i) {
		if (move_check[i] == 1) {
			if (dir[i] == 0) {  // ������ ���� �̵�
				sx[i] = sx[i] + 0.01;
				sy[i] = sy[i] + 0.01;
				if (sx[i] + 0.1 >= 1.0)
					dir[i] = 1;
				if (sy[i] + 0.1 >= 1.0)
					dir[i] = 3;
			}
			else if (dir[i] == 1) {  // ���� ���� �̵�
				sx[i] = sx[i] - 0.01;
				sy[i] = sy[i] + 0.01;
				if (sx[i] - 0.1 <= -1.0)
					dir[i] = 0;
				if (sy[i] + 0.1 >= 1.0)
					dir[i] = 2;
			}
			else if (dir[i] == 2) {  // ���� �Ʒ��� �̵�
				sx[i] = sx[i] - 0.01;
				sy[i] = sy[i] - 0.01;
				if (sx[i] - 0.1 <= -1.0)
					dir[i] = 3;
				if (sy[i] - 0.1 <= -1.0)
					dir[i] = 1;
			}
			else if (dir[i] == 3) {  // ������ �Ʒ��� �̵�
				sx[i] = sx[i] + 0.01;
				sy[i] = sy[i] - 0.01;
				if (sx[i] + 0.1 >= 1.0)
					dir[i] = 2;
				if (sy[i] - 0.1 <= -1.0)
					dir[i] = 0;
			}
			setting_point(check_shape[i], i);
			InitBuffer();
			drawScene();
		}
		else if (move_check[i] == 2) {

		}
	}
	glutTimerFunc(10, Time, 1);
}

void Mouse(int button, int state, int x, int y) {
	float normalized_x, normalized_y;

	normalized_x = (2.0 * x / 800) - 1.0;
	normalized_y = 1.0 - (2.0 * y / 600);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		for (int i = 14; i >= 0; --i) {
			if (check_shape[i] != 0) {
				if (normalized_x > sx[i] - 0.1 && normalized_x < sx[i] + 0.1 && normalized_y > sy[i] - 0.1 && normalized_y < sy[i] + 0.1) {

					left_button = true;
					check_num = i;
					if (move_check[i] == 1 || move_check[i] == 2)
						move_check[i] = 0;
					return;
				}
				else
					check_num = 15;
			}
		}
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		for (int i = 14; i >= 0; --i) {
			if (i != check_num && check_shape[i] != 0) {
				if (normalized_x > sx[i] - 0.1 && normalized_x < sx[i] + 0.1 && normalized_y > sy[i] - 0.1 && normalized_y < sy[i] + 0.1) {
					change_shape(check_num, i);
					setting_point(check_shape[check_num], check_num);
					move_check[check_num] = 1;
					break;
				}
			}
		}
		InitBuffer();
		drawScene();
	}
}

void Motion(int x, int y) {
	float normalized_x, normalized_y;

	normalized_x = (2.0 * x / 800) - 1.0;
	normalized_y = 1.0 - (2.0 * y / 600);
	if (left_button == true) {
		sx[check_num] = normalized_x;
		sy[check_num] = normalized_y;
	}
	setting_point(check_shape[check_num], check_num);
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