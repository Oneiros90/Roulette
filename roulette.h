#include <oneiros.h>

String strBets[7] = {
    "Plein",
    "Cheval",
    "Rouge ou Noir",
    "Even ou Odd",
    "Manque ou Passe",
    "Douzaine",
    "Colonne",
};

typedef struct {
    boolean numbers[37];
    int moltiplicator;
    int amount;
    int kind;
    String bet;
} Bet;

typedef struct {
    String name;
    int cash;
    Bet bets[10];
    int numberOfBets;
} Player;

int rouletteNumbers[37] = {
    0, 32, 15, 19, 4, 21, 2, 25, 17, 34, 6, 27, 13, 36, 11, 30, 8, 23, 10,
    5, 24, 16, 33, 1, 20, 14, 31, 9, 22, 18, 29, 7, 28, 12, 35, 3, 26
};

boolean numbersAreNear(int firstNumber, int secondNumber) {
    if ((firstNumber == 0 && (secondNumber == 1 || secondNumber == 2 || secondNumber == 3)) ||
            (secondNumber == 0 && (firstNumber == 1 || firstNumber == 2 || firstNumber == 3)))
        return TRUE;
    if ((numberColumn(firstNumber) == 2 && numberColumn(secondNumber) == 0) ||
            (numberColumn(secondNumber) == 2 && numberColumn(firstNumber) == 0))
        return FALSE;
    if (firstNumber == secondNumber + 1 || firstNumber == secondNumber - 1)
        return TRUE;
    if (firstNumber == secondNumber + 3 || firstNumber == secondNumber - 3)
        return TRUE;
    return FALSE;
}

int numberDozen(int number) {
    return (number - 1) / 12;
}

int numberColumn(int number) {
    return (number + 2) % 3;
}

void cleanPlayerBets(Player *player) {
    int i = 0, j = 0;
    for (i = 0; i < (*player).numberOfBets; i++) {
        (*player).bets[i].moltiplicator = 0;
        (*player).bets[i].amount = 0;
        (*player).bets[i].bet = 0;
        for (j = 0; j <= 36; j++) {
            (*player).bets[i].numbers[j] = FALSE;
        }
    }
    (*player).numberOfBets = 0;
}

void initPlayer(Player *player, String name) {
    (*player).name = name;
    if (stringEqualsStringIgnoreCase((*player).name, "god"))
        (*player).cash = 99999999;
    else
        (*player).cash = 1500;
    cleanPlayerBets(player);
}

void initBet(Player *player, int kind, int amount, String str) {
    Bet *bet = &((*player).bets[(*player).numberOfBets]);
    (*bet).kind = kind;
    switch (kind) {
        case 1: (*bet).moltiplicator = 36;
            break;
        case 2: (*bet).moltiplicator = 18;
            break;
        case 3: (*bet).moltiplicator = 2;
            break;
        case 4: (*bet).moltiplicator = 2;
            break;
        case 5: (*bet).moltiplicator = 2;
            break;
        case 6: (*bet).moltiplicator = 3;
            break;
        case 7: (*bet).moltiplicator = 3;
            break;
    }
    (*bet).amount = amount;
    (*bet).bet = str;
    (*player).cash -= amount;
    (*player).numberOfBets++;
}

boolean betOnNumber(Player *player, int number, int amount) {
    if (number < 0 || number > 36)
        return FALSE;
    (*player).bets[(*player).numberOfBets].numbers[number] = TRUE;
    initBet(player, 1, amount, intToString(number));
    return TRUE;
}

boolean betOnCheval(Player *player, int firstNumber, int secondNumber, int amount) {
    if (firstNumber < 0 || firstNumber > 36 || secondNumber < 0 || secondNumber > 36 || !numbersAreNear(firstNumber, secondNumber))
        return FALSE;
    (*player).bets[(*player).numberOfBets].numbers[firstNumber] = TRUE;
    (*player).bets[(*player).numberOfBets].numbers[secondNumber] = TRUE;
    String str1 = intToString(firstNumber);
    String str2 = intToString(secondNumber);
    strcat(str1, ",");
    strcat(str1, str2);
    initBet(player, 2, amount, str1);
    return TRUE;
}

boolean betOnColor(Player *player, int color, int amount) {
    if (color != RED && color != BLACK)
        return FALSE;
    int j;
    for (j = 0; j <= 36; j++)
        if (redOrBlack(j) == color) {
            (*player).bets[(*player).numberOfBets].numbers[j] = TRUE;
        }
    initBet(player, 3, amount, intToString(color));
    return TRUE;
}

boolean betOnPairOrImpair(Player *player, int pairOrImpair, int amount) {
    if (pairOrImpair != 0 && pairOrImpair != 1)
        return FALSE;
    int j;
    for (j = 1; j <= 36; j++)
        if (j % 2 == pairOrImpair) {
            (*player).bets[(*player).numberOfBets].numbers[j] = TRUE;
        }
    initBet(player, 4, amount, intToString(pairOrImpair));
    return TRUE;
}

boolean betOnManqueOrPasse(Player *player, int manqueOrPasse, int amount) {
    if (manqueOrPasse != 0 && manqueOrPasse != 1)
        return FALSE;
    int j;
    for (j = 1; j <= 36; j++)
        if (j / 19 == manqueOrPasse) {
            (*player).bets[(*player).numberOfBets].numbers[j] = TRUE;
        }
    initBet(player, 5, amount, intToString(manqueOrPasse));
    return TRUE;
}

boolean betOnDozen(Player *player, int dozen, int amount) {
    if (dozen < 0 && dozen > 2)
        return FALSE;
    int j;
    for (j = 1; j <= 36; j++)
        if (numberDozen(j) == dozen) {
            (*player).bets[(*player).numberOfBets].numbers[j] = TRUE;
        }
    initBet(player, 6, amount, intToString(dozen));
    return TRUE;
}

boolean betOnColumn(Player *player, int column, int amount) {
    if (column < 0 && column > 2)
        return FALSE;
    int j;
    for (j = 1; j <= 36; j++)
        if (numberColumn(j) == column) {
            (*player).bets[(*player).numberOfBets].numbers[j] = TRUE;
        }
    initBet(player, 7, amount, intToString(column));
    return TRUE;
}

//------------------------------------------------------------------------------

int redOrBlack(int n) {
    switch (n) {
        case 1: case 3: case 5: case 7: case 9: case 12: case 14: case 16: case 18:
        case 19: case 21: case 23: case 25: case 27: case 30: case 32: case 34: case 36:
            return RED;
        default:
            return BLACK;
    }
}

int redOrWhite(int n) {
    return (redOrBlack(n) == RED) ? RED : WHITE;
}

void drawLogo(int n) {
    int color = RED;
    switch (n) {
        case 0:
            outString(" ,ggggggggggg,                                                                  \n", color);
            break;
        case 1:
            outString("dP\"\"\"88\"\"\"\"\"\"Y8,                          ,dPYb,           I8      I8           \n", color);
            break;
        case 2:
            outString("Yb,  88      `8b                          IP'`Yb           I8      I8           \n", color);
            break;
        case 3:
            outString(" `\"  88      ,8P                          I8  8I        8888888888888888        \n", color);
            break;
        case 4:
            outString("     88aaaad8P\"                           I8  8'           I8      I8           \n", color);
            break;
        case 5:
            outString("     88\"\"\"\"Yb,      ,ggggg,   gg      gg  I8 dP   ,ggg,    I8      I8    ,ggg,  \n", color);
            break;
        case 6:
            outString("     88     \"8b    dP\"  \"Y8gggI8      8I  I8dP   i8\" \"8i   I8      I8   i8\" \"8i \n", color);
            break;
        case 7:
            outString("     88      `8i  i8'    ,8I  I8,    ,8I  I8P    I8, ,8I  ,I8,    ,I8,  I8, ,8I \n", color);
            break;
        case 8:
            outString("     88       Yb,,d8,   ,d8' ,d8b,  ,d8b,,d8b,_  `YbadP' ,d88b,  ,d88b, `YbadP' \n", color);
            break;
        case 9:
            outString("     88        Y8P\"Y8888P\"   8P'\"Y88P\"`Y88P'\"Y88888P\"Y8888P\"\"Y8  8P\"\"Y8888P\"Y888\n", color);
    }
}

void drawAnimatedLogo() {
    int y = getCursor().Y;
    int i = 0;
    for (i = 0; i <= 9; i++) {
        drawLogo(i);
        Sleep(20);
        clearLine(i + y);
    }
    for (i = 9; i >= 0; i--) {
        setCursor(0, y + i);
        drawLogo(i);
        Sleep(20);
        clearLine(y + i);
    }
    setCursor(0, y);
    for (i = 0; i <= 9; i++) {
        drawLogo(i);
        Sleep(40);
    }
    outString("\n                          Progetto di Lorenzo Valente\n\n", GREEN);
    Sleep(70);

}

void drawNumbers(int n, Player *currentPlayer) {
    while (n <= 36) {
        outString("| ", YELLOW);
        int i = 0;
        for (i = 0; i < (*currentPlayer).numberOfBets; i++)
            if ((*currentPlayer).bets[i].numbers[n] == TRUE) {
                setBackgroundColor(WHITE);
            }
        if (n <= 9)
            printf(" ");
        outInt(n, redOrBlack(n));
        setBackgroundColor(GREEN);
        printf(" ");
        n += 3;
    }
}

void drawTable(Player *currentPlayer) {
    newLine();
    setBackgroundColor(BLACK);
    printf("  ");
    setBackgroundColor(GREEN);
    outString("     _____________________________________________________________________  \n", YELLOW);
    setBackgroundColor(BLACK);
    printf("  ");
    setBackgroundColor(GREEN);
    outString("    / |    |    |    |    |    |    |    |    |    |    |    |    |      |  \n", YELLOW);
    setBackgroundColor(BLACK);
    printf("  ");
    setBackgroundColor(GREEN);
    outString("   /  ", YELLOW);
    drawNumbers(3, currentPlayer);
    outString("| ", YELLOW);
    outString("2to1", WHITE);
    outString(" |  \n", YELLOW);
    setBackgroundColor(BLACK);
    printf("  ");
    setBackgroundColor(GREEN);
    outString("  /   |____|____|____|____|____|____|____|____|____|____|____|____|______|  \n", YELLOW);
    setBackgroundColor(BLACK);
    printf("  ");
    setBackgroundColor(GREEN);
    outString(" /    |    |    |    |    |    |    |    |    |    |    |    |    |      |  \n", YELLOW);
    setBackgroundColor(BLACK);
    printf("  ");
    setBackgroundColor(GREEN);
    outString(" | ", YELLOW);
    boolean zero = FALSE;
    int i = 0;
    for (i = 0; i < (*currentPlayer).numberOfBets; i++)
        if ((*currentPlayer).bets[i].numbers[0] == TRUE) {
            int color = getBackgroundColor();
            setBackgroundColor(WHITE);
            outString(" 0", BLACK);
            setBackgroundColor(color);
            i = -1;
            zero = TRUE;
            break;
        }
    if (!zero)
        outString(" 0", WHITE);
    printf(" ");
    drawNumbers(2, currentPlayer);
    outString("| ", YELLOW);
    outString("2to1", WHITE);
    outString(" |  \n", YELLOW);
    setBackgroundColor(BLACK);
    printf("  ");
    setBackgroundColor(GREEN);
    outString(" \\    |____|____|____|____|____|____|____|____|____|____|____|____|______|  \n", YELLOW);
    setBackgroundColor(BLACK);
    printf("  ");
    setBackgroundColor(GREEN);
    outString("  \\   |    |    |    |    |    |    |    |    |    |    |    |    |      |  \n", YELLOW);
    setBackgroundColor(BLACK);
    printf("  ");
    setBackgroundColor(GREEN);
    outString("   \\  ", YELLOW);
    drawNumbers(1, currentPlayer);
    outString("| ", YELLOW);
    outString("2to1", WHITE);
    outString(" |  \n", YELLOW);
    setBackgroundColor(BLACK);
    printf("  ");
    setBackgroundColor(GREEN);
    outString("    \\_|____|____|____|____|____|____|____|____|____|____|____|____|______|  \n", YELLOW);
    setBackgroundColor(BLACK);
    printf("  ");
    setBackgroundColor(GREEN);
    outString("      |                   |                   |                   |         \n", YELLOW);
    setBackgroundColor(BLACK);
    printf("  ");
    setBackgroundColor(GREEN);
    outString("      |       ", YELLOW);
    outString("1st 12", WHITE);
    outString("      |       ", YELLOW);
    outString("2nd 12", WHITE);
    outString("      |       ", YELLOW);
    outString("3rd 12", WHITE);
    outString("      |         \n", YELLOW);
    setBackgroundColor(BLACK);
    printf("  ");
    setBackgroundColor(GREEN);
    outString("      |___________________|___________________|___________________|         \n", YELLOW);
    setBackgroundColor(BLACK);
    printf("  ");
    setBackgroundColor(GREEN);
    outString("      |         |         |         |         |         |         |         \n", YELLOW);
    setBackgroundColor(BLACK);
    printf("  ");
    setBackgroundColor(GREEN);
    outString("      | ", YELLOW);
    outString("MANQUE", WHITE);
    outString("  |  ", YELLOW);
    outString("EVEN", WHITE);
    outString("   |   ", YELLOW);
    outString("RED", RED);
    outString("   |  ", YELLOW);
    outString("BLACK", BLACK);
    outString("  |   ", YELLOW);
    outString("ODD", WHITE);
    outString("   |  ", YELLOW);
    outString("PASSE", WHITE);
    outString("  |         \n", YELLOW);
    setBackgroundColor(BLACK);
    printf("  ");
    setBackgroundColor(GREEN);
    outString("      |_________|_________|_________|_________|_________|_________|         \n", YELLOW);
    setBackgroundColor(BLACK);
    printf("  ");
    setBackgroundColor(GREEN);
    setBackgroundColor(BLACK);
}

int drawRoulette() {
    outString("_____________________________________________________________________________________________________________", GRAY);
    newLine();
    int i = 0;
    for (i = 0; i < 36; i++) {
        outString("|", GRAY);
        if (rouletteNumbers[i] < 10)
            printf(" ");
        outInt(rouletteNumbers[i], redOrWhite(rouletteNumbers[i]));
    }
    outString("|", GRAY);
}

int throwBall() {
    randomize();
    int number = randomInt(1, 73);
    double i = 0;
    for (i = 1; i < 2000; i += i * i * 0.0005) {
        if (number == 72)
            number = 1;
        else
            number++;
        drawBall(number);
        Sleep((int) i);
    }
    flush();
    beep();
    return rouletteNumbers[(number-1)/2];
}

int drawBall(int ballPosition) {
    if (ballPosition < 0 || ballPosition > 74) {
        return 1;
    }
    COORD aux = getCursor();
    setCursor(aux.X - 2, aux.Y);
    printf("  ");
    if (ballPosition != 0) {
        ballPosition += (ballPosition - 1) / 2;
        setCursor(ballPosition, getCursor().Y);
        outString("@", WHITE);
    }
}
