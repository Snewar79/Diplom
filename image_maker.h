
#include <math.h>
#include <GL/glut.h>


void display();
void specialKeys();

double rotate_y = 0; 
double rotate_x = 0;

std::vector<std::vector<glm::vec3>> faces_data;
std::vector<std::vector<glm::vec3>> inside_data;

void draw_box()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_POLYGON);

    glColor4f( 1.0, 1.0, 1.0, 0.5 );     glVertex3f(  0.5, -0.5, -0.5 );      // P1 красная
    glVertex3f(  0.5,  0.5, -0.5 );      // P2 зеленая
    glVertex3f( -0.5,  0.5, -0.5 );      // P3 синяя
    glVertex3f( -0.5, -0.5, -0.5 );      // P4 фиолетовая

    glEnd();

        // Белая сторона — ЗАДНЯЯ
    glBegin(GL_POLYGON);
    glVertex3f(  0.5, -0.5, 0.5 );
    glVertex3f(  0.5,  0.5, 0.5 );
    glVertex3f( -0.5,  0.5, 0.5 );
    glVertex3f( -0.5, -0.5, 0.5 );
    glEnd();

    // Фиолетовая сторона — ПРАВАЯ
    glBegin(GL_POLYGON);
    glVertex3f( 0.5, -0.5, -0.5 );
    glVertex3f( 0.5,  0.5, -0.5 );
    glVertex3f( 0.5,  0.5,  0.5 );
    glVertex3f( 0.5, -0.5,  0.5 );
    glEnd();

    // Зеленая сторона — ЛЕВАЯ
    glBegin(GL_POLYGON);
    glVertex3f( -0.5, -0.5,  0.5 );
    glVertex3f( -0.5,  0.5,  0.5 );
    glVertex3f( -0.5,  0.5, -0.5 );
    glVertex3f( -0.5, -0.5, -0.5 );
    glEnd();

    // Синяя сторона — ВЕРХНЯЯ
    glBegin(GL_POLYGON);
    glVertex3f(  0.5,  0.5,  0.5 );
    glVertex3f(  0.5,  0.5, -0.5 );
    glVertex3f( -0.5,  0.5, -0.5 );
    glVertex3f( -0.5,  0.5,  0.5 );
    glEnd();

    // Красная сторона — НИЖНЯЯ
    glBegin(GL_POLYGON);
    glVertex3f(  0.5, -0.5, -0.5 );
    glVertex3f(  0.5, -0.5,  0.5 );
    glVertex3f( -0.5, -0.5,  0.5 );
    glVertex3f( -0.5, -0.5, -0.5 );
    glEnd();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void draw_faces_data()
{
    /*
    for (int i = 0; i < faces_data.size(); i++)
    {
        //std::cout << "lets pring grain " << i << "\n";
        //std::cout << faces_data[i][0][0] << faces_data[i][0][1] <<  faces_data[i][0][2] << "\n";
        glBegin(GL_TRIANGLES);
        glColor4f( 0.5, 0.5, 0.5, 1 );     
            glVertex3f(  faces_data[i][0][0], faces_data[i][0][1], faces_data[i][0][2] );      // P1 is red
            glVertex3f(  faces_data[i][1][0], faces_data[i][1][1], faces_data[i][1][2] );      // P2 is green
            glVertex3f(  faces_data[i][2][0], faces_data[i][2][1], faces_data[i][2][2] );      // P3 is blue 
        glEnd();
    }

           glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

               for (int i = 0; i < faces_data.size(); i++)
    {
        //std::cout << "lets pring grain " << i << "\n";
        //std::cout << faces_data[i][0][0] << faces_data[i][0][1] <<  faces_data[i][0][2] << "\n";
        glBegin(GL_TRIANGLES);
        glColor3f( 0.1, 0.1, 0.1 );     
            glVertex3f(  faces_data[i][0][0], faces_data[i][0][1], faces_data[i][0][2] );      // P1 is red
            glVertex3f(  faces_data[i][1][0], faces_data[i][1][1], faces_data[i][1][2] );      // P2 is green
            glVertex3f(  faces_data[i][2][0], faces_data[i][2][1], faces_data[i][2][2] );      // P3 is blue 
        glEnd();
    }
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    */
    for (int i = 0; i < inside_data.size(); i++)
    {
        //std::cout << "lets prin grain inside" << i << "\n";
        //std::cout << faces_data[i][0][0] << faces_data[i][0][1] <<  faces_data[i][0][2] << "\n";
        glBegin(GL_TRIANGLES);
        glColor3f( 1, 0, 0 );     
            glVertex3f(  inside_data[i][0][0], inside_data[i][0][1], inside_data[i][0][2] );      // P1 is red
            glVertex3f(  inside_data[i][1][0], inside_data[i][1][1], inside_data[i][1][2] );      // P2 is green
            glVertex3f(  inside_data[i][2][0], inside_data[i][2][1], inside_data[i][2][2] );      // P3 is blue 
        glEnd();
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    for (int i = 0; i < inside_data.size(); i++)
    {
        //std::cout << "lets prin grain inside" << i << "\n";
        //std::cout << faces_data[i][0][0] << faces_data[i][0][1] <<  faces_data[i][0][2] << "\n";
        glBegin(GL_TRIANGLES);
        glColor3f( 0.1, 0.1, 0.1 );     
            glVertex3f(  inside_data[i][0][0], inside_data[i][0][1], inside_data[i][0][2] );      // P1 is red
            glVertex3f(  inside_data[i][1][0], inside_data[i][1][1], inside_data[i][1][2] );      // P2 is green
            glVertex3f(  inside_data[i][2][0], inside_data[i][2][1], inside_data[i][2][2] );      // P3 is blue 
        glEnd();
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void display(){
    //  Clear screen and Z-buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();

    glRotatef( rotate_x, 1.0, 0.0, 0.0 );
    glRotatef( rotate_y, 0.0, 1.0, 0.0 );

    //glScalef( 0.0, 0.2, 0.2 );          // Not included
    draw_faces_data();
    draw_box();

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

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glLineWidth(2.0f);

    }
};
