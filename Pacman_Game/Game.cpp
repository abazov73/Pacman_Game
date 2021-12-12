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

#define MAX_NUM_RECORDS 10


char SavePath[] = "Saves\\save.txt";
char RecordsPath[] = "Saves\\records.txt";



int map[HEIGHT][WIDTH];
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

struct Player {
	int x;
	int y;
	int direction;
	int nextDirection;
	char name[20];
};

struct Records {
	char name[20];
	int score;
	unsigned int year;
	unsigned int month;
	unsigned int day;
	unsigned int hour;
	unsigned int minute;
	unsigned int second;

};

int numRecords = 0;

int recordDrawnFlag = 0;

Records records[MAX_NUM_RECORDS + 1];

Player player;

Ghost ghost1 = { 1, RIGHT, 0};
Ghost ghost2 = { 2, UP, 0};
Ghost ghost3 = { 3, UP, 0};
Ghost ghost4 = { 4, LEFT, 0};

Ghost ghosts[4] = { ghost1, ghost2, ghost3, ghost4 };

int objectSizeX = 50;
int objectSizeY = 50;

int score = 1;



void DrawMap(HDC hdc) {
	HFONT hFont = CreateFont(25,
		0, 0, 0, 0, 0, 0, 0,
		DEFAULT_CHARSET,
		0, 0, 0, 0,
		L"Courier New"
	);
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
			else if (map[i][j] == ENEMY1) { // Враг 1
				RECT movingObject = { j * objectSizeX + 10,       i * objectSizeY + 10,
							(j + 1) * objectSizeX - 10, (i + 1) * objectSizeY - 10 };
				FillRect(hdc, &object, hBrushEmpty);
				FillRect(hdc, &movingObject, hBrushEnemy1);
			}
			else if (map[i][j] == ENEMY2) { // Враг 2
				RECT movingObject = { j * objectSizeX + 10,       i * objectSizeY + 10,
							(j + 1) * objectSizeX - 10, (i + 1) * objectSizeY - 10 };
				FillRect(hdc, &object, hBrushEmpty);
				FillRect(hdc, &movingObject, hBrushEnemy2);
			}
			else if (map[i][j] == ENEMY3) { // Враг 3
				RECT movingObject = { j * objectSizeX + 10,       i * objectSizeY + 10,
							(j + 1) * objectSizeX - 10, (i + 1) * objectSizeY - 10 };
				FillRect(hdc, &object, hBrushEmpty);
				FillRect(hdc, &movingObject, hBrushEnemy3);
			}
			else if (map[i][j] == ENEMY4) { // Враг 4
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

void savePlayerName(char* name) {
	strcpy(player.name, name);
}

void drawMenu(HDC hdc) {
	HFONT hFont = CreateFont(25,
		0, 0, 0, 0, 0, 0, 0,
		DEFAULT_CHARSET,
		0, 0, 0, 0,
		L"Courier New"
	);
	SetBkMode(hdc, TRANSPARENT);
	HBRUSH hBrushMenu = CreateSolidBrush(RGB(0, 0, 0));
	RECT screen = { 0, 0, 1000, 609 };
	FillRect(hdc, &screen, hBrushMenu);
	SelectObject(hdc, hFont);
	SetTextColor(hdc, RGB(255, 255, 0));
	TCHAR  scoreText[] = _T("PACMAN");
	TextOut(hdc, 435, 100, (LPCWSTR)scoreText, _tcslen(scoreText));
	DeleteObject(hFont);
	DeleteObject(hBrushMenu);
}


void saveGame() {
	FILE* save = fopen(SavePath, "wt");
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			fprintf(save, "%d ", map[i][j]);
		}
		fprintf(save, "\n");
	}
	fprintf(save, "%d %d %d %d", player.x, player.y, player.direction, player.direction);
	fprintf(save, "\n");
	fprintf(save, "%d ", score);
	fprintf(save, "%s", player.name);
	fclose(save);
}

void loadGame() {
	FILE* save = fopen(SavePath, "rt");
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			fscanf(save, "%d", &map[i][j]);
		}
	}
	fscanf(save, "%d %d %d %d", &player.x, &player.y, &player.direction, &player.nextDirection);
	fscanf(save, "%d %s", &score, &player.name);
	fclose(save);
}

void tryChangePlayerDirection() {
	switch (player.nextDirection)
	{
	case UP:
		if (map[player.y - 1][player.x] == EMPTY || map[player.y - 1][player.x] == FOOD) {
			player.direction = UP;
			player.nextDirection = STOP;
		}
		break;
	case RIGHT:
		if (map[player.y][player.x + 1] == EMPTY || map[player.y][player.x + 1] == FOOD) {
			player.direction = RIGHT;
			player.nextDirection = STOP;
		}
		break;
	case DOWN:
		if (map[player.y + 1][player.x] == EMPTY || map[player.y + 1][player.x] == FOOD) {
			player.direction = DOWN;
			player.nextDirection = STOP;
		}
		break;
	case LEFT:
		if (map[player.y][player.x - 1] == EMPTY || map[player.y][player.x - 1] == FOOD) {
			player.direction = LEFT;
			player.nextDirection = STOP;
		}
		break;
	case STOP:
		break;
	}
}

void setNextDirection(int nextDirection) {
	player.nextDirection = nextDirection;
}

void movePlayer() {
	int i = 0;
	int j = 0;
	int stopSearchFlag = 0;
	while (i < HEIGHT) {
		j = 1;
		while (j < WIDTH) {
			if (map[i][j] == PLAYER) {
				stopSearchFlag = 1;
				switch (player.direction) {
				case UP:
					if (map[i - 1][j] == WALL) {
						player.direction = STOP;
						break;
					}
					if (map[i - 1][j] == FOOD) score++;
					if (map[i - 1][j] < 4) map[i - 1][j] = PLAYER;
					player.y--;
					map[i][j] = EMPTY;
					break;
				case RIGHT:
					if (map[i][j + 1] == WALL) {
						player.direction = STOP;
						break;
					}
					if (map[i][j + 1] == FOOD) score++;
					if (map[i][j + 1] < 4) map[i][j + 1] = PLAYER;
					player.x++;
					map[i][j] = EMPTY;
					break;
				case DOWN:
					if (map[i + 1][j] == WALL) {
						player.direction = STOP;
						break;
					}
					if (map[i + 1][j] == FOOD) score++;
					if (map[i + 1][j] < 4) map[i + 1][j] = PLAYER;
					player.y++;
					map[i][j] = EMPTY;
					break;
				case LEFT:
					if (map[i][j - 1] == WALL) {
						player.direction = STOP;
						break;
					}
					if (map[i][j - 1] == FOOD) score++;
					if (map[i][j - 1] < 4) map[i][j - 1] = PLAYER;
					player.x--;
					map[i][j] = EMPTY;
					break;
				case STOP:
					break;
				}
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

int CompareRecords(int index1, int index2)
{
	if (records[index1].score < records[index2].score)
		return -1;
	if (records[index1].score > records[index2].score)
		return +1;


	return 0;

}


void InsertRecord(char name[])
{
	strcpy(records[numRecords].name, name);
	records[numRecords].score = score;

	SYSTEMTIME st;
	// Получаем текущее время
	GetLocalTime(&st);

	// и разбрасываем его по полям в таблицу рекордов
	records[numRecords].year = st.wYear;
	records[numRecords].month = st.wMonth;
	records[numRecords].day = st.wDay;

	records[numRecords].hour = st.wHour;
	records[numRecords].minute = st.wMinute;
	records[numRecords].second = st.wSecond;
	int i = numRecords;
	while (i > 0) {
		if (CompareRecords(i - 1, i) < 0) {
			Records temp = records[i];
			records[i] = records[i - 1];
			records[i - 1] = temp;
		}
		i--;
	}
	// Если таблица заполнена не полностью
	if (numRecords < MAX_NUM_RECORDS)
		// следующий раз новый рекорд будет занесен в новый элемент
		numRecords++;
}

void saveRecords() {
	FILE* save = fopen(RecordsPath, "wt");
	fprintf(save, "%d\n", numRecords);
	for (int i = 0; i < numRecords; i++) {
		fprintf(save, "%d %d %d %d %d %d %s %d \n",
			records[i].day, records[i].month, records[i].year,
			records[i].hour, records[i].minute, records[i].second,
			records[i].name, records[i].score
		);
	}
	fclose(save);
}

void loadRecords() {
	FILE* save = fopen(RecordsPath, "rt");
	fscanf(save, "%d", &numRecords);
	for (int i = 0; i < numRecords; i++) {
		fscanf(save, "%d %d %d %d %d %d %s %d \n",
			&records[i].day, &records[i].month, &records[i].year,
			&records[i].hour, &records[i].minute, &records[i].second,
			&records[i].name, &records[i].score
		);
	}
	fclose(save);
}

void drawFinalScreen(HDC hdc, int msg) {
	HBRUSH hBrushMenu = CreateSolidBrush(RGB(0, 0, 0));
	RECT screen = { 0, 0, 1000, 609 };
	FillRect(hdc, &screen, hBrushMenu);

	HFONT hFont1;
	hFont1 = CreateFont(16, 0, 0, 0, 0, 0, 0, 0,
		DEFAULT_CHARSET, 0, 0, 0, 0,
		L"Courier New"
	);
	SelectObject(hdc, hFont1);
	SetTextColor(hdc, RGB(0, 64, 64));

	if (msg == 0) {
		TCHAR  string1[] = _T("Победа!");
		TextOut(hdc, 250, 10, (LPCWSTR)string1, _tcslen(string1));
	}
	else {
		TCHAR  string1[] = _T("Поражение!");
		TextOut(hdc, 160, 10, (LPCWSTR)string1, _tcslen(string1));
	}

	TCHAR  string2[] = _T("! No ! Дата       ! Время    ! Имя                  !  Очки  !");
	TextOut(hdc, 100, 50, (LPCWSTR)string2, _tcslen(string2));

	if (!recordDrawnFlag) {
		loadRecords();
		InsertRecord(player.name);
		saveRecords();
		score = 0;
	}
	int i;
	for (i = 0; i < numRecords; i++) {
		TCHAR  string2[80];
		char str[80];
		sprintf(str, "! %2d ! %02d.%02d.%04d ! %02d:%02d:%02d ! %-20s ! %4d   !",
			i + 1,
			records[i].day, records[i].month, records[i].year,
			records[i].hour, records[i].minute, records[i].second,
			records[i].name, records[i].score
		);
		OemToChar(str, string2);
		TextOut(hdc, 100, 24 * (i + 1) + 50, (LPCWSTR)string2, _tcslen(string2));
	}
	recordDrawnFlag = 1;
	DeleteObject(hFont1);
	DeleteObject(hBrushMenu);
}

void scanForVictory() {
	if (score == 100) {
		setGameMode(2); //победа!!!
	}
}

void scanForPlayer() {
	int i = 0;
	int j = 0;
	int stopSearchFlag = 0;
	while (i < HEIGHT) {
		j = 0;
		while (j < WIDTH) {
			if (map[i][j] == PLAYER) {
				stopSearchFlag = 1;
				break;
			}
			j++;
		}
		if (stopSearchFlag) {
			break;
		}
		i++;
	}
	if (!stopSearchFlag) { //gameOver
		setGameMode(3);
	}
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

int loadLevel(int levelNum, HWND hWnd) {
	char num = levelNum + '0';
	char levelPath[] = {'L','e','v','e','l','s','\\','L','e','v','e','l', num ,'.','t','x','t', '\0'};
	FILE* level = fopen(levelPath, "rt");
	if (level == NULL) {
		MessageBoxA(hWnd, (LPCSTR)"Неверно открытый файл!", (LPCSTR)"Ошибка!", MB_OK);
		return 0;
	}
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			fscanf(level, "%d", &map[i][j]);
		}
	}
	fscanf(level, "%d %d %d %d", &player.x, &player.y, &player.direction, &player.nextDirection);
	fscanf(level, "%d %d %d %d", &ghosts[0].direction, &ghosts[1].direction, &ghosts[2].direction, &ghosts[3].direction);
	ghosts[0].onFoodFlag = 0;
	ghosts[1].onFoodFlag = 0;
	ghosts[2].onFoodFlag = 0;
	ghosts[3].onFoodFlag = 0;
	return 1;
}