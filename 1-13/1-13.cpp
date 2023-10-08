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

GLuint shaderProgramID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체
int quadrant;
BOOL left_button;
GLfloat Shape[2][12];    // 도형의 좌표
GLfloat colors[2][12];  // 도형 꼭지점의 색상
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
	int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position"); //	: 0  Shader의 'layout (location = 0)' 부분
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

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'r':   // 리셋
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