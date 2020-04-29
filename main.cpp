
#include "diff_system.h"
#include "model_parser.h"
#include "image_maker.h"
#include <thread>

void create_image_loop(int *argc, char** argv)
{
    //std::cout << *argc << "\n";
    image_maker img(argc, argv);
    glutMainLoop();
}

int main(int argc, char** argv)
{
    std::thread t1(create_image_loop, &argc, argv);

    model_parser parser;

    std::vector<glm::vec3> out_vertices;
    std::vector<glm::vec3> out_faces;
    std::vector<std::vector<glm::vec3>> true_faces;

    bool res = parser.loadOBJ("untitled.obj", out_vertices, out_faces, true_faces);
    std::cout << "res = " << res << "\n";

    faces_data = true_faces;

    t1.join();
    return 0;
}