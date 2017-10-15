#ifndef IMAGE_H
#define IMAGE_H

#include <SFML/Graphics.hpp>

class Image
{
public:
	Image(std::string&);
	~Image();

	void draw(sf::RenderWindow&);
private:
	sf::Texture texture;
	sf::Sprite sprite;

};

#endif