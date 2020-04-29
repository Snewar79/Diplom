#pragma once
#include <glm/glm.hpp>
#include <fstream>
#include <vector>
//g++ main.cpp -o test -lglut -lGLU -lGL

class model_parser{

public:
    model_parser(){}

    bool loadOBJ(
        const char * path,
        std::vector<glm::vec3> & out_vertices,
        std::vector<glm::vec3> & out_faces,
        std::vector<std::vector<glm::vec3>>& true_faces
    ){

        std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
        std::vector< glm::vec3 > temp_vertices;
        std::vector< glm::vec3 > temp_faces;

        FILE * file = fopen(path, "r");
        if( file == NULL ){
            printf("Impossible to open the file !\n");
            return false;
        }

        while( true ){
            char lineHeader[128];

            int res = fscanf(file, "%s", lineHeader);
            if (res == EOF)
                break;

            if ( strcmp( lineHeader, "v" ) == 0 ){
                glm::vec3 vertex;
                fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
                temp_vertices.push_back(vertex);
            }
            else if ( strcmp( lineHeader, "f" ) == 0 ){
                glm::vec3 vertex;
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
                if (matches != 9){
                    printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                    return false;
                }

                vertex.x = vertexIndex[0];
                vertex.y = vertexIndex[1];
                vertex.z = vertexIndex[2];
                temp_faces.push_back(vertex);
            }
            
        }
        
        out_vertices = temp_vertices;
        out_faces = temp_faces;

        for (int i = 0; i < temp_faces.size(); i++)
        {
            std::cout << "process [" << i << "] face\n";
            std::cout << temp_faces[i][0] << " ";
            std::cout << temp_faces[i][1] << " ";
            std::cout << temp_faces[i][2] << "\n";

            std::vector<glm::vec3> temp;
            temp.push_back(out_vertices[temp_faces[i][0] - 1]);
            temp.push_back(out_vertices[temp_faces[i][1] - 1]);
            temp.push_back(out_vertices[temp_faces[i][2] - 1]);

            true_faces.push_back(temp);
        }

        cetrize(true_faces);

        return true;
    }

    void cetrize(std::vector<std::vector<glm::vec3>>& true_faces)
    {
        double max_x, min_x;
        double max_y, min_y;
        double max_z, min_z;

        max_x = true_faces[0][0][0];
        min_x = true_faces[0][0][0];

        max_y = true_faces[0][0][1];
        min_y = true_faces[0][0][1];

        max_z = true_faces[0][0][2];
        min_z = true_faces[0][0][2];

        for (int i = 0; i < true_faces.size(); i++)
        {
            for (int point = 0; point < 3; point++)
            {
                if (true_faces[i][point][0] > max_x)
                    max_x = true_faces[i][point][0];

                if (true_faces[i][point][0] < min_x)
                    min_x = true_faces[i][point][0];

                if (true_faces[i][point][1] > max_y)
                    max_y = true_faces[i][point][1];

                if (true_faces[i][point][1] < min_y)
                    min_y = true_faces[i][point][1];

                if (true_faces[i][point][2] > max_z)
                    max_z = true_faces[i][point][2];

                if (true_faces[i][point][2] < min_z)
                    min_z = true_faces[i][point][2];


            }
        }

        std::cout << "x == " << max_x - min_x << "\n";
        std::cout << "y == " << max_y - min_y << "\n";
        std::cout << "y == " << max_z - min_z << "\n";

        double x_length = (max_x - min_x) / 2;
        double y_length = (max_y - min_y) / 2;
        double z_length = (max_z - min_z) / 2;

        for (int i = 0; i < true_faces.size(); i++)
        {
            for (int point = 0; point < 3; point++)
            {
                true_faces[i][point][0] = true_faces[i][point][0] - (max_x - x_length);
                true_faces[i][point][1] = true_faces[i][point][1] - (max_y - y_length);
                true_faces[i][point][2] = true_faces[i][point][2] - (max_z - z_length);
            }
        }


    }

};