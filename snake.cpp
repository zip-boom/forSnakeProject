// snake.cpp
#include <ncurses.h>
#include <vector>


// 요소값 설정
#define WALL 1
#define IMMUNEWALL 2
#define HEAD 3
#define TAIL 4
#define GROWTHITEM 5
#define POISONITEM 6
#define GATE 7


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

int main() {

    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    int rows = 21;
    int cols = 21;
    std::vector<std::vector<int>> map(rows, std::vector<int>(cols));

    initMap(map);
    dispMap(map);

    getch();
    endwin();
    
    return 0;
}
