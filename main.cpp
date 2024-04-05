#include <cmath>
#include <fstream>
#include <vector>

// Функция для нормализации значений в массиве
std::vector<std::vector<float>> normalize(const std::vector<std::vector<float>>& array) 
{
    float max = 0;
    for (const auto& row: array) 
    {
        for (float value: row) 
        {
            if (value > max) 
            {
                max = value;
            }
        }
    }
    
    std::vector<std::vector<float>> normalized_array(array.size(), std::vector<float>(array[0].size(), 0));

    for (size_t i = 0; i < array.size(); i++) 
    {
        for (size_t j = 0; j < array[i].size(); j++) 
        {
            normalized_array[i][j] = array[i][j]/max;
        }
    }

    return normalized_array;
}

// Функция для создания прямоугольной щели
std::vector<std::vector<float>> create_slit(int width, int start_index, int end_index) 
{
    std::vector<std::vector<float>> slit(width, std::vector<float>(width, 0));

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
std::vector<std::vector<float>> calculate_diffraction_pattern(const std::vector<std::vector<float>>& slit, float z, float lambda, float scale) 
{
    static float PI = 3.1415;
    std::vector<std::vector<float>> screen(slit.size(), std::vector<float>(slit[0].size(), 0));
    float k = 2 * PI / lambda;

    for (size_t screen_x = 0; screen_x < screen.size(); screen_x++) 
    {
        for (size_t screen_y = 0; screen_y < screen[0].size(); screen_y++) 
        {
            float Re = 0;
            float Im = 0;

            for (size_t tr_x = 0; tr_x < slit.size(); tr_x++) 
            {
                for (size_t tr_y = 0; tr_y < slit[0].size(); tr_y++) 
                {
                    float dx = (screen_x - tr_x) / scale;
                    float dy = (screen_y - tr_y) / scale;

                    float ro = sqrt(pow(dx, 2) + pow(dy, 2));
                    float R = sqrt(pow(ro, 2) + pow(z, 2));
                    float alpha = atan(ro / z);

                    float koef = slit[tr_x][tr_y] * cos(alpha) / (lambda*R);

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
void write_to_file(const std::vector<std::vector<float>>& array, const std::string& filename) 
{
    std::ofstream file(filename);

    if (file.is_open()) 
    {
        for (const auto& row: array) 
        {
            for (float value: row) 
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
    int width = 100;
    int start_index = 49;
    int end_index = 51;

    std::vector<std::vector<float>> slit = create_slit(width, start_index, end_index);

    float z = 0.05 * 10;
    float lambda = 500 * pow(10, -9);
    float scale = 20000;

    std::vector<std::vector<float>> screen = calculate_diffraction_pattern(slit, z, lambda, scale);

    write_to_file(screen, "screen.txt");

}
