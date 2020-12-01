#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
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
    const char *kolor;
    int wartosc;
};

typedef struct list {
    card karta;
    struct list* next;
}list_t;

void init(list_t* head) {
    head->next = NULL;
}

void addElement(list_t* l, int value, const char* color) {
    list_t* x = (list_t*)malloc(sizeof(list_t));
    x->karta.wartosc = value;
    x->karta.kolor = color;
    x->next = l->next;
    l->next = x;
}

void removeElement(list_t* l) {
    if (l->next != NULL) {
        void* x = l->next;
        l->next = l->next->next;
        free(x);
    }
}

void print(list_t* h) {
    h = h->next;
    while (h != NULL) {
        cout << h->karta.wartosc << " " << h->karta.kolor << " ";
        h = h->next;
    }
}

void printToFile(list_t* h, int i, FILE* fp) {
    fprintf (fp, "%i player hand cards: ", i + 1);

    while (h->next != NULL) {
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


void stworzTalie(int k, int g, int gv, int o, const char *colors[KOLORY], card tabelaKart[], int *tabelaWartosci) {
    for (int i = 0; i < g; i++) {
        tabelaKart[i].wartosc = gv;
        tabelaKart[i].kolor = "green";
    }

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < o; j++) {
            tabelaKart[j + (i * o) + g].wartosc = tabelaWartosci[j];
            tabelaKart[j + (i * o) + g].kolor = colors[i];
        }
    }
}

void reverseList(list_t** l) {
    list_t* prev = NULL;
    list_t* cur = *l;
    list_t* next = NULL;
    while (cur != NULL) {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    *l = prev;
}

void dajDoRak(int o, int k, int g, int n, list_t** players, card *tabelaKart) {
    for (int i = 0; i < o * k + g; i++) {
        list_t* cur = players[i % n];
        addElement(cur, tabelaKart[i].wartosc, tabelaKart[i].kolor);
        cur = cur->next;
    }
    for (int i = 0; i < n; i++) {

        reverseList(&players[i]);
    }
}

void wypiszStanDoPliku(int n, int k, int o, card *tabelaKart, int g, list_t** players, FILE* fp) {

    int wielkoscPelnych = ((o * k + g) / n) + 1;
    fprintf(fp, "active player = 1\n");
    fprintf(fp, "players number = %i\n", n);

    for (int i = 0; i < n; i++) {
        printToFile(players[i], i, fp);

        fprintf(fp, "\n");
        fprintf(fp, "%i player deck cards:\n", i + 1);
    }
    for (int i = 0; i < k; i++) {
        fprintf(fp, "%i pile cards:\n", i + 1);
    }
    fclose(fp);
}

int LiczbaGraczy() {
    FILE* loadFile;
    loadFile = fopen("save.txt", "r");

    for (int i = 0; i < 2; i++) {
        char line[1000];
        fgets(line, 1000, loadFile);
        int index = 0;
        if (i == 1) {
            char nGraczy;
            while (line[index] != '\n') {
                nGraczy = line[index];
                index++;
            }
            return (int)nGraczy - '0';
        }
    }
    fclose(loadFile);
}

void wczytajStanWartosci(int* cardCount, int* greenCount, int* greenValue, int* pileCount, int* greenValueCheck, int* deckCardCount) {

    FILE* loadFile;
    loadFile = fopen("save.txt", "r");

    const char* colorName = NULL;
    int value = 0;
    int indexKolorow = 0;
    const char* green = "green";
    char line[1000];
    int i = 0;
    int playerNumber = 0;
    int greenValueBufor = 0;

    while(fgets(line, 1000, loadFile) != NULL) {
        int x = 0;
        if (i == 1) {
            playerNumber = (int)line[17] - '0';
            //(*pileCount) -= 3;
        }
        if (i > 1 and i < 2 * playerNumber + 2) {
            int z = 0;
            int flaga = 0;
            char lineEnd[1000];
            while (line[x] != '\n') {
                if (line[x] == ':') {
                    flaga = flaga + 1;
                }

                if (flaga != 0) {
                    lineEnd[z] = line[x + 1];
                    z++;
                }
                x++;
            }

            lineEnd[z] = '\0';
            if (i % 2 == 0) {

                int y = 0;
                int even = 0;

                while (lineEnd[y + 1] != '\0') {
                    char element[10];
                    int index = 0;
                    int flagaDwa = 0;

                    if (lineEnd[y] != '\n') {
                        while (lineEnd[y] != ' ') {
                            element[index] = lineEnd[y];
                            index++;
                            y++;
                            flagaDwa++;
                        }
                    }

                    if (flagaDwa > 0) {
                        element[index] = '\0';

                        if (even % 2 == 0) {
                            value = (int)element[0] - '0';
                            (*cardCount)++;
                            even++;
                        }
                        else {
                            colorName = element;
                            int compareIndex = 0;
                            int flague = 0;
                            while (element[compareIndex] != '\0') {
                                if (element[compareIndex] == green[compareIndex]) {
                                    compareIndex++;
                                }
                                else {
                                    break;
                                }
                                flague++;
                            }
                            if (flague == 5) {
                                (*greenCount)++;
                                *greenValue = value;
                                if (*greenValue != greenValueBufor) {
                                    greenValueBufor = *greenValue;
                                    (*greenValueCheck)++;
                                }
                            }
                            even++;
                        }
                    }
                    y++;
                }
            }
            if (i % 2 == 1) {

                int y = 0;
                int even = 0;

                while (lineEnd[y + 1] != '\0') {
                    char element[10];
                    int index = 0;
                    int flagaDwa = 0;

                    if (lineEnd[y] != '\n') {
                        while (lineEnd[y] != ' ') {
                            element[index] = lineEnd[y];
                            index++;
                            y++;
                            flagaDwa++;
                        }
                    }

                    const char* colorName = NULL;

                    if (flagaDwa > 0) {
                        element[index] = '\0';

                        if (even % 2 == 0) {
                            value = (int)element[0] - '0';
                            (*deckCardCount)++;
                            even++;
                        }
                        else {
                            colorName = element;
                            int compareIndex = 0;
                            int flague = 0;
                            while (element[compareIndex] != '\0') {
                                if (element[compareIndex] == green[compareIndex]) {
                                    compareIndex++;
                                }
                                else {
                                    break;
                                }
                                flague++;
                            }
                            if (flague == 5) {
                                (*greenCount)++;
                                *greenValue = value;
                                if (*greenValue != greenValueBufor) {
                                    greenValueBufor = *greenValue;
                                    (*greenValueCheck)++;
                                }
                            }
                            even++;
                        }
                    }
                    y++;
                }
            }
        }
        if (i >= 2 * playerNumber + 2) {
            int z = 0;
            int flaga = 0;
            char lineEnd[1000];
            while (line[x] != '\n') {
                if (line[x] == ':') {
                    flaga = flaga + 1;
                }

                if (flaga != 0) {
                    lineEnd[z] = line[x + 1];
                    z++;
                }
                x++;
            }

            lineEnd[z] = '\0';

            int y = 0;
            int even = 0;

            while (lineEnd[y + 1] != '\0') {
                char element[10];
                int index = 0;
                int flagaDwa = 0;
                int valueLength = 0;

                if (lineEnd[y] != '\n') {
                    while (lineEnd[y] != ' ') {
                        element[index] = lineEnd[y];
                        index++;
                        y++;
                        flagaDwa++;
                        valueLength++;
                    }
                }

                const char* colorName = NULL;

                if (flagaDwa > 0) {
                   element[index] = '\0';

                    if (even % 2 == 0) {
                        if (valueLength < 2) {
                            value = (int)element[0] - '0';
                        }
                        else {
                            value = 10 * ((int)element[0] - '0') + (int)element[1] - '0';
                        }
                        (*pileCount)++;
                        even++;
                    }
                    else {
                        colorName = element;
                        int compareIndex = 0;
                        int flague = 0;
                        while (element[compareIndex] != '\0') {
                            if (element[compareIndex] == green[compareIndex]) {
                                compareIndex++;
                            }
                            else {
                                break;
                            }
                            flague++;
                        }
                        if (flague == 5) {
                            (*greenCount)++;
                            *greenValue = value;
                            if (*greenValue != greenValueBufor) {
                                greenValueBufor = *greenValue;
                                (*greenValueCheck)++;
                            }
                        }
                        even++;
                    }
                }
                y++;
            }
            
        }
        char line[1000];
        i++;
    }
    fclose(loadFile);
}


void wczytajStan(int n, int* tabelaWartosciLoaded, int cardCount, int* tabelaKolorowLoaded, list_t** players, const char** colorsWithGreen, list_t** deckCards, int* tabelaWartosciDeck, int* tabelaKolorowDeck) {

    FILE* loadFile;
    loadFile = fopen("save.txt", "r");

    const char* colorName = NULL;
    int value = 0;
    int indexWartosci = 0;
    int indexWartosciDeck = 0;
    int indexKolorow = 0;
    int indexKolorowDeck = 0;
    const char* green = "green";
    const char* blue = "blue";
    const char* red = "red";
    const char* violet = "violet";
    const char* yellow = "yellow";
    const char* white = "white";
    const char* black = "black";
    int i = 0;

    for (i; i < 2 + 3 * n; i++) {
        char line[1000];
        fgets(line, 1000, loadFile);
        int x = 0;
        if (i > 1 and i < 2 * n + 2) {
            list_t* cur = players[i/2 - 1];
            list_t* curDeck = deckCards[i/2 - 1];
            int z = 0;
            int flaga = 0;
            char lineEnd[1000];
            while (line[x] != '\n') {
                if (line[x] == ':') {
                    flaga = flaga + 1;
                }

                if (flaga != 0) {
                    lineEnd[z] = line[x+1];
                    z++;
                }
                x++;
            }

            lineEnd[z] = '\0';
            if (i % 2 == 0) {

                int y = 0;
                int even = 0;

                while (lineEnd[y + 1] != '\0') {
                    char element[10];
                    int index = 0;
                    int flagaDwa = 0;
                    int valueLength = 0;

                    if (lineEnd[y] != '\n') {
                        while (lineEnd[y] != ' ') {
                            element[index] = lineEnd[y];
                            index++;
                            y++;
                            flagaDwa++;
                            valueLength++;
                        }
                    }

                    if (flagaDwa > 0) {
                        element[index] = '\0';

                        if (even % 2 == 0) {
                            if (valueLength < 2) {
                                value = (int)element[0] - '0';
                            }
                            else {
                                value = 10 * ((int)element[0] - '0') + (int)element[1] - '0';
                            }
                            tabelaWartosciLoaded[indexWartosci] = value;
                            indexWartosci++;
                            even++;
                        }
                        else {
                            if (element[0] != 'b') {
                                if (element[0] == 'g') {
                                    tabelaKolorowLoaded[indexKolorow] = GREEN;
                                    addElement(cur, value, colorsWithGreen[tabelaKolorowLoaded[indexKolorow] - 1]);
                                    cur = cur->next;
                                    indexKolorow++;
                                }
                                if (element[0] == 'r') {
                                    tabelaKolorowLoaded[indexKolorow] = RED;
                                    addElement(cur, value, colorsWithGreen[tabelaKolorowLoaded[indexKolorow] - 1]);
                                    cur = cur->next;
                                    indexKolorow++;
                                }
                                if (element[0] == 'y') {
                                    tabelaKolorowLoaded[indexKolorow] = YELLOW;
                                    addElement(cur, value, colorsWithGreen[tabelaKolorowLoaded[indexKolorow] - 1]);
                                    cur = cur->next;
                                    indexKolorow++;
                                }
                                if (element[0] == 'w') {
                                    tabelaKolorowLoaded[indexKolorow] = WHITE;
                                    addElement(cur, value, colorsWithGreen[tabelaKolorowLoaded[indexKolorow] - 1]);
                                    cur = cur->next;
                                    indexKolorow++;
                                }
                                if (element[0] == 'v') {
                                    tabelaKolorowLoaded[indexKolorow] = VIOLET;
                                    addElement(cur, value, colorsWithGreen[tabelaKolorowLoaded[indexKolorow] - 1]);
                                    cur = cur->next;
                                    indexKolorow++;
                                }
                            }
                            else if (element[2] == 'u') {
                                tabelaKolorowLoaded[indexKolorow] = BLUE;
                                addElement(cur, value, colorsWithGreen[tabelaKolorowLoaded[indexKolorow] - 1]);
                                cur = cur->next;
                                indexKolorow++;
                            }
                            else {
                                tabelaKolorowLoaded[indexKolorow] = BLACK;
                                addElement(cur, value, colorsWithGreen[tabelaKolorowLoaded[indexKolorow] - 1]);
                                cur = cur->next;
                                indexKolorow++;
                            }
                            even++;
                        }
                    }
                    y++;
                }
            }
            if (i % 2 == 1) {

                int y = 0;
                int even = 0;

                while (lineEnd[y + 1] != '\0') {
                    char element[10];
                    int index = 0;
                    int flagaDwa = 0;
                    int valueLength = 0;

                    if (lineEnd[y] != '\n') {
                        while (lineEnd[y] != ' ') {
                            element[index] = lineEnd[y];
                            index++;
                            y++;
                            flagaDwa++;
                            valueLength++;
                        }
                    }

                    if (flagaDwa > 0) {
                        element[index] = '\0';

                        if (even % 2 == 0) {
                            if (valueLength < 2) {
                                value = (int)element[0] - '0';
                            }
                            else {
                                value = 10 * ((int)element[0] - '0') + (int)element[1] - '0';
                            }
                            tabelaWartosciDeck[indexWartosciDeck] = value;
                            indexWartosciDeck++;
                            even++;
                        }
                        else {
                            if (element[0] != 'b') {
                                if (element[0] == 'g') {
                                    tabelaKolorowDeck[indexKolorowDeck] = GREEN;
                                    addElement(curDeck, value, colorsWithGreen[tabelaKolorowDeck[indexKolorowDeck] - 1]);
                                    curDeck = curDeck->next;
                                    indexKolorowDeck++;
                                }
                                if (element[0] == 'r') {
                                    tabelaKolorowDeck[indexKolorowDeck] = RED;
                                    addElement(curDeck, value, colorsWithGreen[tabelaKolorowDeck[indexKolorowDeck] - 1]);
                                    curDeck = curDeck->next;
                                    indexKolorowDeck++;
                                }
                                if (element[0] == 'y') {
                                    tabelaKolorowDeck[indexKolorowDeck] = YELLOW;
                                    addElement(curDeck, value, colorsWithGreen[tabelaKolorowDeck[indexKolorowDeck] - 1]);
                                    curDeck = curDeck->next;
                                    indexKolorowDeck++;
                                }
                                if (element[0] == 'w') {
                                    tabelaKolorowDeck[indexKolorowDeck] = WHITE;
                                    addElement(curDeck, value, colorsWithGreen[tabelaKolorowDeck[indexKolorowDeck] - 1]);
                                    curDeck = curDeck->next;
                                    indexKolorowDeck++;
                                }
                                if (element[0] == 'v') {
                                    tabelaKolorowDeck[indexKolorowDeck] = VIOLET;
                                    addElement(curDeck, value, colorsWithGreen[tabelaKolorowDeck[indexKolorowDeck] - 1]);
                                    curDeck = curDeck->next;
                                    indexKolorowDeck++;
                                }
                            }
                            else if (element[2] == 'u') {
                                tabelaKolorowDeck[indexKolorowDeck] = BLUE;
                                addElement(curDeck, value, colorsWithGreen[tabelaKolorowDeck[indexKolorowDeck] - 1]);
                                curDeck = curDeck->next;
                                indexKolorowDeck++;
                            }
                            else {
                                tabelaKolorowDeck[indexKolorowDeck] = BLACK;
                                addElement(curDeck, value, colorsWithGreen[tabelaKolorowDeck[indexKolorowDeck] - 1]);
                                curDeck = curDeck->next;
                                indexKolorowDeck++;
                            }
                            even++;
                        }
                    }
                    y++;
                }
            }
        }
        else if (i >= 2 * n + 2) {
            char line[1000];
            fgets(line, 1000, loadFile);
        }
    }
    fclose(loadFile);
}

void colorNumber(list_t** playerHand, int numOfPlayers, const char** colors, int sposob) {
    
    //int blueCount = 0;
    //int redCount = 0;
    //int yellowCount = 0;
    //int violetCount = 0;
    //int whiteCount = 0;
    //int blackCount = 0;

    int colorCount[6] = { 0, 0, 0, 0, 0, 0 }; //blue, red, violet, yellow, white, black

    for (int i = 0; i < numOfPlayers; i++) {
        if (sposob == 1) {
            playerHand[i] = playerHand[i]->next;

            while (playerHand[i] != NULL) {
                if (playerHand[i]->karta.kolor[0] != 'b') {

                    if (playerHand[i]->karta.kolor[0] == 'r') {
                        colorCount[1]++;
                    }
                    if (playerHand[i]->karta.kolor[0] == 'y') {
                        colorCount[3]++;
                    }
                    if (playerHand[i]->karta.kolor[0] == 'w') {
                        colorCount[4]++;
                    }
                    if (playerHand[i]->karta.kolor[0] == 'v') {
                        colorCount[2]++;
                    }
                }
                else if (playerHand[i]->karta.kolor[2] == 'u') {
                    colorCount[0]++;
                }
                else {
                    colorCount[5]++;
                }
                playerHand[i] = playerHand[i]->next;
            }
        }
        else {
            while (playerHand[i]->next != NULL) {
                if (playerHand[i]->karta.kolor[0] != 'b') {

                    if (playerHand[i]->karta.kolor[0] == 'r') {
                        colorCount[1]++;
                    }
                    if (playerHand[i]->karta.kolor[0] == 'y') {
                        colorCount[2]++;
                    }
                    if (playerHand[i]->karta.kolor[0] == 'w') {
                        colorCount[4]++;
                    }
                    if (playerHand[i]->karta.kolor[0] == 'v') {
                        colorCount[3]++;
                    }
                }
                else if (playerHand[i]->karta.kolor[2] == 'u') {
                    colorCount[0]++;
                }
                else {
                    colorCount[5]++;
                }
                playerHand[i] = playerHand[i]->next;
            }
        }
    }
    int ilosc;
    int flaga = 0;
    for (int i = 0; i < 6; i++) {
        if (colorCount[i] > 0) {
            ilosc = colorCount[i];
        }
    }

    for (int i = 0; i < 6; i++) {
        if (colorCount[i] > 0 and colorCount[i] != ilosc) {
            cout << "At least two colors with a different number of cards were found:" << endl;
            flaga++;
            break;
        }
    }

    if (flaga > 0) {
        for (int i = 0; i < 6; i++) {
            if (colorCount[i] > 0) {
                cout << colors[i] << " cards are " << colorCount[i] << endl;
            }
        }
    }
    else {
        cout << "The number cards of all colors is equal: " << ilosc << endl;
    }
}

int main(){

    int n; //l. graczy
    int k; //l. kotlow
    int g; //l. zielonych kart
    int gv; //wartosc zielonych kart
    int o; //l. kart innych kolorow
    card* tabelaKart = NULL;
    int* tabelaWartosci = NULL;
    int* tabelaWartosciLoaded = NULL;
    int* tabelaKolorowLoaded = NULL;
    int* tabelaPileLoaded = NULL;
    int* tabelaWartosciDeck = NULL;
    int* tabelaKolorowDeck = NULL;
    list_t** players = NULL;
    list_t** deckCards = NULL;

    FILE* fp;
    fp = fopen("save.txt", "a");


    const char* colors[KOLORY] = { "blue", "red", "violet", "yellow", "white", "black" };
    const char* colorsWithGreen[COLORSWITHGREEN] = { "green", "blue", "red", "violet", "yellow", "white", "black" };

    char option;
    
    cout << "l - wczytaj | g - wygeneruj" << endl;
    cin >> option;

    int sposob;
    switch (option) {
        case 'g':
            sposob = 0;
            FILE * fpclear;
            fpclear = fopen("save.txt", "w");
            fclose(fpclear);

            cin >> n >> k >> g >> gv >> o;
            sprawdzZmienne(n, k, g, gv, o);

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
            stworzTalie(k, g, gv, o, colors, tabelaKart, tabelaWartosci);

            players = (list_t**)malloc(n * sizeof(list_t*));
            for (int i = 0; i < n; i++) {
                players[i] = (list_t*)malloc(sizeof(list_t));
                init(players[i]);
            }

            dajDoRak(o, k, g, n, players, tabelaKart);
            wypiszStanDoPliku(n, k, o, tabelaKart, g, players, fp);

            break;

        case 'l':
            sposob = 1;
            int greenCount = 0;
            int cardCount = 0;
            int pileCount = 0;
            int deckCardCount = 0;
            int greenValue;
            int greenValueCheck = 0;

            wczytajStanWartosci(&cardCount, &greenCount, &greenValue, &pileCount, &greenValueCheck, &deckCardCount);

            tabelaWartosciLoaded = (int*)malloc(cardCount*sizeof(int));
            if (tabelaWartosciLoaded == NULL) {
                cout << "Allocation error" << endl;
                return 1;
            }

            tabelaKolorowLoaded = (int*)malloc(cardCount * sizeof(int));
            if (tabelaKolorowLoaded == NULL) {
                cout << "Allocation error" << endl;
                return 1;
            }

            tabelaKolorowDeck = (int*)malloc(deckCardCount * sizeof(int));
            if (tabelaKolorowDeck == NULL) {
                cout << "Allocation error" << endl;
                return 1;
            }

            tabelaWartosciDeck = (int*)malloc(deckCardCount * sizeof(int));
            if (tabelaWartosciDeck == NULL) {
                cout << "Allocation error" << endl;
                return 1;
            }

            n = LiczbaGraczy();

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


            wczytajStan(n, tabelaWartosciLoaded, cardCount, tabelaKolorowLoaded, players, colorsWithGreen, deckCards, tabelaWartosciDeck, tabelaKolorowDeck);
    
            if (greenCount == 0) {
                cout << "Green cards does not exist" << endl;
            }
            else {
                cout << "Green number: " << greenCount << endl;
                if (greenValueCheck > 1) {
                    cout << "Different green cards values occurred" << endl;
                }
                else {
                    cout << "Green value: " << greenValue << endl;
                }
            }

            cout << "Deck card number: " << deckCardCount << endl;
            cout << "Card number: " << cardCount << endl;
            cout << "Pile number: " << pileCount << endl;

            cout << "hands: " << endl;
            for (int i = 0; i < n; i++) {
                print(players[i]);
                cout << endl;
            }
            cout << endl;
            cout << "decks: " << endl;
            for (int i = 0; i < n; i++) {
                print(deckCards[i]);
                cout << endl;
            }

            break;
    }

    colorNumber(players, n, colors, sposob);

    free(tabelaKart);
    free(tabelaWartosci);
    free(tabelaWartosciLoaded);
    free(tabelaKolorowDeck);
    free(tabelaWartosciDeck);
    free(tabelaKolorowLoaded);

    fclose(fp);
    return 0;
}