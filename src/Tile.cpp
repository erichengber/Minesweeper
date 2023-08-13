//
// Created by Eric Hengber on 4/10/2023.
//

#include "Tile.h"

Tile::Tile(TextureManager& texture) {
    // Reset
    mine = false;
    flag = false;
    hidden = true;
    adjacentTiles.clear();
    adjMines = 0;

    //sprite.setTexture(texture.getTexture("tile_hidden"));
}

bool Tile::getMine() {
    return mine;
}

bool Tile::getFlag() {
    return flag;
}

bool Tile::getHidden() {
    return hidden;
}

void Tile::setMine(bool _mine) {
    mine = _mine;
}

void Tile::setHidden(bool _hidden) {
    hidden = _hidden;
}

void Tile::setFlag(bool _flag) {
    flag = _flag;
}

void Tile::addAdjTile(Tile *adjTile) {
    adjacentTiles.push_back(adjTile);
}

Tile *Tile::getAdjTiles(int i) {
    return adjacentTiles[i];
}

int Tile::getAdjMines() {
    return adjMines;
}

void Tile::setAdjMines(int _adjMines) {
    adjMines = _adjMines;
}

int Tile::getAdjTileSize() {
    return adjacentTiles.size();
}

