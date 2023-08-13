//
// Created by Eric Hengber on 4/10/2023.
//

#pragma once
#include <vector>
#include <iostream>
#include "Tile.h"
#include "TextureManager.h"
using namespace std;

class Board {
    vector<Tile> board;
    int rows;
    int cols;
    int mines;
    bool debug;
    bool gameOver = false;
    bool win = false;
    int flagCount = 0;
    int mineCount;
    bool pause = false;
    bool leaderboard = false;
    int revealedTiles = 0;
    bool noClick = false;
public:
    Board(int _rows, int _cols, int _mines, TextureManager& texture);

    void initializeBoard(TextureManager& texture);
    void drawBoard(sf::RenderWindow& window);
    void initializeMines();
    void leftClickTile(sf::RenderWindow& window, TextureManager& texture);
    void rightClickTile(sf::RenderWindow& window, TextureManager &texture);
    sf::Vector2i click(sf::RenderWindow &window);
    void toggleDebug(sf::RenderWindow &window, TextureManager& texture);
    void pauseGame(sf::RenderWindow &window, TextureManager& texture);
    void initializeAdjTiles();
    void initializeAdjMines();
    void revealTile(sf::RenderWindow& window, TextureManager& texture, Tile& tile);
    void winBehavior(sf::RenderWindow& window, TextureManager& texture);
    void displayCounterDigits(sf::RenderWindow &window, vector<sf::Sprite>& digits);
    vector<int> convertToDigits();

    // Clock
    vector<int> convertToSecondsDigits(sf::Clock time);
    vector<int> convertToMinutesDigits(sf::Clock time);
    void displayTimerDigits(sf::RenderWindow &window, vector<sf::Sprite>& digits, vector<int> secs, vector<int> mins);
    vector<int> convertToSecondsDigits(int time);
    vector<int> convertToMinutesDigits(int time);

    //Leaderboard
    void lbTiles(sf::RenderWindow& window, TextureManager& texture);

    //Getters
    bool getGameOver();
    bool getDebug();
    bool getPause();
    bool getWin();
    bool getLeaderboard();

    // Setter
    void setPause(bool _pause);
    void setLeaderboard(bool lb);
    void setNoClick(bool nC);
    void setMineCount(int mines);
};
