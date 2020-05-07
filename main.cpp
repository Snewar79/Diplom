
#include "diff_system.h"
#include "model_parser.h"
#include "image_maker.h"
#include <thread>
#include <eigen3/Eigen/Dense>
#include <string>

using Eigen::MatrixXd;

model_parser parser;

double gloabl_time = 0;

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

    std::cout << "parametrs: a = 0.32, h = 0.01, tau = 0.1\n";
    std::cout << "time = " << gloabl_time << "\n";
}

void print_menu(){
    std::cout << "1. Load model\n";
    std::cout << "2. Parse model\n";
    std::cout << "3. View mode\n";
    std::cout << "4. Parsed mode\n";
    std::cout << "5. Save temp\n";
    std::cout << "6. Set parametrs\n";
    std::cout << "7. Check temp\n";
    std::cout << "8. Set type of point\n";
    std::cout << "9. Do step\n";
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
        glutPostRedisplay();
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
        solve_system = diff_system(side);
        solve_system.set_exist(exist_points);

        std::cout << "Ok parsing, press enter...";
        getchar();
        glutPostRedisplay();
        getchar();
    }

    if (c == '3')
    {
        glutDisplayFunc(display_model);
        glutPostRedisplay();
        view_mode = "model view";
    }

    if (c == '4')
    {
        glutDisplayFunc(display_data);
        glutPostRedisplay();
        view_mode = "parsed model";
    }

    if (c == '5')
    {
        std::cout << "Enter temperature: ";
        double temp;
        std::cin >> temp;
        solve_system.set_temp_all(temp);
        glutPostRedisplay();
    }
    

    if (c == '9')
    {
        solve_system.forward_step();
        glutPostRedisplay();
        gloabl_time = gloabl_time + 0.1;
    }

    if (c == '7'){
        int x, y, z;
        std::cin >> x >> y >> z;
        std::cout << "temp is: " << temp_data[x][y][z];
        getchar();
        getchar();
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