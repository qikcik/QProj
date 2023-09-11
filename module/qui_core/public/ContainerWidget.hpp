#pragma once
#include "Widget.hpp"


namespace qui
{
    enum class Orientation {
        Vertical, Horizontal
    };

    enum class JustifyMainAxis {
        Start, End, Center, SpaceBetween
    };

    class ContainerWidget : public Widget
    {
    public:
        ContainerWidget* appendChild(std::shared_ptr<Widget>);
        ContainerWidget* setOrientation(Orientation);
        ContainerWidget* setJustifyAxis(JustifyMainAxis);

        void laidOut(qui::Rect) override;
        void draw(sf::RenderTarget&) override;
        sf::Vector2f getSize() const override;

    protected:
        Orientation orientation {Orientation::Vertical};
        JustifyMainAxis justifyMainAxis {JustifyMainAxis::Start};

        std::vector<std::shared_ptr<Widget>> children;
    };
}