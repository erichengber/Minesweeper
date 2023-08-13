//
// Created by Eric Hengber on 4/10/2023.
//

#pragma once
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
using namespace std;

class Tile {
    bool mine = false;
    bool flag = false;
    bool hidden = true;
    sf::Texture texture;
    sf::Texture background;
    vector<Tile*> adjacentTiles;
    int adjMines;
public:
    sf::Sprite sprite;
    sf::Sprite backgroundSprite;
    Tile(TextureManager& texture);

    // Getters
    bool getMine();
    bool getFlag();
    bool getHidden();
    Tile* getAdjTiles(int i);
    int getAdjMines();
    int getAdjTileSize();

    // Setters
    void setMine(bool _mine);
    void setHidden(bool _hidden);
    void setFlag(bool _flag);
    void addAdjTile(Tile* adjTile);
    void setAdjMines(int _adjMines);
};

