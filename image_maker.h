
#include <math.h>
#include <GL/glut.h>


void display();
void specialKeys();

double rotate_y = 0; 
double rotate_x = 0;

std::vector<std::vector<glm::vec3>> faces_data;

void draw_faces_data()
{
    for (int i = 0; i < faces_data.size(); i++)
    {
        //std::cout << "lets pring grain " << i << "\n";
        //std::cout << faces_data[i][0][0] << faces_data[i][0][1] <<  faces_data[i][0][2] << "\n";
        glBegin(GL_TRIANGLES);
        glColor3f( 1.0, 0.0, 0.0 );     glVertex3f(  faces_data[i][0][0], faces_data[i][0][1], faces_data[i][0][2] );      // P1 is red
        glColor3f( 0.0, 1.0, 0.0 );     glVertex3f(  faces_data[i][1][0], faces_data[i][1][1], faces_data[i][1][2] );      // P2 is green
        glColor3f( 0.0, 0.0, 1.0 );     glVertex3f(  faces_data[i][2][0], faces_data[i][2][1], faces_data[i][2][2] );      // P3 is blue 
        glEnd();
    }

}

void display(){
    //  Clear screen and Z-buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();

    glRotatef( rotate_x, 1.0, 0.0, 0.0 );
    glRotatef( rotate_y, 0.0, 1.0, 0.0 );

    glScalef( 0.2, 0.2, 0.2 );          // Not included


    draw_faces_data();

    glFlush();
    glutSwapBuffers();
}

// ----------------------------------------------------------
// specialKeys() Callback Function
// ----------------------------------------------------------
void specialKeys( int key, int x, int y ) {
 
  //  Right arrow - increase rotation by 5 degree
  if (key == GLUT_KEY_RIGHT)
    rotate_y += 5;
 
  //  Left arrow - decrease rotation by 5 degree
  else if (key == GLUT_KEY_LEFT)
    rotate_y -= 5;
 
  else if (key == GLUT_KEY_UP)
    rotate_x += 5;
 
  else if (key == GLUT_KEY_DOWN)
    rotate_x -= 5;
 
  //  Request display update
  glutPostRedisplay();
 
}

class image_maker{

public:
    image_maker(int * argc, char** argv){
        glutInit(argc,argv);
 
        //  Request double buffered true color window with Z-buffer
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
        
        // Create window
        glutCreateWindow("Awesome Cube");

        //  Enable Z-buffer depth test
        glEnable(GL_DEPTH_TEST);

        // Callback functions
        glutDisplayFunc(display);
        glutSpecialFunc(specialKeys);

    }
};
