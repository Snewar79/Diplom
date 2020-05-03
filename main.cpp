
#include "diff_system.h"
#include "model_parser.h"
#include "image_maker.h"
#include <thread>
#include <eigen3/Eigen/Dense>
#include <string>

using Eigen::MatrixXd;

model_parser parser;

void create_image_loop(int *argc, char** argv)
{
    image_maker img(argc, argv);
    glutMainLoop();
}

void print_info()
{
    std::cout << "loaded model: " << loadedModel << "\n";
    std::cout << "view mode: " << view_mode << "\n";

    if (size_is_ok)
    {
        std::cout << "length of side: " << size_in_meters << "\n";
    }
    else{
        std::cout << "length of side: " << "not set" << "\n";
    }
}

void print_menu(){
    std::cout << "1. Load model\n";
    std::cout << "2. Parse model\n";
    std::cout << "3. View mode\n";
    std::cout << "4. Parsed mode\n";
}

void choose_menu(char c){

    if (c == '1'){
        std::cout << "Enter path to model: ";
        string path;
        std::cin >> path;
        std::cout << "path is: " << path << "\n";

        std::vector<glm::vec3> out_vertices;
        std::vector<glm::vec3> out_faces;
        std::vector<std::vector<glm::vec3>> true_faces;

        bool result = parser.loadOBJ(path.c_str(), out_vertices, out_faces, true_faces);
        faces_data.clear();

        if (result)
        {
            std::cout << "Ok, model is loaded!\nPress enter to continue...";
            loadedModel = path;
            faces_data = true_faces;
            exist_points.clear();
        }
        else
        {
            std::cout << "Fail loading model, check file and try again\n";
            loadedModel = "none";
            exist_points.clear();
        }

        
        getchar();
        getchar();
        return;
    }

    if (c == '2'){
        if (loadedModel == "none"){
            std::cout << "model is not loaded!\nPress enter...";
            getchar();
            getchar();
            return;
        }

        std::cout << "Enter segmentation size: ";
        int side;
        std::cin >> side;

        std::vector<std::vector<std::vector<bool>>> parsed_data = parser.segmentation(faces_data, side);

        exist_points = parsed_data;

        std::cout << "Ok parsing, press enter...";
        getchar();
        getchar();
    }

    if (c == '3')
    {
        glutDisplayFunc(display_model);
    }

    if (c == '4')
    {
        glutDisplayFunc(display_data);
    }
}

int main(int argc, char** argv)
{
    std::thread t1(create_image_loop, &argc, argv);

    system("clear");

    while (1){
        system("clear");
        print_info();
        print_menu();
        char choose = getchar();
        //std::cout << choose << "\n";

        choose_menu(choose);

    }


    t1.join();
    return 0;
}