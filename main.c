// ***************************** ROULETTE 1.0 **********************************
// ************************ AUTORE: LORENZO VALENTE ****************************
// *********************** oneiros.valente@gmail.com ***************************

#include <roulette.h>

Player players[10];
int numberOfPlayers;
int turn;

void betting(), throw(), results();

// *****************************************************************************

void outBet(Player *player, int n) {
    outString("Puntata n.", YELLOW);
    outInt(n + 1, YELLOW);
    outString(": ", YELLOW);
    outInt((*player).bets[n].amount, GREEN);
    switch ((*player).bets[n].kind) {
        case 1:
            outString(" sul numero ", YELLOW);
            outString((*player).bets[n].bet, redOrWhite(stringToInt((*player).bets[n].bet)));
            break;
        case 2:
            outString(" sul cavallo ", YELLOW);
            outString((*player).bets[n].bet, GREEN);
            break;
        case 3:
            outString(" sul colore ", YELLOW);
            if (stringEqualsInt((*player).bets[n].bet, RED))
                outString("ROSSO", RED);
            else
                outString("NERO", WHITE);
            break;
        case 4:
            outString(" sui numeri ", YELLOW);
            if (stringEqualsInt((*player).bets[n].bet, 0))
                outString("PARI", BLUE);
            else
                outString("DISPARI", LIME);
            break;
        case 5:
            outString(" sui ", YELLOW);
            if (stringEqualsInt((*player).bets[n].bet, 0))
                outString("MANQUE", PINK);
            else
                outString("PASSE", BORDEAUX);
            break;
        case 6:
            outString(" sulla dozzina ", YELLOW);
            outInt(stringToInt((*player).bets[n].bet) + 1, PURPLE);
            break;
        case 7:
            outString(" sulla colonna ", YELLOW);
            outInt(stringToInt((*player).bets[n].bet) + 1, OCRA);
            break;
    }
}

void results(int number) {
    clear();
    setConsoleSize(50, 80);
    setConsoleCentered();
    boolean stillPlaying = FALSE;
    for (turn = 0; turn < numberOfPlayers; turn++) {
        newLine();
        Player *currentPlayer = &players[turn];
        starredString((*currentPlayer).name, YELLOW, GREEN);
        int i = 0;
        for (i = 0; i < (*currentPlayer).numberOfBets; i++) {
            outBet(currentPlayer, i);
            newLine();
            outInt((*currentPlayer).bets[i].amount, GREEN);
            if ((*currentPlayer).bets[i].numbers[number] == FALSE)
                (*currentPlayer).bets[i].moltiplicator = 0;
            outString(" X ", GRAY);
            outInt((*currentPlayer).bets[i].moltiplicator, RED);
            outString(" = ", GRAY);
            int winning = (*currentPlayer).bets[i].moltiplicator * (*currentPlayer).bets[i].amount;
            outInt(winning, RED);
            (*currentPlayer).cash += winning;
            newLines(2);
        }
        if ((*currentPlayer).cash > 0)
            stillPlaying = TRUE;
        outString("Cash: ", RED);
        outInt((*currentPlayer).cash, RED);
        newLines(2);
        cleanPlayerBets(currentPlayer);
    }
    if (stillPlaying == FALSE) {
        outStringLine("Partita terminata!", YELLOW);
        pause();
        clear();
        main();
    } else {
        char choice;
        do {
            outString("Vuoi continuare a giocare? ", YELLOW);
            outString("[", GREEN);
            outChar('S', LIME);
            outString("/", GREEN);
            outChar('N', RED);
            outString("] ", GREEN);
            choice = inChar(RED);
            if (choice != 'S' && choice != 's' && choice != 'N' && choice != 'n')
                outString("Input non valido. Inserisci \"S\" se vuoi continuare a giocare, altrimenti inserisci \"N\"\n", RED);
        } while (choice != 'S' && choice != 's' && choice != 'N' && choice != 'n');
        if (choice == 'S' || choice == 's')
            betting();
    }
}

void throw() {
    clear();
    setConsoleSize(14, 110);
    setConsoleCentered();
    outString("Les jeux sont faits, rien ne va plus!", YELLOW);
    newLines(2);
    starredString("LANCIO DELLA PALLINA", YELLOW, GREEN);
    drawRoulette();
    newLines(2);
    pause();
    clearLine(getCursor().Y - 1);
    setCursor(0, getCursor().Y - 2);
    int number = throwBall();
    newLines(2);
    outString("Le numero gagnant est ", YELLOW);
    outInt(number, redOrWhite(number));
    outString("!", YELLOW);
    newLines(2);
    pause();
    results(number);
}

void betting() {
    for (turn = 0; turn < numberOfPlayers; turn++) {
        int operation = 0;
        do {
            Player *currentPlayer = &players[turn];
            clear();
            drawTable(currentPlayer);
            newLines(2);
            outString("Cash: ", RED);
            outInt((*currentPlayer).cash, YELLOW);
            newLines(2);
            if ((*currentPlayer).numberOfBets >= 1) {
                starredString("BETS", RED, YELLOW);
                int i = 0;
                for (i = 0; i < (*currentPlayer).numberOfBets; i++) {
                    outBet(currentPlayer, i);
                    newLine();
                }
                newLine();
            }
            starredString(stringToUpper((*currentPlayer).name), BLUE, YELLOW);
            if ((*currentPlayer).cash == 0) {
                outString("Cash terminato, turno terminato\n\n", RED);
                operation = 0;
                pause();
            } else {
                String strings[7] = {
                    " - Punta su un singolo numero",
                    " - Punta su due numeri adiacenti",
                    " - Punta sui Rossi o sui Neri",
                    " - Punta sui Pari o sui Dispari",
                    " - Punta sui primi 18 o sugli ultimi 18",
                    " - Punta su una dozzina",
                    " - Punta su una colonna",
                };
                menu2(7, strBets, strings, RED, YELLOW, RED, GREEN, "Termina il tuo turno");
                operation = controlledInInt("\n\nFaites vos jeux, messieurs! ", YELLOW, RED, 0, 7, "Operazione non valida!");
                switch (operation) {
                    case 1:
                    {
                        int number = controlledInInt("Inserisci il numero: ", YELLOW, RED, 0, 36, "Numero non valido!\n");
                        int bet = controlledInInt("Quanto vuoi puntare? ", YELLOW, RED, 0, (*currentPlayer).cash, "Puntata non valida!\n");
                        betOnNumber(currentPlayer, number, bet);
                        break;
                    }
                    case 2:
                    {
                        int firstNumber, secondNumber;
                        boolean control;
                        do {
                            firstNumber = controlledInInt("Inserisci il primo numero: ", YELLOW, RED, 0, 36, "Numero non valido!\n");
                            secondNumber = controlledInInt("Inserisci il secondo numero: ", YELLOW, RED, 0, 36, "Numero non valido!\n");
                            control = numbersAreNear(firstNumber, secondNumber);
                            if (control == FALSE)
                                outStringLine("I due numeri devono essere adiacenti!\n", RED);
                        } while (control == FALSE);
                        int bet = controlledInInt("Quanto vuoi puntare? ", YELLOW, RED, 0, (*currentPlayer).cash, "Puntata non valida!\n");
                        betOnCheval(currentPlayer, firstNumber, secondNumber, bet);
                        break;
                    }
                    case 3:
                    {
                        char charColor;
                        do {
                            outString("Vuoi puntare sui Rossi o sui Neri? ", YELLOW);
                            outString("[", GREEN);
                            outChar('R', RED);
                            outString("/", GREEN);
                            outChar('N', WHITE);
                            outString("] ", GREEN);
                            charColor = inChar(RED);
                            if (charColor != 'R' && charColor != 'r' && charColor != 'N' && charColor != 'n')
                                outString("Input non valido. Inserisci \"R\" per Rosso o \"N\" per Nero\n", RED);
                        } while (charColor != 'R' && charColor != 'r' && charColor != 'N' && charColor != 'n');
                        double bet = controlledInInt("Quanto vuoi puntare? ", YELLOW, RED, 0, (*currentPlayer).cash, "Puntata non valida!\n");
                        betOnColor(currentPlayer, (charColor == 'R' || charColor == 'r') ? RED : BLACK, bet);
                        break;
                    }
                    case 4:
                    {
                        char charPorI;
                        do {
                            outString("Vuoi puntare sui Pari o sui Dispari? ", YELLOW);
                            outString("[", GREEN);
                            outChar('P', BLUE);
                            outString("/", GREEN);
                            outChar('D', LIME);
                            outString("] ", GREEN);
                            charPorI = inChar(RED);
                            if (charPorI != 'P' && charPorI != 'p' && charPorI != 'D' && charPorI != 'd')
                                outString("Input non valido. Inserisci \"P\" per Pari o \"D\" per Dispari\n", RED);
                        } while (charPorI != 'P' && charPorI != 'p' && charPorI != 'D' && charPorI != 'd');
                        double bet = controlledInInt("Quanto vuoi puntare? ", YELLOW, RED, 0, (*currentPlayer).cash, "Puntata non valida!\n");
                        betOnPairOrImpair(currentPlayer, (charPorI == 'P' || charPorI == 'p') ? 0 : 1, bet);
                        break;
                    }
                    case 5:
                    {
                        char charMoP;
                        do {
                            outString("Vuoi puntare sui Manque o sui Passe? ", YELLOW);
                            outString("[", GREEN);
                            outChar('M', PINK);
                            outString("/", GREEN);
                            outChar('P', BORDEAUX);
                            outString("] ", GREEN);
                            charMoP = inChar(RED);
                            if (charMoP != 'M' && charMoP != 'm' && charMoP != 'P' && charMoP != 'p')
                                outString("Input non valido. Inserisci \"M\" per Manque o \"P\" per Passe\n", RED);
                        } while (charMoP != 'M' && charMoP != 'm' && charMoP != 'P' && charMoP != 'p');
                        double bet = controlledInInt("Quanto vuoi puntare? ", YELLOW, RED, 0, (*currentPlayer).cash, "Puntata non valida!\n");
                        betOnManqueOrPasse(currentPlayer, (charMoP == 'M' || charMoP == 'm') ? 0 : 1, bet);
                        break;
                    }
                    case 6:
                    {
                        int dozen;
                        do {
                            outString("Su quale dozzina vuoi puntare) ", YELLOW);
                            outString("[", GREEN);
                            outInt(1, PURPLE);
                            outString("/", GREEN);
                            outInt(2, PURPLE);
                            outString("/", GREEN);
                            outInt(3, PURPLE);
                            outString("] ", GREEN);
                            dozen = inInt(RED);
                            if (dozen < 1 || dozen > 3)
                                outString("Input non valido. Inserici il numero della dozzina (1, 2 o 3)\n", RED);
                        } while (dozen < 1 || dozen > 3);
                        double bet = controlledInInt("Quanto vuoi puntare? ", YELLOW, RED, 0, (*currentPlayer).cash, "Puntata non valida!\n");
                        betOnDozen(currentPlayer, dozen - 1, bet);
                        break;
                    }
                    case 7:
                    {
                        int column;
                        do {
                            outString("Su quale colonna vuoi puntare? ", YELLOW);
                            outString("[", GREEN);
                            outInt(1, OCRA);
                            outString("/", GREEN);
                            outInt(2, OCRA);
                            outString("/", GREEN);
                            outInt(3, OCRA);
                            outString("] ", GREEN);
                            column = inInt(RED);
                            if (column < 1 || column > 3)
                                outString("Input non valido. Inserici il numero della colonna (1, 2 o 3)\n", RED);
                        } while (column < 1 || column > 3);
                        double bet = controlledInInt("Quanto vuoi puntare? ", YELLOW, RED, 0, (*currentPlayer).cash, "Puntata non valida!\n");
                        betOnColumn(currentPlayer, column - 1, bet);
                        break;
                    }
                }
                newLine();
                pause();
            }
        } while (operation != 0);
    }
    throw();
}

int main(int argc, char** argv) {
    setConsoleSize(14, 81);
    setConsoleCentered();
    Sleep(100);
    setTitle("Roulette 1.0 by Oneiros");
    drawAnimatedLogo();
    pause();
    setConsoleSize(50, 80);
    setConsoleCentered();
    clear();
    starredString("BENVENUTI", YELLOW, RED);
    numberOfPlayers = controlledInInt("Inserisci il numero di giocatori: ", WHITE, RED, 1, 10, "Inserire un numero da 1 a 10\n\n");
    newLines(2);
    for (turn = 0; turn < numberOfPlayers; turn++) {
        outString("[", GRAY);
        outString("Giocatore ", RED);
        outInt(turn + 1, RED);
        outString("] ", GRAY);
        outString("Inserisci il tuo nome: ", WHITE);
        initPlayer(&players[turn], inString(RED));
    }
    newLine();
    pause();
    betting();
}
