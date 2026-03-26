//
// Created by Michael Ostrowski on 3/26/26.
//

#ifndef PROJECT2_GROUP114_HEALTHRECORD_H
#define PROJECT2_GROUP114_HEALTHRECORD_H

#include <string>
#include <iostream>

struct HealthRecord {
    int id;
    int age;
    std::string gender;
    double body_mass_index;
    int daily_steps;
    double hours_of_sleep;
    double water_intake;
    int calories_consumed;
    int smoking_status;
    int alcohol_consumption;
    int resting_heart_rate;
    int systolic_blood_pressure;
    int diastolic_blood_pressure;
    int cholesterol_level;
    int family_history_of_disease;
    int disease_risk_score;

    HealthRecord()
        : id(0), age(0), gender(""), body_mass_index(0.0), daily_steps(0),
          hours_of_sleep(0.0), water_intake(0.0), calories_consumed(0), smoking_status(0),
          alcohol_consumption(0), resting_heart_rate(0), systolic_blood_pressure(0),
          diastolic_blood_pressure(0), cholesterol_level(0), family_history_of_disease(0),
          disease_risk_score(0) {}
    HealthRecord(int id, int age, const std::string& gender, double body_mass_index, int daily_steps,
                 double hours_of_sleep, double water_intake, int calories_consumed, int smoking_status,
                 int alcohol_consumption, int resting_heart_rate, int systolic_blood_pressure, int diastolic_blood_pressure,
                 int cholesterol_level, int family_history_of_disease, int disease_risk_score)
                     : id(id), age(age), gender(gender), body_mass_index(body_mass_index), daily_steps(daily_steps),
                       hours_of_sleep(hours_of_sleep), water_intake(water_intake), calories_consumed(calories_consumed),
                       smoking_status(smoking_status), alcohol_consumption(alcohol_consumption), resting_heart_rate(resting_heart_rate),
                       systolic_blood_pressure(systolic_blood_pressure), diastolic_blood_pressure(diastolic_blood_pressure),
                       cholesterol_level(cholesterol_level), family_history_of_disease(family_history_of_disease),
                       disease_risk_score(disease_risk_score) {}

    void print() const {
        std::cout << "ID: " << id << '\n'
                  << "Age: " << age << '\n'
                  << "Gender: " << gender << '\n'
                  << "Body Mass Index: " << body_mass_index << '\n'
                  << "Daily Steps: " << daily_steps << '\n'
                  << "Hours of Sleep: " << hours_of_sleep << '\n'
                  << "Water Intake: " << water_intake << '\n'
                  << "Calories Consumed: " << calories_consumed << '\n'
                  << "Smoking Status: " << smoking_status << '\n'
                  << "Alcohol Consumption: " << alcohol_consumption << '\n'
                  << "Resting Heart Rate: " << resting_heart_rate << '\n'
                  << "Systolic Blood Pressure: " << systolic_blood_pressure << '\n'
                  << "Diastolic Blood Pressure: " << diastolic_blood_pressure << '\n'
                  << "Cholesterol Level: " << cholesterol_level << '\n'
                  << "Family History of Disease: " << family_history_of_disease << '\n'
                  << "Disease Risk Score: " << disease_risk_score << '\n';
    }
};

#endif //PROJECT2_GROUP114_HEALTHRECORD_H