
#include "diff_system.h"
#include "model_parser.h"
#include "image_maker.h"
#include <thread>
#include <eigen3/Eigen/Dense>

using Eigen::MatrixXd;

void create_image_loop(int *argc, char** argv)
{
    //std::cout << *argc << "\n";
    image_maker img(argc, argv);
    glutMainLoop();
}

double module_vector(glm::vec3 a)
{
    return sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
}

double trinagle_size(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
    double a_side = module_vector(b - a);
    double b_side = module_vector(c - b);
    double c_side = module_vector(a - c);

    //std::cout << a_side << "\n";

    double p = (a_side + c_side + b_side) / 2;

    return sqrt(p * (p - a_side) * (p - b_side) * (p - c_side));
}

bool isCross(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 x, glm::vec3 r)
{
    a = a - x;
    b = b - x;
    c = c - x; 
    double p12 = b[0] - a[0];
    double p22 = b[1] - a[1];
    double p32 = b[2] - a[2];

    double p13 = c[0] - a[0];
    double p23 = c[1] - a[1];
    double p33 = c[2] - a[2];

    double ac, bc, cc, dc;
    ac = p22 * p33 - p23 * p32;
    bc = p13 * p32 - p12 * p33;
    cc = p12 * p23 - p13 * p22;
    dc =    a[0] * (p23 * p32 - p22 * p33) +
            a[1] * (p12 * p33 - p13 * p32) +
            a[2] * (p13 * p22 - p12 * p23);

    //std::cout << "dc == " << dc << "\n";

    double determ = (ac * r[0] + bc * r[1] + cc * r[2]);

    if (abs(determ) < 0.000001)
    {
        return false;
    }

    //std::cout << "determ == " << determ << "\n";

    double t = - dc / (ac * r[0] + bc * r[1] + cc * r[2]);

    //std::cout << "t  == " << t << "\n";

    if (t < 0)
        return false;

    glm::vec3 point = r;

    point *= t;

    point = point + x;
    a += x;
    b += x;
    c += x;

    //std::cout << "cross_point = " << point[0] << " " << point[1] << " " << point[2] << "\n";

    double abc = trinagle_size(a, b, c);
    double abd = trinagle_size(a, b, point);
    double bcd = trinagle_size(point, b, c);
    double acd = trinagle_size(a, point, c);

    //std::cout << " s1 = " << abc << " s2 = " << abd + bcd + acd << "\n";

    if (abs(abc - (abd + bcd + acd)) < 0.0001){
        //std:: cout << "return true\n";
        return true;
    }

    return false;
}

bool check_point(glm::vec3 point, std::vector<std::vector<glm::vec3>> true_faces){

    int inside = 0;
    int outside = 0;

    glm::vec3 r;
    for (double x = -0.5; x < 0.5; x += 0.2){
        for (double y = -0.5; y < 0.5; y += 0.2){
            for (double z = -0.5; z < 0.5; z += 0.2){
                r = glm::vec3({x, y, z});
                    int cross_count = 0;
                    for (int i = 0; i < true_faces.size(); i++){
                        if (isCross(true_faces[i][0], true_faces[i][1], true_faces[i][2], point, r))
                        {
                            cross_count += 1;
                        }
                    }

                    if (cross_count % 2 == 1)
                    {
                        inside += 1;
                    }
                    else
                    {
                        outside += 1;
                    }

            }
        }
    }

    //std::cout << "inside = " << inside << " outside = " << outside << "\n";

    if ( inside > outside)
    {
        return true;
    }

    return false;
}

void generate_cube(std::vector<std::vector<glm::vec3>> &faces, glm::vec3 point)
{

    glm::vec3 p1 = {0, 0, 0};
    glm::vec3 p2 = {0, 0.05, 0};
    glm::vec3 p3 = {0.05, 0.05, 0};
    glm::vec3 p4 = {0.05, 0.05, 0};

    glm::vec3 p5 = {0.05, 0, 0.05};
    glm::vec3 p6 = {0, 0, 0.05};
    glm::vec3 p7 = {0, 0.05, 0.05};
    glm::vec3 p8 = {0.05, 0.05, 0.05};

    p1 += point;
    p2 += point;
    p3 += point;
    p4 += point;
    p5 += point;
    p6 += point;
    p7 += point;
    p8 += point;

    faces.push_back(std::vector<glm::vec3>{p1, p6, p5});
    faces.push_back(std::vector<glm::vec3>{p1, p4, p5});

    faces.push_back(std::vector<glm::vec3>{p1, p3, p2});
    faces.push_back(std::vector<glm::vec3>{p1, p3, p4});

    faces.push_back(std::vector<glm::vec3>{p8, p7, p2});
    faces.push_back(std::vector<glm::vec3>{p3, p8, p2});

    faces.push_back(std::vector<glm::vec3>{p5, p6, p8});
    faces.push_back(std::vector<glm::vec3>{p6, p8, p7});

    faces.push_back(std::vector<glm::vec3>{p4, p3, p8});
    faces.push_back(std::vector<glm::vec3>{p8, p4, p5});

    faces.push_back(std::vector<glm::vec3>{p1, p2, p7});
    faces.push_back(std::vector<glm::vec3>{p1, p7, p6});
}

int main(int argc, char** argv)
{

    MatrixXd m(2,2);
    m(0,0) = 3;
    m(1,0) = 2.5;
    m(0,1) = -1;
    m(1,1) = m(1,0) + m(0,1);
    std::cout << m << std::endl;
    /*std::thread t1(create_image_loop, &argc, argv);

    model_parser parser;

    std::vector<glm::vec3> out_vertices;
    std::vector<glm::vec3> out_faces;
    std::vector<std::vector<glm::vec3>> true_faces;

    bool res = parser.loadOBJ("untitled.obj", out_vertices, out_faces, true_faces);
    std::cout << "res = " << res << "\n";

    faces_data = true_faces;


    t1.join();
    */

   diff_system system(10);

    system.forward_step();
    system.forward_step();
    system.forward_step();
    system.forward_step();
    system.forward_step();
    system.forward_step();
    system.forward_step();
    system.forward_step();
    system.forward_step();
    system.forward_step();
    system.forward_step();
    system.forward_step();
    system.forward_step();
    system.forward_step();
    system.forward_step();
    system.forward_step();
    system.forward_step();
    system.forward_step();
    system.forward_step();
    system.forward_step();
    system.forward_step();
    system.forward_step();
    system.forward_step();
    system.forward_step();
    system.forward_step();
    system.forward_step();
    system.forward_step();
    system.forward_step();
    system.forward_step();
    system.forward_step();
    system.forward_step();
    system.forward_step();


    return 0;
}