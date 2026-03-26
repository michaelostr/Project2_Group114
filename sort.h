//
// Created by Luke Dunham on 3/24/2026.
//
#include <iostream>
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

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

template <typename T, typename C>
int partition(T arr[], int l, int h, C comp) {
    T pivot = arr[l];
    int up = l;
    int down = h;
    while (up < down) {
        for (int i = up; i < h; i++) {
            if (comp(pivot, arr[i])) {
                break;
            }
            up++;
        }
        for (int j = down; j > l; j--) {
            if (comp(arr[j], pivot)) {
                break;
            }
            down--;
        }
        if (up < down) {
            swap(&arr[up], &arr[down]);
        }
        return down;
    }
}

template <typename T, typename C>
void quickSort(T arr[], int low, int high, C comp) {
    if (low < high) {
        T pivot = partition(arr, low, high, comp);
        quickSort(arr, low, pivot - 1, comp);
        quickSort(arr, pivot + 1, high, comp);
    }
}

#endif //UNTITLED_SORT_H