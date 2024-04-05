#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

// Функция для создания прямоугольной щели
std::vector<std::vector<double>> create_slit(size_t width, size_t start_index, size_t end_index) 
{
    std::vector<std::vector<double>> slit(width, std::vector<double>(width, 0));

    for (int i = 0; i < width; i++) 
    {
        for (int j = start_index; j < end_index; j++) 
        {
            slit[i][j] = 1;
        }
    }

    return slit;
}

// Основная функция для расчета дифракции
std::vector<std::vector<double>> calculate_diffraction_pattern(const std::vector<std::vector<double>>& slit, double z, double lambda, double scale) 
{
    static float PI = 3.1415;
    std::vector<std::vector<double>> screen(slit.size(), std::vector<double>(slit[0].size(), 0));
    double k = 2 * PI / lambda;

    for (size_t screen_x = 0; screen_x < screen.size(); screen_x++) 
    {
        std::cout << "screen_x = " << screen_x << "\n";

        for (size_t screen_y = 0; screen_y < screen[0].size(); screen_y++) 
        {
            double Re = 0;
            double Im = 0;

            for (size_t tr_x = 0; tr_x < slit.size(); tr_x++) 
            {
                for (size_t tr_y = 0; tr_y < slit[0].size(); tr_y++) 
                {
                    double dx = (screen_x - tr_x) / scale;
                    double dy = (screen_y - tr_y) / scale;

                    double ro = sqrt(pow(dx, 2) + pow(dy, 2));
                    double R = sqrt(pow(ro, 2) + pow(z, 2));
                    double alpha = atan(ro / z);

                    double koef = slit[tr_x][tr_y] * cos(alpha) / (lambda*R);

                    Im += (-1) * koef * cos(k*R);
                    Re += koef * sin(k*R);
                }
            }

            screen[screen_x][screen_y] = pow(Im, 2) + pow(Re, 2);
        }
    }

    return screen;
}

// функция для занесенния полученных данных в файл
void write_to_file(const std::vector<std::vector<double>>& array, const std::string& filename) 
{
    std::ofstream file(filename);

    if (file.is_open()) 
    {
        for (const auto& row: array) 
        {
            for (double value: row) 
            {
                file << value << " ";
            }
            file << "\n";
        }

        file.close();
    }
}

int main() 
{
    size_t width = 100;
    size_t start_index = 495;
    size_t end_index = 505;
    std::vector<std::vector<double>> slit = create_slit(width, start_index, end_index);

    double z = 0.05 * 10;
    double lambda = 500 * pow(10, -9);
    double scale = 20000;

    std::vector<std::vector<double>> screen = calculate_diffraction_pattern(slit, z, lambda, scale);

    write_to_file(screen, "screen.txt");
}
