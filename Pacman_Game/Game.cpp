#include "Game.h"
#include "framework.h"
//�����
#define HEIGHT 11
#define WIDTH 20

//�������
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

int Ghost1Direction = RIGHT;
int Ghost2Direction = UP;
int Ghost3Direction = UP;
int Ghost4Direction = LEFT;

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
// 0 - ��������
// 1 - �����
// 2 - �����
// 3 - ���
// 4 - ������� 1
// 5 - ������� 2
// 6 - ������� 3
// 7 - ������� 4

//����� ��� �������� - (5;9) � (5;10)
//����������� �������� ���� ��� ������ - (4;9) � (4;10)

int objectSizeX = 50;
int objectSizeY = 50;

int score = 0;

void DrawMap(HDC hdc) {
	HBRUSH hBrushEmpty = CreateSolidBrush(RGB(0, 0, 0)); // ����� ������� ���� - ������

	HBRUSH hBrushWall = CreateSolidBrush(RGB(0, 0, 100)); // ����� ����� - ����� �����

	HBRUSH hBrushPlayer = CreateSolidBrush(RGB(255, 255, 0)); // ����� ������ - ������

	HBRUSH hBrushEnemy = CreateSolidBrush(RGB(100, 0, 255)); // ����� ������ - ����������

	HBRUSH hBrushFood = CreateSolidBrush(RGB(246, 138, 0)); // ����� ��� - ���������

	int i, j;
	i = 0;
	j = 0;
	while (i < HEIGHT) {
		j = 0;
		while (j < WIDTH) {
			RECT object = { j * objectSizeX,       i * objectSizeY, // ������� ������ �������
						    (j + 1) * objectSizeX, (i + 1) * objectSizeY };
			if (map[i][j] == EMPTY) { // ��������
				FillRect(hdc, &object, hBrushEmpty);
			}
			else if (map[i][j] == WALL) { // �����
				FillRect(hdc, &object, hBrushWall);
			}
			else if (map[i][j] == PLAYER) { // �����
				RECT movingObject = { j * objectSizeX + 10,       i * objectSizeY + 10, 
							(j + 1) * objectSizeX - 10, (i + 1) * objectSizeY - 10};
				FillRect(hdc, &object, hBrushEmpty);
				FillRect(hdc, &movingObject, hBrushPlayer);
			}
			else if (map[i][j] == ENEMY1 || map[i][j] == ENEMY2 || map[i][j] == ENEMY3 || map[i][j] == ENEMY4) { // ����
				RECT movingObject = { j * objectSizeX + 10,       i * objectSizeY + 10,
							(j + 1) * objectSizeX - 10, (i + 1) * objectSizeY - 10 };
				FillRect(hdc, &object, hBrushEmpty);
				FillRect(hdc, &movingObject, hBrushEnemy);
			}
			else if (map[i][j] == FOOD) { // ���
				RECT food = { j * objectSizeX + 20, i * objectSizeY + 20,
							  (j + 1) * objectSizeX - 20, (i + 1) * objectSizeY - 20 };
				FillRect(hdc, &object, hBrushEmpty);
				FillRect(hdc, &food, hBrushFood);
			}
			j++;
		}
		i++;
	}
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
		if (map[i][j - 1] != WALL && map[i][j + 1] != WALL && map[i][j - 1] < 4 && map[i][j - 1] > 4) {
			randomNum = rand() % 2;
			if (randomNum == 0) direction = LEFT;
			else direction = RIGHT;
		}
		else if (map[i][j + 1] != WALL && map[i][j + 1] < 4) direction = RIGHT;
		else if (map[i][j - 1] != WALL && map[i][j - 1] < 4) direction = LEFT;
		else direction = DOWN;
		break;
	case RIGHT:
		if (map[i - 1][j] != WALL && map[i + 1][j] != WALL && map[i - 1][j] < 4 && map[i - 1][j] > 4) {
			randomNum = rand() % 2;
			if (randomNum == 0) direction = UP;
			else if (map[i + 1][j] != WALL && map[i + 1][j] < 4) direction = DOWN;
			else direction = UP;
		}
		else if (map[i + 1][j] != WALL && map[i + 1][j] < 4) direction = DOWN;
		else if (map[i - 1][j] != WALL && map[i - 1][j] < 4) direction = UP;
		else direction = LEFT;
		break;
	case DOWN:
		if (map[i][j + 1] != WALL && map[i][j - 1] != WALL && map[i][j + 1] < 4 && map[i][j + 1] > 4) {
			randomNum = rand() % 2;
			if (randomNum == 0) direction = RIGHT;
			else direction = LEFT;
		}
		else if (map[i][j - 1] != WALL && map[i][j - 1] < 4) direction = LEFT;
		else if (map[i][j + 1] != WALL && map[i][j + 1] < 4) direction = RIGHT;
		else direction = UP;
		break;
	case LEFT:
		if (map[i + 1][j] != WALL && map[i - 1][j] != WALL && map[i + 1][j] < 4 && map[i + 1][j] > 4) {
			randomNum = rand() % 2;
			if (randomNum == 0) direction = DOWN;
			else direction = UP;
		}
		else if (map[i - 1][j] != WALL && map[i - 1][j] < 4) direction = UP;
		else if (map[i + 1][j] != WALL && map[i + 1][j] < 4) direction = DOWN;
		else direction = RIGHT;
		break;
	}
	
	switch (direction)
	{
	case UP:
		map[i - 1][j] = -1 * ghostNum;
		map[i][j] = EMPTY;
		break;
	case RIGHT:
		map[i][j + 1] = -1 * ghostNum;
		map[i][j] = EMPTY;
		break;
	case DOWN:
		map[i + 1][j] = -1 * ghostNum;
		map[i][j] = EMPTY;
		break;
	case LEFT:
		map[i][j - 1] = -1 * ghostNum;
		map[i][j] = EMPTY;
		break;
	}

	switch (ghostNum) {
	case 4:
		Ghost1Direction = direction;
		break;
	case 5:
		Ghost2Direction = direction;
		break;
	case 6:
		Ghost3Direction = direction;
		break;
	case 7:
		Ghost4Direction = direction;
		break;
	}
}

void MoveGhosts() {
	int i = 0;
	while (i < HEIGHT) {
		int j = 0;
		while (j < WIDTH) {
			if (map[i][j] == ENEMY1) {
				if ((i == 5 && j == 9) || (i == 5 && j == 10)) Ghost1Direction = UP;
				int randNum = rand() % 2;
				if (i == 7 && j > 2 && randNum == 1 && map[i + 1][j] != WALL && !((i == 5 && j == 9) || (i == 5 && j == 10)) && !((i == 4 && j == 9) || (i == 4 && j == 10))) {
					map[i + 1][j] = ENEMY1;
					map[i][j] = EMPTY;
				}
				else {
					switch (Ghost1Direction) {
					case UP:
						if (map[i - 1][j] == WALL || map[i-1][j] >= 4) ChangeGhostDirection(4, UP, i, j);
						else {
							map[i - 1][j] = -1 * ENEMY1;
							map[i][j] = EMPTY;
						}
						break;

					case RIGHT:
						if (map[i][j + 1] == WALL || map[i][j + 1] >= 4) ChangeGhostDirection(4, RIGHT, i, j);
						else {
							map[i][j + 1] = -1 * ENEMY1;
							map[i][j] = EMPTY;
						}
						break;

					case DOWN:
						if (map[i + 1][j] == WALL || map[i + 1][j] >= 4) ChangeGhostDirection(4, DOWN, i, j);
						else {
							map[i + 1][j] = -1 * ENEMY1;
							map[i][j] = EMPTY;
						}
						break;

					case LEFT:
						if (map[i][j - 1] == WALL || map[i][j-1] >= 4) ChangeGhostDirection(4, LEFT, i, j);
						else {
							map[i][j - 1] = -1 * ENEMY1;
							map[i][j] = EMPTY;
						}
						break;

					}
				}
			}
			if (map[i][j] == ENEMY2) {
				if ((i == 5 && j == 9) || (i == 5 && j == 10)) Ghost2Direction = UP;
				int randNum = rand() % 2;
				if (i == 7 && j > 2 && randNum == 1 && map[i + 1][j] != WALL && !((i == 5 && j == 9) || (i == 5 && j == 10)) && !((i == 4 && j == 9) || (i == 4 && j == 10))) {
					map[i + 1][j] = ENEMY2;
					map[i][j] = EMPTY;
				}
				else {
					switch (Ghost2Direction) {
					case UP:
						if (map[i - 1][j] == WALL || map[i - 1][j] >= 4) ChangeGhostDirection(5, UP, i, j);
						else {
							map[i - 1][j] = -1 * ENEMY2;
							map[i][j] = EMPTY;
						}
						break;

					case RIGHT:
						if (map[i][j + 1] == WALL || map[i][j+1] >= 4) ChangeGhostDirection(5, RIGHT, i, j);
						else {
							map[i][j + 1] = -1 * ENEMY2;
							map[i][j] = EMPTY;
						}
						break;

					case DOWN:
						if (map[i + 1][j] == WALL || map[i + 1][j] >= 4) ChangeGhostDirection(5, DOWN, i, j);
						else {
							map[i + 1][j] = -1 * ENEMY2;
							map[i][j] = EMPTY;
						}
						break;

					case LEFT:
						if (map[i][j - 1] == WALL || map[i][j-1] >= 4) ChangeGhostDirection(5, LEFT, i, j);
						else {
							map[i][j - 1] = -1 * ENEMY2;
							map[i][j] = EMPTY;
						}
						break;

					}
				}
			}
			if (map[i][j] == ENEMY3) {
				if ((i == 5 && j == 9) || (i == 5 && j == 10)) Ghost3Direction = UP;
				int randNum = rand() % 2;
				if (i == 7 && j > 2 && randNum == 1 && map[i + 1][j] != WALL && !((i == 5 && j == 9) || (i == 5 && j == 10)) && !((i == 4 && j == 9) || (i == 4 && j == 10))) {
					map[i + 1][j] = ENEMY3;
					map[i][j] = EMPTY;
				}
				else {
					switch (Ghost3Direction) {
					case UP:
						if (map[i - 1][j] == WALL || map[i - 1][j] >= 4) ChangeGhostDirection(6, UP, i, j);
						else {
							map[i - 1][j] = -1 * ENEMY3;
							map[i][j] = EMPTY;
						}
						break;

					case RIGHT:
						if (map[i][j + 1] == WALL || map[i][j+1] >= 4) ChangeGhostDirection(6, RIGHT, i, j);
						else {
							map[i][j + 1] = -1 * ENEMY3;
							map[i][j] = EMPTY;
						}
						break;

					case DOWN:
						if (map[i + 1][j] == WALL || map[i + 1][j] >= 4) ChangeGhostDirection(6, DOWN, i, j);
						else {
							map[i + 1][j] = -1 * ENEMY3;
							map[i][j] = EMPTY;
						}
						break;

					case LEFT:
						if (map[i][j - 1] == WALL || map[i][j-1] >= 4) ChangeGhostDirection(6, LEFT, i, j);
						else {
							map[i][j - 1] = -1 * ENEMY3;
							map[i][j] = EMPTY;
						}
						break;

					}
				}
			}
			if (map[i][j] == ENEMY4) {
				if ((i == 5 && j == 9) || (i == 5 && j == 10)) Ghost4Direction = UP;
				int randNum = rand() % 2;
				if (i == 7 && j > 2 && randNum == 1 && map[i + 1][j] != WALL && !((i == 5 && j == 9) || (i == 5 && j == 10)) && !((i == 4 && j == 9) || (i == 4 && j == 10))) {
					map[i + 1][j] = ENEMY1;
					map[i][j] = EMPTY;
				}
				else {
					switch (Ghost4Direction) {
					case UP:
						if (map[i - 1][j] == WALL || map[i - 1][j] >= 4) ChangeGhostDirection(7, UP, i, j);
						else {
							map[i - 1][j] = -1 * ENEMY4;
							map[i][j] = EMPTY;
						}
						break;

					case RIGHT:
						if (map[i][j + 1] == WALL || map[i][j+1] >= 4) ChangeGhostDirection(7, RIGHT, i, j);
						else {
							map[i][j + 1] = -1 * ENEMY4;
							map[i][j] = EMPTY;
						}
						break;

					case DOWN:
						if (map[i + 1][j] == WALL || map[i + 1][j] >= 4) ChangeGhostDirection(7, DOWN, i, j);
						else {
							map[i + 1][j] = -1 * ENEMY4;
							map[i][j] = EMPTY;
						}
						break;

					case LEFT:
						if (map[i][j - 1] == WALL || map[i][j-1] >= 4) ChangeGhostDirection(7, LEFT, i, j);
						else {
							map[i][j - 1] = -1 * ENEMY4;
							map[i][j] = EMPTY;
						}
						break;

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
				E1 = true;
			}
			else if (map[i][j] == -1 * ENEMY2) {
				map[i][j] = ENEMY2;
				E2 = true;
			}
			else if (map[i][j] == -1 * ENEMY3) {
				map[i][j] = ENEMY3;
				E3 = true;
			}
			else if (map[i][j] == -1 * ENEMY4) {
				map[i][j] = ENEMY4;
				E4 = true;
			}
			j++;
		}
		i++;
	}
	if (!E1) map[5][8] = 4;
	if (!E2) map[5][9] = 5;
	if (!E3) map[5][10] = 6;
	if (!E4) map[5][11] = 7;
}