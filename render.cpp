
/* Copyright (c) Mark J. Kilgard, 1997. */

/* This program is freely distributable without licensing fees 
   and is provided without guarantee or warrantee expressed or 
   implied. This program is -not- in the public domain. */

/* This program was requested by Patrick Earl; hopefully someone else
   will write the equivalent Direct3D immediate mode program. */

#include <GL/glut.h>
#include "model_parser.h"
#include <vector>
#include <iostream>

double drand()
{
    return (double)(rand() % 100) / 99;
}

GLfloat light_diffuse[] = {1.0, 0.0, 0.0, 1.0};  /* Red diffuse light. */
GLfloat light_position[] = {3.0, 3.0, 3.0, 0.0};  /* Infinite light location. */

void
drawBox(void)
{
    std::vector< glm::vec3 > vertices;
    std::vector< glm::vec3 > faces;
    model_parser parser;

    bool res = parser.loadOBJ("cube.obj", vertices, faces);
    std::cout << "res = " << res << " size = " << faces.size() <<"\n";
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < faces.size(); i++){
       glBegin(GL_TRIANGLES);
        glColor3d(0.5, 1, 1);      // рисуем треугольник
        glVertex3d(vertices[faces[i].x].x, vertices[faces[i].x].y, vertices[faces[i].x].z);
        glVertex3d(vertices[faces[i].y].x, vertices[faces[i].y].y, vertices[faces[i].y].z);
        glVertex3d(vertices[faces[i].z].x, vertices[faces[i].z].y, vertices[faces[i].z].z);
       glEnd();
    }
}

void
display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  drawBox();
  glutSwapBuffers();
}

void
init(void)
{
  /* Setup cube vertex data. */

  /* Enable a single OpenGL light. */
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);

  /* Use depth buffering for hidden surface elimination. */
  glEnable(GL_DEPTH_TEST);

  /* Setup the view of the cube. */
  glMatrixMode(GL_PROJECTION);
  gluPerspective( /* field of view in degree */ 40.0,
    /* aspect ratio */ 1.0,
    /* Z near */ 1.0, /* Z far */ 10.0);
  glMatrixMode(GL_MODELVIEW);
  gluLookAt(0.0, 0.0, 5.0,  /* eye is at (0,0,5) */
    0.0, 0.0, 0.0,      /* center is at (0,0,0) */
    0.0, 1.0, 0.);      /* up is in positive Y direction */

  /* Adjust cube position to be asthetic angle. */
  glTranslatef(0.0, 0.0, -1.0);
  glRotatef(60, 1.0, 0.0, 0.0);
  glRotatef(-20, 0.0, 0.0, 1.0);
}

int
main(int argc, char **argv)
{
  srand(0);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("red 3D lighted cube");
  glutDisplayFunc(display);
  init();
  glutMainLoop();

  return 0;             /* ANSI C requires main to return int. */
}

