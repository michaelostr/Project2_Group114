#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <iomanip>
#include "CSVLoader.h"
#include "sort.h"


using Comp = bool(*)(const HealthRecord&, const HealthRecord&);

Comp getComparator(const std::string& category) {
    if (category == "Age")         return compAge;
    if (category == "Gender")      return compGender;
    if (category == "Bmi")         return compBMI;
    if (category == "Steps")       return compSteps;
    if (category == "Sleep")       return compSleep;
    if (category == "Water")       return compWater;
    if (category == "Calories")    return compCalories;
    if (category == "Smoking")     return compSmoke;
    if (category == "Alcohol")     return compAlcohol;
    if (category == "Heart")       return compHeart;
    if (category == "Systolic")    return compSystolic;
    if (category == "Diastolic")   return compDiastolic;
    if (category == "Cholesterol") return compCholesterol;
    if (category == "Family")      return compHistory;
    if (category == "Risk")        return compRisk;
    return compID;
}

bool isValidCategory(const std::string& input) {
    return input == "Age"      || input == "Gender"   || input == "Bmi"      ||
           input == "Steps"    || input == "Sleep"    || input == "Water"    ||
           input == "Calories" || input == "Smoking"  || input == "Alcohol"  ||
           input == "Heart"    || input == "Systolic" || input == "Diastolic"||
           input == "Cholesterol" || input == "Family" || input == "Risk";
}


struct BenchmarkResult {
    double avgMerge;
    double avgQuick;
};

BenchmarkResult runBenchmark(const std::vector<HealthRecord>& original, Comp comp) {
    int n = static_cast<int>(original.size());
    std::vector<HealthRecord> buf(original);


    double totalMerge = 0.0;
    for (int i = 0; i < 8; i++) {
        buf = original;
        HealthRecord* arr = buf.data();
        auto t0 = std::chrono::high_resolution_clock::now();
        mergeSort(arr, 0, n - 1, comp);
        auto t1 = std::chrono::high_resolution_clock::now();
        totalMerge += std::chrono::duration<double, std::milli>(t1 - t0).count();
    }


    double totalQuick = 0.0;
    for (int i = 0; i < 8; i++) {
        buf = original;
        HealthRecord* arr = buf.data();
        auto t0 = std::chrono::high_resolution_clock::now();
        quickSort(arr, 0, n - 1, comp);
        auto t1 = std::chrono::high_resolution_clock::now();
        totalQuick += std::chrono::duration<double, std::milli>(t1 - t0).count();
    }

    return { totalMerge / 8.0, totalQuick / 8.0 };
}


std::string toLower(const std::string& s) {
    std::string out = s;
    for (char& c : out) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return out;
}

std::string trim(const std::string& s) {
    size_t a = 0, b = s.size();
    while (a < b && std::isspace(static_cast<unsigned char>(s[a]))) a++;
    while (b > a && std::isspace(static_cast<unsigned char>(s[b-1]))) b--;
    return s.substr(a, b - a);
}


std::vector<HealthRecord> searchRecords(const std::vector<HealthRecord>& data,
                                        const std::string& rawQuery) {

    size_t pos = rawQuery.find(" in ");
    if (pos == std::string::npos) return {};

    std::string value    = trim(rawQuery.substr(0, pos));
    std::string category = trim(rawQuery.substr(pos + 4));

    if (!category.empty()) {
        category[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(category[0])));
        for (size_t i = 1; i < category.size(); i++)
            category[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(category[i])));
    }

    std::vector<HealthRecord> results;

    auto matchInt = [&](int field) {
        try { return field == std::stoi(value); }
        catch (...) { return false; }
    };
    auto matchDouble = [&](double field) {
        try { return std::abs(field - std::stod(value)) < 1e-6; }
        catch (...) { return false; }
    };
    auto matchString = [&](const std::string& field) {
        return toLower(field) == toLower(value);
    };

    for (const auto& r : data) {
        bool match = false;
        if      (category == "Age")         match = matchInt(r.age);
        else if (category == "Gender")      match = matchString(r.gender);
        else if (category == "Bmi")         match = matchDouble(r.body_mass_index);
        else if (category == "Steps")       match = matchInt(r.daily_steps);
        else if (category == "Sleep")       match = matchDouble(r.hours_of_sleep);
        else if (category == "Water")       match = matchDouble(r.water_intake);
        else if (category == "Calories")    match = matchInt(r.calories_consumed);
        else if (category == "Smoking")     match = matchInt(r.smoking_status);
        else if (category == "Alcohol")     match = matchInt(r.alcohol_consumption);
        else if (category == "Heart")       match = matchInt(r.resting_heart_rate);
        else if (category == "Systolic")    match = matchInt(r.systolic_blood_pressure);
        else if (category == "Diastolic")   match = matchInt(r.diastolic_blood_pressure);
        else if (category == "Cholesterol") match = matchInt(r.cholesterol_level);
        else if (category == "Family")      match = matchInt(r.family_history_of_disease);
        else if (category == "Risk")        match = matchInt(r.disease_risk_score);
        if (match) results.push_back(r);
    }
    return results;
}

bool isSearchQuery(const std::string& input) {
    return input.find(" in ") != std::string::npos;
}


std::string fmt(double ms, int decimals = 4) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(decimals) << ms << " ms";
    return ss.str();
}


int main() {
    enum Screen { MAIN, SELECT, SORT, SEARCH };


    std::vector<HealthRecord> data;
    try {
        data = CSVLoader::loadHealthData("health_lifestyle_dataset.csv");
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << "Loaded " << data.size() << " records.\n";

    Screen currentScreen = MAIN;
    std::string userInput;
    std::string selectedCategory;


    BenchmarkResult benchResult = {0.0, 0.0};
    bool benchDone = false;


    std::vector<HealthRecord> searchResults;
    int searchPage = 0;
    std::string searchQuery;
    sf::RenderWindow window(sf::VideoMode({1000, 700}), "Health & Lifestyle Sorting Analysis");

    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("background.jpg")) {
        std::cout << "Failed to load background\n";
        return 1;
    }
    sf::Sprite background(bgTexture);
    background.setScale({ 1000.f / bgTexture.getSize().x, 700.f / bgTexture.getSize().y });

    sf::RectangleShape overlay;
    overlay.setSize(sf::Vector2f(1000.f, 700.f));
    overlay.setFillColor(sf::Color(0, 0, 0, 200));

    sf::Font font;
    if (!font.openFromFile("font.ttf")) {
        std::cout << "Failed to load font\n";
        return 1;
    }

    auto makeText = [&](const std::string& str, unsigned int size,
                        sf::Vector2f pos, bool centered = true) -> sf::Text {
        sf::Text t(font);
        t.setString(str);
        t.setCharacterSize(size);
        t.setFillColor(sf::Color::White);
        t.setOutlineThickness(0.5f);
        t.setOutlineColor(sf::Color::White);
        if (centered) {
            auto b = t.getLocalBounds();
            t.setOrigin({ b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f });
        }
        t.setPosition(pos);
        return t;
    };

    sf::Text title = makeText("Analysis on Sorting Algorithms\n  in Health and Lifestyle", 52, {500.f, 195.f});
    title.setOutlineThickness(1.f);

    sf::Text subtitle   = makeText("Type \"Enter\" to continue\n   Type \"Exit\" to exit", 36, {500.f, 360.f});
    sf::Text escapeHint = makeText("Press Escape to go back", 22, {500.f, 660.f});

    sf::Text selectTitle = makeText("Select Category", 56, {500.f, 120.f});
    selectTitle.setOutlineThickness(1.f);

    sf::Text selectSub(font);
    selectSub.setString("\t\t\t Type the category to sort by \n \t\t\t\t\t\t\t\t\tOR\n Type \" <value> in <category> \" to search ");
    selectSub.setCharacterSize(40);
    selectSub.setFillColor(sf::Color::White);
    selectSub.setOutlineThickness(0.5);
    selectSub.setOutlineColor(sf::Color::White);
    {
        auto selBounds = selectSub.getLocalBounds();
        selectSub.setOrigin({
            selBounds.position.x + selBounds.size.x / 2.f,
            selBounds.position.y + selBounds.size.y / 2.f
        });
        selectSub.setPosition({500.f, 350.f});
    }

    sf::Text inputDisplay(font);
    inputDisplay.setCharacterSize(32);
    inputDisplay.setFillColor(sf::Color::Cyan);
    inputDisplay.setOutlineThickness(0.5f);
    inputDisplay.setOutlineColor(sf::Color::Cyan);


    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* te = event->getIf<sf::Event::TextEntered>()) {
                unsigned int unicode = te->unicode;
                if (unicode == 8) {
                    if (!userInput.empty()) userInput.pop_back();
                } else if (unicode >= 32 && unicode < 128) {
                    char c = static_cast<char>(unicode);
                    if (userInput.empty())
                        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
                    else
                        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
                    userInput += c;
                }
            }

            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {

                if (key->code == sf::Keyboard::Key::Escape) {
                    if (currentScreen == SORT || currentScreen == SEARCH) {
                        currentScreen = SELECT;
                        userInput.clear();
                        benchDone = false;
                    }
                }

                if (key->code == sf::Keyboard::Key::Enter) {
                    if (userInput == "Exit") {
                        window.close();
                    }
                    else if (currentScreen == MAIN && userInput == "Enter") {
                        currentScreen = SELECT;
                        userInput.clear();
                    }
                    else if (currentScreen == SELECT && isValidCategory(userInput)) {
                        selectedCategory = userInput;
                        Comp comp = getComparator(selectedCategory);
                        benchResult = runBenchmark(data, comp);
                        benchDone = true;
                        currentScreen = SORT;
                        userInput.clear();
                    }
                    else if (currentScreen == SELECT && isSearchQuery(userInput)) {
                        searchQuery   = userInput;
                        searchResults = searchRecords(data, userInput);
                        searchPage    = 0;
                        currentScreen = SEARCH;
                        userInput.clear();
                    }
                    else {
                        userInput.clear();
                    }
                }

                if (currentScreen == SEARCH && !searchResults.empty()) {
                    if (key->code == sf::Keyboard::Key::Right)
                        searchPage = std::min(searchPage + 1, (int)searchResults.size() - 1);
                    if (key->code == sf::Keyboard::Key::Left)
                        searchPage = std::max(searchPage - 1, 0);
                }
            }
        }

        inputDisplay.setString("> " + userInput);
        {
            auto b = inputDisplay.getLocalBounds();
            inputDisplay.setOrigin({ b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f });
            inputDisplay.setPosition({500.f, 590.f});
        }

        window.clear();
        window.draw(background);
        window.draw(overlay);


        if (currentScreen == MAIN) {
            window.draw(title);
            window.draw(subtitle);
            window.draw(inputDisplay);
        }

        else if (currentScreen == SELECT) {
            window.draw(selectTitle);
            window.draw(selectSub);
            window.draw(inputDisplay);
        }

        else if (currentScreen == SORT) {
            window.draw(makeText("Sorting Results", 56, {500.f, 90.f}));
            window.draw(makeText("Category: " + selectedCategory, 28, {500.f, 160.f}));
            window.draw(makeText("(average of 8 runs each)", 22, {500.f, 195.f}));

            if (benchDone) {
                window.draw(makeText("Merge Sort", 36, {270.f, 290.f}));
                window.draw(makeText(fmt(benchResult.avgMerge), 40, {270.f, 345.f}));

                window.draw(makeText("Quick Sort", 36, {730.f, 290.f}));
                window.draw(makeText(fmt(benchResult.avgQuick), 40, {730.f, 345.f}));

                double diff = std::abs(benchResult.avgMerge - benchResult.avgQuick);
                std::string winner = (benchResult.avgMerge < benchResult.avgQuick)
                                     ? "Merge Sort" : "Quick Sort";
                window.draw(makeText(winner + " wins by " + fmt(diff) + "!", 32, {500.f, 460.f}));

                double total = benchResult.avgMerge + benchResult.avgQuick;
                float mergeW = static_cast<float>(benchResult.avgMerge / total * 700.0);
                float quickW = 700.f - mergeW;

                sf::RectangleShape mergeBar({mergeW, 28.f});
                mergeBar.setFillColor(sf::Color(70, 130, 180, 220));
                mergeBar.setPosition({150.f, 510.f});

                sf::RectangleShape quickBar({quickW, 28.f});
                quickBar.setFillColor(sf::Color(220, 90, 70, 220));
                quickBar.setPosition({150.f + mergeW, 510.f});

                window.draw(mergeBar);
                window.draw(quickBar);

                window.draw(makeText("Merge", 18, {150.f + mergeW / 2.f, 543.f}));
                window.draw(makeText("Quick", 18, {150.f + mergeW + quickW / 2.f, 543.f}));
            }

            window.draw(escapeHint);
        }

        else if (currentScreen == SEARCH) {
            window.draw(makeText("Search Results", 56, {500.f, 70.f}));
            window.draw(makeText("Query: " + searchQuery, 22, {500.f, 130.f}));

            if (searchResults.empty()) {
                window.draw(makeText("No records found.", 36, {500.f, 350.f}));
            } else {
                const HealthRecord& r = searchResults[searchPage];

                window.draw(makeText(
                    "Record " + std::to_string(searchPage + 1) +
                    " of "    + std::to_string(searchResults.size()),
                    24, {500.f, 168.f}));

                std::string left =
                    "ID: "                + std::to_string(r.id)                      + "\n"
                    "Age: "               + std::to_string(r.age)                     + "\n"
                    "Gender: "            + r.gender                                  + "\n"
                    "BMI: "               + std::to_string(r.body_mass_index)         + "\n"
                    "Daily Steps: "       + std::to_string(r.daily_steps)             + "\n"
                    "Sleep Hours: "       + std::to_string(r.hours_of_sleep)          + "\n"
                    "Water Intake: "      + std::to_string(r.water_intake)            + "\n"
                    "Calories: "          + std::to_string(r.calories_consumed);

                std::string right =
                    "Smoking: "           + std::to_string(r.smoking_status)          + "\n"
                    "Alcohol: "           + std::to_string(r.alcohol_consumption)     + "\n"
                    "Heart Rate: "        + std::to_string(r.resting_heart_rate)      + "\n"
                    "Systolic BP: "       + std::to_string(r.systolic_blood_pressure) + "\n"
                    "Diastolic BP: "      + std::to_string(r.diastolic_blood_pressure)+ "\n"
                    "Cholesterol: "       + std::to_string(r.cholesterol_level)       + "\n"
                    "Family History: "    + std::to_string(r.family_history_of_disease)+ "\n"
                    "Disease Risk Score: "+ std::to_string(r.disease_risk_score);

                sf::Text leftText(font);
                leftText.setString(left);
                leftText.setCharacterSize(26);
                leftText.setFillColor(sf::Color::White);
                leftText.setPosition({80.f, 200.f});
                leftText.setOutlineThickness(.5f);
                leftText.setOutlineColor(sf::Color::White);

                sf::Text rightText(font);
                rightText.setString(right);
                rightText.setCharacterSize(26);
                rightText.setFillColor(sf::Color::White);
                rightText.setPosition({530.f, 200.f});
                rightText.setOutlineThickness(.5f);
                rightText.setOutlineColor(sf::Color::White);

                window.draw(leftText);
                window.draw(rightText);

                if (searchResults.size() > 1)
                    window.draw(makeText("use left and right arrow keys to browse results", 20, {500.f, 620.f}));
            }

            window.draw(escapeHint);
        }

        window.display();
    }

    return 0;
}