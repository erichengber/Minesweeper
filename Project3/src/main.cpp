#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <cctype>
#include <chrono>
#include "TextureManager.h"
#include "Board.h"
#include "Time.h"
#include "Leaderboard.h"
using namespace std;

void setText(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

int main() {
    TextureManager texture;

    // Read Config File
    ifstream file("files/board_config.cfg");
    string line;

    getline(file, line);
    int columns = stoi(line);

    getline(file, line);
    int rows = stoi(line);

    getline(file, line);
    int mines = stoi(line);

    // Dimensions
    int width = columns * 32;
    int height = rows * 32 + 100;

    // LEADERBOARD WINDOW


    // WELCOME WINDOW
    sf::RenderWindow welcomeWindow(sf::VideoMode(width, height), "Minesweeper");

    sf::Font font;
    font.loadFromFile("files/font.ttf");

    sf::Text welcomeText("WELCOME TO MINESWEEPER!", font, 24);
    welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(welcomeText, width / 2.0f, height / 2.0f - 150);

    sf::Text inputText("Enter your name:", font, 20);
    welcomeText.setStyle(sf::Text::Bold);
    setText(inputText, width / 2.0f, height / 2.0f - 75);

    sf::Text input("|", font, 18);
    input.setStyle(sf::Text::Bold);
    input.setFillColor(sf::Color::Yellow);
    setText(input, width / 2.0f, height / 2.0f - 45);
    string name;

    bool gameRun = false;

    while (welcomeWindow.isOpen()) {
        sf::Event event;
        while (welcomeWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                welcomeWindow.close();
            } else if (event.type == sf::Event::TextEntered) {
                if (name.size() <= 9) {
                    if (isalpha(event.text.unicode))
                        if (name.size() == 0)
                            name += toupper(event.text.unicode);
                        else
                            name += tolower(event.text.unicode);
                }
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::BackSpace) {
                    if (!name.empty())
                        name.pop_back();
                }
                if (event.key.code == sf::Keyboard::Return && name.size() != 0) {
                    welcomeWindow.close();
                    gameRun = true;
                }
            }
        }

        input.setString(name + "|");
        setText(input, width / 2.0f, height / 2.0f - 45);

        welcomeWindow.clear(sf::Color::Blue);
        welcomeWindow.draw(welcomeText);
        welcomeWindow.draw(inputText);
        welcomeWindow.draw(input);
        welcomeWindow.display();
    }

    // GAME WINDOW
    Board board(rows, columns, mines, texture);
    sf::RenderWindow gameWindow(sf::VideoMode(width, height), "Minesweeper");
    board.initializeBoard(texture);
    //board.initializeMines(texture);

    // Buttons
    sf::Sprite debugButton(texture.getTexture("debug"));
    debugButton.setPosition(columns*32-304, 32*(rows+0.5f));
    auto debugBounds = debugButton.getGlobalBounds();

    sf::Sprite pause(texture.getTexture("pause"));
    pause.setPosition(columns*32-240, 32*(rows+0.5f));
    auto pauseBounds = pause.getGlobalBounds();

    sf::Sprite leaderboard(texture.getTexture("leaderboard"));
    leaderboard.setPosition(columns*32-176, 32*(rows+0.5f));
    auto lbBounds = leaderboard.getGlobalBounds();

    sf::Sprite happyFace(texture.getTexture("face_happy"));
    happyFace.setPosition(columns*32/2.0-32, 32*(rows+0.5f));
    auto happyFaceBounds = happyFace.getGlobalBounds();

    vector<sf::Sprite> counterDigits;
    sf::Sprite cDigit(texture.getTexture("digits"));
    for (int i = 0; i < 11; i++) {
        cDigit.setTextureRect(sf::IntRect(i*21,0,21,32));
        counterDigits.push_back(cDigit);
    }

    vector<sf::Sprite> timerDigits;
    sf::Sprite tDigit(texture.getTexture("digits"));
    for (int i = 0; i < 11; i++) {
        tDigit.setTextureRect(sf::IntRect(i*21,0,21,32));
        timerDigits.push_back(tDigit);
    }

    Time timer;
    Leaderboard lbObj;
    lbObj.readLbFile();

    while (gameWindow.isOpen() && gameRun) {
        //cout << time.getElapsedTime().asSeconds() << endl;
        //cout << minutes[0] << minutes[1] << ":" << seconds[0] << seconds[1] << endl;

        // Draw
        gameWindow.clear(sf::Color::White);

        int time = timer.getTotalTime();
        vector<int> seconds = board.convertToSecondsDigits(time);
        vector<int> minutes = board.convertToMinutesDigits(time);
        board.displayTimerDigits(gameWindow, timerDigits, seconds, minutes);
        if (board.getPause()) {
            timer.pause();
        } else if (!board.getPause()){
            timer.unpause();
        }

        if(board.getWin() | board.getGameOver()){
            timer.winLose();
        }

        if (!lbObj.getLbOpen() && board.getLeaderboard()){
            board.setLeaderboard(true);
            board.lbTiles(gameWindow, texture);
        }

        board.drawBoard(gameWindow);
        gameWindow.draw(debugButton);
        board.displayCounterDigits(gameWindow, counterDigits);
        gameWindow.draw(pause);
        gameWindow.draw(leaderboard);
        gameWindow.draw(happyFace);
        gameWindow.display();

        sf::Event event;
        while (gameWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                gameWindow.close();
            }
            // Mouse click
            if (event.type == sf::Event::MouseButtonPressed) {
                // Left click
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Tile
                    board.leftClickTile(gameWindow, texture);
                    // Debug Button
                    if (debugBounds.contains(board.click(gameWindow).x, board.click(gameWindow).y) &&
                        !board.getPause()) {
                        board.toggleDebug(gameWindow, texture);
                    }
                    // Happy Face
                    if (happyFaceBounds.contains(board.click(gameWindow).x, board.click(gameWindow).y)) {
                        timer.reset();
                        // Reset Board
                        board.initializeBoard(texture);
                        //board.initializeMines(texture);
                        board.drawBoard(gameWindow);
                        lbObj.setAddWin(false);
                        board.setNoClick(false);
                        board.setMineCount(mines);
                    }
                    // Pause/Play
                    if (pauseBounds.contains(board.click(gameWindow).x, board.click(gameWindow).y)) {
                        if (!board.getPause() && !board.getGameOver() && !board.getWin()) {
                            pause.setTexture(texture.getTexture("play"));
                            board.pauseGame(gameWindow, texture);
                        } else if (board.getPause() && !board.getGameOver() && !board.getWin()) {
                            pause.setTexture(texture.getTexture("pause"));
                            board.pauseGame(gameWindow, texture);
                        }
                    }
                    // Leaderboard
                    if (lbBounds.contains(board.click(gameWindow).x, board.click(gameWindow).y)) {
                        if (!lbObj.getLbOpen()) {
                            board.lbTiles(gameWindow, texture);
                            lbObj.displayLeaderboardWindow(columns, rows, font);
                        }
                    }
                }
                    // Right click
                if (event.mouseButton.button == sf::Mouse::Right) {
                    if (!board.getGameOver() && !board.getWin()) {
                        board.rightClickTile(gameWindow, texture);
                    }
                }
                // Lose
                if (board.getGameOver()) {
                    happyFace.setTexture(texture.getTexture("face_lose"));
                } else {
                    happyFace.setTexture(texture.getTexture("face_happy"));
                }
                // Win
                if (board.getWin()) {
                    happyFace.setTexture(texture.getTexture("face_win"));
                    board.winBehavior(gameWindow, texture);
                    lbObj.addWinner(time, name);
                }
            }
        }
    }


    return 0;
}