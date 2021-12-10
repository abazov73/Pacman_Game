#define _CRT_SECURE_NO_WARNINGS 

#include "Game.h"
#include "framework.h"
//Карта
#define HEIGHT 11
#define WIDTH 20

//Объекты
#define EMPTY 0
#define WALL 1
#define PLAYER 2
#define FOOD 3
#define ENEMY1 4
#define ENEMY2 5
#define ENEMY3 6
#define ENEMY4 7

#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4
#define STOP 0

int map[HEIGHT][WIDTH] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 3, 3, 3, 3, 1, 3, 3, 3, 3, 3, 3, 3, 3, 1, 3, 3, 3, 3, 1,
	1, 3, 1, 1, 3, 1, 3, 1, 1, 1, 1, 1, 1, 3, 1, 3, 1, 1, 3, 1,
	1, 3, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 3, 1,
	1, 3, 1, 3, 1, 1, 3, 1, 1, 5, 6, 1, 1, 3, 1, 1, 3, 1, 3, 1,
	1, 3, 3, 3, 3, 3, 3, 1, 4, 0, 0, 7, 1, 3, 3, 3, 3, 3, 3, 1,
	1, 3, 1, 3, 1, 1, 3, 1, 1, 1, 1, 1, 1, 3, 1, 1, 3, 1, 3, 1,
	1, 3, 1, 3, 3, 3, 3, 3, 3, 2, 3, 3, 3, 3, 3, 3, 3, 1, 3, 1,
	1, 3, 1, 1, 3, 1, 3, 1, 1, 1, 1, 1, 1, 3, 1, 3, 1, 1, 3, 1,
	1, 3, 3, 3, 3, 1, 3, 3, 3, 3, 3, 3, 3, 3, 1, 3, 3, 3, 3, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};
// 0 - свободно
// 1 - стена
// 2 - игрок
// 3 - еда
// 4 - призрак 1
// 5 - призрак 2
// 6 - призрак 3
// 7 - призрак 4

//выход для монстров - (5;9) и (5;10)
//блокировать движение вниз для игрока - (4;9) и (4;10)

struct Ghost {
	int number;
	int direction;
	int onFoodFlag;
};

Ghost ghost1 = { 1, RIGHT, 0};
Ghost ghost2 = { 2, UP, 0};
Ghost ghost3 = { 3, UP, 0};
Ghost ghost4 = { 4, LEFT, 0};

Ghost ghosts[4] = { ghost1, ghost2, ghost3, ghost4 };

int objectSizeX = 50;
int objectSizeY = 50;

int score = 1;

HFONT hFont = CreateFont(25,
	0, 0, 0, 0, 0, 0, 0,
	DEFAULT_CHARSET,
	0, 0, 0, 0,
	L"Courier New"
);

void DrawMap(HDC hdc) {
	HBRUSH hBrushEmpty = CreateSolidBrush(RGB(0, 0, 0)); // кисть пустого поля - черный

	HBRUSH hBrushWall = CreateSolidBrush(RGB(0, 0, 100)); // кисть стены - темно синий

	HBRUSH hBrushPlayer = CreateSolidBrush(RGB(255, 255, 0)); // кисть игрока - желтый

	HBRUSH hBrushEnemy1 = CreateSolidBrush(RGB(255, 0, 0)); // кисть призрака №1 - красный 
	
	HBRUSH hBrushEnemy2 = CreateSolidBrush(RGB(128, 166, 255)); // кисть призрака №2 - голубой

	HBRUSH hBrushEnemy3 = CreateSolidBrush(RGB(255, 128, 0)); // кисть призрака №3 - оранжевый

	HBRUSH hBrushEnemy4 = CreateSolidBrush(RGB(0xFF, 0xC0, 0xCB)); // кисть призрака №4 - розовый

	HBRUSH hBrushFood = CreateSolidBrush(RGB(246, 138, 0)); // кисть еды - оранжевый

	SetBkMode(hdc, TRANSPARENT);

	int i, j;
	i = 0;
	j = 0;
	while (i < HEIGHT) {
		j = 0;
		while (j < WIDTH) {
			RECT object = { j * objectSizeX,       i * objectSizeY, // размеры одного объекта
						    (j + 1) * objectSizeX, (i + 1) * objectSizeY };
			if (map[i][j] == EMPTY) { // Свободно
				FillRect(hdc, &object, hBrushEmpty);
			}
			else if (map[i][j] == WALL) { // Стена
				FillRect(hdc, &object, hBrushWall);
			}
			else if (map[i][j] == PLAYER) { // Игрок
				RECT movingObject = { j * objectSizeX + 10,       i * objectSizeY + 10, 
							(j + 1) * objectSizeX - 10, (i + 1) * objectSizeY - 10};
				FillRect(hdc, &object, hBrushEmpty);
				FillRect(hdc, &movingObject, hBrushPlayer);
			}
			else if (map[i][j] == ENEMY1) { // Враг
				RECT movingObject = { j * objectSizeX + 10,       i * objectSizeY + 10,
							(j + 1) * objectSizeX - 10, (i + 1) * objectSizeY - 10 };
				FillRect(hdc, &object, hBrushEmpty);
				FillRect(hdc, &movingObject, hBrushEnemy1);
			}
			else if (map[i][j] == ENEMY2) {
				RECT movingObject = { j * objectSizeX + 10,       i * objectSizeY + 10,
							(j + 1) * objectSizeX - 10, (i + 1) * objectSizeY - 10 };
				FillRect(hdc, &object, hBrushEmpty);
				FillRect(hdc, &movingObject, hBrushEnemy2);
			}
			else if (map[i][j] == ENEMY3) {
				RECT movingObject = { j * objectSizeX + 10,       i * objectSizeY + 10,
							(j + 1) * objectSizeX - 10, (i + 1) * objectSizeY - 10 };
				FillRect(hdc, &object, hBrushEmpty);
				FillRect(hdc, &movingObject, hBrushEnemy3);
			}
			else if (map[i][j] == ENEMY4) {
				RECT movingObject = { j * objectSizeX + 10,       i * objectSizeY + 10,
							(j + 1) * objectSizeX - 10, (i + 1) * objectSizeY - 10 };
				FillRect(hdc, &object, hBrushEmpty);
				FillRect(hdc, &movingObject, hBrushEnemy4);
			}
			else if (map[i][j] == FOOD) { // Еда
				RECT food = { j * objectSizeX + 20, i * objectSizeY + 20,
							  (j + 1) * objectSizeX - 20, (i + 1) * objectSizeY - 20 };
				FillRect(hdc, &object, hBrushEmpty);
				FillRect(hdc, &food, hBrushFood);
			}
			j++;
		}
		i++;
	}

	SelectObject(hdc, hFont);
	SetTextColor(hdc, RGB(255, 255, 255));
	TCHAR  scoreText[] = _T("Счёт: ");
	TextOut(hdc, 5, (objectSizeY + 2) * (HEIGHT-1), (LPCWSTR)scoreText, _tcslen(scoreText));


	char sScore[5];
	TCHAR  tsScore[5];
	sprintf(sScore, "%d", score);
	OemToChar(sScore, tsScore);
	TextOut(hdc, 115, (objectSizeY + 2) * (HEIGHT-1), (LPCWSTR)tsScore, _tcslen(tsScore));


	DeleteObject(hBrushEmpty);
	DeleteObject(hBrushWall);
	DeleteObject(hBrushPlayer);
	DeleteObject(hBrushEnemy1);
	DeleteObject(hBrushEnemy2);
	DeleteObject(hBrushEnemy3);
	DeleteObject(hBrushEnemy4);
	DeleteObject(hBrushFood);

}

void MoveLeft() {
	int i = 0;
	int j = 1;
	int stopSearchFlag = 0;
	while (i < HEIGHT) {
		j = 1;
		while (j < WIDTH) {
			if (map[i][j] == PLAYER) {
				stopSearchFlag = 1;
				if (map[i][j - 1] == WALL) break;
				if (map[i][j - 1] == FOOD) score++;
				map[i][j - 1] = PLAYER;
				map[i][j] = EMPTY;
				break;
			}
			j++;
		}
		if (stopSearchFlag == 1) break;
		i++;
	}
}

void MoveUp() {
	int i = 1;
	int j = 0;
	int stopSearchFlag = 0;
	while (i < HEIGHT) {
		j = 0;
		while (j < WIDTH) {
			if (map[i][j] == PLAYER) {
				stopSearchFlag = 1;
				if (map[i - 1][j] == WALL) break;
				if (map[i - 1][j] == FOOD) score++;
				map[i - 1][j] = PLAYER;
				map[i][j] = EMPTY;
				break;
			}
			j++;
		}
		if (stopSearchFlag == 1) break;
		i++;
	}
}

void MoveRight() {
	int i = 0;
	int j = 0;
	int stopSearchFlag = 0;
	while (i < HEIGHT) {
		j = 0;
		while (j < WIDTH - 1) {
			if (map[i][j] == PLAYER) {
				stopSearchFlag = 1;
				if (map[i][j + 1] == WALL) break;
				if (map[i][j + 1] == FOOD) score++;
				map[i][j + 1] = PLAYER;
				map[i][j] = EMPTY;
				break;
			}
			j++;
		}
		if (stopSearchFlag == 1) break;
		i++;
	}
}

void MoveDown() {
	int i = 0;
	int j = 0;
	int stopSearchFlag = 0;
	while (i < HEIGHT - 1) {
		j = 0;
		while (j < WIDTH) {
			if (map[i][j] == PLAYER) {
				stopSearchFlag = 1;
				if (map[i + 1][j] == WALL) break;
				if (map[i + 1][j] == FOOD) score++;
				map[i + 1][j] = PLAYER;
				map[i][j] = EMPTY;
				break;
			}
			j++;
		}
		if (stopSearchFlag == 1) break;
		i++;
	}
}

void ChangeGhostDirection(int ghostNum, int direction, int i, int j) {
	int randomNum = rand() % 2;
	switch (direction) {
	case UP:
		if (map[i][j - 1] > -1 && map[i][j + 1] > -1 && map[i][j - 1] != WALL && map[i][j + 1] != WALL && map[i][j - 1] < 4 && map[i][j + 1] < 4) { //фикисить по этому образцу
			randomNum = rand() % 2;
			if (randomNum == 0) direction = LEFT;
			else direction = RIGHT;
		}
		else if (map[i][j + 1] > -1 && map[i][j + 1] != WALL && map[i][j + 1] < 4) direction = RIGHT;
		else if (map[i][j - 1] > -1 && map[i][j - 1] != WALL && map[i][j - 1] < 4) direction = LEFT;
		else if (map[i + 1][j] > -1 && map[i + 1][j] != WALL && map[i + 1][j] < 4) direction = DOWN;
		else direction = STOP;
		break;
	case RIGHT:
		if (map[i - 1][j] > -1 && map[i + 1][j] > -1 && map[i - 1][j] != WALL && map[i + 1][j] != WALL && map[i - 1][j] < 4 && map[i + 1][j] < 4) {
			randomNum = rand() % 2;
			if (randomNum == 0) direction = UP;
			else if (map[i + 1][j] != WALL && map[i + 1][j] < 4) direction = DOWN;
			else direction = UP;
		}
		else if (map[i + 1][j] > -1 && map[i + 1][j] != WALL && map[i + 1][j] < 4) direction = DOWN;
		else if (map[i - 1][j] > -1 && map[i - 1][j] != WALL && map[i - 1][j] < 4) direction = UP;
		else if (map[i][j - 1] > -1 && map[i][j - 1] != WALL && map[i][j - 1] < 4) direction = LEFT;
		else direction = STOP;
		break;
	case DOWN:
		if (map[i][j + 1] > -1 && map[i][j - 1] > -1 && map[i][j + 1] != WALL && map[i][j - 1] != WALL && map[i][j - 1] < 4 && map[i][j + 1] < 4) {
			randomNum = rand() % 2;
			if (randomNum == 0) direction = RIGHT;
			else direction = LEFT;
		}
		else if (map[i][j - 1] > -1 && map[i][j - 1] != WALL && map[i][j - 1] < 4) direction = LEFT;
		else if (map[i][j + 1] > -1 && map[i][j + 1] != WALL && map[i][j + 1] < 4) direction = RIGHT;
		else if (map[i - 1][j] > -1 && map[i - 1][j] != WALL && map[i - 1][j] < 4) direction = UP;
		else direction = STOP;
		break;
	case LEFT:
		if (map[i + 1][j] > -1 && map[i - 1][j] > -1 && map[i + 1][j] != WALL && map[i - 1][j] != WALL && map[i - 1][j] < 4 && map[i + 1][j] < 4) {
			randomNum = rand() % 2;
			if (randomNum == 0) direction = DOWN;
			else direction = UP;
		}
		else if (map[i - 1][j] > -1 && map[i - 1][j] != WALL && map[i - 1][j] < 4) direction = UP;
		else if (map[i + 1][j] > -1 && map[i + 1][j] != WALL && map[i + 1][j] < 4) direction = DOWN;
		else if (map[i][j + 1] > -1 && map[i][j + 1] != WALL && map[i][j + 1] < 4) direction = RIGHT;
		else direction = STOP;
		break;
	}
	
	switch (direction)
	{
	case UP:
		if (ghosts[ghostNum - 4].onFoodFlag) map[i][j] = FOOD;
		else map[i][j] = EMPTY;

		if (map[i - 1][j] == FOOD) ghosts[ghostNum - 4].onFoodFlag = 1;
		else ghosts[ghostNum - 4].onFoodFlag = 0;

		map[i - 1][j] = -1 * ghostNum;
		break;
	case RIGHT:
		if (ghosts[ghostNum - 4].onFoodFlag) map[i][j] = FOOD;
		else map[i][j] = EMPTY;

		if (map[i][j + 1] == FOOD) ghosts[ghostNum - 4].onFoodFlag = 1;
		else ghosts[ghostNum - 4].onFoodFlag = 0;

		map[i][j + 1] = -1 * ghostNum;
		break;
	case DOWN:
		if (ghosts[ghostNum - 4].onFoodFlag) map[i][j] = FOOD;
		else map[i][j] = EMPTY;

		if (map[i + 1][j] == FOOD) ghosts[ghostNum - 4].onFoodFlag = 1;
		else ghosts[ghostNum - 4].onFoodFlag = 0;

		map[i + 1][j] = -1 * ghostNum;
		break;
	case LEFT:
		if (ghosts[ghostNum - 4].onFoodFlag) map[i][j] = FOOD;
		else map[i][j] = EMPTY;

		if (map[i][j - 1] == FOOD) ghosts[ghostNum - 4].onFoodFlag = 1;
		else ghosts[ghostNum - 4].onFoodFlag = 0;

		map[i][j - 1] = -1 * ghostNum;
		break;
	case STOP:
		break;
	}

	ghosts[ghostNum - 4].direction = direction;
}


void MoveGhosts() {
	int i = 0;
	while (i < HEIGHT) {
		int j = 0;
		while (j < WIDTH) {
			if (map[i][j] > 3 && map[i][j] < 8) {
				int ghostNum = map[i][j]; // номер призрака на поле. Отнять 4 для номера в массиве
				if ((i == 5 && j == 9) || (i == 5 && j == 10)) ghosts[ghostNum - 4].direction = UP;
				int randNum = rand() % 3;
				if (i == 7 && j > 2 && j < 17 && randNum == 1 && map[i + 1][j] != WALL && map[i + 1][j] > -1 && map[i + 1][j] < 4 && !((i == 5 && j == 9) || (i == 5 && j == 10)) && !((i == 4 && j == 9) || (i == 4 && j == 10))) {
					if (ghosts[ghostNum - 4].onFoodFlag) map[i][j] = FOOD;
					else map[i][j] = EMPTY;

					if (map[i + 1][j] == FOOD) ghosts[ghostNum - 4].onFoodFlag = 1;
					else ghosts[ghostNum - 4].onFoodFlag = 0;

					map[i + 1][j] = -1 * ghostNum;
					ghosts[ghostNum - 4].direction = DOWN;
				}
				else if (i == 5 && j == 1 && randNum == 0 && map[i][j + 1] != WALL && map[i][j + 1] > -1 && map[i][j + 1] < 4 && !((i == 5 && j == 9) || (i == 5 && j == 10)) && !((i == 4 && j == 9) || (i == 4 && j == 10))) {
					if (ghosts[ghostNum - 4].onFoodFlag) map[i][j] = FOOD;
					else map[i][j] = EMPTY;

					if (map[i][j + 1] == FOOD) ghosts[ghostNum - 4].onFoodFlag = 1;
					else ghosts[ghostNum - 4].onFoodFlag = 0;

					map[i][j + 1] = -1 * ghostNum;
					ghosts[ghostNum - 4].direction = RIGHT;
				}
				else {
					switch (ghosts[ghostNum - 4].direction) {
					case UP:
						if (map[i - 1][j] == WALL || map[i-1][j] >= 4 || map[i - 1][j] < 0) ChangeGhostDirection(ghostNum, UP, i, j);
						else {
							if (ghosts[ghostNum - 4].onFoodFlag) map[i][j] = FOOD;
							else map[i][j] = EMPTY;

							if (map[i - 1][j] == FOOD) ghosts[ghostNum - 4].onFoodFlag = 1;
							else ghosts[ghostNum - 4].onFoodFlag = 0;

							map[i - 1][j] = -1 * ghostNum;
						}
						break;

					case RIGHT:
						if (map[i][j + 1] == WALL || map[i][j + 1] >= 4 || map[i][j + 1] < 0) ChangeGhostDirection(ghostNum, RIGHT, i, j);
						else {
							if (ghosts[ghostNum - 4].onFoodFlag) map[i][j] = FOOD;
							else map[i][j] = EMPTY;

							if (map[i][j + 1] == FOOD) ghosts[ghostNum - 4].onFoodFlag = 1;
							else ghosts[ghostNum - 4].onFoodFlag = 0;

							map[i][j + 1] = -1 * ghostNum;
						}
						break;


					case DOWN:
						if (map[i + 1][j] == WALL || map[i + 1][j] >= 4 || map[i + 1][j] < 0) ChangeGhostDirection(ghostNum, DOWN, i, j);
						else {
							if (ghosts[ghostNum - 4].onFoodFlag) map[i][j] = FOOD;
							else map[i][j] = EMPTY;

							if (map[i + 1][j] == FOOD) ghosts[ghostNum - 4].onFoodFlag = 1;
							else ghosts[ghostNum - 4].onFoodFlag = 0;

							map[i + 1][j] = -1 * ghostNum;
						}
						break;

					case LEFT:
						if (map[i][j - 1] == WALL || map[i][j-1] >= 4 || map[i][j - 1] < 0) ChangeGhostDirection(ghostNum, LEFT, i, j);
						else {
							if (ghosts[ghostNum - 4].onFoodFlag) map[i][j] = FOOD;
							else map[i][j] = EMPTY;

							if (map[i][j - 1] == FOOD) ghosts[ghostNum - 4].onFoodFlag = 1;
							else ghosts[ghostNum - 4].onFoodFlag = 0;

							map[i][j - 1] = -1 * ghostNum;
						}
						break;
					case STOP:
						if (map[i - 1][j] == EMPTY || map[i - 1][j] == FOOD || map[i - 1][j] == PLAYER) {
							if (ghosts[ghostNum - 4].onFoodFlag) map[i][j] = FOOD;
							else map[i][j] = EMPTY;

							if (map[i - 1][j] == FOOD) ghosts[ghostNum - 4].onFoodFlag = 1;
							else ghosts[ghostNum - 4].onFoodFlag = 0;

							map[i - 1][j] = -1 * ghostNum;
							ghosts[ghostNum - 4].direction = UP;
						}
						else if (map[i][j + 1] == EMPTY || map[i][j + 1] == FOOD || map[i][j + 1] == PLAYER) {
							if (ghosts[ghostNum - 4].onFoodFlag) map[i][j] = FOOD;
							else map[i][j] = EMPTY;

							if (map[i][j + 1] == FOOD) ghosts[ghostNum - 4].onFoodFlag = 1;
							else ghosts[ghostNum - 4].onFoodFlag = 0;

							map[i][j + 1] = -1 * ghostNum;
							ghosts[ghostNum - 4].direction = RIGHT;
						}
						else if (map[i + 1][j] == EMPTY || map[i + 1][j] == FOOD || map[i + 1][j] == PLAYER) {
							if (ghosts[ghostNum - 4].onFoodFlag) map[i][j] = FOOD;
							else map[i][j] = EMPTY;

							if (map[i + 1][j] == FOOD) ghosts[ghostNum - 4].onFoodFlag = 1;
							else ghosts[ghostNum - 4].onFoodFlag = 0;

							map[i + 1][j] = -1 * ghostNum;
							ghosts[ghostNum - 4].direction = DOWN;
						}
						else if (map[i][j - 1] == EMPTY || map[i][j - 1] == FOOD || map[i][j - 1] == PLAYER) {
							if (ghosts[ghostNum - 4].onFoodFlag) map[i][j] = FOOD;
							else map[i][j] = EMPTY;

							if (map[i][j - 1] == FOOD) ghosts[ghostNum - 4].onFoodFlag = 1;
							else ghosts[ghostNum - 4].onFoodFlag = 0;

							map[i][j - 1] = -1 * ghostNum;
							ghosts[ghostNum - 4].direction = LEFT;
						}
						else {
							//продолжаем ждать
						}
					}
				}
			}
			j++;
		}
		i++;
	}
	i = 0;
	bool E1 = false;
	bool E2 = false;
	bool E3 = false;
	bool E4 = false;
	while (i < HEIGHT) {
		int j = 0;
		while (j < WIDTH) {
			if (map[i][j] == -1 * ENEMY1) {
				map[i][j] = ENEMY1;
			}
			else if (map[i][j] == -1 * ENEMY2) {
				map[i][j] = ENEMY2;
			}
			else if (map[i][j] == -1 * ENEMY3) {
				map[i][j] = ENEMY3;
			}
			else if (map[i][j] == -1 * ENEMY4) {
				map[i][j] = ENEMY4;
			}
			j++;
		}
		i++;
	}

	i = 0;
	while (i < HEIGHT) {
		int j = 0;
		while (j < WIDTH) {
			if (map[i][j] == ENEMY1) {
				E1 = true;
			}
			else if (map[i][j] == ENEMY2) {
				E2 = true;
			}
			else if (map[i][j] == ENEMY3) {
				E3 = true;
			}
			else if (map[i][j] == ENEMY4) {
				E4 = true;
			}
			j++;
		}
		i++;
	}

	if (!E1) {
		map[5][8] = ENEMY1;
		ghosts[0].onFoodFlag = 0;
	}
	if (!E2) {
		map[5][9] = ENEMY2;
		ghosts[1].onFoodFlag = 0;
	}
	if (!E3) {
		map[5][10] = ENEMY3;
		ghosts[2].onFoodFlag = 0;
	}
	if (!E4) {
		map[5][11] = ENEMY4;
		ghosts[3].onFoodFlag = 0;
	}
}