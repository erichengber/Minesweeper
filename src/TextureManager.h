//
// Created by Eric Hengber on 4/10/2023.
//

#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <SFML/Graphics.hpp>
using namespace std;

class TextureManager {
    map <string, sf::Texture> textures;
    map <string, sf::Sprite> sprites;
public:
    TextureManager(){
        importTexture("debug");
        importTexture("digits");
        importTexture("face_happy");
        importTexture("face_lose");
        importTexture("face_win");
        importTexture("flag");
        importTexture("leaderboard");
        importTexture("mine");
        importTexture("number_1");
        importTexture("number_2");
        importTexture("number_3");
        importTexture("number_4");
        importTexture("number_5");
        importTexture("number_6");
        importTexture("number_7");
        importTexture("number_8");
        importTexture("pause");
        importTexture("play");
        importTexture("tile_hidden");
        importTexture("tile_revealed");
    }

    void importTexture(string fileName){
        textures[fileName].loadFromFile("files/images/" + fileName + ".png");
    }

    sf::Sprite textureToSprite(string& texture){
        sf::Sprite sprite;
        sprite.setTexture(textures[texture]);
        return sprite;
    }

    sf::Texture& getTexture(string findTexture){
        if(textures.find(findTexture) == textures.end()){
            importTexture(findTexture);
        }
        return textures[findTexture];
    }
};

