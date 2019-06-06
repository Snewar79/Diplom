
#include <string>
#include <iostream>
#include <vector>
#include <math.h>

using namespace std;



vector<double> gauss(vector<vector<double>> a, vector<double> y, int n) 
{
  vector<double> x;
  int  max;
  int k, index;
  const double eps = 0.00001;  // точность

    for (int i = 0; i < n; i++)
        x.push_back(0);
  
  
  k = 0;
  while (k < n) 
  {
    // Поиск строки с максимальным a[i][k]
    max = abs(a[k][k]);
    index = k;
    for (int i = k + 1; i < n; i++) 
    {
      if (abs(a[i][k]) > max)
      {
        max = abs(a[i][k]);
        index = i;
      }
    }
    // Перестановка строк
    if (max < eps) 
    {
      // нет ненулевых диагональных элементов
      cout << "Решение получить невозможно из-за нулевого столбца ";
      cout << index << " матрицы A" << endl;
      throw - 1;
    }
    for (int j = 0; j < n; j++) 
    {
      double temp = a[k][j];
      a[k][j] = a[index][j];
      a[index][j] = temp;
    }
    double temp = y[k];
    y[k] = y[index];
    y[index] = temp;
    // Нормализация уравнений
    for (int i = k; i < n; i++) 
    {
      double temp = a[i][k];
      if (abs(temp) < eps) continue; // для нулевого коэффициента пропустить
      for (int j = 0; j < n; j++) 
        a[i][j] = a[i][j] / temp;
      y[i] = y[i] / temp;
      if (i == k)  continue; // уравнение не вычитать само из себя
      for (int j = 0; j < n; j++)
        a[i][j] = a[i][j] - a[k][j];
      y[i] = y[i] - y[k];
    }
    k++;
  }
  // обратная подстановка
  for (k = n - 1; k >= 0; k--)
  {
    x[k] = y[k];
    for (int i = 0; i < k; i++)
      y[i] = y[i] - a[i][k] * x[k];
  }
  return x;
}




int main()
{

    cout << "All is ok!\n";

    double length = 8;
    double h = 0.25;
    double tau = 0.25;
    double st_sum = 0;

    vector<double> fi;

    int block_count = length / h;

    for (int i = 0; i < block_count; i++)
        fi.push_back(i * 3.5432);

    for (int i = 0; i < block_count; i++)
      st_sum += fi[i];

    for (int i = 0; i < block_count; i++)
        cout << fi[i] << " ";
    cout << "\n\n";

    vector<vector<double>> matrix;

    vector<double> tmp;


    for (int i = 0; i < block_count; i++)
        tmp.push_back(0);

    for (int i = 0; i < block_count; i++)
        matrix.push_back(tmp);

    for (int i = 0; i < block_count; i++)
        matrix[i][i] = (h * h + 2 * tau) / (h * h);

    for (int i = 0; i < block_count - 1; i++)
    {
        matrix[i][i + 1] =  -1.0 * tau / (h * h);
        matrix[i + 1][i] =  -1.0 * tau / (h * h);
    }

    matrix[0][1] = (-1 * tau) / (h * h + tau);
    matrix[block_count - 1][block_count - 2] = (-1 * tau) / (h * h + tau);


    matrix[0][0] = (h * h + 2 * tau) / (h * h + tau);
    matrix[block_count - 1][block_count - 1] = (h * h + 2 * tau) / (h * h + tau);


    for (int i = 0; i < block_count; i++)
    {
        for (int j = 0; j < block_count; j++)
            cout << matrix[i][j] << " ";
        cout << "\n";
    }
    cout << "\n";
    


   vector<double> result = fi;

    for (int i = 0; i < 10000; i++)
    {
        result = gauss(matrix, result, block_count);

        


    }

    for (int j = 0; j < block_count; j++)
            cout << result[j] << " ";
        cout << "\n\n";

    double end_sum = 0;

    for (int j = 0; j < block_count; j++)
      end_sum += result[j];



      cout << "End = " << end_sum << " start = " << st_sum << "\n";

    return 0;
}







