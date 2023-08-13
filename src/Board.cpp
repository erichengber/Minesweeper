//
// Created by Eric Hengber on 4/10/2023.
//

#include "Board.h"
#include "Tile.h"
#include "SFML/Graphics.hpp"
#include "random.h"

Board::Board(int _rows, int _cols, int _mines, TextureManager& texture) {
    rows = _rows;
    cols = _cols;
    mines = _mines;
    mineCount = mines;
}

void Board::drawBoard(sf::RenderWindow& window) {
    for (int i = 0; i < board.size(); ++i) {
        window.draw(board[i].backgroundSprite);
        window.draw(board.at(i).sprite);
    }
    //window.display();
}

void Board::initializeBoard(TextureManager& texture) {
    gameOver = false;
    win = false;
    flagCount = 0;
    revealedTiles = 0;
    board.clear();

    // Construct Board
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            Tile tile(texture);

            tile.backgroundSprite.setTexture(texture.getTexture("tile_revealed"));
            tile.backgroundSprite.setPosition(32*j, 32*i);

            tile.sprite.setTexture(texture.getTexture("tile_hidden"));
            tile.sprite.setPosition(32*j, 32*i);

            board.push_back(tile);
        }
    }
    initializeMines();
    initializeAdjTiles();
    initializeAdjMines();
}

void Board::initializeMines() {
    // Assign Mines
    vector<int> mineNums;
    for (int i = 0; i < mines; ++i) {
        int rand = Random::Int(0, board.size());
        if (find(mineNums.begin(), mineNums.end(), rand) == mineNums.end()){
            mineNums.push_back(rand);
        } else{
            i--;
        }
    }

    for (int j = 0; j < mines; ++j) {
        board[mineNums[j]].setMine(true);
        //cout << mineNums[j] << endl;
    }
}

void Board::leftClickTile(sf::RenderWindow& window, TextureManager& texture) {
    sf::Mouse mouse;
    auto coordinate = mouse.getPosition(window);
    if (!gameOver && !win) {
        for (int i = 0; i < board.size(); ++i) {
            auto tileBounds = board[i].sprite.getGlobalBounds();
            if (tileBounds.contains(coordinate.x, coordinate.y) && board[i].getHidden() && !board[i].getFlag())
                revealTile(window, texture, board[i]);
        }
    }
}

void Board::revealTile(sf::RenderWindow& window, TextureManager& texture, Tile& tile){
        if (!tile.getMine() && tile.getHidden() && !tile.getFlag() && !pause) {
            if (tile.getAdjMines() == 8) {
                tile.sprite.setTexture(texture.getTexture("number_8"));
                tile.setHidden(false);
                revealedTiles++;
            } else if (tile.getAdjMines() == 7){
                tile.sprite.setTexture(texture.getTexture("number_7"));
                tile.setHidden(false);
                revealedTiles++;
            } else if (tile.getAdjMines() == 6){
                tile.sprite.setTexture(texture.getTexture("number_6"));
                tile.setHidden(false);
                revealedTiles++;
            } else if (tile.getAdjMines() == 5){
                tile.sprite.setTexture(texture.getTexture("number_5"));
                tile.setHidden(false);
                revealedTiles++;
            } else if (tile.getAdjMines() == 4){
                tile.sprite.setTexture(texture.getTexture("number_4"));
                tile.setHidden(false);
                revealedTiles++;
            } else if (tile.getAdjMines() == 3){
                tile.sprite.setTexture(texture.getTexture("number_3"));
                tile.setHidden(false);
                revealedTiles++;
            } else if (tile.getAdjMines() == 2){
                tile.sprite.setTexture(texture.getTexture("number_2"));
                tile.setHidden(false);
                revealedTiles++;
            } else if (tile.getAdjMines() == 1){
                tile.sprite.setTexture(texture.getTexture("number_1"));
                tile.setHidden(false);
                revealedTiles++;
            } else{
                tile.sprite.setTexture(texture.getTexture("tile_revealed"));
                tile.setHidden(false);
                revealedTiles++;
                for (int i = 0; i < tile.getAdjTileSize(); ++i) {
                    revealTile(window, texture, *tile.getAdjTiles(i));
                }
            }
        } else if (tile.getMine() && tile.getAdjMines() == 0){
            // Reveal Mine
            tile.sprite.setTexture(texture.getTexture("mine"));
            tile.backgroundSprite.setTexture(texture.getTexture("tile_hidden"));
            // Reveal All Mines
            for (int j = 0; j < board.size(); ++j) {
                if (board[j].getMine() && board[j].getHidden()) {
                    board[j].backgroundSprite.setTexture(texture.getTexture("tile_hidden"));
                    board[j].sprite.setTexture(texture.getTexture("mine"));
                }
            }
            gameOver = true;
        }
}

void Board::rightClickTile(sf::RenderWindow& window, TextureManager& texture) {
    sf::Mouse mouse;
    auto coordinate = mouse.getPosition(window);
    if (!gameOver) {
        for (int i = 0; i < board.size(); ++i) {
            auto tileBounds = board[i].sprite.getGlobalBounds();
            if (tileBounds.contains(coordinate.x, coordinate.y) && board[i].getHidden())
                if (!board[i].getFlag()) {
                    board[i].sprite.setTexture(texture.getTexture("flag"));
                    board[i].backgroundSprite.setTexture(texture.getTexture("tile_hidden"));
                    board[i].setFlag(true);
                    flagCount += 1;
                    mineCount -= 1;
                } else if (board[i].getFlag()) {
                    board[i].sprite.setTexture(texture.getTexture("tile_hidden"));
                    board[i].setFlag(false);
                    flagCount -= 1;
                    mineCount += 1;
                }
        }
    }
}

bool Board::getGameOver() {
    return gameOver;
}

bool Board::getDebug() {
    return debug;
}

sf::Vector2i Board::click(sf::RenderWindow &window) {
    sf::Mouse mouse;
    auto coordinate = mouse.getPosition(window);
    return coordinate;
}

void Board::toggleDebug(sf::RenderWindow &window, TextureManager& texture) {
    if (!gameOver && !win) {
        if (!debug) {
            for (int i = 0; i < board.size(); ++i) {
                if (board[i].getMine()) {
                    board[i].backgroundSprite.setTexture(texture.getTexture("tile_hidden"));
                    board[i].sprite.setTexture(texture.getTexture("mine"));
                }
            }
            debug = true;
        } else if (debug) {
            for (int i = 0; i < board.size(); ++i) {
                if (board[i].getMine()) {
                    board[i].sprite.setTexture(texture.getTexture("tile_hidden"));
                }
                if (board[i].getFlag()) {
                    board[i].backgroundSprite.setTexture(texture.getTexture("tile_hidden"));
                    board[i].sprite.setTexture(texture.getTexture("flag"));
                }
            }
            debug = false;
        }
    }
}

bool Board::getPause() {
    return pause;
}

void Board::setPause(bool _pause) {
    pause = _pause;
}

void Board::pauseGame(sf::RenderWindow &window, TextureManager &texture) {
    if (!gameOver) {
        if (!pause) {
            for (int i = 0; i < board.size(); ++i) {
                board[i].sprite.setTexture(texture.getTexture("tile_revealed"));
                window.draw(board[i].sprite);
            }
            pause = true;
        } else if (pause) {
            for (int i = 0; i < board.size(); ++i) {
                if (board[i].getHidden()) {
                    board[i].sprite.setTexture(texture.getTexture("tile_hidden"));
                } else if (!board[i].getHidden()) {
                    if (board[i].getAdjMines() == 0){
                        board[i].sprite.setTexture(texture.getTexture("tile_revealed"));
                    } else if (board[i].getAdjMines() == 1){
                        board[i].sprite.setTexture(texture.getTexture("number_1"));
                    } else if (board[i].getAdjMines() == 2){
                        board[i].sprite.setTexture(texture.getTexture("number_2"));
                    } else if (board[i].getAdjMines() == 3){
                        board[i].sprite.setTexture(texture.getTexture("number_3"));
                    } else if (board[i].getAdjMines() == 4){
                        board[i].sprite.setTexture(texture.getTexture("number_4"));
                    } else if (board[i].getAdjMines() == 5){
                        board[i].sprite.setTexture(texture.getTexture("number_5"));
                    } else if (board[i].getAdjMines() == 6){
                        board[i].sprite.setTexture(texture.getTexture("number_6"));
                    } else if (board[i].getAdjMines() == 7){
                        board[i].sprite.setTexture(texture.getTexture("number_7"));
                    } else if (board[i].getAdjMines() == 8){
                        board[i].sprite.setTexture(texture.getTexture("number_8"));
                    }
                }
                if (board[i].getFlag()) {
                    board[i].sprite.setTexture(texture.getTexture("flag"));
                }
            }
            pause = false;
        }
    }
}

void Board::initializeAdjTiles() {
    for (int i = 0; i < board.size(); ++i) {
        // Corner Tiles
        if (i == 0){ // Top Left
            board[i].addAdjTile(&board[i + 1]);                // [i] []
            board[i].addAdjTile(&board[cols]);                //  [] []
            board[i].addAdjTile(&board[cols + 1]);
        } else if (i == cols * rows - cols){ // Bottom Left
            board[i].addAdjTile(&board[cols * rows - cols + 1]);        // [] []
            board[i].addAdjTile(&board[cols * rows - cols - cols]);     // [i] []
            board[i].addAdjTile(&board[cols * rows - cols - cols + 1]);
        } else if (i == cols - 1){ // Top Right
            board[i].addAdjTile(&board[cols * 2 - 1]);        // [] [i]
            board[i].addAdjTile(&board[cols * 2 - 2]);       // [] []
            board[i].addAdjTile(&board[cols - 2]);
        } else if (i == cols * rows - 1){ // Bottom Right
            board[i].addAdjTile(&board[cols * rows - 1 - cols]);        // [] []
            board[i].addAdjTile(&board[cols * rows - 1 - cols - 1]);     // [] [i]
            board[i].addAdjTile(&board[cols * rows - 1 - 1]);
        }
        // Edge Tiles
        else if (i % cols == 0){ // Left Edge
            board[i].addAdjTile(&board[i + 1]);               // [] []
            board[i].addAdjTile(&board[i + 1 - cols]);       // [i] []
            board[i].addAdjTile(&board[i + 1 + cols]);      // [] []
            board[i].addAdjTile(&board[i + cols]);
            board[i].addAdjTile(&board[i - cols]);
        }
        else if ((i + 1) % cols == 0){ // Right Edge
            board[i].addAdjTile(&board[i - 1]);               // [] []
            board[i].addAdjTile(&board[i - cols]);          // [] [i]
            board[i].addAdjTile(&board[i + cols]);          // [] []
            board[i].addAdjTile(&board[i + cols - 1]);
            board[i].addAdjTile(&board[i - cols - 1]);
        }
        else if (i > 0 && i < cols - 1){ // Top Edge
            board[i].addAdjTile(&board[i - 1]);               // [] [i] []
            board[i].addAdjTile(&board[i + 1]);              // [] [] []
            board[i].addAdjTile(&board[i + cols - 1]);
            board[i].addAdjTile(&board[i + cols]);
            board[i].addAdjTile(&board[i + cols + 1]);
        }
        else if (i > rows * cols - cols && i < rows * cols - 1){ // Bottom Edge
            board[i].addAdjTile(&board[i + 1]);               // [] [] []
            board[i].addAdjTile(&board[i - 1]);             // [] [i] []
            board[i].addAdjTile(&board[i - cols]);
            board[i].addAdjTile(&board[i - cols - 1]);
            board[i].addAdjTile(&board[i - cols + 1]);
        }
        // Interior Tiles
        else{
            board[i].addAdjTile(&board[i + 1]);               // [] [] []
            board[i].addAdjTile(&board[i - 1]);             // [] [i] []
            board[i].addAdjTile(&board[i - cols]);          // [] [] []
            board[i].addAdjTile(&board[i + cols]);
            board[i].addAdjTile(&board[i - cols - 1]);
            board[i].addAdjTile(&board[i - cols + 1]);
            board[i].addAdjTile(&board[i + cols + 1]);
            board[i].addAdjTile(&board[i + cols - 1]);
        }
    }
}

void Board::initializeAdjMines() {
    for (int i = 0; i < board.size(); ++i) {
        if (!board[i].getMine()){ // if not a mine
            int adjMineCount = 0;
            for (int j = 0; j < board[i].getAdjTileSize(); ++j) {
                Tile* temp = board[i].getAdjTiles(j);
                if (temp->getMine()){
                    adjMineCount++;
                }
            }
            if (adjMineCount == 1){
                board[i].setAdjMines(1);
            } else if (adjMineCount == 2){
                board[i].setAdjMines(2);
            } else if (adjMineCount == 3){
                board[i].setAdjMines(3);
            } else if (adjMineCount == 4){
                board[i].setAdjMines(4);
            } else if (adjMineCount == 5){
                board[i].setAdjMines(5);
            } else if (adjMineCount == 6){
                board[i].setAdjMines(6);
            } else if (adjMineCount == 7){
                board[i].setAdjMines(7);
            } else if (adjMineCount == 8){
                board[i].setAdjMines(8);
            } else{
                board[i].setAdjMines(0);
            }
        }
    }
}

bool Board::getWin() {
    //cout << revealedTiles << endl;
    if(revealedTiles == (rows * cols) - mines){
        win = true;
    }
    return win;
}

void Board::winBehavior(sf::RenderWindow &window, TextureManager &texture) {
    if (win & !noClick) {
        for (int i = 0; i < board.size(); ++i) {
            if (board[i].getMine()) {
                board[i].backgroundSprite.setTexture(texture.getTexture("tile_hidden"));
                board[i].sprite.setTexture(texture.getTexture("flag"));
                board[i].setFlag(true);
                flagCount++;
                mineCount--;
            }
        }
    }
    noClick = true;
}

void Board::displayCounterDigits(sf::RenderWindow &window, vector<sf::Sprite>& digits) {
    vector<int> num = convertToDigits();
    int j = 0;
    for (int i = 0; i < num.size(); ++i) {
        digits[num[i]].setPosition(j + 33, 32 * (rows + 0.5f) + 16);
        j += 21;
        window.draw(digits[num[i]]);
    }
    if (mineCount < 0) {
        digits[10].setPosition(12, 32 * (rows + 0.5f) + 16);
        window.draw(digits[10]);
    }
}

vector<int> Board::convertToDigits() {
    vector<int> digits;
    int num = mineCount;
    digits.push_back(::abs(num) % 10);
    num = ::floor(::abs(num) / 10);
    digits.push_back(::abs(num) % 10);
    num = ::floor(::abs(num) / 10);
    digits.push_back(::abs(num) % 10);
    std::reverse(digits.begin(), digits.end());
    return digits;
}

vector<int> Board::convertToSecondsDigits(sf::Clock time) {
    vector<int> seconds;
    int num = time.getElapsedTime().asSeconds();
    if (num % 60 < 10 && seconds.size() <= 1){
        seconds.push_back(0);
        seconds.push_back(num % 60);
    }else if (num % 60 >= 10){
        num = num % 60;
        seconds.push_back(num % 10);
        num = ::floor(num / 10);
        seconds.push_back(num % 10);
        std::reverse(seconds.begin(), seconds.end());
    }
    return seconds;
}

vector<int> Board::convertToMinutesDigits(sf::Clock time) {
    vector<int> minutes;
    int num = ::floor(time.getElapsedTime().asSeconds() / 60);
    if (num < 10){
        minutes.push_back(0);
        minutes.push_back(num);
    } else{
        minutes.push_back(::abs(num) % 10);
        num = ::floor(::abs(num) / 60);
        minutes.push_back(num % 10);
    }
    return minutes;
}


void Board::displayTimerDigits(sf::RenderWindow &window, vector<sf::Sprite>& digits, vector<int> secs, vector<int> mins) {
    int j = 0;
    for (int i = 0; i < mins.size(); ++i) {
        digits[mins[i]].setPosition((cols*32)-97 + j, 32 * (rows + 0.5f) + 16);
        j += 21;
        window.draw(digits[mins[i]]);
    }
    int k = 0;
    for (int i = 0; i < secs.size(); ++i) {
        digits[secs[i]].setPosition((cols*32)-54 + k, 32 * (rows + 0.5f) + 16);
        k += 21;
        window.draw(digits[secs[i]]);
    }
}

vector<int> Board::convertToSecondsDigits(int time) {
    vector<int> seconds;
    int num = time;
    if (num % 60 < 10 && seconds.size() <= 1){
        seconds.push_back(0);
        seconds.push_back(num % 60);
    }else if (num % 60 >= 10){
        num = num % 60;
        seconds.push_back(num % 10);
        num = ::floor(num / 10);
        seconds.push_back(num % 10);
        std::reverse(seconds.begin(), seconds.end());
    }
    return seconds;
}

vector<int> Board::convertToMinutesDigits(int time) {
    vector<int> minutes;
    int num = ::floor(time / 60);
    if (num < 10){
        minutes.push_back(0);
        minutes.push_back(num);
    } else{
        minutes.push_back(::abs(num) % 10);
        num = ::floor(::abs(num) / 60);
        minutes.push_back(num % 10);
    }
    return minutes;
}

void Board::lbTiles(sf::RenderWindow &window, TextureManager &texture) {
    if (!leaderboard){
        for (int i = 0; i < board.size(); ++i) {
            board[i].sprite.setTexture(texture.getTexture("tile_revealed"));
            window.draw(board[i].sprite);
        }
        leaderboard = true;
        window.display();
    } else if (leaderboard){
        for (int i = 0; i < board.size(); ++i) {
            if (board[i].getHidden()) {
                board[i].sprite.setTexture(texture.getTexture("tile_hidden"));
            } else if (!board[i].getHidden()) {
                if (board[i].getAdjMines() == 0){
                    board[i].sprite.setTexture(texture.getTexture("tile_revealed"));
                } else if (board[i].getAdjMines() == 1){
                    board[i].sprite.setTexture(texture.getTexture("number_1"));
                } else if (board[i].getAdjMines() == 2){
                    board[i].sprite.setTexture(texture.getTexture("number_2"));
                } else if (board[i].getAdjMines() == 3){
                    board[i].sprite.setTexture(texture.getTexture("number_3"));
                } else if (board[i].getAdjMines() == 4){
                    board[i].sprite.setTexture(texture.getTexture("number_4"));
                } else if (board[i].getAdjMines() == 5){
                    board[i].sprite.setTexture(texture.getTexture("number_5"));
                } else if (board[i].getAdjMines() == 6){
                    board[i].sprite.setTexture(texture.getTexture("number_6"));
                } else if (board[i].getAdjMines() == 7){
                    board[i].sprite.setTexture(texture.getTexture("number_7"));
                } else if (board[i].getAdjMines() == 8){
                    board[i].sprite.setTexture(texture.getTexture("number_8"));
                }
            }
            if (board[i].getFlag()) {
                board[i].sprite.setTexture(texture.getTexture("flag"));
            }
        }
        leaderboard = false;
    }
}

bool Board::getLeaderboard() {
    return leaderboard;
}

void Board::setLeaderboard(bool lb) {
    leaderboard = lb;
}

void Board::setNoClick(bool nC) {
    noClick = nC;
}

void Board::setMineCount(int mines) {
    mineCount = mines;
}



