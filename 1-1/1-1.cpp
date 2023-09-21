#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void TimerFunction(int value);
int random_check = 0;

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
	glutCreateWindow("Example"); // 윈도우 생성 (윈도우 이름)

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {    // glew 초기화
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(Keyboard);
	glutMainLoop(); // 이벤트 처리 시작 
}

float r = 1.0f;
float g = 1.0f;
float b = 1.0f;

GLvoid drawScene() { //--- 콜백 함수: 그리기 콜백 함수 
	glClearColor(r, g, b, 1.0f); // 바탕색을 ‘blue’ 로 지정
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다.
	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) { //--- 콜백 함수: 다시 그리기 콜백 함수
	glViewport(0, 0, w, h);
}

void TimerFunction(int value) {
	srand(time(NULL));
	int rand_r = rand() % 100, rand_g = rand() % 100, rand_b = rand() % 100;
	r = 0.01f * rand_r, g = 0.01f * rand_g, b = 0.01f * rand_b;
	glutPostRedisplay(); // 화면 재 출력
	if (random_check == 1)
		glutTimerFunc(200, TimerFunction, 1); // 타이머함수 재 설정
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	srand(time(NULL));
	int rand_r = rand() % 100, rand_g = rand() % 100, rand_b = rand() % 100;
	switch (key) {
	case 'c':    // 청록색
		r = 0.0f, g = 0.3f, b = 0.4f;
		break;
	case 'm':    // 자홍색
		r = 1.0f, g = 0.0f, b = 1.0f;
		break;
	case 'y':    // 노랑색
		r = 1.0f, g = 1.0f, b = 0.0f;
		break;
	case 'a':    // 랜덤색
		r = 0.01f * rand_r, g = 0.01f * rand_g, b = 0.01f * rand_b;
		break;
	case 'w':    // 하얀색
		r = 1.0f, g = 1.0f, b = 1.0f;
		break;
	case 'k':    // 검정색
		r = 0.0f, g = 0.0f, b = 0.0f;
		break;
	case 't':    // 타이머를 설정하여 특정 시간마다 랜덤색으로 변경
		random_check = 1;
		glutTimerFunc(200, TimerFunction, 1); 
		break;
	case 's':    // 타이머 종료
		random_check = 0;
		break;
	case 'q':    // 프로그램 종료
		std::cout << "프로그램을 종료합니다." << std::endl;
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}