#include <raylib.h>
#include <vector>
#include <string>
#include <map>

using namespace std;

static const int SCREEN_WIDTH = 400;
static const int SCREEN_HEIGHT = 400;
static const int BOARD_SIZE = 4;
static const int GAP = 4;

static map<int, Color> COLOR_MAP = {
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
static vector<unsigned int> board = {};

static void InitGame(void);
static void UpdateGame(void);
static void DrawGame(void);

int main()
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "2048");

	InitGame();

	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		UpdateGame();
		DrawGame();
	}

	CloseWindow();

	return 0;
}

void SpawnRandomTwo(void)
{
	vector<unsigned int> emptyCellsIndex = {};

	for (size_t i = 0; i < board.size(); i++)
	{
		if (board[i] == 0)
		{
			emptyCellsIndex.push_back(i);
		}
	}

	int randomIndex = GetRandomValue(0, emptyCellsIndex.size() - 1);

	board[randomIndex] = 2;
}

void InitBoard(void)
{
	for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
	{
		board.push_back(0);
	}

	SpawnRandomTwo();
}

void InitGame(void)
{
	InitBoard();
}

void MoveDown(void)
{
	for (int i = board.size() - 1; i >= 0; i--)
	{
		int row = (i + 1) / BOARD_SIZE;
		int col = (i + 1) % BOARD_SIZE;

		if (row > 0 && board[i] == 0)
		{
			for (int upperRow = row - 1; upperRow >= 0; upperRow -= 1)
			{
				if (board[upperRow * col - 1] != 0)
				{
					board[i] = 2;
					board[upperRow * col - 1] = 0;
					break;
				}
			}
		}
	}
}

void UpdateGame(void)
{
	if (IsKeyPressed(KEY_DOWN))
	{
		MoveDown();
	}
}

void DrawGame(void)
{
	BeginDrawing();

	ClearBackground(BLACK);

	for (int i = 0; i < board.size(); i++)
	{
		int row = i / BOARD_SIZE;
		int col = i % BOARD_SIZE;

		string text = to_string(board[i]);
		Vector2 textDimension = MeasureTextEx(GetFontDefault(), text.c_str(), GetFontDefault().baseSize * 4, 0);

		int cellSize = SCREEN_WIDTH / BOARD_SIZE;

		float textX = row * cellSize + (cellSize - textDimension.x) / 2 + GAP / 2;
		float textY = col * cellSize + (cellSize - textDimension.y) / 2 + GAP / 2;

		DrawRectangle(row * cellSize + GAP / 2, col * cellSize + GAP / 2, cellSize - GAP / 2, cellSize - GAP / 2, WHITE);

		Color textColor = COLOR_MAP.count(board[i]) ? COLOR_MAP[board[i]] : BLACK;

		DrawText(text.c_str(), textX, textY, GetFontDefault().baseSize * 4, textColor);
	}

	EndDrawing();
}
