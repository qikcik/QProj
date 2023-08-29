#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

namespace qui
{
    struct Rect
    {
        float x {0};
        float y {0};
        float w {0};
        float h {0};
    };

    class Widget
    {
    public:
        virtual ~Widget() = default;

        virtual sf::Vector2f getSize() const = 0;

        virtual void laidOut(Rect) = 0;
        virtual void draw(sf::RenderTarget&) = 0;
    };
}