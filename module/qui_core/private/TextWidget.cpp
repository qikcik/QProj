#include "TextWidget.hpp"
using namespace qui;

TextWidget* TextWidget::setFont(sf::Font& inFont)
{
    textToDraw.setFont(inFont);
    return this;
}

TextWidget* TextWidget::setCharacterSize(unsigned int inSize)
{
    textToDraw.setCharacterSize(inSize);
    return this;
}

TextWidget* TextWidget::setText(const std::string& inText)
{
    textToDraw.setString(inText);
    return this;
}

void TextWidget::laidOut(Rect inRect)
{
    textToDraw.setOrigin(textToDraw.getLocalBounds().left, textToDraw.getLocalBounds().top);
    textToDraw.setPosition(inRect.x,inRect.y);
}

void TextWidget::draw(sf::RenderTarget& target)
{
    target.draw(textToDraw);
}

sf::Vector2f TextWidget::getSize() const
{
    return {textToDraw.getGlobalBounds().width,textToDraw.getGlobalBounds().height};
}
