//
// Created by ldunh on 3/25/2026.
//
#include <iostream>
#include "sort.h"
#include <chrono>
using namespace std;
#ifndef UNTITLED_BENCHMARK_H
#define UNTITLED_BENCHMARK_H

template <typename T, typename C>
double clockMerge(T arr[], int left, int right, C comp) {
    using Clock = chrono::high_resolution_clock;
    Clock::time_point startTime = Clock::now();
    chrono::duration<double, milli> elapsed;
    mergeSort(arr, left, right, comp);
    Clock::time_point endTime = Clock::now();
    elapsed = endTime - startTime;
    return elapsed.count();
}

template <typename T, typename C>
double clockQuick(T arr[], int left, int right, C comp) {
    using Clock = chrono::high_resolution_clock;
    Clock::time_point startTime = Clock::now();
    chrono::duration<double, milli> elapsed;
    quickSort(arr, left, right, comp);
    Clock::time_point endTime = Clock::now();
    elapsed = endTime - startTime;
    return elapsed.count();
}

template <typename T, typename C>
vector<double> benchmark(T arr[], int left, int right, C comp) {
    vector<double> times;
    double averageMerge;
    for (int i = 0; i < 8; i++) {
        times.push_back(clockMerge(arr, left, right, comp));
        averageMerge += times[i];
    }
    averageMerge /= 8;
    times.push_back(averageMerge);
    double averageQuick;
    for (int i = 0; i < 8; i++) {
        times.push_back(clockQuick(arr, left, right, comp));
        averageQuick += times[i + 9];
    }
    averageQuick /= 8;
    times.push_back(averageQuick);
    return times;
}
#endif //UNTITLED_BENCHMARK_H