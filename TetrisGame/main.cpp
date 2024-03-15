#include <SFML/Graphics.hpp>
#include <ctime>
#include<stdio.h>
#include<iostream>

int main() {
	const int fieldWidth = 10;
	const int fieldHeight = 20;

	enum class Type {
		None,
		O,
		I,
		L,//�s�WL�Xù�����
		J,//�s�WJ�Xù�����
		Z,//�s�WZ�Xù�����
		S,//�s�WS�Xù�����
		T,//�s�WT�Xù�����
		
	};

	Type field[fieldWidth][fieldHeight] = {};

	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("background.png")) {
		return EXIT_FAILURE;
	}

	sf::Texture yellowTexture;
	if (!yellowTexture.loadFromFile("yellow.png")) {
		return EXIT_FAILURE;
	}

	sf::Texture lightBlueTexture;
	if (!lightBlueTexture.loadFromFile("light_blue.png")) {
		return EXIT_FAILURE;
	}

	sf::Texture greenTexture;
	if (!greenTexture.loadFromFile("green.png")) {
		return EXIT_FAILURE;
	}

	//�s�W�Xù������C��K��
	sf::Texture redTexture;
	if (!redTexture.loadFromFile("red.png")) {
		return EXIT_FAILURE;
	}

	//�s�W�Xù������C��K��
	sf::Texture orangeTexture;
	if (!orangeTexture.loadFromFile("orange.png")) {
		return EXIT_FAILURE;
	}
	//�s�W�Xù������C��K��
	sf::Texture blueTexture;
	if (!blueTexture.loadFromFile("blue.png")) {
		return EXIT_FAILURE;
	}
	//�s�W�Xù������C��K��
	sf::Texture purpleTexture;
	if (!purpleTexture.loadFromFile("purple.png")) {
		return EXIT_FAILURE;
	}


	std::map<Type, std::vector<std::vector<sf::Vector2i>>> shapes = {
		{
			Type::O,
			{
			// O:     OO    (0,-1) (1,-1)
			//        OO    (0, 0) (1, 0)
			{
				sf::Vector2i(0, 0),
				sf::Vector2i(1, 0),
				sf::Vector2i(0,-1),
				sf::Vector2i(1,-1),
			}
		},
	},
	{
		Type::I,
		{
			// I:     OOOO   (-1, 0) (0, 0) (1,0) (2,0)  shape_Is[0]
			{
				sf::Vector2i(-1, 0),
				sf::Vector2i(0, 0),
				sf::Vector2i(1, 0),
				sf::Vector2i(2, 0),
			},
			{
				sf::Vector2i(0,-2),
				sf::Vector2i(0,-1),
				sf::Vector2i(0, 0),
				sf::Vector2i(0, 1),
			}
		}
	},

		//�s�WL�Xù�����
	{
		Type::L,
		{   
			{
				sf::Vector2i(-1, 0),
				sf::Vector2i(0, 0),
				sf::Vector2i(1, 0),
				sf::Vector2i(1, -1),
			},
			{
				sf::Vector2i(0,1),
				sf::Vector2i(0,0),
				sf::Vector2i(0, -1),
				sf::Vector2i(-1, -1),
			},
			
			{
				sf::Vector2i(-1,0),
				sf::Vector2i(-1,-1),
				sf::Vector2i(0, -1),
				sf::Vector2i(1,-1),
			},
			{
				sf::Vector2i(-1,1),
				sf::Vector2i(0,1),
				sf::Vector2i(-1,0),
				sf::Vector2i(-1,-1),
			},
		}
	},

		//�s�WJ�Xù�����
	{
		Type::J,
		{
			{
				sf::Vector2i(-1, -1),
				sf::Vector2i(0, -1),
				sf::Vector2i(1, -1),
				sf::Vector2i(1, 0),
			},
			{
				sf::Vector2i(-1,1),
				sf::Vector2i(-1,0),
				sf::Vector2i(-1,-1),
				sf::Vector2i(0,-1),
			},
			{
				sf::Vector2i(-1,1),
				sf::Vector2i(-1,0),
				sf::Vector2i(0,1),
				sf::Vector2i(1,1),
			},
			{
				sf::Vector2i(0,1),
				sf::Vector2i(1,1),
				sf::Vector2i(1,0),
				sf::Vector2i(1,-1),
			},
		}
	},

		//�s�WZ�Xù�����
	{
		Type::Z,
		{
			{
				sf::Vector2i(-1,-1),
				sf::Vector2i(0,-1),
				sf::Vector2i(0, 0),
				sf::Vector2i(1, 0),
			},
			{
				sf::Vector2i(0,-1),
				sf::Vector2i(0,0),
				sf::Vector2i(-1,0),
				sf::Vector2i(-1,1),
			},
		}
	},
		//�s�WS�Xù�����
	{
		Type::S,
		{
			{
				sf::Vector2i(-1,0),
				sf::Vector2i(0,0),
				sf::Vector2i(0,-1),
				sf::Vector2i(1,-1),
			},
			{
				sf::Vector2i(0,-2),
				sf::Vector2i(0,-1),
				sf::Vector2i(1,-1),
				sf::Vector2i(1,0),
			},
		}
	},
		//�s�WT�Xù�����
	{
		Type::T,
		{
			{
				sf::Vector2i(-1,0),
				sf::Vector2i(0,0),
				sf::Vector2i(0,-1),
				sf::Vector2i(1,0),
			},
			{
				sf::Vector2i(-1,0),
				sf::Vector2i(0,0),
				sf::Vector2i(0,-1),
				sf::Vector2i(0,1),
			},
			{
				sf::Vector2i(-1,0),
				sf::Vector2i(0,0),
				sf::Vector2i(0,1),
				sf::Vector2i(1,0),
			},
			{
				sf::Vector2i(0,0),
				sf::Vector2i(0,-1),
				sf::Vector2i(1,0),
				sf::Vector2i(0,1),
			}
		}
	},
	};

	srand(time(NULL));

	Type currentType = Type(rand() % 7 + 1);
	int currentIndex = 0;

	const int blockWidth = yellowTexture.getSize().x;
	const int blockHeight = yellowTexture.getSize().y;

	const int windowWidth = fieldWidth * blockWidth;
	const int windowHeight = fieldHeight * blockHeight;

	//�s�W�C�ثXù�����
	std::map<Type, sf::Sprite> sprites = {
		{Type::O, sf::Sprite(yellowTexture)},
		{Type::I, sf::Sprite(lightBlueTexture)},
		{Type::L, sf::Sprite(greenTexture)},
		{Type::J, sf::Sprite(redTexture)},
		{Type::Z, sf::Sprite(orangeTexture)},
		{Type::S, sf::Sprite(blueTexture)},
		{Type::T, sf::Sprite(purpleTexture)},
	};

	backgroundTexture.setRepeated(true);
	sf::Sprite backgroundSprite(
		backgroundTexture,
		sf::IntRect(0, 0, windowWidth, windowHeight));

	std::vector<sf::Vector2i> currentShape;
	sf::Sprite currentSprite;

	const sf::Vector2i origin(fieldWidth / 2, 0);
	sf::Vector2i pos(origin);

	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), L"�Xù�����");
	sf::Clock clock;

	sf::Font font;
	if (!font.loadFromFile("arial.ttf")) {
		// ���J�r�����Ѫ��B�z
		return EXIT_FAILURE;
	}

	sf::Text text;
	text.setFont(font);  // �]�w�r��
	text.setString("GG");  // �]�w��r���e
	text.setCharacterSize(250);  // �]�w�r��j�p
	text.setFillColor(sf::Color::Red);  // �]�w��r�C��
	text.setPosition(10, fieldHeight/3* blockHeight);


	bool IsGameover = false;
	while (window.isOpen()) {

		if (field[fieldWidth / 2][0] != Type::None && field[fieldWidth / 2-1][0] != Type::None && field[fieldWidth / 2 + 1][0] != Type::None)
		{
			IsGameover = true;
			
			//window.close();
		}

		currentShape = shapes[currentType][currentIndex];
		currentSprite = sprites[currentType];

		enum class Action {
			Hold,
			MoveDown,
			MoveLeft,
			MoveRight,
			Rotate,
		};
		Action action = Action::Hold;
		sf::Event evt;

		
		sf::Vector2i nextPos(pos); //���U�V��[�t�M�ť���@��쩳���ǳ�

		if (window.pollEvent(evt)) {
			if (evt.type == sf::Event::Closed) {
				window.close();
			}
			if (evt.type == sf::Event::KeyPressed) {
				switch (evt.key.code) {
				case sf::Keyboard::Left:
					action = Action::MoveLeft;
					break;
				case sf::Keyboard::Right:
					action = Action::MoveRight;
					break;
				case sf::Keyboard::Up:
					action = Action::Rotate;
					break;
				
					//�U�V��[�t
				case sf::Keyboard::Down:
					nextPos.y++;
					break;

				//�ť���@��쩳
				case sf::Keyboard::Space:
				{
					int countEmpty = 4;
					while (countEmpty == 4 && nextPos.y < fieldHeight)
					{
						countEmpty = 0;
						nextPos.y++;
						
						for (const sf::Vector2i d : currentShape) {
							sf::Vector2i np = nextPos + d;
							if (np.x >= 0 && np.x < fieldWidth &&
								np.y < fieldHeight &&
								(np.y < 0 || field[np.x][np.y] == Type::None)) {
								countEmpty++;
								//std::cout << nextPos.y << std::endl;
							}
						}
					}
					nextPos.y--;
					pos = nextPos;
				}
					break;
				}
			}
		}
		if (clock.getElapsedTime().asSeconds() >= 0.5f) {
			action = Action::MoveDown;
			clock.restart();
		}

		
		int nextIndex = currentIndex;
		if (action == Action::Rotate) {
			// 0 => 1 => 0 => 1 ...
			nextIndex = (nextIndex + 1) % shapes[currentType].size();
		}
		std::vector<sf::Vector2i> nextShape = shapes[currentType][nextIndex];
		switch (action) {
		case Action::Hold:
			break;
		case Action::MoveDown:
			nextPos.y++;
			break;
		case Action::MoveLeft:
			nextPos.x--;
			break;
		case Action::MoveRight:
			nextPos.x++;
			break;
		}

		int countEmpty = 0;
		for (const sf::Vector2i d : nextShape) {
			sf::Vector2i np = nextPos + d;
			if (np.x >= 0 && np.x < fieldWidth &&
				np.y < fieldHeight &&
				(np.y < 0 || field[np.x][np.y] == Type::None)) {
				countEmpty++;
			}
		}

		if (countEmpty == 4) {
			pos = nextPos;
			currentIndex = nextIndex;
			currentShape = nextShape;
		}
		else {
			if (action == Action::MoveDown) {
				for (const sf::Vector2i& d : currentShape) {
					sf::Vector2i np = pos + d;
					if (np.y >= 0) {
						if (!IsGameover)
						field[np.x][np.y] = currentType;
					}
				}

				for (int y = 0; y < fieldHeight; y++) {
					bool isFull = true;
					for (int x = 0; x < fieldWidth; x++) {
						if (field[x][y] == Type::None) {
							isFull = false;
						}
					}
					if (isFull) {
						for (int ty = y; ty > 0; ty--) {
							for (int x = 0; x < fieldWidth; x++) {
								if (!IsGameover)
								field[x][ty] = field[x][ty - 1];
							}
						}
						for (int x = 0; x < fieldWidth; x++) {
							if (!IsGameover)
							field[x][0] = Type::None;
						}
					}
				}

				pos = origin;
				currentType = Type(rand() % 7 + 1);
				currentIndex = 0;
				currentShape = shapes[currentType][currentIndex];
				currentSprite = sprites[currentType];
			}
		}
		window.clear();

		window.draw(backgroundSprite);

		// ø�s�I�������a
		for (int x = 0; x < fieldWidth; x++) {
			for (int y = 0; y < fieldHeight; y++) {
				if (field[x][y] == Type::None) continue;

				sf::Sprite& s = sprites[field[x][y]];

				s.setPosition(
					float(x * blockWidth),
					float(y * blockHeight));
				window.draw(s);
				
			}
		}
		if (IsGameover)
			window.draw(text);

		// ø�s�������
		// O:     OO    (0,-1) (1,-1)
		//        OO    (0, 0) (1, 0)
		for (const sf::Vector2i& d : currentShape) {
			sf::Vector2i np = pos + d;
			currentSprite.setPosition(
				float(np.x * blockWidth),
				float(np.y * blockHeight));
			if(!IsGameover)
			window.draw(currentSprite);
		}
		window.display();
	}
	return EXIT_SUCCESS;
}