#include "Game.h"
#include "framework.h"
//�����
#define HEIGHT 11
#define WIDTH 20

//�������
#define EMPTY 0
#define WALL 1
#define PLAYER 2
#define ENEMY 3
#define FOOD 4

int map[HEIGHT][WIDTH] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 4, 4, 4, 4, 1, 4, 4, 4, 4, 4, 4, 4, 4, 1, 4, 4, 4, 4, 1,
	1, 4, 1, 1, 4, 1, 4, 1, 1, 1, 1, 1, 1, 4, 1, 4, 1, 1, 4, 1,
	1, 4, 1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 4, 1,
	1, 4, 1, 4, 1, 1, 4, 1, 1, 0, 0, 1, 1, 4, 1, 1, 4, 1, 4, 1,
	1, 4, 4, 4, 4, 4, 4, 1, 3, 0, 0, 3, 1, 4, 4, 4, 4, 4, 4, 1,
	1, 4, 1, 4, 1, 1, 4, 1, 1, 1, 1, 1, 1, 4, 1, 1, 4, 1, 4, 1,
	1, 4, 1, 4, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 4, 4, 4, 1, 4, 1,
	1, 4, 1, 1, 4, 1, 4, 1, 1, 1, 1, 1, 1, 4, 1, 4, 1, 1, 4, 1,
	1, 4, 4, 4, 4, 1, 4, 4, 4, 4, 4, 4, 4, 4, 1, 4, 4, 4, 4, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};
// 0 - ��������
// 1 - �����
// 2 - �����
// 3 - ����
// 4 - ���

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
				FillRect(hdc, &object, hBrushPlayer);
			}
			else if (map[i][j] == ENEMY) { // ����
				FillRect(hdc, &object, hBrushEnemy);
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