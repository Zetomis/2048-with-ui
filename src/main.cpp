#include <raylib.h>
#include <vector>
#include <string>
#include <map>

using namespace std;

const int BOARD_SIZE = 4;
const int CELL_SIZE = 100;
const int CELL_GAP = 4;

map<int, Color> COLOR_MAP = {
    {2, DARKGRAY},
    {4, MAROON},
    {8, ORANGE},
    {16, DARKGREEN},
    {32, DARKBLUE},
    {64, DARKPURPLE},
    {128, DARKBROWN},
    {256, GRAY},
    {512, RED},
    {1024, GOLD},
    {2048, LIME}};

vector<vector<int>> board(BOARD_SIZE, vector<int>(BOARD_SIZE));

void SpawnRandomTwo(void)
{
    vector<int *> emptyCells = {};

    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            if (board[row][col] == 0)
            {
                emptyCells.push_back(&board[row][col]);
            }
        }
    }

    int randomIndex = GetRandomValue(0, emptyCells.size() - 1);
    *emptyCells[randomIndex] = 2;
}

void InitBoard(void)
{
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            board[row][col] = 0;
        }
    }

    SpawnRandomTwo();
}

void DrawGame(void)
{
    BeginDrawing();

    ClearBackground(BLACK);

    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            string text = to_string(board[row][col]);
            Vector2 textDimension = MeasureTextEx(GetFontDefault(), text.c_str(), GetFontDefault().baseSize * 4, 0);

            float textX = col * CELL_SIZE + (CELL_SIZE - textDimension.x) / 2 + CELL_GAP / 2;
            float textY = row * CELL_SIZE + (CELL_SIZE - textDimension.y) / 2 + CELL_GAP / 2;

            Color backgroundColor = board[row][col] == 0 ? WHITE : COLOR_MAP[board[row][col]];
            Color textColor = board[row][col] == 0 ? BLACK : WHITE;

            DrawRectangle(col * CELL_SIZE + CELL_GAP / 2, row * CELL_SIZE + CELL_GAP / 2, CELL_SIZE - CELL_GAP / 2, CELL_SIZE - CELL_GAP / 2, backgroundColor);

            DrawText(text.c_str(), textX, textY, GetFontDefault().baseSize * 4, textColor);
        }
    }

    EndDrawing();
}

void ReverseHorizontally()
{
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE / 2; col++)
        {
            int temp = board[row][col];
            board[row][col] = board[row][BOARD_SIZE - col - 1];
            board[row][BOARD_SIZE - col - 1] = temp;
        }
    }
}

void ReverseVertically()
{
    for (int col = 0; col < BOARD_SIZE; col++)
    {
        for (int row = 0; row < BOARD_SIZE / 2; row++)
        {
            int temp = board[row][col];
            board[row][col] = board[BOARD_SIZE - row - 1][col];
            board[BOARD_SIZE - row - 1][col] = temp;
        }
    }
}

void MoveDown(void)
{
    for (int col = 0; col < BOARD_SIZE; col++)
    {
        for (int row = BOARD_SIZE - 1; row > 0; row--)
        {
            if (board[row][col] == 0)
            {
                for (int aboveRow = row - 1; aboveRow >= 0; aboveRow--)
                {
                    if (board[aboveRow][col] != 0)
                    {
                        board[row][col] = board[aboveRow][col];
                        board[aboveRow][col] = 0;
                        break;
                    }
                }
            }
            if (board[row][col] != 0)
            {
                for (int aboveRow = row - 1; aboveRow >= 0; aboveRow--)
                {
                    if (board[aboveRow][col] != 0 && board[aboveRow][col] != board[row][col])
                        break;

                    if (board[aboveRow][col] == board[row][col])
                    {
                        board[row][col] *= 2;

                        if (board[row][col] == 2048)
                            printf("YOU WON");

                        board[aboveRow][col] = 0;
                        break;
                    }
                }
            }
        }
    }

    SpawnRandomTwo();
}

void MoveRight(void)
{
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = BOARD_SIZE - 1; col > 0; col--)
        {
            if (board[row][col] == 0)
            {
                for (int leftCol = col - 1; leftCol >= 0; leftCol--)
                {
                    if (board[row][leftCol] != 0)
                    {
                        board[row][col] = board[row][leftCol];
                        board[row][leftCol] = 0;
                        break;
                    }
                }
            }

            if (board[row][col] != 0)
            {
                for (int leftCol = col - 1; leftCol >= 0; leftCol--)
                {
                    if (board[row][leftCol] != 0 && board[row][leftCol] != board[row][col])
                        break;

                    if (board[row][leftCol] == board[row][col])
                    {
                        board[row][col] *= 2;

                        if (board[row][col] == 2048)
                            printf("YOU WON");

                        board[row][leftCol] = 0;
                        break;
                    }
                }
            }
        }
    }

    SpawnRandomTwo();
}

void MoveUp(void)
{
    ReverseVertically();
    MoveDown();
    ReverseVertically();
}

void MoveLeft(void)
{
    ReverseHorizontally();
    MoveRight();
    ReverseHorizontally();
}

void UpdateGame(void)
{
    if (IsKeyPressed(KEY_DOWN))
    {
        MoveDown();
    }
    else if (IsKeyPressed(KEY_RIGHT))
    {
        MoveRight();
    }
    else if (IsKeyPressed(KEY_UP))
    {
        MoveUp();
    }
    else if (IsKeyPressed(KEY_LEFT))
    {
        MoveLeft();
    }
}

int main()
{
    InitWindow(BOARD_SIZE * CELL_SIZE, BOARD_SIZE * CELL_SIZE, "2048");

    InitBoard();

    while (!WindowShouldClose())
    {
        UpdateGame();
        DrawGame();
    }

    CloseWindow();

    return 0;
}