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

        double max = x_length;
        if (max < y_length)
            max = y_length;
        if (max < z_length)
            max = z_length;

        max *= 2;

        std::cout << "max length = " << max << "\n";

        for (int i = 0; i < true_faces.size(); i++)
        {
            for (int point = 0; point < 3; point++)
            {
                true_faces[i][point][0] /= max;
                true_faces[i][point][1] /= max;
                true_faces[i][point][2] /= max;
            }
        }

        isCross(true_faces, 10);

    }

    glm::vec3 vec_multiplication(glm::vec3 a, glm::vec3 b){
        glm::vec3 coords;
        
        for(int i = 0; i < 3; i++) 
            coords[i]=a[(i+1)%3]*b[(i+2)%3] -a[(i+2)%3]*b[(i+1)%3];

        return coords;
    }

    double scalar_multiplication(glm::vec3 a, glm::vec3 b)
    {
        double sum = 0;
        for (int i = 0; i < 3; i++)
            sum += a[i] * b[i];

        return sum;
    }

    double vec_module(glm::vec3 a)
    {
        return sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    }

    void trinagle_sides(glm::vec3 a, glm::vec3 b, glm::vec3 c, double &ar, double &br, double &cr)
    {
        glm::vec3 ab, bc, ca;

        ab = b - a;
        bc = c - b;
        ca = a - c;

        ar = vec_module(ab);
        br = vec_module(bc);
        cr = vec_module(ca);

    }

    double trinagle_size(glm::vec3 a, glm::vec3 b, glm::vec3 c)
    {
        double as, bs, cs;

        trinagle_sides(a, b, c, as, bs, cs);

        std::cout << "modulse " << " " << as << " " <<  bs << " " <<  cs << "\n";

        double p = (as + bs + cs) / 2;

        return sqrt(  p * (p - as) * (p - bs) * (p - cs));
    }

    bool isCross(std::vector<std::vector<glm::vec3>> true_faces, int segment_on_side)
    {
        glm::vec3 a = {0, 0, 0};
        glm::vec3 b = {10, 10, 0};
        glm::vec3 c = {0, 0, 10};

        glm::vec3 x = {5, 0, 0}, y = {0, 5, 0};

        glm::vec3 normal = vec_multiplication(b - a, c - a);

        std::cout << " Normal is " << normal[0] << " " << normal[1] << " " << normal[2] << "\n";

        glm::vec3 v = a - x;

        double d = scalar_multiplication(normal, v);

        std::cout << "d = " << d << "\n";

        glm::vec3 w = y - x;

        std::cout << " w is " << w[0] << " " << w[1] << " " << w[2] << "\n";

        double e = scalar_multiplication(normal, w);

        std::cout << "e = " << e << "\n";

        glm::vec3 cross_point;

        if (e != 0)
        {
            glm::vec3 temp = w;
            temp *= d;
            temp /= e;
            cross_point = x + temp;
            //cross_point *= d;
            //cross_point /= e;

            std::cout << " cross_point is " << cross_point[0] << " " << cross_point[1] << " " << cross_point[2] << "\n";
            //std::cout << "d = " << d << "\n";

            double abc = trinagle_size(a, b, c);
            double abd = trinagle_size(a, b, cross_point);
            double bcd = trinagle_size(cross_point, b, c);
            double cad = trinagle_size(a, cross_point, c);

            std::cout << "squares = " << abc << " " << abd + bcd + cad << "\n";

        }
    }


};