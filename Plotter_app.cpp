#include <stdexcept>
#include <math.h>

#include "Plotter_app.hpp"
#include "Matrix3d.hpp"

static const double ROTATION_SPEED_SECONDS = 1;
static const double ROTATION_SPEED_MINUTES = 0.17;

static const double WINDOW_WIDTH  = 1280;
static const double WINDOW_HEIGHT =  720;
static const double COORDINATE_SQUARE_WIDTH = 400;
static const double PASSEPARTOUT = 160;


static const double MOVABLE_VECTORS_SCALE = 0.015;
static const double CLOCK_VECTORS_SCALE = 1;

static const int ANTIALISING_LVL = 10;

static const sf::Color bg_yellow = sf::Color(254, 255, 222);

Plotter_app::Plotter_app()
{

    sf::ContextSettings context;
    context.antialiasingLevel = ANTIALISING_LVL; // сглаживание

    window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Plotter", sf::Style::Default, context);

    font.loadFromFile("AnonymousPro-Bold.ttf");

    plane_movable.setFont(font);
    plane_movable.setSize(sf::Vector2f(COORDINATE_SQUARE_WIDTH, COORDINATE_SQUARE_WIDTH));
    plane_movable.setScaleFactor(MOVABLE_VECTORS_SCALE);
    plane_movable.setCenterRelRoWindow(Vector2d(PASSEPARTOUT * 2 + COORDINATE_SQUARE_WIDTH * 1.5,
                                                PASSEPARTOUT     + COORDINATE_SQUARE_WIDTH * 0.5));

    plane_clock.setFont(font);
    plane_clock.setSize(sf::Vector2f(COORDINATE_SQUARE_WIDTH, COORDINATE_SQUARE_WIDTH));
    plane_clock.setScaleFactor(CLOCK_VECTORS_SCALE);
    plane_clock.setCenterRelRoWindow(Vector2d(PASSEPARTOUT + COORDINATE_SQUARE_WIDTH * 0.5, 
                                              PASSEPARTOUT + COORDINATE_SQUARE_WIDTH * 0.5)); //! в моем файле есть определение одним числом


}

void Plotter_app::run()
{
    sf::Clock clock;
    Vector2d vector_end(1,1);

    bool mouse_pressed_flag = false;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            //plane_movable.show_coordinates_flag = false;
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                if (mouse_pressed_flag) {
                    mouse_pressed_flag = false;
                    plane_movable.show_coordinates_flag = true;
                }
            }
            

            if ((sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right))
                && event.type == sf::Event::MouseMoved 
                && is_presed_near()) 
            {
                mouse_pressed_flag = true;
                vector_end = calc_vector_end_for_mouse(sf::Mouse::getPosition(window));
               plane_movable.show_coordinates_flag = false;
            }

            
            
        }

        float time_passed = clock.restart().asSeconds();

        calc_vectors(time_passed, vector_end);

        window.clear(bg_yellow);
        plane_clock.draw(window, sf::Vector2f(PASSEPARTOUT, PASSEPARTOUT));
        plane_movable.draw(window, sf::Vector2f(PASSEPARTOUT * 2 + COORDINATE_SQUARE_WIDTH, PASSEPARTOUT));
            
        window.display();

    }
}

bool cmp_double (double x, double y, double epsilon = 0.0000001f)
{
   return (fabs(x - y) < epsilon);
}

bool Plotter_app::is_presed_near() 
{
    Vector2d mouse_pos_in_scale = calc_vector_end_for_mouse(sf::Mouse::getPosition(window));
    
    return
       cmp_double(mouse_pos_in_scale.x, plane_movable.vectors->vector.x, 0.3)
    && cmp_double(mouse_pos_in_scale.y, plane_movable.vectors->vector.y, 0.3);
}

Vector2d Plotter_app::calc_vector_end_for_mouse(sf::Vector2i mouse_position) 
{
    return Vector2d(  ((double)mouse_position.x - plane_movable.getCenterRelRoWindow().x) * MOVABLE_VECTORS_SCALE, 
                    - ((double)mouse_position.y - plane_movable.getCenterRelRoWindow().y) * MOVABLE_VECTORS_SCALE);
}

void Plotter_app::calc_vectors(double time_passed, Vector2d vector_end)
{
    vector_angle_sec += - ROTATION_SPEED_SECONDS * time_passed;
    Matrix3d transformSec  = Matrix3d::rotationMatrix(vector_angle_sec);
    plane_clock.addVector(Vector_with_origin(Vector2d(0,0), transformSec * Vector2d(0,50)));

    vector_angle_min += - ROTATION_SPEED_MINUTES * time_passed;
    Matrix3d transformMin  = Matrix3d::rotationMatrix(vector_angle_min);
    plane_clock.addVector(Vector_with_origin(Vector2d(0,0), transformMin * Vector2d(0,70)));

    plane_movable.addVector(Vector_with_origin(Vector2d(0.0, 0.0), vector_end));
   
}
