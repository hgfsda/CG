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
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
char* filetobuf(const char* file);

GLuint shaderProgramID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체
float r, g, b;
float sx[5], sy[5];
int check_num = 1;        // 몇개의 스파이럴을 그릴지
int check_point_line = 0;   // 점인지 선인지 구분
int check_move;    // 현재 점이 움직이는지     0.안그려지는중 / 1. 커지는중 / 2. 작아지는중
int angle, cnt, point_cnt;
float save_x[5], save_y[5];
GLfloat Shape[5][702];    // 도형의 좌표
GLfloat colors[5][702];  // 도형 꼭지점의 색상
GLuint vao[5], vbo[10];

void reset() {
	r = (rand() % 101) * 0.01;
	g = (rand() % 101) * 0.01;
	b = (rand() % 101) * 0.01;
	angle = cnt = point_cnt = 0;
	check_move = 0;   // 현재 안그려지는 중
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 702; ++j) {
			Shape[i][j] = 0;
			colors[i][j] = 1.0;
		}
	}
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
	glutTimerFunc(10, Time1, 1);
	glutMainLoop();
}

GLvoid drawScene() {
	glClearColor(r, g, b, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position"); //	: 0  Shader의 'layout (location = 0)' 부분
	int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color");

	for (int i = 0; i < check_num; ++i) {
		glBindVertexArray(vao[i]);
		if (check_point_line == 0) {
			glPointSize(3);
			glDrawArrays(GL_POINTS, 0, point_cnt);
		}
		else {
			glLineWidth(3);
			glDrawArrays(GL_LINE_STRIP, 0, point_cnt);
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
		glBufferData(GL_ARRAY_BUFFER, 702 * sizeof(GLfloat), Shape[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[i * 2 + 1]);
		glBufferData(GL_ARRAY_BUFFER, 702 * sizeof(GLfloat), colors[i], GL_STATIC_DRAW);
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

void Time1(int value) {
	float radian = angle * 3.14 / 180.0;
	if (check_move == 1) {
		for (int i = 0; i < check_num; ++i) {
			sx[i] += radian * cos(radian) * 0.002;
			sy[i] += radian * sin(radian) * 0.002;

			Shape[i][cnt] = sx[i];
			Shape[i][cnt+1] = sy[i];
		}
		if (angle < 1170) {
			cnt += 3;
			angle += 10;
			++point_cnt;
		}
		else {
			check_move = 2;  // 0.2222 정도 차이 그럼 0.4444 만큼 이동
			for (int i = 0; i < 5; ++i) {
				sx[i] = save_x[i] + 0.2222;
				sy[i] = save_y[i] - 0.01;
			}
		}
		InitBuffer();
		drawScene();
	}
	else if (check_move == 2) {
		for (int i = 0; i < check_num; ++i) {
			sx[i] += radian * cos(radian) * 0.002;
			sy[i] += radian * sin(radian) * 0.002;

			Shape[i][cnt] = sx[i];
			Shape[i][cnt + 1] = sy[i];
		}
		if (angle > 0) {
			cnt += 3;
			angle -= 10;
			++point_cnt;
		}
		InitBuffer();
		drawScene();
	}
	glutTimerFunc(10, Time1, 1);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '1':   // 입력한 숫자만큼 스파이럴이 그려진다.
		check_num = 1;
		break;
	case '2':
		check_num = 2;
		break;
	case '3':
		check_num = 3;
		break;
	case '4':
		check_num = 4;
		break;
	case '5':
		check_num = 5;
		break;
	case 'p':    // 점으로 그린다.
		check_point_line = 0;
		InitBuffer();
		drawScene();
		break;
	case 'l':    // 선으로 그린다.
		check_point_line = 1;
		InitBuffer();
		drawScene();
		break;
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
	reset();
	check_move = 1;
	sx[0] = normalized_x;
	sy[0] = normalized_y;
	for (int i = 1; i < 5; ++i) {
		sx[i] = (rand() % 201 - 100) * 0.01;
		sy[i] = (rand() % 201 - 100) * 0.01;
	}
	for (int i = 0; i < 5; ++i) {
		save_x[i] = sx[i];
		save_y[i] = sy[i];
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