/*
 * File:   oneiros.h
 * Author: Oneiros - Lorenzo Valente (oneiros.valente@gmail.com)
 * Version: 1.1
 *
 * Created on 10 maggio 2010, 9.12
 */
#define _WIN32_WINNT 0x0500
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>

#define BLACK 0
#define DARK_BLUE 1
#define GREEN 2
#define TEAL 3
#define BORDEAUX 4
#define PURPLE 5
#define OCRA 6
#define LIGHT_GRAY 7
#define GRAY 8
#define BLUE 9
#define LIME 10
#define LIGHT_BLUE 11
#define RED 12
#define PINK 13
#define YELLOW 14
#define WHITE 15
#define CONTROL_CODE -23081990

typedef char *String;

// ************************** CONSOLE OPERATIONS *******************************

void clear() {
    system("cls");
}

void pause() {
    system("pause");
}

void setTitle(String s) {
    SetConsoleTitle(s);
}

void setCursor(unsigned int line, unsigned int col) {
    COORD coord;
    coord.X = line;
    coord.Y = col;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

COORD getCursor() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
        return csbi.dwCursorPosition;
}

void newLines(int n) {
    int i;
    for (i = 0; i < n; i++)
        printf("\n");
}

void newLine() {
    newLines(1);
}

void clearLine(int line) {
    COORD aux = getCursor();
    setCursor(0, line);
    while (getCursor().Y == line) {
        printf(" ", getCursor().Y);
    }
    setCursor(aux.X, aux.Y);
}

unsigned int getConsoleWindowWidth() {
    RECT r;
    GetWindowRect(GetConsoleWindow(), &r);
    return (r.right - r.left);
}

unsigned int getConsoleWindowHeight() {
    RECT r;
    GetWindowRect(GetConsoleWindow(), &r);
    return (r.bottom - r.top);
}

unsigned int getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
        return csbi.srWindow.Right + 1;
}

unsigned int getConsoleHeight() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
        return csbi.srWindow.Bottom + 1;
}

String intToString(int n);

void setConsoleSize(unsigned int lines, unsigned int cols) {
    String aux = (String) malloc(20);
    strcpy(aux, "mode con: lines=");
    strcat(aux, intToString(lines));
    system(aux);
    strcpy(aux, "mode con: cols=");
    strcat(aux, intToString(cols));
    system(aux);
    free(&aux);
}

void setConsoleLocation(unsigned int x, unsigned int y) {
    SetWindowPos(GetConsoleWindow(), 0, x, y, 0, 0, SWP_NOZORDER || SWP_NOSIZE);
}

void adaptScreenBufferSize() {
    COORD c;
    c.Y = getConsoleHeight();
    c.X = getConsoleWidth();
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void setConsoleCentered() {
    RECT r;
    GetWindowRect(GetConsoleWindow(), &r);
    int x = (GetSystemMetrics(SM_CXSCREEN) - getConsoleWindowWidth()) / 2;
    int y = (GetSystemMetrics(SM_CYSCREEN) - getConsoleWindowHeight()) / 2;
    setConsoleLocation(x, y);
}

void beep() {
    printf("\a");
}

void flush() {
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
}

// *************************** COLOR OPERATIONS ********************************

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

int getColor() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
        return csbi.wAttributes & 0xFF;
    return 0;
}

void setTextColor(int color) {
    setColor((getBackgroundColor() * 16) + color);
}

void setBackgroundColor(int color) {
    setColor(getTextColor() + (color * 16));
}

int getTextColor() {
    return getColor() % 16;
}

int getBackgroundColor() {
    return getColor() / 16;
}

// ************************** INTEGER OPERATIONS *******************************

String intToString(int n) {
    String aux = (String) malloc(n / 10);
    itoa(n, aux, 10);
    return aux;
}

// *************************** CHAR OPERATIONS *******************************

char charToUpper(char toUpper) {
    if ((toUpper >= 'a') && (toUpper <= 'z'))
        return (char) (toUpper - 0x20);
    return toUpper;
}

// *************************** STRING OPERATIONS *******************************

void initString(String *string, String input) {
    *string = (String) malloc(strlen(input));
    *string = input;
}

String stringToUpper(String text) {
    int i;
    for (i = 0; i < strlen(text); i++)
        if (text[i] >= 'A' && text[i] <= 'Z' || text[i] >= 'a' && text[i] <= 'z')
            text[i] = charToUpper(text[i]);
    return text;
}

int stringToInt(String s) {
    return atoi(s);
}

boolean stringEqualsString(String s1, String s2) {
    if (strcmp(s1, s2) == 0)
        return TRUE;
    return FALSE;
}

boolean stringEqualsStringIgnoreCase(String s1, String s2) {
    int i = 0;
    for (i = 0; i < strlen(s1); i++)
        if (s1[i] != s2[i] && charToUpper(s1[i]) != s2[i] && s1[i] != charToUpper(s2[i]))
            return FALSE;
    return TRUE;
}

boolean stringEqualsInt(String s, int n) {
    if (strcmp(s, intToString(n)) == 0)
        return TRUE;
    return FALSE;
}

// *************************** OUTPUT OPERATIONS *******************************

void outInt(int integer, int color) {
    int colorAttuale = getTextColor();
    setTextColor(color);
    printf("%d", integer);
    setTextColor(colorAttuale);
}

void outFloat(float number, int color) {
    int colorAttuale = getTextColor();
    setTextColor(color);
    printf("%.2f", number);
    setTextColor(colorAttuale);
}

void outDouble(double number, int color) {
    int colorAttuale = getTextColor();
    setTextColor(color);
    printf("%.4f", number);
    setTextColor(colorAttuale);
}

void outChar(char character, int pColor) {
    int colorAttuale = getTextColor();
    setTextColor(pColor);
    printf("%c", character);
    setTextColor(colorAttuale);
}

void outString(String text, int color) {
    int colorAttuale = getTextColor();
    setTextColor(color);
    printf("%s", text);
    setTextColor(colorAttuale);
}

void outBoolean(boolean boo, int color) {
    if (boo)
        outString("true", color);
    else
        outString("false", color);
}

void outStringLine(String text, int color) {
    outString(text, color);
    newLine();
}

// **************************** INPUT OPERATIONS *******************************

int inInt(int color) {
    int input = 0, oldColor = getTextColor();
    setTextColor(color);
    scanf("%d", &input);
    while (getchar() != '\n') input = CONTROL_CODE;
    setTextColor(oldColor);
    return input;
}

int controlledInInt(String text, int textColor, int inputColor, int from, int to, String error) {
    int input;
    do {
        outString(text, textColor);
        input = inInt(inputColor);
        if (input < from || input > to)
            outString(error, RED);
    } while (input < from || input > to);
    return input;
}

float inFloat(int color) {
    int oldColor = getTextColor();
    setTextColor(color);
    float inserimento;
    scanf("%f", &inserimento);
    while (getchar() != '\n') inserimento = CONTROL_CODE;
    setTextColor(oldColor);
    return inserimento;
}

double inDouble(int color) {
    int oldColor = getTextColor();
    setTextColor(color);
    double inserimento;
    scanf("%f", &inserimento);
    while (getchar() != '\n') inserimento = CONTROL_CODE;
    setTextColor(oldColor);
    return inserimento;
}

char inChar(int color) {
    int oldColor = getTextColor();
    setTextColor(color);
    char c = getchar();
    while (getchar() != '\n');
    setTextColor(oldColor);
    return c;
}

String inString(int color) {
    int oldColor = getTextColor();
    setTextColor(color);
    String input;
    do {
        input = (String) malloc(60);
        fgets(input, 60, stdin);
        if (input[strlen(input) - 1] == '\n') input[strlen(input) - 1] = '\0';
    } while (stringEqualsString(input, ""));
    setTextColor(oldColor);
    return input;
}

// *************************** RANDOM OPERATIONS *******************************

int randomInt(int from, int to) {
    return rand() % (abs(from - to) + 1) + from;
}

void randomize() {
    srand(time(NULL));
}

// ******************************** MENU ***************************************

void starredString(String title, int color, int starColor) {
    int i;
    for (i = 0; i < strlen(title) + 4; i++)
        outString("*", starColor);
    printf("\n");
    outString("* ", starColor);
    outString(title, color);
    outString(" *", starColor);
    printf("\n");
    for (i = 0; i < strlen(title) + 4; i++)
        outString("*", starColor);
    printf("\n\n");
}

void menu(int menuNumber, String menu[], int color, int secondColor, int thirdColor, String end) {
    int i;
    for (i = 0; i < menuNumber; i++) {
        outString("[", secondColor);
        outInt((i + 1), color);
        outString("] ", secondColor);
        outString(menu[i], thirdColor);
        printf("\n");
    }
    outString("[", secondColor);
    outString("0", color);
    outString("] ", secondColor);
    outString(end, thirdColor);
}

void menu2(int menuNumber, String menu[], String menu2[], int color, int color2, int color3, int color4, String end) {
    int i;
    for (i = 0; i < menuNumber; i++) {
        outString("[", color2);
        outInt((i + 1), color);
        outString("] ", color2);
        outString(menu[i], color3);
        outString(menu2[i], color4);
        printf("\n");
    }
    outString("[", color2);
    outString("0", color);
    outString("] ", color2);
    outString(end, color4);
}
