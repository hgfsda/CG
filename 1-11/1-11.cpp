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
void Time_d(int value);
void setting_point(int check_shape, int i);
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
char* filetobuf(const char* file);

GLuint shaderProgramID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체
float r[5], g[5], b[5];
int check_shape[4], save_check_shape[4];   // 현재 도형이 무슨 도형인지 확인 1. 점 / 2. 선 / 3, 삼각형 / 4. 사각형 / 5. 오각형
float move_point[4][2];   // 이동하는 점의 거리
BOOL check_all, check_l, check_t, check_r, check_p, check_d;
// 0 1 직선 / 2 ~ 5 1사분면 / 6 ~ 9 2사분면 / 10 ~ 13 3사분면 / 14 ~17 4사분면
GLfloat Shape[5][36];    // 도형의 좌표
GLfloat colors[5][36];  // 도형 꼭지점의 색상
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

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 2; ++j)
			move_point[i][j] = 0;             // 이동 거리 초기화

	save_check_shape[0] = check_shape[0] = 3;  // 1사분면 삼각형
	Shape[0][0] = 0.5;
	Shape[0][1] = Shape[0][6] = 0.7;
	Shape[0][3] = Shape[0][4] = Shape[0][7] = 0.3;

	save_check_shape[1] = check_shape[1] = 2;  // 2사분면 선
	Shape[1][0] = -0.3;
	Shape[1][1] = 0.7;
	Shape[1][3] = -0.7;
	Shape[1][4] = 0.3;

	save_check_shape[2] = check_shape[2] = 4;  // 3사분면 사각형
	Shape[2][0] = Shape[2][12] = Shape[2][21] = -0.5;
	Shape[2][1] = Shape[2][6] = Shape[2][10] = Shape[2][13] = Shape[2][18] = Shape[2][19] = Shape[2][22] = Shape[2][24] = -0.3;
	Shape[2][3] = Shape[2][4] = Shape[2][7] = Shape[2][9] = Shape[2][15] = Shape[2][16] = Shape[2][25] = -0.7;
	
	save_check_shape[3] = check_shape[3] = 5;  // 4사분면 오각형
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
	//--- 윈도우 생성하기
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
	glutTimerFunc(10, Time_l, 1);
	glutTimerFunc(10, Time_t, 1);
	glutTimerFunc(10, Time_r, 1);
	glutTimerFunc(10, Time_p, 1);
	glutTimerFunc(10, Time_d, 1);
	glutMainLoop();
}

GLvoid drawScene() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position"); //	: 0  Shader의 'layout (location = 0)' 부분
	int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color");

	glBindVertexArray(vao[4]);
	glLineWidth(2);
	glDrawArrays(GL_LINES, 0, 4);

	for (int i = 0; i < 4; ++i) {
		if(check_shape[i] == 1) {
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
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	//-- shader Program
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);
	//--- 세이더 삭제하기
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program 사용하기
	glUseProgram(shaderProgramID);
}

GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수

void make_vertexShaders()
{
	vertexSource = filetobuf("vertex.glsl");
	//--- 버텍스 세이더 객체 만들기
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	//--- 버텍스 세이더 컴파일하기
	glCompileShader(vertexShader);
	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShaders()
{
	fragmentSource = filetobuf("fragment.glsl");
	//--- 프래그먼트 세이더 객체 만들기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	//--- 프래그먼트 세이더 컴파일
	glCompileShader(fragmentShader);
	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

void Time_l(int value) {
	if (check_l == true) {
		for (int i = 0; i < 4; ++i) {
			if (check_shape[i] == 3 && save_check_shape[i] == 2) {
				if (move_point[i][0] < 0.2) {
					move_point[i][0] += 0.01;
					Shape[i][0] -= 0.01;
				}

				if (move_point[i][1] < 0.39) {
					move_point[i][1] += 0.01;
					Shape[i][7] -= 0.01;
				}
				else {
					save_check_shape[i] = 3;
					move_point[i][0] = move_point[i][1] = 0;
				}
			}
		}
		InitBuffer();
		drawScene();
		if (save_check_shape[0] != 2 && save_check_shape[1] != 2 && save_check_shape[2] != 2 && save_check_shape[3] != 2)
			check_l = check_all = false;
	}
	glutTimerFunc(10, Time_l, 1);
}

void Time_t(int value) {
	if (check_t == true) {
		for (int i = 0; i < 4; ++i) {
			if (check_shape[i] == 4 && save_check_shape[i] == 3) {
				if (move_point[i][0] < 0.2) {
					move_point[i][0] += 0.01;
					Shape[i][9] -= 0.01;
					Shape[i][18] += 0.01;
				}
				else {
					save_check_shape[i] = 4;
					move_point[i][0] = move_point[i][1] = 0;
				}
			}
		}
		InitBuffer();
		drawScene();
		if (save_check_shape[0] != 3 && save_check_shape[1] != 3 && save_check_shape[2] != 3 && save_check_shape[3] != 3)
			check_t = check_all = false;
	}
	glutTimerFunc(10, Time_t, 1);
}

void Time_r(int value) {
	if (check_r == true) {
		for (int i = 0; i < 4; ++i) {
			if (check_shape[i] == 5 && save_check_shape[i] == 4) {
				if (move_point[i][0] < 0.1) {
					move_point[i][0] += 0.01;
					Shape[i][3] = Shape[i][15] += 0.01;
					Shape[i][6] = Shape[i][24] -= 0.01;
				}

				if (move_point[i][1] < 0.2) {
					move_point[i][1] += 0.01;
					Shape[i][10] = Shape[i][28] = Shape[i][19] = Shape[i][31] -= 0.01;
					Shape[i][1] = Shape[i][13] = Shape[i][22] -= 0.01;
				}
				else {
					save_check_shape[i] = 5;
					move_point[i][0] = move_point[i][1] = 0;
				}
			}
		}
		InitBuffer();
		drawScene();
		if (save_check_shape[0] != 4 && save_check_shape[1] != 4 && save_check_shape[2] != 4 && save_check_shape[3] != 4)
			check_r = check_all = false;
	}
	glutTimerFunc(10, Time_r, 1);
}

void Time_p(int value) {
	if (check_p == true) {
		for (int i = 0; i < 4; ++i) {
			if (check_shape[i] == 5 && save_check_shape[i] == 1) {
				if (move_point[i][0] < 0.09) {
					move_point[i][0] += 0.01;
					Shape[i][3] = Shape[i][15] += 0.01;
					Shape[i][6] = Shape[i][24] -= 0.01;
				}

				if (move_point[i][1] < 0.19) {
					move_point[i][1] += 0.01;
					Shape[i][34] -= 0.01;
					Shape[i][9] = Shape[i][27] += 0.01;
					Shape[i][18] = Shape[i][30] -= 0.01;
					Shape[i][4] = Shape[i][16] = Shape[i][7] = Shape[i][25] += 0.01;
				}
				else {
					check_shape[i] = 1;
					if (i == 0)
						Shape[0][0] = Shape[0][1] = 0.5;
					else if (i == 1)
						Shape[1][0] = -0.5, Shape[1][1] = 0.5;
					else if (i == 2)
						Shape[2][0] = Shape[2][1] = -0.5;
					else if (i == 3)
						Shape[3][0] = 0.5, Shape[3][1] = -0.5;
					for (int j = 3; j < 36; ++j) {
						Shape[i][j] = 0;
					}
					move_point[i][0] = move_point[i][1] = 0;
				}
			}
		}
		InitBuffer();
		drawScene();
		if (check_shape[0] != 5 && check_shape[1] != 5 && check_shape[2] != 5 && check_shape[3] != 5)
			check_p = check_all = false;
	}
	glutTimerFunc(10, Time_p, 1);
}

void Time_d(int value) {
	if (check_d == true) {
		for (int i = 0; i < 4; ++i) {
			if (check_shape[i] == 2 && save_check_shape[i] == 1) {
				if (move_point[i][0] < 0.2) {
					move_point[i][0] += 0.01;
					Shape[i][0] += 0.01;
					Shape[i][1] += 0.01;
					Shape[i][3] -= 0.01;
					Shape[i][4] -= 0.01;
				}
				else {
					save_check_shape[i] = 2;
					move_point[i][0] = move_point[i][1] = 0;
				}
			}
		}
		InitBuffer();
		drawScene();
		if (save_check_shape[0] != 1 && save_check_shape[1] != 1 && save_check_shape[2] != 1 && save_check_shape[3] != 1)
			check_d = check_all = false;
	}
	glutTimerFunc(10, Time_d, 1);
}

void setting_point(int check_shape, int i) {
	if(check_shape == 1) {
		Shape[i][3] = Shape[i][0];
		Shape[i][4] = Shape[i][1];
	}
	else if (check_shape == 2) {
		Shape[i][6] = Shape[i][0];
		Shape[i][7] = Shape[i][1];
	}
	else if (check_shape == 3) {
		Shape[i][9] = Shape[i][0];
		Shape[i][10] = Shape[i][1];
		Shape[i][12] = Shape[i][0];
		Shape[i][13] = Shape[i][1];
		Shape[i][15] = Shape[i][3];
		Shape[i][16] = Shape[i][4];

		Shape[i][18] = Shape[i][0];
		Shape[i][19] = Shape[i][1];
		Shape[i][21] = Shape[i][0];
		Shape[i][22] = Shape[i][1];
		Shape[i][24] = Shape[i][6];
		Shape[i][25] = Shape[i][7];
	}
	else if (check_shape == 4) {
		Shape[i][27] = Shape[i][9];
		Shape[i][28] = Shape[i][10];
		Shape[i][30] = Shape[i][18];
		Shape[i][31] = Shape[i][19];
		Shape[i][33] = Shape[i][0];
		Shape[i][34] = Shape[i][1];
	}
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'l':   // 선에서 삼각형으로
		if (check_all == false) {
			check_all = true;
			check_l = true;
			for (int i = 0; i < 4; ++i) {
				if (check_shape[i] == 2) {
					setting_point(check_shape[i], i);
					check_shape[i] = 3;
				}
			}
		}
		break;
	case 't':   // 삼각형에서 사각형으로
		if (check_all == false) {
			check_all = true;
			check_t = true;
			for (int i = 0; i < 4; ++i) {
				if (check_shape[i] == 3) {
					setting_point(check_shape[i], i);
					check_shape[i] = 4;
				}
			}
		}
		break;
	case 'r':   // 사각형에서 오각형으로 
		if (check_all == false) {
			check_all = true;
			check_r = true;
			for (int i = 0; i < 4; ++i) {
				if (check_shape[i] == 4) {
					setting_point(check_shape[i], i);
					check_shape[i] = 5;
				}
			}
		}
		break;
	case 'p':   // 오각형에서 점으로
		if (check_all == false) {
			check_all = true;
			check_p = true;
			for (int i = 0; i < 4; ++i) {
				if (check_shape[i] == 5)
					save_check_shape[i] = 1;
			}
		}
		break;
	case 'd':   // 점에서 선으로
		if (check_all == false) {
			check_all = true;
			check_d = true;
			for (int i = 0; i < 4; ++i) {
				if (check_shape[i] == 1) {
					setting_point(check_shape[i], i);
					check_shape[i] = 2;
				}
			}
		}
		break;
	case 'a':   // 모든 도형 변경
		if (check_all == false) {
			check_all = true;
			check_l = check_t = check_r = check_p = check_d = true;
			for (int i = 0; i < 4; ++i) {
				if (check_shape[i] != 5) {
					setting_point(check_shape[i], i);
					check_shape[i] += 1;
				}
				else
					save_check_shape[i] = 1;
			}
		}
		break;
	case 'c':   // 리셋
		reset();
		InitBuffer();
		drawScene();
		break;
	case 'q':   // 프로그램 종료
		std::cout << "프로그램을 종료합니다." << std::endl;
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