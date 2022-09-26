#ifndef __VECTOR__
#define __VECTOR__

#include <SFML/System.hpp>

class Vector2d
{
    public:
    
        double x;
        double y;
        
        Vector2d() : x(0), y(0) {};
        Vector2d(double x, double y) : x(x), y(y) {};
        Vector2d(const sf::Vector2f &sfVec) : x(sfVec.x), y(sfVec.y) {};
    
        Vector2d operator+(const Vector2d &other);
        Vector2d operator-();
        Vector2d operator-(const Vector2d &other);
        Vector2d operator*(double multiplier);
        Vector2d operator/(double divider);
    
        Vector2d &operator+=(const Vector2d &other);
        Vector2d &operator-=(const Vector2d &other);
        Vector2d &operator*=(double multiplier);
        Vector2d &operator/=(double divider);
    
        double length();
        void normalize();
    
        sf::Vector2f toSFMLVector();

};

#endif