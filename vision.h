#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class sf_vision
{
public:
    int team;
    sf::Texture texture;                // 蓝方兵营的纹理
    sf::Sprite sprite;                  // 蓝方兵营的精灵
    sf_vision(int n, int x, int y){
        team=n;
        if (team == 1)
        {
            if (!texture.loadFromFile("C:\\project\\c++\\pjGame\\icon\\barracks_blue.png"))
                exit(1);
        }
        else
            if (!texture.loadFromFile("C:\\project\\c++\\pjGame\\icon\\barracks_red.png"))
                exit(1);
        sprite.setTexture(texture);
        sprite.setScale(0.15f, 0.15f);
        sprite.setPosition(x, y);
    };
    ~sf_vision() {};
    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }
};

