#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>

#include "json.hpp"

using json = nlohmann::json;

bool isColliding(sf::RectangleShape rectangle, sf::CircleShape cercle)
{
	float cerclePosX = cercle.getPosition().x + cercle.getRadius();
	float cerclePosY = cercle.getPosition().y + cercle.getRadius();
	system("cls");
	std::cout << "cercle.getPosition().x : " << cercle.getPosition().x + cercle.getRadius() << "\n"
		<< "cercle.getPosition().y : " << cercle.getPosition().y + cercle.getRadius() << "\n"
		<< "cercle.getRadius() : " << cercle.getRadius() << "\n"
		<< "rectangle.getPosition().x : " << rectangle.getPosition().x << "\n"
		<< "rectangle.getPosition().y : " << rectangle.getPosition().y << "\n"
		<< "rectangle.getSize().x : " << rectangle.getSize().x << "\n"
		<< "rectangle.getSize().y : " << rectangle.getSize().y << "\n";

	double deltaX = cerclePosX - std::max(rectangle.getPosition().x, std::min(cerclePosX, rectangle.getPosition().x + rectangle.getSize().x));
	double deltaY = cerclePosY - std::max(rectangle.getPosition().x, std::min(cerclePosY, rectangle.getPosition().y + rectangle.getSize().y));

	return ((deltaX * deltaX + deltaY * deltaY) < (cercle.getRadius() * cercle.getRadius()));
}

int main()
{
	std::ifstream ifs("data/elements.json");
	json data;
	if (ifs.is_open())
		data = json::parse(ifs);
	else
	{
		std::cerr << "Unable to read the json file.\n";
		system("pause");
		return EXIT_FAILURE;
	}
	
	sf::RenderWindow window(sf::VideoMode(data["windows"]["width"], data["windows"]["height"]), "SFML works!");
	window.setFramerateLimit(data["windows"]["framerateLimit"]);

	sf::CircleShape circle(data["circleShape"]["radius"].get<float>());
	circle.setFillColor(sf::Color::Color(data["circleShape"]["color"]["red"], data["circleShape"]["color"]["green"], data["circleShape"]["color"]["blue"]));

	sf::RectangleShape rectangle(sf::Vector2f(data["squareShape"]["width"], data["squareShape"]["width"]));
	rectangle.setFillColor(sf::Color::Color(data["squareShape"]["color"]["red"], data["squareShape"]["color"]["green"], data["squareShape"]["color"]["blue"]));
	rectangle.setPosition((data["windows"]["width"].get<float>() - data["squareShape"]["width"].get<float>())/2, (data["windows"]["height"].get<float>() - data["squareShape"]["width"].get<float>())/2);

	float speed = data["windows"]["speed"];
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		sf::Vector2f delta_move_circle;
		//Manage inputs
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			if(!(circle.getPosition().x - 1.0f < 0))
				delta_move_circle.x -= 1.0f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			if (!(circle.getPosition().x + circle.getRadius() * 2 + 1.0f > data["windows"]["width"]))
				delta_move_circle.x += 1.0f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			if (!(circle.getPosition().y - 1.0f < 0))
				delta_move_circle.y -= 1.0f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			if (!(circle.getPosition().y + circle.getRadius() * 2 + 1.0f > data["windows"]["height"]))
				delta_move_circle.y += 1.0f;
		}

		circle.setPosition(circle.getPosition() + speed * delta_move_circle);
		if (isColliding(rectangle, circle)) 
		{
			circle.setFillColor(sf::Color::Red);
			rectangle.setFillColor(sf::Color::Red);
		}
		else
		{
			circle.setFillColor(sf::Color::Color(data["circleShape"]["color"]["red"].get<int>(), data["circleShape"]["color"]["green"].get<int>(), data["circleShape"]["color"]["blue"].get<int>()));
			rectangle.setFillColor(sf::Color::Color(data["squareShape"]["color"]["red"].get<int>(), data["squareShape"]["color"]["green"].get<int>(), data["squareShape"]["color"]["blue"].get<int>()));
		}

		window.clear();

		window.draw(circle);
		window.draw(rectangle);

		window.display();
	}

	return 0;
}