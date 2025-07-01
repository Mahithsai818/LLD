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


