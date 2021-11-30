#pragma once
#include <omp.h>
#include <chrono>
#include <string>
#include <iostream>
#include <atomic>
#include <iomanip>


unsigned int omp_signs_num = 100000000;
unsigned int omp_block_size = 930707;
std::atomic<unsigned int> omp_iteration = 0;
long double omp_my_pi = 0;
std::chrono::milliseconds omp_calc_time;

void calc_omp(std::vector<std::string>* args) {
    unsigned int thread_num = std::stoi((*args)[0]);
    omp_signs_num = std::stoi((*args)[1]);
    omp_block_size = std::stoi((*args)[2]);
    omp_my_pi = 0;

    auto start = std::chrono::high_resolution_clock::now();
    #pragma omp parallel for num_threads(thread_num) reduction(+:omp_my_pi) schedule(dynamic, omp_block_size)
    for (int i = 0; i < omp_signs_num; ++i) {
        long double xi = (i + 0.5) / omp_signs_num;
        omp_my_pi += (4.0 / (1.0 + xi * xi));
    }
    omp_my_pi /= omp_signs_num;
    auto stop = std::chrono::high_resolution_clock::now();

    omp_calc_time = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
}

void single_calc_omp(std::vector<std::string>* args) {
    std::cout << "Calculation using the OpenMP:" << std::endl;
    std::cout << "Number of threads: " << (*args)[0] << std::endl;
    std::cout << "Number of signs: " << (*args)[1] << std::endl;
    std::cout << "Block size: " << (*args)[2] << std::endl;
    calc_omp(args);
    std::cout << "Result: " << std::setprecision(14) << std::fixed << omp_my_pi << std::endl;
    std::cout << "Time of calculation: " << std::resetiosflags(std::ios::fixed);
    std::cout << omp_calc_time.count() << " milliseconds" << std::endl;
}

void multiple_calc_omp(std::vector<std::string>* args) {
    unsigned int calc_num = std::stoi((*args)[3]);
    std::chrono::milliseconds time_summ = std::chrono::milliseconds(0);

    std::cout << "Multiple calculation using the OpenMP:" << std::endl;
    std::cout << "Number of threads: " << (*args)[0] << std::endl;
    std::cout << "Number of signs: " << (*args)[1] << std::endl;
    std::cout << "Block size: " << (*args)[2] << std::endl;
    std::cout << "Number of calculations: " << calc_num << std::endl;
    for (unsigned int i = 0; i < calc_num; ++i) {
        calc_omp(args);
        time_summ += omp_calc_time;
    }
    std::cout << "Average time of calculation: " << std::resetiosflags(std::ios::fixed);
    std::cout << time_summ.count() / calc_num << " milliseconds" << std::endl;
}