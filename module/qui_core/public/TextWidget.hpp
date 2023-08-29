#pragma once
#include "Widget.hpp"


namespace qui
{
    class TextWidget : public Widget
    {
    public:
        TextWidget* setFont(sf::Font&);
        TextWidget* setCharacterSize(unsigned int);
        TextWidget* setText(const std::string&);

        void laidOut(qui::Rect) override;
        void draw(sf::RenderTarget&) override;
        sf::Vector2f getSize() const override;

    protected:
        sf::Text textToDraw {};
    };
}