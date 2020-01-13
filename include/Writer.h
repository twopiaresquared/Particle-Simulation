#pragma once

#include <fstream>
#include <iomanip>
#include <vector>

void write_data(const std::vector<std::vector<double>> &all_x_data, const std::vector<std::vector<double>> &all_y_data)
{
    std::ofstream ofs("particle.dat");
    ofs << std::fixed << std::setprecision(8);

    std::size_t size = all_x_data.size();
    std::size_t number_of_particles = all_x_data[0].size();
    for (std::size_t i = 0; i < size; ++i)
    {
        for (std::size_t j = 0; j < number_of_particles; ++j)
        {
            ofs << all_x_data[i][j] << " " << all_y_data[i][j] << std::endl;
        }
    }
}
