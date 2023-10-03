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
void Time1(int value);
void Time2(int value);
void Time3(int value);
void Time4(int value);
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void dir_check(int dir_check[]);
char* filetobuf(const char* file);

GLuint shaderProgramID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü
float r[4], g[4], b[4];
float sx[4], sy[4];
int dir_shape[4];
int check1 = 0, check2 = 0, check3 = 0, check4 = 0;
int save_location_cnt[4];   // ������� �̵����� �� �Ʒ��� �����̴� �Լ�
GLfloat Shape[4][9];    // ������ ��ǥ
GLfloat colors[4][9];  // ���� �������� ����
GLuint vao[4], vbo[8];

void reset() {
	check1 = check2 = check3 = check4 = 0;
	for (int i = 0; i < 4; ++i) {
		dir_shape[i] = 3;
		save_location_cnt[i] = 0;
		r[i] = (rand() % 101) * 0.01, g[i] = (rand() % 101) * 0.01, b[i] = (rand() % 101) * 0.01;
		for (int j = 0; j < 9; ++j) {
			Shape[i][j] = 0;
			if (j % 3 == 0) colors[i][j] = r[i];
			else if (j % 3 == 1) colors[i][j] = g[i];
			else if (j % 3 == 2) colors[i][j] = b[i];
		}
	}
	sx[0] = sx[3] = sy[0] = sy[1] = 0.5;
	sx[1] = sx[2] = sy[2] = sy[3] = -0.5;

	for (int i = 0; i < 4; ++i) {
		Shape[i][0] = sx[i];
		Shape[i][1] = sy[i] + 0.2;
		Shape[i][3] = sx[i] - 0.1;
		Shape[i][4] = sy[i] - 0.1;
		Shape[i][6] = sx[i] + 0.1;
		Shape[i][7] = sy[i] - 0.1;
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
	glutTimerFunc(10, Time1, 1);
	glutTimerFunc(10, Time2, 1);
	glutTimerFunc(10, Time3, 1);
	glutTimerFunc(10, Time4, 1);
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

	for (int i = 0; i < 4; ++i) {
		glBindVertexArray(vao[i]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
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
	glGenVertexArrays(4, vao);
	glGenBuffers(8, vbo);

	for (int i = 0; i < 4; ++i) {
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

void dir_check(int dir_shape, int i) {
	if (dir_shape == 0) {     // ������ ����
		Shape[i][0] = sx[i] + 0.2;
		Shape[i][1] = sy[i];
		Shape[i][3] = sx[i] - 0.1;
		Shape[i][4] = sy[i] + 0.1;
		Shape[i][6] = sx[i] - 0.1;
		Shape[i][7] = sy[i] - 0.1;
	}
	else if (dir_shape == 1) {   //  ���� ����
		Shape[i][0] = sx[i] - 0.2;
		Shape[i][1] = sy[i];
		Shape[i][3] = sx[i] + 0.1;
		Shape[i][4] = sy[i] + 0.1;
		Shape[i][6] = sx[i] + 0.1;
		Shape[i][7] = sy[i] - 0.1;
	}
	else if (dir_shape == 2) {   //  �Ʒ� ����
		Shape[i][0] = sx[i];
		Shape[i][1] = sy[i] - 0.2;
		Shape[i][3] = sx[i] - 0.1;
		Shape[i][4] = sy[i] + 0.1;
		Shape[i][6] = sx[i] + 0.1;
		Shape[i][7] = sy[i] + 0.1;
	}
	else if (dir_shape == 3) {   //  �� ����
		Shape[i][0] = sx[i];
		Shape[i][1] = sy[i] + 0.2;
		Shape[i][3] = sx[i] - 0.1;
		Shape[i][4] = sy[i] - 0.1;
		Shape[i][6] = sx[i] + 0.1;
		Shape[i][7] = sy[i] - 0.1;
	}
}
int dir[4];
void Time1(int value) {
	if (check1 == 1) {
		for (int i = 0; i < 4; ++i) {
			if (dir[i] == 0) {  // ������ ���� �̵�
				sx[i] = sx[i] + 0.01;
				sy[i] = sy[i] + 0.01;
				if (sx[i] + 0.1 >= 1.0) {
					dir[i] = 1;
					dir_shape[i] = 1;
				}
				if (sy[i] + 0.1 >= 1.0) {
					dir[i] = 3;
					dir_shape[i] = 2;
				}
			}
			else if (dir[i] == 1) {  // ���� ���� �̵�
				sx[i] = sx[i] - 0.01;
				sy[i] = sy[i] + 0.01;
				if (sx[i] - 0.1 <= -1.0) {
					dir[i] = 0;
					dir_shape[i] = 0;
				}
				if (sy[i] + 0.1 >= 1.0) {
					dir[i] = 2;
					dir_shape[i] = 2;
				}
			}
			else if (dir[i] == 2) {  // ���� �Ʒ��� �̵�
				sx[i] = sx[i] - 0.01;
				sy[i] = sy[i] - 0.01;
				if (sx[i] - 0.1 <= -1.0) {
					dir[i] = 3;
					dir_shape[i] = 0;
				}
				if (sy[i] - 0.1 <= -1.0) {
					dir[i] = 1;
					dir_shape[i] = 3;
				}
			}
			else if (dir[i] == 3) {  // ������ �Ʒ��� �̵�
				sx[i] = sx[i] + 0.01;
				sy[i] = sy[i] - 0.01;
				if (sx[i] + 0.1 >= 1.0) {
					dir[i] = 2;
					dir_shape[i] = 1;
				}
				if (sy[i] - 0.1 <= -1.0) {
					dir[i] = 0;
					dir_shape[i] = 3;
				}
			}
			dir_check(dir_shape[i], i);
		}
		InitBuffer();
		drawScene();
	}		
	glutTimerFunc(10, Time1, 1);
}

void Time2(int value) {
	if (check2 == 1) {
		for (int i = 0; i < 4; ++i) {
			if (dir[i] == 0) {  // ������ �̵� �� �Ʒ��� �̵�
				if (sx[i] + 0.1 < 1.0) {
					dir_shape[i] = 0;
					sx[i] = sx[i] + 0.01;
				}
				else {
					sy[i] = sy[i] - 0.01;
					++save_location_cnt[i];
					dir_shape[i] = 1;
					if (save_location_cnt[i] == 20) {
						save_location_cnt[i] = 0;
						dir[i] = 1;
					}
					if (sy[i] - 0.1 <= -1.0)
						dir[i] = 3;
				}
			}
			else if (dir[i] == 1) {  // ���� �̵� �� �Ʒ��� �̵�
				if (sx[i] - 0.1 > -1.0) {
					sx[i] = sx[i] - 0.01;
					dir_shape[i] = 1;
				}
				else {
					sy[i] = sy[i] - 0.01;
					++save_location_cnt[i];
					dir_shape[i] = 0;
					if (save_location_cnt[i] == 20) {
						save_location_cnt[i] = 0;
						dir[i] = 0;
					}
					if (sy[i] - 0.1 <= -1.0)
						dir[i] = 2;
				}
			}
			else if (dir[i] == 2) {  // ������ �̵� �� ���� �̵�
				if (sx[i] + 0.1 < 1.0) {
					sx[i] = sx[i] + 0.01;
					dir_shape[i] = 0;
				}
				else {
					sy[i] = sy[i] + 0.01;
					++save_location_cnt[i];
					dir_shape[i] = 1;
					if (save_location_cnt[i] == 20) {
						save_location_cnt[i] = 0;
						dir[i] = 3;
					}
					if (sy[i] + 0.1 >= 1.0)
						dir[i] = 1;
				}
			}
			else if (dir[i] == 3) {  // ���� �̵� �� ���� �̵�
				if (sx[i] - 0.1 > -1.0) {
					sx[i] = sx[i] - 0.01;
					dir_shape[i] = 1;
				}
				else {
					sy[i] = sy[i] + 0.01;
					++save_location_cnt[i];
					dir_shape[i] = 0;
					if (save_location_cnt[i] == 20) {
						save_location_cnt[i] = 0;
						dir[i] = 2;
					}
					if (sy[i] + 0.1 >= 1.0)
						dir[i] = 0;
				}
			}
			dir_check(dir_shape[i], i);
		}
		InitBuffer();
		drawScene();
	}
	glutTimerFunc(10, Time2, 1);
}

void Time3(int value) {
}

void Time4(int value) {
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '1':    //  ƨ���
		if (check1 == 0) {
			for (int i = 0; i < 4; ++i)
				dir[i] = rand() % 4;
			check1 = 1;
			check2 = check3 = check4 = 0;
		}
		else
			check1 = 0;
		break;
	case '2':    //  �¿�� �������
		if (check2 == 0) {
			for (int i = 0; i < 4; ++i)
				dir[i] = rand() % 4;
			check2 = 1;
			check1 = check3 = check4 = 0;
		}
		else
			check2 = 0;
		break;
	case '3':    //  �簢 �����̷�
		if (check3 == 0) {
			check3 = 1;
			check1 = check2 = check4 = 0;
		}
		else
			check3 = 0;
		break;
	case '4':    //  �� �����̷�
		if (check4 == 0) {
			check4 = 1;
			check1 = check3 = check2 = 0;
		}
		else
			check4 = 0;
		break;
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