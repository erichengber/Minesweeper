//
// Created by Eric Hengber on 4/18/2023.
//

#pragma once
#include <map>
#include <iostream>
#include "SFML/Graphics.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>
using namespace std;

class Leaderboard{
    map<int, Leaderboard> leaderboard;
    vector<Leaderboard> lb;
    string name;
    string time;
    int totalSecs;
    bool lbOpen = false;
    bool addWin = false;
public:
    Leaderboard(){
        name = "default";
        time = "default";
        totalSecs = 0;
    }

    Leaderboard(string _name, string _time, int _totalSecs){
        name = _name;
        time = _time;
        totalSecs = _totalSecs;
    }

    void setText(sf::Text &text, float x, float y){
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
        text.setPosition(sf::Vector2f(x, y));
    }

    void displayLeaderboardWindow(int cols, int rows, sf::Font font) {
        lbOpen = true;
        int lbWidth = 16*cols;
        int lbHeight = (rows*16)+50;
        sf::RenderWindow lbWindow(sf::VideoMode(16 * cols, (rows * 16) + 15), "Minesweeper");
        sf::Text lbText("LEADERBOARD", font, 20);
        lbText.setStyle(sf::Text::Bold | sf::Text::Underlined);
        setText(lbText, lbWidth / 2.0f, lbHeight/ 2.0f - 120);

        string allData;
        for (int j = 0; j < lb.size(); ++j) {
            allData += to_string(j+1) + "." + "\t" + lb[j].time + "\t" + lb[j].name + "\n\n";

//            cout << "Name " << lb[j].name << endl;
//            cout << "Time " << lb[j].time << endl;
        }

        while (lbWindow.isOpen()) {

            // Draw
            lbWindow.clear(sf::Color::Blue);

            sf::Text name(allData, font, 18);
            name.setStyle(sf::Text::Bold);
            setText(name, lbWidth / 2.0f, lbHeight/ 2.0f + 20);
            lbWindow.draw(name);

            lbWindow.draw(lbText);
            lbWindow.display();

            sf::Event event;
            while (lbWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    lbWindow.close();
                    lbOpen = false;
                }
            }
        }
    }

    void readLbFile(){
        ifstream file("files/leaderboard.txt");
        string line;


        while (getline(file, line)){
            string rTime;
            string rName;

            istringstream stream(line);
            getline(stream, rTime, ',');
            time = rTime;
            //cout << time << endl;

            getline(stream, rName, ',');
            name = rName;
            //cout << name << endl;

            convertToSecs();
            Leaderboard person(name, time, totalSecs);
            lb.push_back(person);

        }
    }

    void convertToSecs(){
        string sMinutes = time.substr(0,2);
        string sSeconds = time.substr(3,4);
        //cout << sMinutes << " " << sSeconds << endl;

        totalSecs = stoi(sMinutes) * 60 + stoi(sSeconds);
        //cout << totalSecs << endl;
    }

    string convertToMins(int time){
        string winTime;
        string secs;

        if (time % 60 < 10){
            secs += "0";
            secs += to_string(time % 60);
        } else{
            secs += to_string(time % 60);
        }

        int num = ::floor(time / 60);
        if (num < 10){
            winTime += "0";
            winTime += to_string(num);
        } else{
            winTime += (::abs(num) % 10);
            num = ::floor(::abs(num) / 60);
            winTime += (num % 10);
        }
        winTime += ":" + secs;
        return winTime;
    }

    void addWinner(int winSecs, string name){
        if (!addWin) {
            int newIndex = 5;
            for (int i = 0; i < lb.size(); i++) {
                if (winSecs < lb[i].totalSecs) {
                    newIndex--;
                }
            }

            string winMins = convertToMins(winSecs);
            Leaderboard winner(name + "*", winMins, winSecs);
            lb.insert(lb.begin() + newIndex, winner);
            lb.pop_back();
            addWinnerToFile();
            addWin = true;
        }
    }

    void addWinnerToFile(){
        ofstream file("files/leaderboard.txt");
        int starInd;
        for (int i = 0; i < lb.size(); ++i) {
            if (lb[i].name.find("*") > 0){
                starInd = lb[i].name.find("*");
                file << lb[i].time << "," << lb[i].name.substr(0, starInd) << endl;
            } else {
                file << lb[i].time << "," << lb[i].name << endl;
            }
        }
    }

    bool getLbOpen(){
        return lbOpen;
    }

    void setAddWin(bool add){
        addWin = add;
    }
};
