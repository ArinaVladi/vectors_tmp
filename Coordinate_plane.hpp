#ifndef __PLANE__
#define __PLANE__

#include <SFML/Graphics.hpp>

#include "Vector2d.hpp"

struct Vector_with_origin
{
    Vector_with_origin() : origin(Vector2d(0, 0)), vector(Vector2d(0, 0)) {};
    Vector_with_origin(Vector2d origin, Vector2d vector) : origin(origin), vector(vector) {};

    Vector2d origin;
    Vector2d vector;
};

class Coordinate_plane
{
    public:

        Coordinate_plane();
        ~Coordinate_plane();

        void setFont(sf::Font &font);
        void setViewportOrigin(Vector2d origin);
        void setSize(sf::Vector2f size);
        void setScaleFactor(double scaleFactor);
        void setCenterRelRoWindow(Vector2d center);
        Vector2d getCenterRelRoWindow();
        void moveViewport(Vector2d offset);
        void scale(double scale);
        void addVector(Vector_with_origin vec);
        void draw(sf::RenderWindow &canvas, sf::Vector2f plane_top_left_corner);

        bool show_coordinates_flag = false;

        Vector_with_origin *vectors;
        int vectors_num;
        void draw_coordinates(sf::RenderWindow &canvas); //!!!

    private:

        void draw_axes   (sf::RenderWindow &canvas, sf::Vector2f plane_top_left_corner);
        void draw_vectors(sf::RenderWindow &canvas, sf::Vector2f plane_top_left_corner);

        void draw_line(sf::RenderWindow &canvas,
                      sf::Vector2f p1, sf::Vector2f p2, float thickness, sf::Color color);

        void draw_arrow(sf::RenderWindow &canvas,
                        sf::Vector2f p1, 
                        sf::Vector2f p2, 
                        float thickness, 
                        sf::Color color);

        void draw_arrowhead(sf::RenderWindow &canvas,
                            sf::Vector2f p1, 
                            sf::Vector2f p2, 
                            float thickness, 
                            sf::Color color);  

        

        /// Viewport size
        sf::Vector2f plane_size;

        /// Viewport center coordinates
        Vector2d coordinates_origin;

        /// SFML-to-plot coordinate multiplier
        double scaleFactor;

        sf::RectangleShape background;
        sf::ConvexShape    convex_drawer;
        sf::Text           text_drawer;

        // Some derivative values which are common-used

        /// Size of a plot fragment visible through viewport
        Vector2d plotSize;
        /// top-left corner
        Vector2d plotOffset;
        Vector2d center_rel_to_window;
};

#endif