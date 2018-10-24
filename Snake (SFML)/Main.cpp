#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include<iostream>
#include<SFML\Graphics.hpp>
#include<vector>

struct Vector2
{
public:
	int x, y;
	Vector2(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
};

sf::Texture body, apple;
const Vector2 gridDimensions(25,16);

std::vector<Vector2> tail;

Vector2 position(0,0), direction(1,0), spriteBounds(0,0), windowDimensions(0,0);

int main()
{
	apple.loadFromFile("images/apple.jpg");
	body.loadFromFile("images/snakeHead.jpg");
	
	sf::Sprite head(body), appleSprite(apple);
	head.scale(0.075f, 0.075);
	appleSprite.scale(0.075, 0.075);

	spriteBounds.x = std::round(head.getGlobalBounds().width);
	spriteBounds.y = std::round(head.getGlobalBounds().height);

	windowDimensions.x = gridDimensions.x * spriteBounds.x;
	windowDimensions.y = gridDimensions.y * spriteBounds.y;

	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode(windowDimensions.x, windowDimensions.y), "Thomas's Snake");
	window.setFramerateLimit(60);

	sf::Clock clock;
	sf::Time time;

	Vector2 applePos(round((rand() % gridDimensions.x)) * spriteBounds.x, round((rand() % gridDimensions.y)) * spriteBounds.y);
	appleSprite.setPosition(applePos.x, applePos.y);

	int length = 0;

	tail.push_back(Vector2(position.x, position.y));
	bool k = false;
	while (window.isOpen())
	{

		//MOVE IN TICKS
		time = clock.getElapsedTime();
		if (time.asSeconds() > 0.07f)
		{
			Vector2 lastPos(position.x, position.y);
			position.x += direction.x * spriteBounds.x;
			position.y += direction.y * spriteBounds.y;
			head.setPosition(position.x, position.y);

			for (size_t i = tail.size() - 1; i > 0; i--)
			{
				tail[i] = tail[i - 1];
			}

			tail[0] = position;

			clock.restart();
		}

		for (size_t i = 0; i < tail.size(); i++)
		{
			if (tail[i].x == position.x && tail[i].y == position.y)
			{
				if ((i != 0)&&(i != 1))
				{
					window.close();
				}
			}
		}

		for (int i = 0; i < tail.size(); i++)
		{
			if (tail[i].x == applePos.x && tail[i].y == applePos.y)
			{
				applePos = Vector2(round((rand() % gridDimensions.x)) * spriteBounds.x, round((rand() % gridDimensions.y)) * spriteBounds.y);
				appleSprite.setPosition(applePos.x, applePos.y);

				if (tail.size() == 0)
				{
					tail.push_back(Vector2(position.x, position.y));
				}
				else
				{
					tail.push_back(Vector2(tail[tail.size() - 1].x, tail[tail.size() - 1].y));
				}

				length += 1;
			}
		}

		//pickup apple
		if (position.x == applePos.x && position.y == applePos.y)
		{

		}

		//die when out of window
		if (position.x < 0 || position.x > windowDimensions.x || position.y < 0 || position.y > windowDimensions.y)
		{
			window.close();
		}

		sf::Event event;

		while (window.pollEvent(event))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) 
				{ direction = direction.x != 1 ? Vector2(-1, 0) : direction; }
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) 
				{ direction = direction.x != -1 ? Vector2(1, 0) : direction; }
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) 
				{ direction = direction.y != 1 ? Vector2(0, -1) : direction; }
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) 
				{ direction = direction.y != -1 ? Vector2(0, 1) : direction; }
			
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
			{
				window.close();
			}
		}

		window.clear(sf::Color(0,0,1,1));
		
		for (size_t i = 0; i < tail.size(); i++)
		{
			sf::Sprite part(body);
			part.setPosition(tail[i].x, tail[i].y);
			part.scale(0.075, 0.075);
			window.draw(part);
		}

		window.draw(appleSprite);

		window.display();
	}

	return 0;
}
