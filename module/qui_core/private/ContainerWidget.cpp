#include "ContainerWidget.hpp"
#include <numeric>

using namespace qui;

ContainerWidget* ContainerWidget::appendChild(std::shared_ptr<Widget> inWidget)
{
    children.push_back(std::move(inWidget));
    return this;
}

ContainerWidget* ContainerWidget::setOrientation(Orientation inOrientation)
{
    orientation = inOrientation;
    return this;
}

ContainerWidget* ContainerWidget::setJustifyContent(JustifyContent inJustifyContent)
{
    justifyContent = inJustifyContent;
    return this;
}

void ContainerWidget::laidOut(Rect inRect)
{
    //TODO: Implement overflow behaviour - currently undefined

    sf::Vector2f current {0,0};

    sf::Vector2f summedSize {0,0};
    for(auto& childIt : children)
        summedSize += childIt->getSize();

    if(justifyContent == JustifyContent::Start)
    {
        if(orientation == Orientation::Vertical)
            ;
        else
            ;
    }
    else if(justifyContent == JustifyContent::End)
    {
        if(orientation == Orientation::Vertical)
            current.x = inRect.w - summedSize.x;
        else
            current.y = inRect.h - summedSize.y;
    }
    else if(justifyContent == JustifyContent::Center)
    {
        if(orientation == Orientation::Vertical)
            current.x = (inRect.w - summedSize.x) / 2;
        else
            current.y = (inRect.h - summedSize.y) / 2;
    }
    else if(justifyContent == JustifyContent::SpaceBetween)
    {
        if(orientation == Orientation::Vertical)
            current.x = (inRect.w - summedSize.x) / 2;
        else
            current.y = (inRect.h - summedSize.y) / 2;
    }

    for(auto& childIt : children)
    {
        auto size = childIt->getSize();
        childIt->laidOut({inRect.x+current.x,inRect.y+current.y,size.x,size.y});

        if(orientation == Orientation::Vertical)
            current.x += size.x;
        else
            current.y += size.y;
    }
}

void ContainerWidget::draw(sf::RenderTarget& target)
{
    for(auto& childIt : children)
    {
        childIt->draw(target);
    }
}

sf::Vector2f ContainerWidget::getSize() const
{
    return sf::Vector2f();
}
