#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

int getNum(int r, int c, int len, int width, int grid[len][width]) {
    if (grid[r][c] != -1) return grid[r][c];
    int cnt = 0;
    for (int i = (r - 1 >= 0 ? r - 1 : 0); i <= (r + 1 < len ? r + 1 : len - 1); i++) {
        for (int j = (c - 1 >= 0 ? c - 1: 0); j <= (c + 1 < width ? c + 1 : width - 1); j++) {
            if (grid[i][j] == -2) cnt++;
        }
    }
    return cnt;
}

int sweepSquare(int r, int c, int len, int width, int grid[len][width], int * unclicked) {
    if (grid[r][c] == -1) {
        (*unclicked)--;
        grid[r][c] = getNum(r, c, len, width, grid);
    }
    if (grid[r][c] == -2) return 1; // BOOM!
    if (grid[r][c]) return 0;
    
    int queue[len * width], head = 0, tail = 0;
    queue[tail++] = r * width + c;
    while (head < tail) {
        int idx = queue[head++ % (len * width)];
        r = idx / width, c = idx % width;
        for (int i = (r - 1 >= 0 ? r - 1 : 0); i <= (r + 1 < len ? r + 1 : len - 1); i++) {
            for (int j = (c - 1 >= 0 ? c - 1: 0); j <= (c + 1 < width ? c + 1 : width - 1); j++) {
                if (grid[i][j] == -1) {
                    (*unclicked)--;
                    grid[i][j] = getNum(i, j, len, width, grid);
                    if (grid[i][j] == 0) queue[tail++ % (len * width)] = i * width + j;
                }
            }
        }
    }
    if (!(*unclicked)) return 2;
    return 0;
}

void printBoard(int len, int width, int grid[len][width], int endgame) {
    char c;
    for (int i = 1; i <= width; i++) printf("\t%i", i);
    printf("\n");
    for (int i = 0; i < len; i++) {
        printf("%i", i + 1);
        for (int j = 0; j < width; j++) {
            if (grid[i][j] == -2) c = endgame ? 'B' : '+';
            else if (grid[i][j] < 0) c = endgame ? ' ' : '+';
            else if (grid[i][j] == 0) c = ' ';
            else c = grid[i][j] + '0';
            printf("\t%c", c);
        }
        printf("\n\n");
    }
    if (endgame) printf("%s\n", endgame == 1 ? "BOOM! You lose. :(\n" : "Congratulations! You swept all the mines! :)\n");
}

int main() {
    int len = 1, width = 1, mines, x, y, unclicked, endgame;
    char prompt, c;
    while (1) {
        mines = 0, endgame = 0;
        do {
            if (len < 1 || len > 50 || width < 1 || width > 50) printf("Grid size cannot exceed 50 x 50 and cannot contain negative numbers\n");
            printf("Enter grid size: [length] [width] ");
            scanf("%i %i", &len, &width);
        } while (len < 1 || len > 50 || width < 1 || width > 50);
        do {
            if (mines < 0 || mines >= len * width) printf("Number of mines cannot be negative, and cannot exceed the number of tiles\n");
            printf("Enter number of mines: [mines] ");
            scanf("%i", &mines);
        } while (mines < 0 || mines >= len * width);
        unclicked = len * width - mines;
        int grid[len][width];
        memset(grid, -1, len * width * sizeof(int));
        srand(time(0));
        
        for (int i = 0; i < mines; i++) {
            int idx = rand() / (RAND_MAX / (len * width + 1) + 1);
            if (grid[idx / width][idx % width] == -2) i--;
            else grid[idx / width][idx % width] = -2;
        }
        
        while (unclicked) {
            printBoard(len, width, grid, 0);
            printf("Enter tile coordinates for minesweeping: [row] [column] ");
            scanf("%i %i", &x, &y);
            
            endgame = sweepSquare(x - 1, y - 1, len, width, grid, &unclicked);
            if (endgame) {
                printBoard(len, width, grid, endgame);
                break;
            }
        }
        
        do {
            printf("Play again? [y/n] ");
            scanf(" %c", &prompt);
        } while (!(prompt == 'y' || prompt == 'n'));
        if (prompt == 'n') break;
    }
    return 0;
}