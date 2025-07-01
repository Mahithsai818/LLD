#include <iostream>
#include <vector>
#include <map>
#include <deque>
#include <cstdlib>
#include <ctime>
using namespace std;

class IObserver { public: virtual void update(string msg) = 0; virtual ~IObserver() {} };
class SnakeAndLadderConsoleNotifier : public IObserver {
public: void update(string msg) override { cout << "[NOTIFY] " << msg << endl; }
};

class Dice {
    int faces;
public:
    Dice(int f) : faces(f) { srand(time(0)); }
    int roll() { return rand() % faces + 1; }
};

class BoardEntity {
protected: int start, end;
public:
    BoardEntity(int s, int e) : start(s), end(e) {}
    int getStart() const { return start; }
    int getEnd() const { return end; }
    virtual void display() = 0;
    virtual string name() = 0;
    virtual ~BoardEntity() {}
};
class Ladder : public BoardEntity {
public:
    Ladder(int s, int e) : BoardEntity(s, e) { if (e <= s) cout << "Invalid Ladder!\n"; }
    void display() override { cout << "Ladder: " << start << " -> " << end << endl; }
    string name() override { return "LADDER"; }
};
class Snake : public BoardEntity {
public:
    Snake(int s, int e) : BoardEntity(s, e) { if (e >= s) cout << "Invalid Snake!\n"; }
    void display() override { cout << "Snake: " << start << " -> " << end << endl; }
    string name() override { return "SNAKE"; }
};

class Board {
    int size;
    vector<BoardEntity*> entities;
    map<int, BoardEntity*> lookup;
public:
    Board(int s) : size(s * s) {}
    bool canAdd(int pos) { return lookup.find(pos) == lookup.end(); }
    void addEntity(BoardEntity* e) {
        if (canAdd(e->getStart())) {
            entities.push_back(e);
            lookup[e->getStart()] = e;
        }
    }
    BoardEntity* getEntity(int pos) { return lookup.count(pos) ? lookup[pos] : nullptr; }
    int getSize() const { return size; }
    void display() {
        cout << "\n=== Board ===\nSize: " << size << "\n";
        for (size_t i = 0; i < entities.size(); i++) entities[i]->display();
    }
    ~Board() { for (size_t i = 0; i < entities.size(); i++) delete entities[i]; }
};

class BoardSetupStrategy {
public: virtual void setup(Board* b) = 0; virtual ~BoardSetupStrategy() {}
};

class StandardSetup : public BoardSetupStrategy {
public:
    void setup(Board* b) override {
        if (b->getSize() != 100) return;
        vector<pair<int, int>> snakes = {{99, 54}, {95, 75}, {92, 88}, {89, 68}, {74, 53}, {64, 60}, {62, 19}, {49, 11}, {46, 25}, {16, 6}};
        vector<pair<int, int>> ladders = {{2, 38}, {7, 14}, {8, 31}, {15, 26}, {21, 42}, {28, 84}, {36, 44}, {51, 67}, {71, 91}, {78, 98}, {87, 94}};
        for (size_t i = 0; i < snakes.size(); i++) b->addEntity(new Snake(snakes[i].first, snakes[i].second));
        for (size_t i = 0; i < ladders.size(); i++) b->addEntity(new Ladder(ladders[i].first, ladders[i].second));
    }
};

class RandomSetup : public BoardSetupStrategy {
    double snakeProb;
public:
    RandomSetup(double prob) : snakeProb(prob) {}
    void setup(Board* b) override {
        int sz = b->getSize(), entities = sz / 10;
        for (int i = 0; i < entities; i++) {
            bool isSnake = ((double)rand() / RAND_MAX) < snakeProb;
            for (int t = 0; t < 50; t++) {
                int start = rand() % (sz - 10) + 10;
                int end = isSnake ? rand() % (start - 1) + 1 : rand() % (sz - start) + start + 1;
                if (b->canAdd(start) && end < sz) {
                    if (isSnake)
                        b->addEntity(new Snake(start, end));
                    else
                        b->addEntity(new Ladder(start, end));
                    break;
                }
            }
        }
    }
};

class CustomSetup : public BoardSetupStrategy {
    vector<pair<int, int>> snakes, ladders;
public:
    void addSnake(int s, int e) { snakes.push_back(make_pair(s, e)); }
    void addLadder(int s, int e) { ladders.push_back(make_pair(s, e)); }
    void setup(Board* b) override {
        for (size_t i = 0; i < snakes.size(); i++) b->addEntity(new Snake(snakes[i].first, snakes[i].second));
        for (size_t i = 0; i < ladders.size(); i++) b->addEntity(new Ladder(ladders[i].first, ladders[i].second));
    }
};

class Player {
    int pos = 0, score = 0;
    string name;
public:
    Player(string n) : name(n) {}
    string getName() const { return name; }
    int getPos() const { return pos; }
    void setPos(int p) { pos = p; }
    void win() { score++; }
};

class Rules {
public:
    bool valid(int pos, int dice, int max) { return pos + dice <= max; }
    int move(int pos, int dice, Board* b) {
        int mid = pos + dice;
        BoardEntity* e = b->getEntity(mid);
        return e ? e->getEnd() : mid;
    }
    bool won(int pos, int max) { return pos == max; }
};

