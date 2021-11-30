#pragma once
#include <windows.h>
#include <chrono>
#include <string>
#include <iostream>
#include <atomic>
#include <iomanip>

unsigned int api_signs_num = 100000000;
unsigned int api_block_size = 930707;
std::atomic<unsigned int> api_iteration = 0;
long double api_my_pi = 0;
std::chrono::milliseconds api_calc_time;

DWORD WINAPI calc_summand_winapi(LPVOID lpParam) {
    long double xi;
    long double summand;
    unsigned int curr_iteration = api_iteration.fetch_add(1) * api_block_size;

    for (curr_iteration; curr_iteration < api_signs_num; curr_iteration = api_iteration.fetch_add(1) * api_block_size) {
        summand = 0;
        for (uint64_t i = curr_iteration; (i < curr_iteration + api_block_size) && i < api_signs_num; ++i) {
            xi = (i + 0.5) / api_signs_num;
            summand += (4.0 / (1.0 + xi * xi));
        }
        *(reinterpret_cast<long double*>(lpParam)) += summand;
    }

    return 1;
}

void calc_winapi(std::vector<std::string>* args) {
    unsigned int thread_num = std::stoi((*args)[0]);
    api_signs_num = std::stoi((*args)[1]);
    api_block_size = std::stoi((*args)[2]);
    api_iteration = 0;
    api_my_pi = 0;

    HANDLE* threads = new HANDLE[thread_num];
    auto* summands = new long double[thread_num];

    for (unsigned int i = 0; i < thread_num; ++i) {
        threads[i] = CreateThread(NULL, 0, calc_summand_winapi, (LPVOID)(summands + i), CREATE_SUSPENDED, NULL);
        summands[i] = 0;
    }

    auto start_point = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < thread_num; ++i) {
        ResumeThread(threads[i]);
    }
    WaitForMultipleObjects(thread_num, threads, TRUE, INFINITE);
    for (int i = 0; i < thread_num; ++i) {
        api_my_pi += summands[i];
    }
    api_my_pi /= api_signs_num;
    auto stop_point = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < thread_num; ++i) {
        CloseHandle(threads[i]);
    }

    api_calc_time = std::chrono::duration_cast<std::chrono::milliseconds>(stop_point - start_point);

    delete[] summands;
    delete[] threads;
}

void single_calc_winapi(std::vector<std::string>* args) {
    std::cout << "Calculation using the Win32 API:" << std::endl;
    std::cout << "Number of threads: " << (*args)[0] << std::endl;
    std::cout << "Number of signs: " << (*args)[1] << std::endl;
    std::cout << "Block size: " << (*args)[2] << std::endl;
    calc_winapi(args);
    std::cout << "Result: " << std::setprecision(14) << std::fixed << api_my_pi << std::endl;
    std::cout << "Time of calculation: " << std::resetiosflags(std::ios::fixed);
    std::cout << api_calc_time.count() << " milliseconds" << std::endl;
}

void multiple_calc_winapi(std::vector<std::string>* args) {
    unsigned int calc_num = std::stoi((*args)[3]);
    std::chrono::milliseconds time_summ = std::chrono::milliseconds(0);

    std::cout << "Multiple calculation using the Win32 API:" << std::endl;
    std::cout << "Number of threads: " << (*args)[0] << std::endl;
    std::cout << "Number of signs: " << (*args)[1] << std::endl;
    std::cout << "Block size: " << (*args)[2] << std::endl;
    std::cout << "Number of calculations: " << calc_num << std::endl;
    for (unsigned int i = 0; i < calc_num; ++i){
        calc_winapi(args);
        time_summ += api_calc_time;
    }
    std::cout << "Average time of calculation: " << std::resetiosflags(std::ios::fixed);
    std::cout << time_summ.count()/calc_num << " milliseconds" << std::endl;
}
