#include <cassert>
#include <math.h>
#include "Coordinate_plane.hpp"

#define MAX_LABEL_LEN 20

static const float ARROW_WIDTH  = 7.0f;
static const float ARROW_HEIGHT = 10.0f;

static const float GRAPH_THICKNESS  = 2.0f;
static const float AXIS_THICKNESS   = 1.0f;
static const float VECTOR_THICKNESS = 1.0f;

static const float LABEL_Y_HORIZ_INDENT     = 7.0f;
static const float LABEL_X_VERT_INDENT      = 1.0f;
static const float LABEL_BOTTOM_VERT_INDENT = -25.0f;
static const float LABEL_LEFT_HORIZ_INDENT  = 7.0f;
static const float LABEL_RIGHT_HORIZ_INDENT = -7.0f;

static const double GRAPH_STEP = 1.0;

static const double DEFAULT_VIEWPORT_SIZE = 200.0;
static const double DEFAULT_SCALE_FACTOR  = 1 / 10.0;

static const int LABEL_FONT_SIZE = 20;
static const int DOT_VERT_COUNT  = 10;

static const int MAX_VEC_COUNT = 128;

static const double MOVABLE_VECTORS_SCALE = 0.015;

static const sf::Color PLOT_COLOR = sf::Color::Black;
static const sf::Color AXIS_COLOR = sf::Color::Black;
static const sf::Color VEC_COLOR  = sf::Color(98, 153, 57);

static const sf::Color plane_bg_green = sf::Color(221, 255, 188);
static const sf::Color plane_frame_green = sf::Color(145, 199, 136);
static const sf::Color dark_green = sf::Color(82, 115, 77);

static double defaultPlotFunc(double x);


Coordinate_plane::Coordinate_plane()
{
    vectors = new Vector_with_origin[MAX_VEC_COUNT];
    vectors_num = 0;

    background.setFillColor(plane_bg_green);
    background.setOutlineColor(plane_frame_green);
    background.setOutlineThickness(4);

    text_drawer.setFillColor(AXIS_COLOR);
    text_drawer.setCharacterSize(LABEL_FONT_SIZE);

  
    setSize(sf::Vector2f(DEFAULT_VIEWPORT_SIZE, DEFAULT_VIEWPORT_SIZE));
    setViewportOrigin(Vector2d(0, 0));
    setScaleFactor(DEFAULT_SCALE_FACTOR);
}

Coordinate_plane::~Coordinate_plane()
{
    delete[] vectors;
}

void Coordinate_plane::setFont(sf::Font &font)
{
    text_drawer.setFont(font);
}

void Coordinate_plane::setViewportOrigin(Vector2d origin)
{
    coordinates_origin = origin;
}

void Coordinate_plane::setSize(sf::Vector2f size)
{
    plane_size = size;
    background.setSize(size);
}

void Coordinate_plane::setCenterRelRoWindow(Vector2d center) {
    center_rel_to_window = center;
}

Vector2d Coordinate_plane::getCenterRelRoWindow() {
    return center_rel_to_window;
}

void Coordinate_plane::setScaleFactor(double scaleFactor)
{
    this->scaleFactor = scaleFactor;
}

void Coordinate_plane::moveViewport(Vector2d offset)
{
    coordinates_origin += offset * scaleFactor;
}

void Coordinate_plane::addVector(Vector_with_origin vec)
{
    if (vectors_num >= MAX_VEC_COUNT)
        return;

    vectors[vectors_num++] = vec;
}

void Coordinate_plane::draw(sf::RenderWindow &canvas, sf::Vector2f plane_top_left_corner)
{
    // Precalculation of some common-used values

    plotSize = Vector2d(plane_size) * scaleFactor;
    plotOffset = coordinates_origin + Vector2d(-plotSize.x, plotSize.y) / 2.0;

    background.setPosition(plane_top_left_corner);
    canvas.draw(background);

    draw_axes   (canvas, plane_top_left_corner);
    draw_vectors(canvas, plane_top_left_corner);

    if (show_coordinates_flag) {
        draw_coordinates(canvas);
        printf("(%.1f, %.1f)", vectors[0].vector.x, vectors[0].vector.y);
    }    

    vectors_num = 0;
}

void Coordinate_plane::draw_axes(sf::RenderWindow &canvas, sf::Vector2f top_left_corner) // position is top-left corner
{
    char buf[MAX_LABEL_LEN + 1] = {0};

    
        double axes_vertical_offset = plane_size.y / plotSize.y * plotOffset.y;
    
    
        // X axis must be drawn
        
        sf::Vector2f XAxisBegin = top_left_corner + sf::Vector2f(0             , axes_vertical_offset);
        sf::Vector2f XAxisEnd   = top_left_corner + sf::Vector2f(plane_size.x, axes_vertical_offset);

        draw_arrow(canvas, XAxisBegin, XAxisEnd, AXIS_THICKNESS, AXIS_COLOR);

        // Right X label

        snprintf(buf, MAX_LABEL_LEN, "%d", (int)(plotOffset.x + plotSize.x));
        text_drawer.setString(buf);

        sf::FloatRect textBound = text_drawer.getLocalBounds();

        sf::Vector2f rightLabelPos = top_left_corner +
                                     sf::Vector2f(plane_size.x - textBound.width + LABEL_RIGHT_HORIZ_INDENT,
                                                  axes_vertical_offset + LABEL_X_VERT_INDENT);

        text_drawer.setPosition(rightLabelPos);
        canvas.draw(text_drawer);

        // Left X label

        snprintf(buf, MAX_LABEL_LEN, "%d", (int)plotOffset.x);
        text_drawer.setString(buf);

        sf::Vector2f leftLabelPos = top_left_corner +
                                    sf::Vector2f(LABEL_LEFT_HORIZ_INDENT,
                                                 axes_vertical_offset + LABEL_X_VERT_INDENT);
        
        text_drawer.setPosition(leftLabelPos);
        canvas.draw(text_drawer); 
    

    double viewportYAxisOffs = plane_size.y / plotSize.y * (-plotOffset.x);
    
    
        // Y axis must be drawn
        
        sf::Vector2f YAxisBegin = top_left_corner + sf::Vector2f(viewportYAxisOffs, plane_size.y);
        sf::Vector2f YAxisEnd   = top_left_corner + sf::Vector2f(viewportYAxisOffs, 0);

        draw_arrow(canvas, YAxisBegin, YAxisEnd, AXIS_THICKNESS, AXIS_COLOR);

        // Top Y label

        snprintf(buf, MAX_LABEL_LEN, "%d", (int)plotOffset.y);
        text_drawer.setString(buf);

        sf::Vector2f topLabelPos = top_left_corner + sf::Vector2f(viewportYAxisOffs + LABEL_Y_HORIZ_INDENT, 0);


        text_drawer.setPosition(topLabelPos);
        canvas.draw(text_drawer);

        // Bottom Y label

        snprintf(buf, MAX_LABEL_LEN, "%d", (int)(plotOffset.y - plotSize.y));
        text_drawer.setString(buf);

        sf::Vector2f bottomLabelPos = top_left_corner +
                                      sf::Vector2f(viewportYAxisOffs + LABEL_Y_HORIZ_INDENT,
                                                   plane_size.y + LABEL_BOTTOM_VERT_INDENT);

        text_drawer.setPosition(bottomLabelPos);
        canvas.draw(text_drawer); 
    
}

void Coordinate_plane::draw_coordinates(sf::RenderWindow &canvas) {

    char buf[MAX_LABEL_LEN + 1] = {0};


        snprintf(buf, MAX_LABEL_LEN, "(%.1f, %.1f)", vectors[0].vector.x, vectors[0].vector.y);
        text_drawer.setString(buf);

            sf::FloatRect textBound = text_drawer.getLocalBounds();
            sf::Vector2f rightLabelPos = sf::Vector2f(getCenterRelRoWindow().x - textBound.width/2, getCenterRelRoWindow().y - textBound.height/2);

            if (vectors[0].vector.x >= 0) {
                if (vectors[0].vector.y >= 0) {
                    // I 
                    rightLabelPos += sf::Vector2f(  vectors[0].vector.length()/2/ MOVABLE_VECTORS_SCALE,
                                                  - vectors[0].vector.length()/2 / MOVABLE_VECTORS_SCALE );

                }
                else {
                    // IV
                     rightLabelPos += sf::Vector2f( vectors[0].vector.length()/2/ MOVABLE_VECTORS_SCALE,
                                                    vectors[0].vector.length()/2 / MOVABLE_VECTORS_SCALE );

                }
            }
            else {
                if (vectors[0].vector.y >= 0) {
                    // II 
                    rightLabelPos += sf::Vector2f(- vectors[0].vector.length()/2/ MOVABLE_VECTORS_SCALE,
                                                 - vectors[0].vector.length()/2 / MOVABLE_VECTORS_SCALE );

                }
                else {
                    // III
                    rightLabelPos += sf::Vector2f(- vectors[0].vector.length()/2/ MOVABLE_VECTORS_SCALE,
                                                   vectors[0].vector.length()/2 / MOVABLE_VECTORS_SCALE );

                }
            }
            
            text_drawer.setPosition(rightLabelPos);
            
            canvas.draw(text_drawer);

}

void Coordinate_plane::draw_vectors(sf::RenderWindow &canvas, sf::Vector2f plane_top_left_corner)
{
    for (int i = 0; i < vectors_num; i++)
    {
        Vector_with_origin currVec = vectors[i];

        Vector2d vecStart = currVec.origin;
        Vector2d vecEnd   = vecStart + currVec.vector;

        Vector2d startOffs = Vector2d(vecStart.x - plotOffset.x, plotOffset.y - vecStart.y);
        Vector2d endOffs   = Vector2d(vecEnd.x   - plotOffset.x, plotOffset.y - vecEnd.y);

        sf::Vector2f startViewportOffs = sf::Vector2f(plane_size.x  / plotSize.x  * startOffs.x,
                                                      plane_size.y  / plotSize.y  * startOffs.y);

        sf::Vector2f endViewportOffs = sf::Vector2f(plane_size.x  / plotSize.x  * endOffs.x,
                                                    plane_size.y  / plotSize.y  * endOffs.y);

        if (startViewportOffs.x < 0 || startViewportOffs.x > plane_size.x ||
            startViewportOffs.y < 0 || startViewportOffs.y > plane_size.y ||
            endViewportOffs.x   < 0 || endViewportOffs.x   > plane_size.x ||
            endViewportOffs.y   < 0 || endViewportOffs.y   > plane_size.y)
        {
            continue;
        }

        sf::Vector2f vecStartViewport = plane_top_left_corner + startViewportOffs;
        sf::Vector2f vecEndViewport   = plane_top_left_corner + endViewportOffs;

        draw_arrow(canvas, vecStartViewport, vecEndViewport, VECTOR_THICKNESS, VEC_COLOR);
    }
}

void Coordinate_plane::draw_line(sf::RenderWindow &canvas,
                    sf::Vector2f line_begin, 
                    sf::Vector2f line_end, 
                    float thickness, 
                    sf::Color color)
{
    sf::Vector2f line_vec = line_end - line_begin;
    float line_vec_len = sqrtf(line_vec.x * line_vec.x + line_vec.y * line_vec.y);

    sf::Vector2f normal = sf::Vector2f(-line_vec.y, line_vec.x) / line_vec_len;

    sf::Vector2f lower_point = line_begin - normal * thickness;
    sf::Vector2f higher_point = line_begin + normal * thickness;
    sf::Vector2f vert3 = line_end + normal * thickness;
    sf::Vector2f vert4 = line_end - normal * thickness;
    
    convex_drawer.setFillColor(color);

    convex_drawer.setPointCount(4);
    convex_drawer.setPoint(0, lower_point);
    convex_drawer.setPoint(1, higher_point);
    convex_drawer.setPoint(2, vert3);
    convex_drawer.setPoint(3, vert4);

    canvas.draw(convex_drawer);
}

void Coordinate_plane::draw_arrow(sf::RenderWindow &canvas,
                     sf::Vector2f line_begin, 
                     sf::Vector2f line_end, 
                     float thickness, 
                     sf::Color color)
{
    draw_line(canvas, line_begin, line_end, thickness, color);
    draw_arrowhead(canvas, line_begin, line_end, thickness, color);

    canvas.draw(convex_drawer);
}

void Coordinate_plane::draw_arrowhead(sf::RenderWindow &canvas,
                     sf::Vector2f line_begin, 
                     sf::Vector2f arrow_tip, 
                     float thickness, 
                     sf::Color color)
{
    sf::Vector2f line_vec = arrow_tip - line_begin;

    float line_vec_len = sqrtf(line_vec.x * line_vec.x + line_vec.y * line_vec.y); // в тех функциях, которые я писала есть конструктор из сфмл и вычисление длинны

    sf::Vector2f line_vec_norm = line_vec / line_vec_len;
    sf::Vector2f normal = sf::Vector2f(-line_vec_norm.y, line_vec_norm.x);

    sf::Vector2f lower_point = arrow_tip - line_vec_norm * ARROW_HEIGHT - normal * ARROW_WIDTH;
    sf::Vector2f higher_point = arrow_tip - line_vec_norm * ARROW_HEIGHT + normal * ARROW_WIDTH;

    convex_drawer.setFillColor(color);

    convex_drawer.setPointCount(3);
    convex_drawer.setPoint(0, arrow_tip);
    convex_drawer.setPoint(1, lower_point);
    convex_drawer.setPoint(2, higher_point);
}
