// snakeFisrt.cpp
#include <ncurses.h>
#include <vector>

// snakeSecond.cpp
#include <deque>
#include <thread>
#include <chrono>
#include <stdlib.h>

// snakeFifth.cpp
#include <stdio.h>

// snakeFinal.cpp
#include <fstream>
#include <iostream>


// 요소값 설정
#define WALL 1
#define IMMUNEWALL 2
#define HEAD 3
#define TAIL 4
#define GROWTHITEM 5
#define POISONITEM 6
#define GATE 7
#define SLOWITEM 8


// 방향 설정 (위, 아래, 왼쪽, 오른쪽)
enum Direction { UP, DOWN, LEFT, RIGHT };


// Snake의 위치 구조체
struct  Position {

    int y, x;
};


// 게임 상태 구조체
struct GameState {

    int currentLength;
    int maxLength;
    int growthItemCount;
    int poisonItemCount;
    int gateUsageCount;
    int timeElapsed;
    int stage;
};


// 미션 구조체
struct Mission {
    int requiredLength;
    int requiredGrowthItems;
    int requiredPoisonItems;
    int requiredGateUsages;
};


// // Map 정의 및 Map Data 불러오기
// std::vector<std::vector<int>> stageMaps[4] = {
//     {   //  Stage 4
//         {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
//         {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2}
//     },
//     {   //  Stage 4
//         {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
//         {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2}
//     },
//     {   //  Stage 4
//         {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
//         {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2}
//     },
//     {   //  Stage 4
//         {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
//         {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2}
//     }
// };


// Map Data 파일 import
void loadMapFromFile(const std::string& filename, std::vector<std::vector<int>>& map) {
    
    std::ifstream file(filename);

    if (!file) {

        std::cerr << "Failed to open file: " << filename << std::endl;
        exit(1);
    }

    int value;
    map.clear();

    while (file >> value) {

        std::vector<int> row;
        row.push_back(value);
        for (int i = 1; i < 21; ++i) {

            file >> value;
            row.push_back(value);
        }

        map.push_back(row);
    }

    file.close();
}


// Mission 정의
Mission stageMissions[4] = {
    {1, 5, 2, 1},
    {1, 6, 3, 2},
    {1, 7, 4, 3},
    {1, 8, 5, 4}
};


// 점수와 미션을 화면에 표시
void dispScoreAndMission(const GameState& state, const Mission& mission, int col, int tickSpeed) {

    // 점수 표시 부분
    mvprintw(1, col + 2, "Score Board");
    mvprintw(2, col + 2, "B: %d", state.currentLength / state.maxLength);
    mvprintw(3, col + 2, "+: %d", state.growthItemCount);
    mvprintw(4, col + 2, "-: %d", state.poisonItemCount);
    mvprintw(5, col + 2, "G: %d", state.gateUsageCount);
    mvprintw(6, col + 2, "T: %d", state.timeElapsed);
    mvprintw(7, col + 2, "Tick: %d ms", tickSpeed);

    // 미션 표시 부분
    mvprintw(9, col + 2, "Mission");
    mvprintw(10, col + 2, "B: %d", mission.requiredLength);
    mvprintw(11, col + 2, "+: %d", mission.requiredGrowthItems);
    mvprintw(12, col + 2, "-: %d", mission.requiredPoisonItems);
    mvprintw(13, col + 2, "G: %d", mission.requiredGateUsages);
}


// Map 초기 설정 : Wall, Immune Wall 할당
void initMap(std::vector<std::vector<int>>& map, const std::vector<std::vector<int>>& stageMap) {

    map = stageMap;

    // Map 요소들 초기화
    for (int i = 0; i < map.size(); ++i) {

        for (int j = 0; j < map[0].size(); ++j) {

            if (map[i][j] != WALL && map[i][j] != IMMUNEWALL) {

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

                case SLOWITEM:

                    mvprintw(i, j, "S");
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
// snakeFifth.cpp : 게임 상태 갱신 과정 추가
void moveSnake(std::deque<Position>& snake, Direction& dir, std::vector<std::vector<int>>& map, Position& gate1, Position& gate2, int rows, int cols, GameState& state, int& tickSpeed) {
    
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

        state.gateUsageCount++;

        Position exitGate = (newHead.y == gate1.y && newHead.x == gate1.x) ? gate2 : gate1;

        map[gate1.y][gate1.x] = WALL;
        map[gate2.y][gate2.x] = WALL;

        placeGate(map, gate1, gate2);

        std::vector<Direction> exitDirections = {dir};  // 1) 진입 방향과 일치하는 방향
        
        switch (dir) {

            case UP:

                exitDirections.push_back(RIGHT);  // 2) 시계 방향
                exitDirections.push_back(LEFT);   // 3) 반시계 방향
                exitDirections.push_back(DOWN);   // 4) 반대 방향
                break;

            case DOWN:

                exitDirections.push_back(LEFT);
                exitDirections.push_back(RIGHT);
                exitDirections.push_back(UP);
                break;

            case LEFT:

                exitDirections.push_back(UP);
                exitDirections.push_back(DOWN);
                exitDirections.push_back(RIGHT);
                break;

            case RIGHT:

                exitDirections.push_back(DOWN);
                exitDirections.push_back(UP);
                exitDirections.push_back(LEFT);
                break;
        }

        bool exitFound = false;
        for (Direction exitDir : exitDirections) {
            int y = exitGate.y;
            int x = exitGate.x;
            switch (exitDir) {
                case UP: y -= 1; break;
                case DOWN: y += 1; break;
                case LEFT: x -= 1; break;
                case RIGHT: x += 1; break;
            }

            if (map[y][x] != WALL) {
                newHead.y = y;
                newHead.x = x;
                dir = exitDir;
                exitFound = true;
                break;
            }
        }

        if (!exitFound) {  // 모든 방향이 막혀있는 경우 (이론적으로 발생하지 않아야 함)
            
            endwin();
            printf("Game Over!\n");
            exit(0);
        }
    }

    // 아이템 획득 시
    if (map[newHead.y][newHead.x] == GROWTHITEM) {

        state.growthItemCount++;

        // Growth Item : 머리 추가
        snake.push_front(newHead);
        state.currentLength++;

        if (state.currentLength > state.maxLength) {

            state.maxLength = state.currentLength;
        }

        // 먹은 아이템 재배치
        placeItem(map, GROWTHITEM);

    } else if (map[newHead.y][newHead.x] == POISONITEM) {

        state.poisonItemCount++;

        // Poison Item : 꼬리 제거
        if (snake.size() > 1) {

            snake.push_front(newHead);
            Position tail = snake.back();
            snake.pop_back();
            map[tail.y][tail.x] = 0;
            tail = snake.back();
            snake.pop_back();
            map[tail.y][tail.x] = 0;

            state.currentLength--;

        } else {

            endwin();
            printf("Game Over!\n");

            exit(0);
        }

        // 먹은 아이템 재배치
        placeItem(map, POISONITEM);

    } else if (map[newHead.y][newHead.x] == SLOWITEM) {

        tickSpeed += 100;       // SLOWITEM은 재배치되지 않음

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


// 미션 달성 여부 확인
bool checkMission(const GameState& state, const Mission& mission) {

    return state.currentLength >= mission.requiredLength &&

           state.growthItemCount >= mission.requiredGrowthItems &&
           state.poisonItemCount >= mission.requiredPoisonItems &&
           state.gateUsageCount >= mission.requiredGateUsages;
}


// 다음 스테이지로 진행
void nextStage(GameState& state, std::vector<std::vector<int>>& map, std::deque<Position>& snake, Direction& dir, Position& gate1, Position& gate2) {
    
    state.stage++;

    if (state.stage >= 4) {

        endwin();
        printf("You Win!\n");
        exit(0);
    }

    state.currentLength = 1;
    state.growthItemCount = 0;
    state.poisonItemCount = 0;
    state.gateUsageCount = 0;

    loadMapFromFile("stage" + std::to_string(state.stage + 1) + ".txt", map);
    initMap(map, map);
    snake.clear();
    dir = RIGHT;
    initSnake(snake, map);
    placeItem(map, GROWTHITEM);
    placeItem(map, POISONITEM);
    placeItem(map, SLOWITEM);
    placeGate(map, gate1, gate2);
}


void displayWinMessage(int cols) {

    mvprintw(8, cols + 2, "You Win!");
    refresh();
    std::this_thread::sleep_for(std::chrono::seconds(2));
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

    // 초기 상태 설정
    GameState state = {1, 1, 0, 0, 0, 0};
    Mission mission = stageMissions[state.stage];

    loadMapFromFile("stage1.txt", map);
    initMap(map, map);
    initSnake(snake, map);
    placeItem(map, GROWTHITEM);
    placeItem(map, POISONITEM);
    placeItem(map, SLOWITEM);
    placeGate(map, gate1, gate2);
    dispMap(map);

    auto start = std::chrono::steady_clock::now();

    int baseSpeed = 500;  // 초기 틱 속도 (밀리초)

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

        moveSnake(snake, dir, map, gate1, gate2, rows, cols, state, baseSpeed);

        clear();
        dispMap(map);
        dispScoreAndMission(state, mission, cols, baseSpeed);
        refresh();

        auto end = std::chrono::steady_clock::now();
        state.timeElapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

        if (checkMission(state, mission)) {

            displayWinMessage(cols);
            nextStage(state, map, snake, dir, gate1, gate2);
            mission = stageMissions[state.stage];
            loadMapFromFile("stage" + std::to_string(state.stage + 1) + ".txt", map);
            baseSpeed = std::max(200, baseSpeed - 100);

        }

        std::this_thread::sleep_for(std::chrono::milliseconds(baseSpeed));
    }

    endwin();
    
    return 0;
}
