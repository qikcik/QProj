#include <SFML/Graphics.hpp>
#include "TextWidget.hpp"
#include "ContainerWidget.hpp"

using namespace qui;

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Window");
    sf::Font font {};
    font.loadFromFile("asset/consolas.ttf");

    auto widget = std::make_shared<ContainerWidget>();
    widget->setOrientation(Orientation::Vertical)->setJustifyContent(JustifyContent::Center);

    auto text0 = std::make_shared<TextWidget>();
    text0->setFont(font)
        ->setCharacterSize(24)
        ->setText("text");

    widget->appendChild(text0);

    auto text1 = std::make_shared<TextWidget>();
    text1->setFont(font)
            ->setCharacterSize(24)
            ->setText("dupa12");

    widget->appendChild(text1);

    sf::Clock deltaClock;
    float delta = 1 / 60.f;

    widget->laidOut({0,0,(float)window.getSize().x,(float)window.getSize().y});
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if(event.type == sf::Event::Resized)
            {
                sf::View view;
                view.reset(sf::FloatRect(
                        0,
                        0,
                        (float)window.getSize().x,
                        (float)window.getSize().y));
                window.setView(view);
                widget->laidOut({0,0,(float)window.getSize().x,(float)window.getSize().y});
            }
        }
        window.clear(sf::Color{33,33,33,255});
        widget->draw(window);
        window.display();

        delta = deltaClock.restart().asSeconds();
        //std::cout << std::to_string(1/delta) << std::endl;
    }
}