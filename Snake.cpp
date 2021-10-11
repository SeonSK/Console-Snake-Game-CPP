#include<iostream>
#include<conio.h>
#include<time.h>
#include<stdio.h>
#include<Windows.h>

class Snake
{
private:

	int snake_x, snake_y, food_x, food_y, tail_size;
	int track_tail[80][2] = {};
	char key = ' ';

public:

	Snake(int initial_x, int initial_y) : snake_x((initial_x * 2) - 2), snake_y(initial_y)
	{
		food_x = 0;
		food_y = 0;
		
		tail_size = 1;

		srand(unsigned int(time(0)));
	}

	bool SpawnSnakeHead(int current_x, int current_y)
	{
		if (current_x == snake_x && current_y == snake_y)
			return true;
		else
			return false;
	}

	bool SpawnFood(int current_x, int current_y)
	{
		if (current_x == food_x && current_y == food_y)
			return true;
		else
			return false;
	}

	bool SpawnTail(int x, int y)
	{
		for (int i = 0; i < tail_size; i++)
		{
			if (track_tail[i][0] == x && track_tail[i][1] == y)
				return true;
			else
				continue;
		}

		return false;
	}

	void RandomizeFood(int breadth, int height)
	{
		food_x = rand() % (breadth - 4) + 2;

		if (food_x % 2 != 0)
			food_x++;

		food_y = rand() % (height - 1) + 1;
	}

	bool Eat()
	{
		if (snake_x == food_x && snake_y == food_y)
			return true;
		else
			return false;
	}

	void IncrementTail()
	{
		tail_size++;
	}

	void UpdateTail()
	{
		if (tail_size > 1)
		{
			for(int i = tail_size; i > 0; i--)
			{
				track_tail[i][0] = track_tail[i - 1][0];
				track_tail[i][1] = track_tail[i - 1][1];
			}
		}

		track_tail[0][0] = snake_x;
		track_tail[0][1] = snake_y;
	}

	void Loop(int breadth, int height)
	{
		if (snake_x <= 0)
			snake_x = breadth - 2;
		else if (snake_x >= breadth)
			snake_x = 2;
		else if (snake_y <= 0)
			snake_y = height - 1;
		else if(snake_y >= height)
			snake_y = 1;
	}



	void Movement(int breadth, int height)
	{
		static char temp = ' ';

		if (_kbhit())
		{
			temp = key;
			key = _getch();
		}

		if ((temp == 'a' && key == 'd') || (temp == 'w' && key == 's') || (temp == 'd' && key == 'a') || (temp == 's' && key == 'w'))
			key = temp;

		switch (key)
		{
		case 'w':
			snake_y--;
			break;
		case 'a':
			snake_x-=2;
			break;
		case 's':
			snake_y++;
			break;
		case 'd':
			snake_x+=2;
			break;
		}

		Loop(breadth, height);

	}

	bool GameOver()
	{
		if (tail_size > 3)
		{
			for (int i = 0; i < tail_size; i++)
			{
				if (snake_x == track_tail[i][0] && snake_y == track_tail[i][1])
					return true;
			}
		}
		
		return false;
	}

	void ResetSnake()
	{
		tail_size = 1;

		key = ' ';
		snake_x = 40;
		snake_y = 16;
	}

};

class Level: public Snake
{
private:

	int breadth, height, new_breadth, score, sleep_time;
	HANDLE console_handle;

	void ResetCursor(int x, int y)
	{
		COORD pos = { short(x), short(y) };
		SetConsoleCursorPosition(console_handle, pos);
	}

	constexpr void AdjustSpeed(int snake_speed)
	{
		if (snake_speed == 1)
			sleep_time = 100;
		else if (snake_speed == 3)
			sleep_time = 50;
		else if(snake_speed == 5)
			sleep_time = 0;
	}

	void Boundary()
	{
		for (int x = 0; x < breadth; x++)
			std::cout << "# ";
		
		for (int y = 0; y < (height - 1); y++)
		{
			std::cout << "\n#";
			ResetCursor(new_breadth, (y + 1));
			std::cout << '#';
		}

		std::cout << '\n';

		for (int x = 0; x < breadth; x++)
			std::cout << "# ";
	}


public:

	Level(int breadth, int height, int speed) : breadth(breadth), height(height), Snake((breadth / 2), (height / 2))
	{
		console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		new_breadth = (breadth * 2) - 2;
		score = 0;

		AdjustSpeed(speed);
	}

	void HideCursor(bool status)
	{
		CONSOLE_CURSOR_INFO new_info;
		new_info.dwSize = 100;
		new_info.bVisible = status;
		SetConsoleCursorInfo(console_handle, &new_info);
	}

	void SetGameSpeed(int speed)
	{
		AdjustSpeed(speed);
	}

	void Score()
	{
		std::cout << "\n\n\n\n\n\t\t\t********** SCORE **********\n\n\t\t\t\t    " << score;
		score = 0;
	}

	void Start()
	{

		RandomizeFood(new_breadth, height);
		HideCursor(false);
		Boundary();

		while (true)
		{
			UpdateTail();
			Movement(new_breadth, height);

			if (GameOver())
			{
				ResetSnake();
				break;
			}

			for (int y = 1; y < height; y++)
			{
				ResetCursor(1, y);
				for (int x = 1; x < new_breadth; x++)
				{
					if (SpawnSnakeHead(x, y))
					{
						std::cout << "@";
					}
					else if (SpawnTail(x, y))
					{
						std::cout << "o";
					}
					else if (SpawnFood(x, y))
					{
						std::cout << "*";
					}
					else
					{
						std::cout << " ";
					}

					if (Eat())
					{
						RandomizeFood(new_breadth, height);
						IncrementTail();
						score++;
					}

				}
			}

			Sleep(sleep_time);
		}

		Score();
	}
};

void menu()
{
	bool game_over = false;
	int choice = 0;
	int breadth = 40, height = 30, game_speed = 3;

	Level Game(breadth, height, game_speed);

	while (!game_over)
	{

		system("cls");

		Game.HideCursor(true);

		std::cout << "\n 1: START GAME\n 2: ADJUST GAME SPEED\n 0: EXIT\n\t-->";
		std::cin >> choice;

		system("cls");

		switch (choice)
		{
		case 1: 

			Game.Start();
			Sleep(2000);

			break;

		case 2: 
			std::cout << "\n\t Input game speed ( 1(slow) - 3(medium) - 5(fast) ) : ";
			std::cin >> game_speed;

			Game.SetGameSpeed(game_speed);
			break;

		case 3: 
			game_over = true;
			break;

		default: 
			break;
				
		}
	}
}

int main()
{
	menu();
	std::cin.get();
}