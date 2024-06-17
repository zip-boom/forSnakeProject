// snakeFisrt.cpp
#include <ncurses.h>
#include <vector>

// snakeSecond.cpp
#include <deque>
#include <thread>
#include <chrono>
#include <stdlib.h>


// 요소값 설정
#define WALL 1
#define IMMUNEWALL 2
#define HEAD 3
#define TAIL 4
#define GROWTHITEM 5
#define POISONITEM 6
#define GATE 7


// 방향 설정 (위, 아래, 왼쪽, 오른쪽)
enum Direction { UP, DOWN, LEFT, RIGHT };


// Snake의 위치 구조체
struct  Position {

    int y, x;
};


// Map 초기 설정 : Wall, Immune Wall 할당
void initMap(std::vector<std::vector<int>>& map) {

    int rows = map.size();
    int cols = map[0].size();

    for (int i = 0; i < rows; ++i) {

        for (int j = 0; j < cols; ++j) {

            if (i == 0 || i == rows - 1 || j == 0 || j == cols - 1) {

                map[i][j] = (i == 0 && j == 0) || 
                            (i == 0 && j == cols - 1) || 
                            (i == rows - 1 && j == 0) || 
                            (i == rows - 1 && j == cols - 1) ? IMMUNEWALL : WALL;
            
            } else {

                map[i][j] = 0;
            }
        }
    }
}


// Map 표시 기호 할당
// Wall : #
// Immune Wall : X
// Head : O
// Tail : o
// Growth Item : +
// Poison Ttem : -
// Gate : G
void dispMap(const std::vector<std::vector<int>>& map) {

    clear();

    for (size_t i = 0; i < map.size(); ++i) {

        for (size_t j = 0; j < map[i].size(); ++j) {

            switch (map[i][j]) {

                case WALL:

                    mvprintw(i, j, "#");
                    break;

                case IMMUNEWALL:

                    mvprintw(i, j, "X");
                    break;

                case HEAD:

                    mvprintw(i, j, "O");
                    break;

                case TAIL:

                    mvprintw(i, j, "o");
                    break;

                case GROWTHITEM:

                    mvprintw(i, j, "+");
                    break;

                case POISONITEM:

                    mvprintw(i, j, "-");
                    break;

                case GATE:

                    mvprintw(i, j, "G");
                    break;

                default:
                
                    mvprintw(i, j, " ");
                    break;
            }
        }
    }

    refresh();
}


// Snake의 초기값 설정
void initSnake(std::deque<Position>& snake, std::vector<std::vector<int>>& map) {
    
    Position start = {10, 10};
    snake.push_back(start);
    map[start.y][start.x] = HEAD;
}


// 아이템 출현 방법
void placeItem(std::vector<std::vector<int>>& map, int itemType) {

    int rows = map.size();
    int cols = map[0].size();
    int y, x;

    do {

        y = rand() % rows;
        x = rand() % cols;

    } while (map[y][x] != 0);
    
    map[y][x] = itemType;
}


// Gate 출현 방법
void placeGate(std::vector<std::vector<int>>& map, Position& gate1, Position& gate2) {
    
    int rows = map.size();
    int cols = map[0].size();
    int y1, x1, y2, x2;

    do {

        y1 = rand() % rows;
        x1 = rand() % cols;

    } while (map[y1][x1] != WALL);

    do {

        y2 = rand() % rows;
        x2 = rand() % cols;

    } while ((map[y2][x2] != WALL) || (y1 == y2 && x1 == x2));

    gate1 = {y1, x1};
    gate2 = {y2, x2};
    map[gate1.y][gate1.x] = GATE;
    map[gate2.y][gate2.x] = GATE;
}


// Snake의 이동 방법
// snakeThird.cpp : 아이템 획득 시 처리 과정 추가
// snakeFourth.cpp : Gate 통과 시 처리 과정 추가 
void moveSnake(std::deque<Position>& snake, Direction& dir, std::vector<std::vector<int>>& map, const Position& gate1, const Position& gate2, int rows, int cols) {
    
    Position newHead = snake.front();

    switch (dir) {

        case UP:

            newHead.y -= 1;
            break;

        case DOWN:

            newHead.y += 1;
            break;

        case LEFT:

            newHead.x -= 1;
            break;

        case RIGHT:

            newHead.x += 1;
            break;
    }

    // 벽이나 몸통에 충돌 시 게임오버
    if (map[newHead.y][newHead.x] == WALL || map[newHead.y][newHead.x] == IMMUNEWALL || map[newHead.y][newHead.x] == TAIL) {
    
        endwin();
        printf("Game Over!\n");

        exit(0);
    }

    // Gate 통과 시 
    if (map[newHead.y][newHead.x] == GATE) {

        Position exitGate = (newHead.y == gate1.y && newHead.x == gate1.x) ? gate2 : gate1;

        switch (dir) {

            case UP:

                newHead.y = exitGate.y + 1;
                newHead.x = exitGate.x;
                break;

            case DOWN:

                newHead.y = exitGate.y - 1;
                newHead.x = exitGate.x;
                break;

            case LEFT:

                newHead.x = exitGate.x + 1;
                newHead.y = exitGate.y;
                break;

            case RIGHT:

                newHead.x = exitGate.x - 1;
                newHead.y = exitGate.y;
                break;
        }

        if (exitGate.y == 0) {

            newHead.y = exitGate.y + 1;
            dir = DOWN;

        } else if (exitGate.y == rows - 1) {

            newHead.y = exitGate.y - 1;
            dir = UP;

        } else if (exitGate.x == 0) {

            newHead.x = exitGate.x + 1;
            dir = RIGHT;

        } else if (exitGate.x == cols - 1) {

            newHead.x = exitGate.x - 1;
            dir = LEFT;

        } else {
            
            switch (dir) {
                
                case UP:

                    newHead.y = exitGate.y + 1;
                    break;

                case DOWN:

                    newHead.y = exitGate.y - 1;
                    break;

                case LEFT:

                    newHead.x = exitGate.x + 1;
                    break;

                case RIGHT:

                    newHead.x = exitGate.x - 1;
                    break;
            }
        }
    }

    // 아이템 획득 시
    if (map[newHead.y][newHead.x] == GROWTHITEM) {

        // Growth Item : 머리 추가
        snake.push_front(newHead);

        // 먹은 아이템 재배치
        placeItem(map, GROWTHITEM);

    } else if (map[newHead.y][newHead.x] == POISONITEM) {

        // Poison Item : 꼬리 제거
        if (snake.size() > 1) {

            snake.push_front(newHead);
            Position tail = snake.back();
            snake.pop_back();
            map[tail.y][tail.x] = 0;
            tail = snake.back();
            snake.pop_back();
            map[tail.y][tail.x] = 0;

        } else {

            endwin();
            printf("Game Over!\n");

            exit(0);
        }

        // 먹은 아이템 재배치
        placeItem(map, POISONITEM);

    } else {

        Position tail = snake.back();
        snake.pop_back();
        map[tail.y][tail.x] = 0;
        snake.push_front(newHead);
    }

    map[newHead.y][newHead.x] = HEAD;

    // 몸통 갱신
    for (size_t i = 1; i < snake.size(); ++i) {

        map[snake[i].y][snake[i].x] = TAIL;
    }
}


int main() {

    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    srand(time(0));

    int rows = 21;
    int cols = 21;
    std::vector<std::vector<int>> map(rows, std::vector<int>(cols));
    std::deque<Position> snake;
    Direction dir = RIGHT;
    Position gate1, gate2;

    initMap(map);
    initSnake(snake, map);
    placeItem(map, GROWTHITEM);
    placeItem(map, POISONITEM);
    placeGate(map, gate1, gate2);
    dispMap(map);

    while (true) {

        int ch = getch();

        switch (ch) {

            case 'i':

                if (dir != DOWN) dir = UP;
                break;

            case 'k':

                if (dir != UP) dir = DOWN;
                break;

            case 'j':

                if (dir != RIGHT) dir = LEFT;
                break;

            case 'l':

                if (dir != LEFT) dir = RIGHT;
                break;
        }

        moveSnake(snake, dir, map, gate1, gate2, rows, cols);
        dispMap(map);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    endwin();
    
    return 0;
}
