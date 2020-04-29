#include <GL/glut.h>
#include <math.h>
#include <iostream>
 
// инициализация переменных цвета в 1.0
// треугольник - белый
float red=1.0f, blue=1.0f, green=1.0f;
 
float postion[] = {0, 0, 0};
float offset[] = {0, 0, 5};
float angely = 0;
float angelx = 3.14 / 2;
float look[] = {0, 0, 0};

// угол поворота
float angle = 0.0f;
 
void changeSize(int w, int h) {
	// предотвращение деления на ноль
	if (h == 0)
		h = 1;
	float ratio =  w * 1.0 / h;
	// используем матрицу проекции
	glMatrixMode(GL_PROJECTION);
	// обнуляем матрицу
	glLoadIdentity();
	// установить параметры вьюпорта
	glViewport(0, 0, w, h);
	// установить корректную перспективу
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	// вернуться к матрице проекции
	glMatrixMode(GL_MODELVIEW);
}
 
void renderScene(void) {
 
	// очистить буфер цвета и глубины.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// обнулить трансформацию
	glLoadIdentity();
	// установить камеру
	gluLookAt( postion[0] + offset[0], postion[1] + offset[1], postion[2] + offset[2],
		   look[0] + offset[0], look[1] + offset[1],  look[2] + offset[2],
		   0.0f, 1.0f,  0.0f);
	//поворот на заданную величину
	//glRotatef(angle, 0.0f, 1.0f, 0.0f);
	// установить цвет модели
	glColor3f(red, green, blue);
	glBegin(GL_TRIANGLES);
		glColor3d(1, 0.0, 0.0); 
		glVertex3f(1,-1.0f, 0.0f);
		glVertex3f( 1.0f, 1.0f, 0.0);
		glVertex3f( 0.0f,0.0f, 1.0);


		glColor3d(0.0, 1.0, 0.0); 
		glVertex3f( 1.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);
		glVertex3f( 0.0f, 0.0f, 1.0);

		glColor3d(0.0, 0.0, 1.0); 
		glVertex3f( -1.0f, 1.0f, 0.0f);
		glVertex3f( -1.0f, -1.0f, 0.0f);
		glVertex3f(  0.0f, 0.0f, 1.0f);


		glColor3d(0.5, 0.5, 0.5); 
		glVertex3f( -1.0f, -1.0f, 0.0f);
		glVertex3f(  1.0f,  -1.0f,  0.0f);
		glVertex3f(  0.0f, 0.0f, 1.0f);
	glEnd();
 
    

    std::cout << angelx << " " << angely << "\n";

	glutSwapBuffers();
}
 
void processNormalKeys(unsigned char key, int x, int y) {
 
	if (key == 27)
		exit(0);

    if (key == 'a')
    {
        float perp[] = {0, 0};

        float x = sin(angely) * 0.1;
        float y = cos(angely) * 0.1;
        float tmp = x;
        x = y;
        y = tmp;
        x *= -1;

        offset[0] += x;
        offset[1] -= 0;
        offset[2] += y;
    }

    if (key == 'd')
    {
        float perp[] = {0, 0};

        float x = sin(angely) * 0.1;
        float y = cos(angely) * 0.1;
        float tmp = x;
        x = y;
        y = tmp;
        x *= -1;

        offset[0] -= x;
        offset[1] -= 0;
        offset[2] -= y;
    }

    if (key == 'w')
    {
        //angelx -= 0.1;
        offset[0] -= postion[0];
        offset[1] -= postion[1];
        offset[2] -= postion[2];
    }

    if (key == 's')
    {
        offset[0] += postion[0];
        offset[1] += postion[1];
        offset[2] += postion[2];
    }

    postion[0] = 0.1 * sin(angelx) * sin(angely);
    postion[2] = 0.1 * sin(angelx) * cos(angely);
    postion[1] = 0.1 * cos(angelx);
}
 
void processSpecialKeys(int key, int x, int y) {
 
	switch(key) {
		case GLUT_KEY_F1 :
				red = 1.0;
				green = 0.0;
				blue = 0.0; break;
		case GLUT_KEY_F2 :
				red = 0.0;
				green = 1.0;
				blue = 0.0; break;
		case GLUT_KEY_F3 :
				red = 0.0;
				green = 0.0;
				blue = 1.0; break;
	}

    if (key == GLUT_KEY_RIGHT)
    {
        angely -= 0.1;
    }

    if (key == GLUT_KEY_LEFT)
    {
        angely += 0.1;
    }

    if (key == GLUT_KEY_UP)
    {
        angelx -= 0.1;
    }

    if (key == GLUT_KEY_DOWN)
    {
        angelx += 0.1;
    }

    postion[0] = 0.1 * sin(angelx) * sin(angely);
    postion[2] = 0.1 * sin(angelx) * cos(angely);
    postion[1] = 0.1 * cos(angelx);
}
 
int main(int argc, char **argv) {
 
	// инициализация
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(400,400);
	glutCreateWindow("Урок 4");
 
	// регистрация
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
 
	// наши новые функции
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
 
	// основной цикл
	glutMainLoop();
 
	return 1;
}