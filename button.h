#pragma once

#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cassert>

struct Button {
    
    public:

        Button(std::string init_label, 
                  sf::Vector2f size, 
                  int text_size, 
                  sf::Color background_color, 
                  sf::Color text_color, 
                  char* sound_file_name);
        void set_font(sf::Font &fonts);
        void set_text_size(int charecter_size);
        void set_background_color(sf::Color color);
        void set_label_color(sf::Color color);
        void set_position(sf::Vector2f position);
        void draw(sf::RenderWindow &window);

        void highlight(sf::Color active_background_color, sf::Color active_text_color, sf::RenderWindow &window);
        void click(sf::Event::EventType event);

        bool is_mouse_over(sf::RenderWindow &window);
        sf::RectangleShape button;
        sf::Text label;

        sf::Color text_color;
        sf::Color background_color;

        sf::SoundBuffer sound_buff;
        sf::Sound            sound;

    private:
        
};