#include "button.h"

Button::Button(std::string init_label, 
                  sf::Vector2f size, 
                  int text_size, 
                  sf::Color init_background_color, 
                  sf::Color init_text_color, 
                  char* sound_file_name) {
    
    text_color = init_text_color;
    background_color = init_background_color;

    label.setString(init_label);
    label.setColor(text_color);
    label.setCharacterSize(text_size);
    button.setSize(size);
    button.setFillColor(background_color);
    button.setOutlineThickness(5);
    button.setOutlineColor(text_color);

    assert (sound_buff.loadFromFile(sound_file_name));    
    sound.setBuffer(sound_buff);
}

void Button::set_font(sf::Font &fonts) {
	label.setFont(fonts);
}
 
void Button::set_background_color(sf::Color color) {
    button.setFillColor(color);
}

void Button::set_label_color(sf::Color color) {
    label.setFillColor(color);
}

void Button::set_position(sf::Vector2f position) {

    sf::FloatRect button_rectangle = button.getLocalBounds();
    button.setOrigin(button_rectangle.left + button_rectangle.width / 2, button_rectangle.top + button_rectangle.height / 2);
    button.setPosition(position);
  
    sf::FloatRect label_rectangle = label.getLocalBounds();
    label.setOrigin(label_rectangle.left + label_rectangle.width / 2, label_rectangle.top + label_rectangle.height / 2);
    label.setPosition(position);
}

void Button::draw(sf::RenderWindow &window) {
    window.draw(button);
    window.draw(label);
}

bool Button::is_mouse_over(sf::RenderWindow &window) {

    sf::FloatRect button_rectangle = button.getLocalBounds();

    float mouse_x = sf::Mouse::getPosition(window).x;
    float mouse_y = sf::Mouse::getPosition(window).y;

    float button_x = button.getPosition().x;
    float button_y = button.getPosition().y;

    float button_x_plus_width  = button_x + button.getLocalBounds().width;
    float button_y_plus_height = button_y + button.getLocalBounds().height; 

    if ((mouse_x < button.getPosition().x + button_rectangle.width/2) && // mouse is to the left of right button boarder
        (mouse_x > button.getPosition().x - button_rectangle.width/2)            && // mouse is to the right of left button boarder
        (mouse_y < button.getPosition().y + button_rectangle.height/2) &&
        (mouse_y > button.getPosition().y - button_rectangle.height/2)) {
            return true;
    }
    return false;
}

void Button::highlight(sf::Color active_background_color, sf::Color active_text_color, sf::RenderWindow &window) {
    
    if (is_mouse_over(window)) {
        set_background_color(active_background_color);
        set_label_color(active_text_color);
        button.setOutlineColor(active_text_color);
    }
    else {
        set_background_color(background_color);
        set_label_color(text_color);
        button.setOutlineColor(text_color);
    }
}

void Button::click(sf::Event::EventType event_type) {
    if (event_type == sf::Event::EventType::MouseButtonPressed ) {
        sound.play();
        button.setOutlineThickness(10);
    }
    else if (event_type == sf::Event::EventType::MouseButtonReleased) {
        button.setOutlineThickness(5);
    }
    else {
        printf("Button click error: wrong event");
    }
}