#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#define KOLORY 6
#define COLORSWITHGREEN 7
#define GREEN 1
#define BLUE 2
#define RED 3
#define VIOLET 4
#define YELLOW 5
#define WHITE 6
#define BLACK 7

struct card {
    char kolor[10];
    int wartosc;
};

typedef struct list {
    card karta;
    struct list* next;
}list_t;

typedef struct intlist {
    int value;
    struct intlist* next;
}intlist_t;

void init(list_t* head) {
    head->next = NULL;
}

void initInt(intlist_t* head) {
    head->next = NULL;
}

void addElementInt(intlist_t* l, int value) {
    intlist_t* x = (intlist_t*)malloc(sizeof(intlist_t));
    x->value = value;
    x->next = l->next;
    l->next = x;
}

void addElement(list_t* l, int value, char color[10]) {
    list_t* x = (list_t*)malloc(sizeof(list_t));
    x->karta.wartosc = value;
    strcpy(x->karta.kolor, color);
    x->next = l->next;
    l->next = x;
}

void addLast(list_t* l, int value, char color[10]) {
    list_t* cur = l;
    list_t* x = (list_t*)malloc(sizeof(list_t));
    x->karta.wartosc = value;
    strcpy(x->karta.kolor, color);
    x->next = NULL;
    cur = cur->next;
    if (cur == NULL) {
        cur = x;
    }
    else {
        while (l != NULL) {
            cur = cur->next;
        }
        cur = x;
    }
    l->next = cur;
}

void removeElementFront(list_t* l) {
    list_t* returnList = (list_t*)malloc(sizeof(list_t));
    list_t* head = l;
    list_t* deleteObject;
    if (l == NULL) {
        cout << "List already empty" << endl;
    }
    else {
        head = head->next;
        deleteObject = head;
        head = head->next;
        l->next = head;
        free(deleteObject);
    }

}

void print(list_t* h) {
    h = h->next;
    while (h != NULL) {
        cout << h->karta.wartosc << " " << h->karta.kolor << " ";
        h = h->next;
    }
}

void printInt(intlist_t* h) {
    h = h->next;
    while (h != NULL) {
        cout << h->value << " ";
        h = h->next;
    }
}

void printToFile(list_t* h, FILE* fp) {
    h = h->next;
    while (h != NULL) {
        fprintf(fp, "%i %s ", h->karta.wartosc, h->karta.kolor);
        h = h->next;
    }
}

void sprawdzZmienne(int n, int k, int g, int gv, int o) {
    if (n < 2 or n > 6) {
        cout << "n wrong" << endl;
        cin >> n;
    }
    if (k < 1 or k > 6) {
        cout << "k wrong" << endl;
        cin >> k;
    }
    if (g < 1 or g > 9) {
        cout << "g wrong" << endl;
        cin >> g;
    }
    if (gv < 1 or gv > 10) {
        cout << "gv wrong" << endl;
        cin >> gv;
    }
    if (o < 1 or o > 20) {
        cout << "o wrong" << endl;
        cin >> o;
    }
}

void wczytajWartosci(int tabela[], int o, int k, card tabelaKart[]) {

    for (int i = 0; i < o; i++) {
        int wartosc;
        cin >> wartosc;
        tabela[i] = wartosc;
    }

    for (int j = 0; j < k; j++) {
        for (int i = 0; i < o; i++) {
            tabelaKart[i + j * (k - 1)].wartosc = tabela[i];
        }
    }
}


void stworzTalie(int k, int g, int gv, int o, const char* colors[KOLORY], card tabelaKart[], int* tabelaWartosci) {
    for (int i = 0; i < g; i++) {
        tabelaKart[i].wartosc = gv;
        strcpy(tabelaKart[i].kolor, "green");
    }

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < o; j++) {
            tabelaKart[j + (i * o) + g].wartosc = tabelaWartosci[j];
            strcpy(tabelaKart[j + (i * o) + g].kolor, colors[i]);
        }
    }

    FILE* taliasave;
    taliasave = fopen("talia_save.txt", "w");

    for (int i = 0; i < o * k + g; i++) {
        fprintf(taliasave, "%i %s ", tabelaKart[i].wartosc, tabelaKart[i].kolor);
    }
}

void reverseList(list_t* l) {
    list_t* prev = NULL;
    list_t* cur = l->next;
    list_t* next = NULL;
    while (cur != NULL) {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    l->next = prev;
}

void dajDoRak(int o, int k, int g, int n, list_t** players, card* tabelaKart) {
    list_t* cur = NULL;
    for (int i = 0; i < o * k + g; i++) {
        cur = players[i % n];
        addElement(cur, tabelaKart[i].wartosc, tabelaKart[i].kolor);
        cur = cur->next;
    }
    for (int i = 0; i < n; i++) {
        reverseList(players[i]);
    }
}

void wypiszStanDoPliku(int numberOfPlayers, int k, list_t** players, FILE* fp, int* activePlayer, list_t** deckCards, list_t** pileCards, int exploTreshold) {

    for (int i = 0; i < k; i++) {
        reverseList(pileCards[i]);
    }

    fprintf(fp, "active player = %i\n", (*activePlayer));
    fprintf(fp, "players number = %i\n", numberOfPlayers);
    fprintf(fp, "explosion treshold = %i\n", exploTreshold);

    for (int i = 0; i < numberOfPlayers; i++) {
        fprintf(fp, "%i player hand cards: ", i + 1);
        printToFile(players[i], fp);

        fprintf(fp, "\n");
        fprintf(fp, "%i player deck cards: ", i + 1);
        printToFile(deckCards[i], fp);
        fprintf(fp, "\n");
    }
    for (int i = 0; i < k; i++) {
        fprintf(fp, "%i pile cards: ", i + 1);
        printToFile(pileCards[i],fp);
        fprintf(fp, "\n");
    }
}

int numberOfPlayers() {
    FILE* loadPlayerNumber;
    loadPlayerNumber = fopen("save.txt", "r");
    int n;
    fscanf(loadPlayerNumber, "%*s %*s %*c %*d");
    fscanf(loadPlayerNumber, "%*s %*s %*c %d", &n);

    fclose(loadPlayerNumber);

    return n;
}

void loadGameState(int* activePlayer, int numOfPlayers, int* exploTreshold, int* iloscPile, list_t** players, list_t** pileCards, list_t** deckCards, list_t* allCards) {
    FILE* fp;
    fp = fopen("save.txt", "r");
    int active;
    int value;
    char color[10];
    list_t* curAll = allCards;
    fscanf(fp, "%*s %*s %*c %d", &active);
    (*activePlayer) = active;
    fscanf(fp, "%*s %*s %*c %*d");
    int explo;
    fscanf(fp, "%*s %*s %*c %d", &explo);
    (*exploTreshold) = explo;

    fscanf(fp, "%*d %*s %*s %*s");
    for (int i = 0; i < 2 * numOfPlayers; i++) {
        list_t* curPlayers = players[i / 2];
        list_t* curDeck;
        if (i == 1) {
            curDeck = deckCards[0];
        }
        else {
            curDeck = deckCards[i / 2];
        }
        while (1) {
            fscanf(fp, "%d %s", &value, color);
            if (strcmp(color, "player") == 0) {
                fscanf(fp, "%*s %*s");
                break;
            }
            if (strcmp(color, "pile") == 0) {
                break;
            }
            if (i % 2 == 0) {
                addElement(curPlayers, value, color);
                addElement(curAll, value, color);
                curPlayers = curPlayers->next;
                curAll = curAll->next;
            }
            else {
                addElement(curDeck, value, color);
                addElement(curAll, value, color);
                curDeck = curDeck->next;
                curAll = curAll->next;
            }
        }
    }
    fscanf(fp, "%*s");
    char colorPile[10];
    int koniec = 1;
    int index = 0;
    while (koniec) {
        list_t* curPile = pileCards[index];
        index++;
        (*iloscPile)++;
        while (strcmp(colorPile, "pile") != 0) {
            fscanf(fp, "%d %s", &value, colorPile);
            if (strcmp(colorPile, "pile") != 0 and strcmp(colorPile, "cards:") != 0) {
                addElement(curPile, value, colorPile);
                addElement(curAll, value, colorPile);
                curPile = curPile->next;
                curAll = curAll->next;
            }
            if (feof(fp) != 0) {
                koniec = 0;
                break;
            }
        }
        fscanf(fp, "%s", colorPile);
    }
    fclose(fp);
}

void checkGreens(list_t* allCards, int* greenCount, int* greenValue) {
    allCards = allCards->next;
    int greenValueBufor = 0;
    int flaga = 0;
    while (allCards != NULL) {
        if (strcmp(allCards->karta.kolor, "green") == 0) {
            (*greenCount)++;
            (*greenValue) = allCards->karta.wartosc;
            if ((*greenValue) != greenValueBufor) {
                greenValueBufor = (*greenValue);
                flaga++;
            }
        }
        allCards = allCards->next;
    }
    if ((*greenCount) == 0) {
        cout << "Green cards does not exist" << endl;
    }
    else if (flaga > 1) {
        cout << "Different green card values occured" << endl;
    }
    else {
        cout << "Found " << (*greenCount) << " green cards, all with " << (*greenValue) << " value" << endl;
    }
    cout << endl;
}

void colorNumber(list_t* allCards, const char** colors, int sposob, int* equal, int* equalSize, int colorCheck[KOLORY]) {

    int colorCount[KOLORY];//blue, red, violet, yellow, white, black

    for (int i = 0; i < KOLORY; i++) {
        colorCount[i] = 0;
    }

    allCards = allCards->next;
    while (allCards != NULL) {
        if (strcmp(allCards->karta.kolor, "blue") == 0) {
            colorCount[0]++;
        }
        if (strcmp(allCards->karta.kolor, "red") == 0) {
            colorCount[1]++;
        }
        if (strcmp(allCards->karta.kolor, "violet") == 0) {
            colorCount[2]++;
        }
        if (strcmp(allCards->karta.kolor, "yellow") == 0) {
            colorCount[3]++;
        }
        if (strcmp(allCards->karta.kolor, "white") == 0) {
            colorCount[4]++;
        }
        if (strcmp(allCards->karta.kolor, "black") == 0) {
            colorCount[5]++;
        }
        allCards = allCards->next;
    }

    int ilosc;
    int flaga = 0;
    for (int i = 0; i < KOLORY; i++) {
        if (colorCount[i] > 0) {
            ilosc = colorCount[i];
        }
    }

    for (int i = 0; i < KOLORY; i++) {
        if (colorCount[i] > 0 and colorCount[i] != ilosc) {
            cout << "At least two colors with a different number of cards were found:" << endl;
            flaga++;
            break;
        }

    }

    if (flaga > 0) {
        for (int i = 0; i < KOLORY; i++) {
            if (colorCount[i] > 0) {
                cout << colors[i] << " cards are " << colorCount[i] << endl;
            }
        }
        cout << endl;
    }
    else {
        cout << "The number cards of all colors is equal: " << ilosc << endl;
        (*equal) = 1;
        (*equalSize) = ilosc;

        for (int i = 0; i < KOLORY; i++) {
            if (colorCount[i] > 0) {
                colorCheck[i] = i + 1;
            }
        }
    }
}

void swapTwo(intlist_t* a, intlist_t* b) {
    int bufor = a->value;
    a->value = b->value;
    b->value = bufor;
}

void bubbleSort(intlist_t* l) {
    l = l->next;
    int swapped;

    intlist_t* leftPointer;
    intlist_t* rightPointer = NULL;

    if (l != NULL) {
        do {
            swapped = 0;
            leftPointer = l;
            while (leftPointer->next != rightPointer) {
                if (leftPointer->value > leftPointer->next->value) {
                    swapTwo(leftPointer, leftPointer->next);
                    swapped = 1;
                }
                leftPointer = leftPointer->next;
            }
            rightPointer = leftPointer;
        } while (swapped == 1);
    }
}

void rozlozNaKolory(intlist_t** listOfColorCards, list_t* allCards) {
    intlist_t* curZero = listOfColorCards[0];
    intlist_t* curOne = listOfColorCards[1];
    intlist_t* curTwo = listOfColorCards[2];
    intlist_t* curThree = listOfColorCards[3];
    intlist_t* curFour = listOfColorCards[4];
    intlist_t* curFive = listOfColorCards[5];

    allCards = allCards->next;
    while (allCards != NULL) {
        if (strcmp(allCards->karta.kolor, "blue") == 0) {
            addElementInt(curZero, allCards->karta.wartosc);
            curZero = curZero->next;
        }
        if (strcmp(allCards->karta.kolor, "red") == 0) {
            addElementInt(curOne, allCards->karta.wartosc);
            curOne = curOne->next;
        }
        if (strcmp(allCards->karta.kolor, "violet") == 0) {
            addElementInt(curTwo, allCards->karta.wartosc);
            curTwo = curTwo->next;
        }
        if (strcmp(allCards->karta.kolor, "yellow") == 0) {
            addElementInt(curThree, allCards->karta.wartosc);
            curThree = curThree->next;
        }
        if (strcmp(allCards->karta.kolor, "white") == 0) {
            addElementInt(curFour, allCards->karta.wartosc);
            curFour = curFour->next;
        }
        if (strcmp(allCards->karta.kolor, "black") == 0) {
            addElementInt(curFive, allCards->karta.wartosc);
            curFive = curFive->next;
        }
        allCards = allCards->next;
    }

    printInt(listOfColorCards[5]);

    for (int i = 0; i < KOLORY; i++) {
        bubbleSort(listOfColorCards[i]);
    }
    cout << endl;
}

void sprawdzKolory(intlist_t** listOfColorCards, int equal, const char** colors, int colorCheck[KOLORY], intlist_t** listOfBeg) {

    int equalValues = 0;
    int flaga = 0;
    int firstNotEmpty = 0;
    int notEqual = 0; //1 if not equal, 0 if equal

    for (int i = 0; i < KOLORY; i++) {
        listOfBeg[i] = listOfColorCards[i];
        if (colorCheck[i] > 0 and flaga == 0) {
            firstNotEmpty = colorCheck[i] - 1;
            flaga = 1;
        }
    }

    if (equal == 1) {
        for (int i = 0; i < KOLORY; i++) {
            listOfColorCards[firstNotEmpty] = listOfBeg[firstNotEmpty]->next;
            listOfColorCards[i] = listOfBeg[i]->next;

            if (notEqual == 0) {
                if (listOfBeg[i]->next != NULL and colorCheck[i] >= 0) {
                    while (listOfColorCards[firstNotEmpty] != NULL) {
                        if (listOfColorCards[firstNotEmpty]->value == listOfColorCards[i]->value) {
                            listOfColorCards[firstNotEmpty] = listOfColorCards[firstNotEmpty]->next;
                            listOfColorCards[i] = listOfColorCards[i]->next;
                        }
                        else {
                            notEqual = 1;
                            break;
                        }
                    }
                }
            }
            else {
                cout << "The values of cards of all colors are not identical: " << endl;
                for (int j = 0; j < KOLORY; j++) {
                    if (listOfBeg[j]->next != NULL) {
                        cout << colors[j] << " cards values: ";
                        printInt(listOfBeg[j]);
                        cout << endl;
                    }
                }
                break;
            }
        }
        if (notEqual == 0) {
            if (notEqual == 0) {
                cout << endl;
                cout << "The values of cards of all colors are identical: " << endl;
                printInt(listOfBeg[firstNotEmpty]);
                cout << endl;
            }
        }
    }
    else {
        cout << "The values of cards of all colors are not identical: " << endl;
        for (int j = 0; j < KOLORY; j++) {
            if (listOfBeg[j]->next != NULL) {
                cout << colors[j] << " cards values: ";
                printInt(listOfBeg[j]);
                cout << endl;
            }
        }
    }
}

int listSize(list_t* h) {
    h = h->next;
    int size = 0;
    while (h != NULL) {
        size++;
        h = h->next;
    }
    return size;
}

void sprawdzKoloryPiles(int iloscPiles, list_t** pileCards) {
    for (int i = 0; i < iloscPiles; i++) {
        list_t* curPile = pileCards[i];
        curPile = curPile->next;
        while (curPile != NULL) {
            if (curPile->next != NULL) {
                if (strcmp(curPile->karta.kolor, curPile->next->karta.kolor) != 0) {
                    cout << "Two different colors were found on the " << i + 1 << " pile" << endl;
                }
            }
            curPile = curPile->next;
        }
    }
}

void sprawdzExploPiles(int iloscPiles, list_t** pileCards, int exploTreshold) {
    for (int i = 0; i < iloscPiles; i++) {
        int sumaWart = 0;
        list_t* curPile = pileCards[i];
        curPile = curPile->next;
        while (curPile != NULL) {
            sumaWart += curPile->karta.wartosc;
            curPile = curPile->next;
        }
        if (sumaWart > exploTreshold) {
            cout << "Pile number " << i + 1 << " should explode earlier" << endl;
        }
    }
}

void sprawdzStan(int numOfPlayers, list_t** players) {
    for (int i = 1; i < numOfPlayers; i++) {
        int roznica = (listSize(players[i - 1]) - listSize(players[i]));
        if (roznica > 2 or roznica < -2) {
            cout << "The number of players cards on hand is wrong" << endl;
            break;
        }
    }
}

int checkIfHasColor(const char* color, list_t** pileCards, int i) { //1 jesli ma, 0 jesli nie ma 
    list_t* bufor = pileCards[i];
    bufor = bufor->next;
    while (bufor != NULL) {
        if (strcmp(bufor->karta.kolor, color) == 0) {
            return 1;
            break;
        }
        bufor = bufor->next;
    }
    return 0;
}

int checkIfOnlyGreen(list_t** pileCards, int i) {
    list_t* bufor = pileCards[i];
    bufor = bufor->next;
    int onlyGreen = 1; //1 same zielone, 0 nie same zielone
    while (bufor != NULL) {
        if (strcmp(bufor->karta.kolor, "green") != 0){
            onlyGreen = 0;
        }
        bufor = bufor->next;
    }
    return onlyGreen;
}

int sprawdzEksplozje(int exploTreshold, list_t** pileCards, list_t** poczatekPile, int iloscPile) {

    for (int i = 0; i < iloscPile; i++) {
        int sumaWartosci = 0;
        list_t* bufor = (list_t*)malloc(sizeof(list_t));
        bufor = pileCards[i]->next;
        while (bufor != NULL) {
            sumaWartosci += bufor->karta.wartosc;
            bufor = bufor->next;
        }
        if (sumaWartosci > exploTreshold) {
            return i + 1;
            break;
        }
    }
    return 0;
}

void dodajDoDecku(list_t** pileCards, list_t** deckCards, int explo, int* activePlayer) {
    list_t* pileBeg = pileCards[explo];
    pileBeg = pileBeg->next;
    list_t* cur = deckCards[(*activePlayer) - 1];
    while (pileBeg != NULL) {
        addElement(cur, pileBeg->karta.wartosc, pileBeg->karta.kolor);
        cur = cur->next;
        pileBeg = pileBeg->next;
    }
    pileCards[explo]->next = NULL;
}

void turn(list_t** players, list_t** pileCards, int numOfPlayers, int* activePlayer, int iloscPile, int exploTreshold, list_t** poczatekPile, list_t** deckCards) {
    const char* green = "green";
    int flaga = 0;
    int flagaDwa = 1;
    if (strcmp(players[(*activePlayer) - 1]->next->karta.kolor, "green") == 0) {
        flaga++;
        list_t* playersBeg = players[(*activePlayer) - 1];
        playersBeg = playersBeg->next;
        addElement(pileCards[0], playersBeg->karta.wartosc, playersBeg->karta.kolor);
        removeElementFront(players[(*activePlayer) - 1]);
        int explo = sprawdzEksplozje(exploTreshold, pileCards, poczatekPile, iloscPile);
        if (explo > 0) {
            dodajDoDecku(pileCards, deckCards, explo - 1, activePlayer);
        }
        (*activePlayer)++;
        if ((*activePlayer) > numOfPlayers) {
            (*activePlayer) = 1;
        }
    }
    else {
        for (int i = 0; i < iloscPile; i++) {
            if (checkIfHasColor(players[(*activePlayer) - 1]->next->karta.kolor, pileCards, i) == 1) {
                flaga++;
                list_t* playersBeg = players[(*activePlayer) - 1];
                playersBeg = playersBeg->next;
                addElement(pileCards[i], playersBeg->karta.wartosc, playersBeg->karta.kolor);
                removeElementFront(players[(*activePlayer) - 1]);
                int explo = sprawdzEksplozje(exploTreshold, pileCards, poczatekPile, iloscPile);
                if (explo > 0) {
                    dodajDoDecku(pileCards, deckCards, explo - 1, activePlayer);
                }
                (*activePlayer)++;
                if ((*activePlayer) > numOfPlayers) {
                    (*activePlayer) = 1;
                }
                break;
            }
        }
    }
    if(flaga == 0) {
        for (int i = 0; i < iloscPile; i++) {
            if (checkIfOnlyGreen(pileCards, i) == 1 or listSize(pileCards[i]) == 0) {
                flagaDwa = 0;
                list_t* playersBeg = players[(*activePlayer) - 1];
                playersBeg = playersBeg->next;
                addElement(pileCards[i], playersBeg->karta.wartosc, playersBeg->karta.kolor);
                removeElementFront(players[(*activePlayer) - 1]);
                int explo = sprawdzEksplozje(exploTreshold, pileCards, poczatekPile, iloscPile);
                if (explo > 0) {
                    dodajDoDecku(pileCards, deckCards, explo - 1, activePlayer);
                }
                (*activePlayer)++;
                if ((*activePlayer) > numOfPlayers) {
                    (*activePlayer) = 1;
                }
                break;
            }
        }
    }
}

int allHandsEmpty(list_t** players, int numOfPlayers) {
    int flaga = 0;
    for (int i = 0; i < numOfPlayers; i++) {
        if (listSize(players[i]) == 0) {
            flaga++;
        }
    }
    if (flaga == numOfPlayers) {
        return 1;
    }
    else {
        return 0;
    }
}

void immunity(int** immuneList, int numOfPlayers, list_t** deckCards, const char** colors, int iloscPile, int wynikiGraczy[KOLORY]) {
    for (int i = 0; i < numOfPlayers; i++) {
        list_t* cur = deckCards[i];
        cur = cur->next;
        while (cur != NULL) {
            if (strcmp(cur->karta.kolor, "blue") == 0) {
                immuneList[i][BLUE - 2]++;
            }
            if (strcmp(cur->karta.kolor, "red") == 0) {
                immuneList[i][RED - 2]++;
            }
            if (strcmp(cur->karta.kolor, "violet") == 0) {
                immuneList[i][VIOLET - 2]++;
            }
            if (strcmp(cur->karta.kolor, "yellow") == 0) {
                immuneList[i][YELLOW - 2]++;
            }
            if (strcmp(cur->karta.kolor, "white") == 0) {
                immuneList[i][WHITE - 2]++;
            }
            if (strcmp(cur->karta.kolor, "black") == 0) {
                immuneList[i][BLACK - 2]++;
            }
            cur = cur->next;
        }
    }

    for (int j = 0; j < iloscPile; j++) {
        int max = 0;
        int indexMax;
        int remisy = 0;
        for (int i = 0; i < numOfPlayers; i++) {
            if (immuneList[i][j] > max) {
                indexMax = i;
                max = immuneList[i][j];
            }
            else if (immuneList[i][j] > 0 and immuneList[i][j] == max) {
                remisy++;
            }
        }
        if (remisy == 0) {
            cout << "gracz " << indexMax + 1 << " odporny na " << colors[j] << endl;
            int sumaDoOdjecia = 0;
            list_t* cur = deckCards[indexMax];
            cur = cur->next;
            while (cur != NULL) {
                if (strcmp(cur->karta.kolor, colors[j]) == 0) {
                    sumaDoOdjecia += 1;
                }
                cur = cur->next;
            }
            wynikiGraczy[indexMax] = wynikiGraczy[indexMax] - sumaDoOdjecia;
        }
    }
}

void scores(list_t** deckCards, int numOfPlayers, int** immuneList, const char** colors, int iloscPile) {
    int wynikiGraczy[KOLORY];
    for (int i = 0; i < numOfPlayers; i++) {
        int sumaPunktow = 0;
        list_t* cur = deckCards[i];
        cur = cur->next;
        while (cur != NULL) {
            if (cur->karta.kolor == "green") {
                sumaPunktow += 2;
            }
            else {
                sumaPunktow += 1;
            }
            cur = cur->next;
        }
        wynikiGraczy[i] = sumaPunktow;
        //cout << "Wynik Gracza " << i + 1 << " = " << wynikiGraczy[i] << endl;
    }

    immunity(immuneList, numOfPlayers, deckCards, colors, iloscPile, wynikiGraczy);

    for (int i = 0; i < numOfPlayers; i++) {
        cout << "Wynik Gracza " << i + 1 << " = " << wynikiGraczy[i] << endl;
    }
}

int main() {

    int n; //l. graczy
    int k; //l. kotlow
    int g; //l. zielonych kart
    int gv; //wartosc zielonych kart
    int o; //l. kart innych kolorow
    int exploTreshold;
    int activePlayer;
    int iloscPile = 0;
    card* tabelaKart = NULL;
    int* tabelaWartosci = NULL;
    int* immuneColors = NULL;
    list_t** players = NULL;
    list_t** deckCards = NULL;
    list_t** pileCards = NULL;
    intlist_t** listOfColorCards = NULL;
    intlist_t** listOfBeg = NULL;
    int** immuneList = NULL;
    list_t** poczatekPile = NULL;
    list_t allCards;
    int* wynikiGraczy = NULL;

    const char* colors[KOLORY] = { "blue", "red", "violet", "yellow", "white", "black" };
    const char* colorsWithGreen[COLORSWITHGREEN] = { "green", "blue", "red", "violet", "yellow", "white", "black" };

    char option;

    cout << "l - wczytaj | g - wygeneruj" << endl;
    cin >> option;

    int sposob;
    switch (option) {
    case 'g':
        activePlayer = 1;
        sposob = 0;

        cin >> n >> k >> g >> gv >> o;
        sprawdzZmienne(n, k, g, gv, o);

        iloscPile = k;

        tabelaKart = (card*)malloc(o * k * sizeof(card) + g * sizeof(card));
        if (tabelaKart == NULL) {
            cout << "Allocation error" << endl;
            return 1;
        }

        tabelaWartosci = (int*)malloc(o * k * sizeof(int) + g * sizeof(int));
        if (tabelaWartosci == NULL) {
            cout << "Allocation error" << endl;
            return 1;
        }

        wczytajWartosci(tabelaWartosci, o, k, tabelaKart);
        cin >> exploTreshold;
        stworzTalie(k, g, gv, o, colors, tabelaKart, tabelaWartosci);

        players = (list_t**)malloc(n * sizeof(list_t*));
        for (int i = 0; i < n; i++) {
            players[i] = (list_t*)malloc(sizeof(list_t));
            init(players[i]);
        }

        deckCards = (list_t**)malloc(n * sizeof(list_t*));
        for (int i = 0; i < n; i++) {
            deckCards[i] = (list_t*)malloc(sizeof(list_t));
            init(deckCards[i]);
        }

        pileCards = (list_t**)malloc(KOLORY * sizeof(list_t*));
        for (int i = 0; i < KOLORY; i++) {
            pileCards[i] = (list_t*)malloc(sizeof(list_t));
            init(pileCards[i]);
        }

        dajDoRak(o, k, g, n, players, tabelaKart);

        FILE* fp;
        fp = fopen("save_begin_status.txt", "w");

        wypiszStanDoPliku(n, k, players, fp, &activePlayer, deckCards, pileCards, exploTreshold);

        fclose(fp);

        free(tabelaKart);
        free(tabelaWartosci);

        break;

    case 'l':
        sposob = 1;
        int greenCount = 0;
        int cardCount = 0;
        int iloscKartPile = 0;
        int deckCardCount = 0;
        int greenValue = 0;
        int equal = 0;
        int equalSize;
        int colorCheck[KOLORY]; //blue, red, violet, yellow, white, black
        int flagaOne = 0;
        int flagaTwo = 0;
        int flagaThree = 0;

        for (int i = 0; i < KOLORY; i++) {
            colorCheck[i] = -1;
        }

        int allCardsTogether = iloscKartPile + deckCardCount + cardCount;
        n = numberOfPlayers();

        players = (list_t**)malloc(n * sizeof(list_t*));
        for (int i = 0; i < n; i++) {
            players[i] = (list_t*)malloc(sizeof(list_t));
            init(players[i]);
        }

        deckCards = (list_t**)malloc(n * sizeof(list_t*));
        for (int i = 0; i < n; i++) {
            deckCards[i] = (list_t*)malloc(sizeof(list_t));
            init(deckCards[i]);
        }

        pileCards = (list_t**)malloc(KOLORY * sizeof(list_t*));
        for (int i = 0; i < KOLORY; i++) {
            pileCards[i] = (list_t*)malloc(sizeof(list_t));
            init(pileCards[i]);
        }

        listOfColorCards = (intlist_t**)malloc(KOLORY * sizeof(intlist_t*));
        for (int i = 0; i < KOLORY; i++) {
            listOfColorCards[i] = (intlist_t*)malloc(sizeof(intlist_t));
            initInt(listOfColorCards[i]);
        }

        listOfBeg = (intlist_t**)malloc(KOLORY * sizeof(intlist_t*));
        for (int i = 0; i < KOLORY; i++) {
            listOfBeg[i] = (intlist_t*)malloc(sizeof(intlist_t));
            initInt(listOfBeg[i]);
        }

        poczatekPile = (list_t**)malloc(sizeof(list_t*));
        for (int i = 0; i < iloscPile; i++) {
            poczatekPile[i] = (list_t*)malloc(sizeof(list_t));
            init(poczatekPile[i]);
        }

        init(&allCards);

        loadGameState(&activePlayer, n, &exploTreshold, &iloscPile, players, pileCards, deckCards, &allCards);
        checkGreens(&allCards, &greenCount, &greenValue);
        colorNumber(&allCards, colors, sposob, &equal, &equalSize, colorCheck);
        rozlozNaKolory(listOfColorCards, &allCards);
        sprawdzKolory(listOfColorCards, equal, colors, colorCheck, listOfBeg);
        //sprawdzStan(players, exploTreshold, pileCards, n, iloscPile, poczatekPile, &flagaOne, &flagaTwo, &flagaThree);
        sprawdzStan(n, players);
        sprawdzKoloryPiles(iloscPile, pileCards);
        sprawdzExploPiles(iloscPile, pileCards, exploTreshold);

        cout << endl;
        break;
    }

    FILE* save;
    save = fopen("save_results.txt", "w");

    int koniec = 1;
    while (allHandsEmpty(players, n) != 1) {
        turn(players, pileCards, n, &activePlayer, iloscPile, exploTreshold, poczatekPile, deckCards);
        wypiszStanDoPliku(n, iloscPile, players, save, &activePlayer, deckCards, pileCards, exploTreshold);
        fprintf(save, "\n");
    }

    fclose(save);

    immuneList = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        immuneList[i] = (int*)malloc(KOLORY * sizeof(int));
        for (int j = 0; j < KOLORY; j++) {
            immuneList[i][j] = 0;
        }
    }

    scores(deckCards, n, immuneList, colors, iloscPile);

    char end;
    cin >> end;

    return 0;
}