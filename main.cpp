
#include "diff_system.h"
#include "model_parser.h"
#include <GL/freeglut.h>

double drand()
{
    return (double)(rand() % 100) / 99;
}

void displayMe(void)
{

    std::vector< glm::vec3 > vertices;
    std::vector< glm::vec3 > faces;
    model_parser parser;

    bool res = parser.loadOBJ("cube.obj", vertices, faces);
    std::cout << "res = " << res << " size = " << faces.size() <<"\n";
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < faces.size(); i++){
       glBegin(GL_TRIANGLES);
        glColor3d(drand(), drand(), drand());      // рисуем треугольник
        glVertex3d(vertices[faces[i].x].x, vertices[faces[i].x].y, vertices[faces[i].x].z);
        glVertex3d(vertices[faces[i].y].x, vertices[faces[i].y].y, vertices[faces[i].y].z);
        glVertex3d(vertices[faces[i].z].x, vertices[faces[i].z].y, vertices[faces[i].z].z);
       glEnd();
    }

    glFlush();
}

int main(int argc, char** argv)
{
    srand(0);

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(400, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hello world!");

    glMatrixMode(GL_PROJECTION);
    glEnable(GL_DEPTH_TEST);

    gluPerspective( /* field of view in degree */ 40.0,
    /* aspect ratio */ 1.0,
    /* Z near */ 1.0, /* Z far */ 10.0);

    glMatrixMode(GL_MODELVIEW);

    gluLookAt(2.0, 3.0, 5.0,  /* eye is at (0,0,5) */
    0.0, 0.0, 0.0,      /* center is at (0,0,0) */
    0.0, 1.0, 0.0);      /* up is in positive Y direction */

    glutDisplayFunc(displayMe);
    glutMainLoop();

    return 0;
}