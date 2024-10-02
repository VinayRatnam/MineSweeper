#pragma once
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

vector<vector<string>> loadLeaderboardData(string filename) {
    ifstream file(filename);

    if (file.is_open()) {
        vector<vector<string>> filedata;

        string curr_string;
        while(getline(file, curr_string)) {
            istringstream stream(curr_string);

            string curr_name;
            string curr_time;
            getline(stream, curr_time, ',');
            getline(stream, curr_name, ',');

            filedata.push_back({curr_time, curr_name});
        }

        return filedata;
    }
    else {
        cout << "file did not open" << endl;
    }
}

string allText(vector<vector<string>> filedata) {
    string all_text;
    for (int i = 1; i <= 5; i++) {
        all_text += to_string(i);
        all_text += ".\t";
        all_text += filedata[i - 1][0];
        all_text += "\t";
        all_text += filedata[i-1][1];
        all_text += "\n\n";
    }

    return all_text;
}

bool updateLeaderboard(int user_time, string name, vector<vector<string>>& data) {
    vector<int> old_times_in_seconds;

    for (int i = 0; i < data.size(); i++) {
        string curr_data = data[i][0];
        int time_in_seconds = 0;

        //convert from string to time in seconds
        time_in_seconds += stoi(curr_data.substr(0, 2)) * 60;
        time_in_seconds += stoi(curr_data.substr(3, 2));

        old_times_in_seconds.push_back(time_in_seconds);
    }

    int placement;
    for (int i = 0; i < old_times_in_seconds.size(); i++) {
        if (user_time < old_times_in_seconds[i]) {
            //move back leaderboard

            for (int j = data.size() - 1; j > i; j--) {
                data[j] = data[j - 1];
            }

            int minutes_passed = user_time / 60;
            int seconds_passed = user_time - (minutes_passed * 60);

            string minutes_passed_string;
            string seconds_passed_string;

            if (minutes_passed < 10) {
                minutes_passed_string += "0";
            }
            minutes_passed_string += to_string(minutes_passed);

            if (seconds_passed < 10) {
                seconds_passed_string += "0";
            }
            seconds_passed_string += to_string(seconds_passed);

            string new_time = minutes_passed_string + ":" + seconds_passed_string;
            data[i] = {new_time, name + "*"};
            return true;
        }
    }
    return false;
}