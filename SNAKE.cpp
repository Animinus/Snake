#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <thread>
#include <deque>
#include <string>

#define WORLDSIZE 17

using namespace std;

int posX = ((WORLDSIZE - 1) / 2), posY = ((WORLDSIZE - 1) / 2);
char gameGrid[WORLDSIZE][WORLDSIZE];
char direction  = 'a';
int score		= 0;
int GAMESPEED   = 500;
bool running	= false;

struct pos { int x; int y; };
deque <pos> positions;

void gameLoop();
void detectInput();
bool detectHit(int x, int y);
void spawnApple();

int main()
{
	srand(time(NULL));

	cout << "Welcome to BTEC snake game. \n";
	cout << "Loading..." << endl;

	for (int i = 0; i < WORLDSIZE; i++)
	{
		for (int j = 0; j < WORLDSIZE; j++)
		{
			if ((j == 0) | (j == WORLDSIZE - 1) | (i == 0) | (i == WORLDSIZE - 1))
			{
				gameGrid[i][j] = '#';
			} else {
				gameGrid[i][j] = ' ';
			}
		};
	};

	spawnApple();

	thread userInput(detectInput);
	
	gameLoop();

	system("cls");
	cout << "Game Over!" << endl;
	userInput.join();
	
};

void gameLoop()
{
	running = true;

	//INIT CHARACTER
	for (int i = 0; i < 3; i++)
	{
		pos temp = {posX, posY+(i+1)};
		positions.push_back(temp);
	}

	DWORD currentTick = GetTickCount64();
	int sleepTime;

	while (running)
	{
		pos t1 = { posX, posY };
		gameGrid[posX][posY] = ' ';

		if (direction == 'a')
		{
			posY -= 1;
			if (!detectHit(posX, posY))
			{
				gameGrid[posX][posY] = '<';
			}
		} else if (direction == 'w') {
			posX -= 1;
			if (!detectHit(posX, posY))
			{
				gameGrid[posX][posY] = '^';
			}
		} else if (direction == 'd') {
			posY += 1;
			if (!detectHit(posX, posY))
			{
				gameGrid[posX][posY] = '>';
			}
		} else if (direction == 's') {
			posX += 1;
			if (!detectHit(posX, posY))
			{
				gameGrid[posX][posY] = 'v';
			}
		}

		if (running == false) { break; } //instant endscreen
		
		system("cls");

		//Ugly
		positions.push_front(t1);
		pos toRemove = positions.back();
		gameGrid[toRemove.x][toRemove.y] = ' ';
		positions.pop_back();

		for (int i = 0; i < positions.size(); i++) //SET TAIL
		{
			pos temp = positions.at(i);
			gameGrid[temp.x][temp.y] = '*';
		}

		for (int i = 0; i < WORLDSIZE; i++) // DRAW BOARD
		{
			string line;
			for (int j = 0; j < WORLDSIZE; j++)
			{
				line = line + ' ' + gameGrid[i][j] + ' ';
				//line << ' ' << gameGrid[i][j] << ' ';
			}
			cout << line << endl;
		}
		cout << "\t\t     Score: " << score << endl;

		currentTick += (GAMESPEED-score);
		sleepTime = currentTick - GetTickCount64();
		if (sleepTime >= 0 )
		{
			Sleep(sleepTime);
		} else {
			//We are running behind
		}
	}
}

bool detectHit(int x, int y)
{
	//Negative hit
	if (gameGrid[x][y] == '#' || gameGrid[x][y] == '*')
	{
		running = false;
		return true;
	}

	//Positive hit
	if (gameGrid[x][y] == '@')
	{
		spawnApple();
	}

	return false;
}

void spawnApple()
{
	bool openSpace = false;
	int randX, randY;

	if (running)
	{
		score += 50;
		positions.push_back(positions.back());
	}

	while (!openSpace)
	{
		randX = rand() % ((((WORLDSIZE - 1) - 1) + 1) + 1), randY = rand() % ((((WORLDSIZE - 1) - 1) + 1) + 1);
		if (gameGrid[randX][randY] == ' ')
		{
			openSpace = true;
			gameGrid[randX][randY] = '@';
		}
	}

}

void detectInput() //Threaded task
{
	while (true)
	{
		char input = _getch();

		switch (input)
		{
		case 'w':
			direction = 'w';
			break;
		case 'a':
			direction = 'a';
			break;
		case 's':
			direction = 's';
			break;
		case 'd':
			direction = 'd';
			break;
		}
	}
}

