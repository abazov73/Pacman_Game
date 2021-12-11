
// Pacman_Game.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Pacman_Game.h"
#include "Game.h"
#include <stdio.h>

#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4
#define STOP 0
#define MAX_LOADSTRING 100
#define _CRT_SECURE_NO_WARNINGS

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
int gameMode = 0; // 0 - меню
                  // 1 - игра
                  // 2 - финальнай экран (победа)
                  // 3 - финальнай экран (поражение)

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PACMANGAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PACMANGAME));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PACMANGAME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PACMANGAME);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 1000, 609, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


void setGameMode(int gm) {
    gameMode = gm;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hBtn; // дескриптор кнопки
    static HWND hEdt1; // дескрипторы поля редактирования

    switch (message)
    {
    case WM_COMMAND:
        {
        if (lParam == (LPARAM)hBtn) {
            TCHAR StrT[20];
            char str[20];
            GetWindowText(hEdt1, StrT, sizeof(StrT));
            wcstombs(str, StrT, 20);
            SetFocus(hWnd);
            gameMode = 1;
            savePlayerName(str);
            InvalidateRect(hWnd, NULL, TRUE);
            DestroyWindow(hBtn);
            DestroyWindow(hEdt1);
        }
        else {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        }
        break;
    case WM_CREATE:
        hInst = ((LPCREATESTRUCT)lParam)->hInstance; // дескриптор приложения
        // Создаем и показываем поле редактирования - для ввода имени рекордсмена
        hEdt1 = CreateWindowW(_T("edit"), _T("Noname"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT, 400, 250, 160, 20,
            hWnd, 0, hInst, NULL);
        ShowWindow(hEdt1, SW_SHOWNORMAL);

        // Создаем и показываем кнопку
        hBtn = CreateWindowW(_T("button"), _T("Начать!"),
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            400, 300, 160, 20, hWnd, 0, hInst, NULL);
        ShowWindow(hBtn, SW_SHOWNORMAL);

        SetTimer(hWnd, 1, 300, 0);
        srand(GetTickCount64());
        break;
    case WM_TIMER:
        if (gameMode == 1) {
            MoveGhosts();
            tryChangePlayerDirection();
            movePlayer();
            scanForVictory();
            scanForPlayer();
        }
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            if (gameMode == 0) {
                drawMenu(hdc);
            }
            else if (gameMode == 1) {
                DrawMap(hdc);
            }
            else if (gameMode == 2) {
                drawFinalScreen(hdc, 0);
            }
            else {
                drawFinalScreen(hdc, 1);
            }
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_KEYDOWN:
        switch (wParam) {
        case 0x41: // клавиша A
            setNextDirection(LEFT);
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case 0x57:
            setNextDirection(UP); // клавиша W
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case 0x44: // клавиша D
            setNextDirection(RIGHT);
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case 0x53: // клавиша S
            setNextDirection(DOWN);
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_F5: // клавиша F5
            saveGame();
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_F9: // клавиша F9
            loadGame();
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
