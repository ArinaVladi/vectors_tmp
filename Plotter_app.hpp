#ifndef __PLOTTER_APP__
#define __PLOTTER_APP__

#include <SFML/Graphics.hpp>

#include "Coordinate_plane.hpp"

class Plotter_app
    {
        public:
            Plotter_app();
            void run();

        private:

            void calc_vectors(double time_passed, Vector2d vector_end);
            bool is_presed_near();
            Vector2d calc_vector_end_for_mouse(sf::Vector2i mouse_position);

            Coordinate_plane plane_clock;
            Coordinate_plane plane_movable;

            sf::RenderWindow window;
            sf::Font font;

            double vector_angle_sec;
            double vector_angle_min;

    };

#endif