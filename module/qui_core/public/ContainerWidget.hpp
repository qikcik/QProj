#pragma once
#include "Widget.hpp"


namespace qui
{
    enum class Orientation {
        Vertical, Horizontal
    };

    enum class JustifyContent {
        Start, End, Center, SpaceBetween
    };

    class ContainerWidget : public Widget
    {
    public:
        ContainerWidget* appendChild(std::shared_ptr<Widget>);
        ContainerWidget* setOrientation(Orientation);
        ContainerWidget* setJustifyContent(JustifyContent);

        void laidOut(qui::Rect) override;
        void draw(sf::RenderTarget&) override;
        sf::Vector2f getSize() const override;

    protected:
        Orientation orientation {Orientation::Vertical};
        JustifyContent justifyContent {JustifyContent::Start};

        std::vector<std::shared_ptr<Widget>> children;
    };
}