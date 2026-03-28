//
// Created by Luke Dunham on 3/24/2026.
//
#include <iostream>
#include <utility>
#include "HealthRecord.h"
using namespace std;

#ifndef UNTITLED_SORT_H
#define UNTITLED_SORT_H

bool compID(const HealthRecord& lhs, const HealthRecord& rhs) {
    return lhs.id < rhs.id;
}

bool compAge(const HealthRecord& lhs, const HealthRecord& rhs) {
    return lhs.age < rhs.age;
}

bool compGender(const HealthRecord& lhs, const HealthRecord& rhs) {
    return lhs.gender < rhs.gender;
}

bool compBMI(const HealthRecord& lhs, const HealthRecord& rhs) {
    return lhs.body_mass_index < rhs.body_mass_index;
}

bool compSteps(const HealthRecord& lhs, const HealthRecord& rhs) {
    return lhs.daily_steps < rhs.daily_steps;
}

bool compSleep(const HealthRecord& lhs, const HealthRecord& rhs) {
    return lhs.hours_of_sleep < rhs.hours_of_sleep;
}

bool compWater(const HealthRecord& lhs, const HealthRecord& rhs) {
    return lhs.water_intake < rhs.water_intake;
}

bool compCalories(const HealthRecord& lhs, const HealthRecord& rhs) {
    return lhs.calories_consumed < rhs.calories_consumed;
}

bool compSmoke(const HealthRecord& lhs, const HealthRecord& rhs) {
    return lhs.smoking_status < rhs.smoking_status;
}

bool compAlcohol(const HealthRecord& lhs, const HealthRecord& rhs) {
    return lhs.alcohol_consumption < rhs.alcohol_consumption;
}

bool compHeart(const HealthRecord& lhs, const HealthRecord& rhs) {
    return lhs.resting_heart_rate < rhs.resting_heart_rate;
}

bool compSystolic(const HealthRecord& lhs, const HealthRecord& rhs) {
    return lhs.systolic_blood_pressure < rhs.systolic_blood_pressure;
}

bool compDiastolic(const HealthRecord& lhs, const HealthRecord& rhs) {
    return lhs.diastolic_blood_pressure < rhs.diastolic_blood_pressure;
}

bool compCholesterol(const HealthRecord& lhs, const HealthRecord& rhs) {
    return lhs.cholesterol_level < rhs.cholesterol_level;
}

bool compHistory(const HealthRecord& lhs, const HealthRecord& rhs) {
    return lhs.family_history_of_disease < rhs.family_history_of_disease;
}

bool compRisk(const HealthRecord& lhs, const HealthRecord& rhs) {
    return lhs.disease_risk_score < rhs.disease_risk_score;
}

template <typename T, typename C>
void merge(T arr[], int left, int mid, int right, C comp) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    T* L = new T[n1];
    T* R = new T[n2];
    for (int i = 0; i < n1; i++) {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];
    }
    int i = 0;
    int j = 0;
    int k = left;
    while (i < n1 && j < n2) {
        if (comp(L[i], R[j])) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    delete[] L;
    delete[] R;
}

template <typename T, typename C>
void mergeSort(T arr[], int left, int right, C comp){
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid, comp);
        mergeSort(arr, mid + 1, right, comp);
        merge(arr, left, mid, right, comp);
    }
}

template<typename T>
void swap(T *a, T *b) {
    T temp = *a;
    *a = *b;
    *b = temp;
}


template <typename T, typename C>
std::pair<int,int> partition3(T arr[], int l, int h, C comp) {
    // Median-of-three: compare first, middle, last and put median in arr[l]
    int mid = l + (h - l) / 2;
    if (comp(arr[mid], arr[l])) swap(&arr[mid], &arr[l]);
    if (comp(arr[h],   arr[l])) swap(&arr[h],   &arr[l]);
    if (comp(arr[mid], arr[l])) swap(&arr[mid], &arr[l]);
    // arr[l] is now the median of the three

    T pivot = arr[l];
    int lt = l;
    int gt = h;
    int i  = l + 1;
    while (i <= gt) {
        if (comp(arr[i], pivot)) {
            swap(&arr[i], &arr[lt]);
            lt++;
            i++;
        } else if (comp(pivot, arr[i])) {
            swap(&arr[i], &arr[gt]);
            gt--;
        } else {
            i++;
        }
    }
    return {lt, gt};
}

template <typename T, typename C>
void quickSort(T arr[], int low, int high, C comp) {
    if (low < high) {
        auto [lt, gt] = partition3(arr, low, high, comp);
        quickSort(arr, low, lt - 1, comp);
        quickSort(arr, gt + 1, high, comp);
    }
}

#endif //UNTITLED_SORT_H