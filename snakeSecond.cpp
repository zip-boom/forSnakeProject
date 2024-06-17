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


// Snake의 이동 방법
void moveSnake(std::deque<Position>& snake, Direction dir, std::vector<std::vector<int>>& map) {
    
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

    // 머리 추가
    snake.push_front(newHead);
    map[newHead.y][newHead.x] = HEAD;

    // 꼬리 제거
    Position tail = snake.back();
    snake.pop_back();
    map[tail.y][tail.x] = 0;

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

    int rows = 21;
    int cols = 21;
    std::vector<std::vector<int>> map(rows, std::vector<int>(cols));
    std::deque<Position> snake;
    Direction dir = RIGHT;

    initMap(map);
    initSnake(snake, map);
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

        moveSnake(snake, dir, map);
        dispMap(map);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    endwin();
    
    return 0;
}
