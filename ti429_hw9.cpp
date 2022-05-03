#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>
using namespace std;

class Game;
class Critters;
class Ant;
class Doodlebug;

const int ANT_START = 100;
const int DOODLE_START = 5;
const int MAP_MAX = 20;

const int ANT_BREED = 3;
const int DOODLE_BREED = 8;
const int DOODLE_STARVE = 3;

const int UP = 1;
const int DOWN = 2;
const int LEFT = 3;
const int RIGHT = 4;

class Game {
    friend class Critters;
    friend class Ant;
    friend class Doodlebug;
    int gameIteration;
    int x;
    int y;
    Critters* map[MAP_MAX][MAP_MAX];
public:
    Game();
    ~Game() = default;
    void StartGame();
    void nextIteration();
    void PrintMap();
    int random_gen(const int& min, const int& max) const;

};

class Critters {
protected:
    Game* currPtr;
    int x;
    int y;
    int breedTime;
    int gameIteration;
public:
    Critters() : currPtr(nullptr), x(0), y(0), breedTime(0), gameIteration(0) {}
    Critters(Game* currPtr, int x, int y);
    virtual ~Critters() = default;
    virtual int getType() {
        return 0;
    };
    virtual void move();
    virtual void randmove(int newX, int newY);
    virtual void breed();
    bool validPoints(int x, int y);
    virtual void starve() {}
};

class Ant : public Critters {
public:
    Ant() : Critters() {}
    Ant(Game* currPtr, int x, int y) : Critters(currPtr, x, y) {}
    int getType() { return 2; }
    void breed();
};

class Doodlebug : public Critters {
    int starveTime;
public:
    Doodlebug() : Critters(), starveTime(0) {}
    Doodlebug(Game* currPtr, int x, int y);
    int getType() { return 1; }
    void move();
    void breed();
    void starve();
};

void Doodlebug::starve() {
    if (starveTime >= DOODLE_STARVE) {
        delete currPtr->map[x][y];
        currPtr->map[x][y] = nullptr;
    }
    else return;
}
void Critters::breed() {
    breedTime = 0;
}

bool Critters::validPoints(int x, int y) {
    if (x >= 0 && x < MAP_MAX && y >= 0 && y < MAP_MAX)
        return true;
    else
        return false;
}

int Game::random_gen(const int& min, const int& max) const {
    return rand() % (max - min + 1) + min;
}


void Critters::randmove(int newX, int newY) {
    srand(time(NULL));
    int position = rand() % 4;
    switch (position) {
    case 1:
        newY++;
        break;
    case 2:
        newY--;
        break;
    case 3:
        newX--;
        break;
    case 4:
        newY++;
    }

}

void Critters::move() {
    if (gameIteration == currPtr->gameIteration) return;
    breedTime++;
    int newX = x;
    int newY = y;
    randmove(newX, newY);
    if((currPtr->map[newX][newY] == nullptr) && (validPoints(newX,newY)== true)){
        currPtr->map[x][y] = nullptr;
        currPtr->map[newX][newY] = this;
        x = newX;
        y = newY;
    }
}

Critters::Critters(Game* currPtr, int x, int y) {
    this->currPtr = currPtr;
    this->x = x;
    this->y = y;
    breedTime = 0;
}


void Doodlebug::breed() {
    if (breedTime < DOODLE_BREED) return;
    else {
        if ((currPtr->map[x + 1][y] == nullptr) && (validPoints(x + 1, y) == true)) {
            currPtr->map[x + 1][y] = new Doodlebug(currPtr, x + 1, y);
            breedTime = 0;
        }
        else if ((currPtr->map[x - 1][y] == nullptr) && (validPoints(x - 1, y) == true)) {
            currPtr->map[x - 1][y] = new Doodlebug(currPtr, x - 1, y);
            breedTime = 0;
        }
        else if ((currPtr->map[x][y + 1] == nullptr) && (validPoints(x, y + 1) == true)) {
            currPtr->map[x][y + 1] = new Doodlebug(currPtr, x, y + 1);
            breedTime = 0;
        }
        else if ((currPtr->map[x][y - 1] == nullptr) && (validPoints(x, y - 1) == true)) {
            currPtr->map[x][y - 1] = new Doodlebug(currPtr, x, y - 1);
            breedTime = 0;
        }
        else
            return;

    }
}

void Ant::breed() {
    if (breedTime < ANT_BREED) return;
    else {
        if ((currPtr->map[x + 1][y] == nullptr) && (validPoints(x + 1, y) == true)) {
            currPtr->map[x + 1][y] = new Ant(currPtr, x + 1, y);
            breedTime = 0;
        }
        else if ((currPtr->map[x - 1][y] == nullptr) && (validPoints(x - 1, y) == true)) {
            currPtr->map[x - 1][y] = new Ant(currPtr, x - 1, y);
            breedTime = 0;
        }
        else if ((currPtr->map[x][y + 1] == nullptr) && (validPoints(x, y + 1) == true)) {
            currPtr->map[x][y + 1] = new Ant(currPtr, x, y + 1);
            breedTime = 0;
        }
        else if ((currPtr->map[x][y - 1] == nullptr) && (validPoints(x, y - 1) == true)) {
            currPtr->map[x][y - 1] = new Ant(currPtr, x, y - 1);
            breedTime = 0;
        }
        else
            return;

    }
}

Doodlebug::Doodlebug(Game* currPtr, int x, int y) : Critters(currPtr, x, y) {
    starveTime = 0;
    gameIteration = currPtr->gameIteration;
}


Game::Game() {
    gameIteration = 0;
    srand(time(NULL));
    for (int x = 0; x < MAP_MAX; x++)
        for (int y = 0; y < MAP_MAX; y++)
            map[x][y] = nullptr;

    int x, y;
    int doodleCount = 0;
    int antCount = 0;

    while (doodleCount < DOODLE_START) {
        x = random_gen(0, MAP_MAX - 1);
        y = random_gen(0, MAP_MAX - 1);
        if (map[x][y] == nullptr) {
            map[x][y] = new Doodlebug(this, x, y);
            doodleCount++;
        }
    }

    while (antCount < ANT_START) {
        x = random_gen(0, MAP_MAX - 1);
        y = random_gen(0, MAP_MAX - 1);
        if (map[x][y] == nullptr) {
            map[x][y] = new Ant(this, x, y);
            antCount++;
        }
    }
}


void Game::nextIteration() {
    gameIteration++;
    for (int x = 0; x < MAP_MAX; x++) {
        for (int y = 0; y < MAP_MAX; y++) {
            if (map[x][y] == nullptr) continue;
            if (map[x][y]->getType() == 1)
                map[x][y]->move();
        }
    }
    for (int x = 0; x < MAP_MAX; x++) {
        for (int y = 0; y < MAP_MAX; y++) {
            if (map[x][y] == nullptr) continue;
            if (map[x][y]->getType() == 2)
                map[x][y]->move();
        }
    }
    for (int x = 0; x < MAP_MAX; x++) {
        for (int y = 0; y < MAP_MAX; y++) {
            if (map[x][y] == nullptr) continue;
            if (map[x][y]->getType() == 1)
                map[x][y]->starve();
        }
    }

    for (int x = 0; x < MAP_MAX; x++) {
        for (int y = 0; y < MAP_MAX; y++) {
            if (map[x][y] == nullptr) continue;
            if (map[x][y]->getType() == 2)
                map[x][y]->breed();
        }
    }
    for (int x = 0; x < MAP_MAX; x++) {
        for (int y = 0; y < MAP_MAX; y++) {
            if (map[x][y] == nullptr) continue;
            if (map[x][y]->getType() == 1)
                map[x][y]->breed();
        }
    }

    PrintMap();
}

void Game::PrintMap() {
    for (int x = 0; x < MAP_MAX; x++) {
        for (int y = 0; y < MAP_MAX; y++) {
            if (map[x][y] == nullptr)
                cout << "- ";
            else if (map[x][y]->getType() == 1)
                cout << "X ";
            else
                cout << "o ";
        }
        cout << endl;
    }
}


void Doodlebug::move() {
    if (gameIteration == currPtr->gameIteration) return;
    int xRIGHT = x + 1;
    int xLEFT = x - 1;
    int yUP = y + 1;
    int yDOWN = y - 1;
    breedTime++;
    if (validPoints(xRIGHT, y) == true) {
        if (currPtr->map[xRIGHT][y] != nullptr) {
            if (currPtr->map[xRIGHT][y]->getType() == 2) {
                delete currPtr->map[xRIGHT][y];
                currPtr->map[xRIGHT][y] = this;
                currPtr->map[x][y] = nullptr;
                x = xRIGHT;
                starveTime = 0;
                return;
            }
        }
    }
    else if (validPoints(xLEFT, y) == true) {
        if (currPtr->map[xLEFT][y] != nullptr) {
            if (currPtr->map[xLEFT][y]->getType() == 2) {
                delete currPtr->map[xLEFT][y];
                currPtr->map[xLEFT][y] = this;
                currPtr->map[x][y] = nullptr;
                x = xLEFT;
                starveTime = 0;
                return;
            }
        }

    }
    else if (validPoints(x, yUP) == true) {
        if (currPtr->map[x][yUP] != nullptr) {
            if (currPtr->map[x][yUP]->getType() == 2) {
                delete currPtr->map[x][yUP];
                currPtr->map[x][yUP] = this;
                currPtr->map[x][y] = nullptr;
                y = yUP;
                starveTime = 0;
                return;
            }
        }
    }
    else if (validPoints(x, yDOWN) == true) {
        if (currPtr->map[x][yDOWN] != nullptr) {
            if (currPtr->map[x][yDOWN]->getType() == 2) {
                delete currPtr->map[x][yDOWN];
                currPtr->map[x][yDOWN] = this;               
                currPtr->map[x][y] = nullptr;
                y = yDOWN;
                starveTime = 0;
                return;
            }
        }
    }

    else {
        Critters::move();
        starveTime++;
    }
}

void Game::StartGame() {
    PrintMap();
    char input;
    cout << "Please press Enter to move to the next time step " << endl;
    cin.get(input);

    while (input == '\n')
    {
        nextIteration();
        cout << "Please press Enter to move to the next time step " << endl;
        cin.get(input);
    }
}

int main()
{
    Game one;
    one.StartGame();
}

