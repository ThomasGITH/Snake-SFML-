#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include<iostream>
#include<SFML\Graphics.hpp>
#include<vector>

//Project made/written by Thomas Linssen

//Vector2 struct for cleaner-looking code
struct Vector2
{
	int x, y;
	Vector2(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
};

sf::Texture body, apple, background;
const Vector2 gridDimensions(25,16);

std::vector<Vector2> tail;

Vector2 position(0,0), direction(1,0), spriteBounds(0,0), windowDimensions(0,0);

bool enableGrid = true;

int main()
{
	apple.loadFromFile("images/apple.jpg");
	body.loadFromFile("images/snakeHead.jpg");
	background.loadFromFile("images/blankImage.jpg");
	
	sf::Sprite head(body), appleSprite(apple);
	head.scale(0.075f, 0.075);
	appleSprite.scale(0.075, 0.075);

	spriteBounds.x = std::round(head.getGlobalBounds().width);
	spriteBounds.y = std::round(head.getGlobalBounds().height);

	windowDimensions.x = gridDimensions.x * spriteBounds.x;
	windowDimensions.y = gridDimensions.y * spriteBounds.y;

	//For more/better randomization
	srand(time(NULL));

	sf::RenderWindow window(sf::VideoMode(windowDimensions.x, windowDimensions.y), "Thomas's Snake");
	window.setKeyRepeatEnabled(false);
	window.setFramerateLimit(60);

	Vector2 applePos(round((rand() % gridDimensions.x)) * spriteBounds.x, round((rand() % gridDimensions.y)) * spriteBounds.y);
	appleSprite.setPosition(applePos.x, applePos.y);

	tail.push_back(Vector2(position.x, position.y));

	sf::Clock clock;
	sf::Time time;

	while (window.isOpen())
	{
		//Move in ticks
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

		//Die when head collides with tail
		for (size_t i = 0; i < tail.size(); i++)
		{
			if (tail[i].x == position.x && tail[i].y == position.y)
			{
				if ((i != 0) && (i != 1))
				{
					window.close();
				}
			}
		}

		//Pickup & spawn apple
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

			}
		}

		//Die when out of window
		if (position.x < 0 || position.x >= windowDimensions.x || position.y < 0 || position.y >= windowDimensions.y)
		{
			window.close();
		}

		sf::Event event;

		//Checking-booleans to prevent bugs
		bool hasPressed = false, isDone = false;
		while (window.pollEvent(event))
		{

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && !hasPressed)
			{ direction = direction.x != 1 ? Vector2(-1, 0) : direction; hasPressed = true; }
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && !hasPressed)
			{ direction = direction.x != -1 ? Vector2(1, 0) : direction; hasPressed = true; }
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && !hasPressed)
			{ direction = direction.y != 1 ? Vector2(0, -1) : direction; hasPressed = true; }
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && !hasPressed)
			{ direction = direction.y != -1 ? Vector2(0, 1) : direction; hasPressed = true; }

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::G) && !isDone)
			{
				enableGrid = !enableGrid;
				isDone = true;
			}

			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
			{
				window.close();
			}
		}

		window.clear(sf::Color(0, 0, 1, 1));

		//Draws background-grid
		if (enableGrid){
			for (size_t i = 0; i < gridDimensions.y; i++)
			{
				for (size_t j = 0; j < gridDimensions.x; j++)
				{
					sf::Sprite backgroundSprite(background);
					backgroundSprite.setPosition(j * spriteBounds.x, i * spriteBounds.y);
					backgroundSprite.scale(0.075, 0.075);
					window.draw(backgroundSprite);
				}
			}
		}

		//Draws the snake
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
