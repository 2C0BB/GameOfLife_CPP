#include "raylib.h"
#include <iostream>

#define MAP_COLUMNS 20
#define MAP_ROWS 20

#define DELAY 0.2

class Cell {
	private:
		int state;
		int next_state;

	public:
		Cell() {
			state = 0;
		}

		int is_alive() {
			return state;
		}

		void set_next_state(int neighbours) {

			next_state = 0;

			if (neighbours == 3) {
				next_state = 1;
			}
			if (neighbours == 2 && state == 1) {
				next_state = 1;
			}
		}

		void become_next_state() {
			state = next_state;
		}

		void swap_state() {
			if (state) {
				state = 0;
			} else {
				state = 1;
			}
		}
};

class Map {
	private:
		int size;

	public:
		Cell cells[MAP_ROWS][MAP_COLUMNS];

		Map(int _size) {
			size = _size;			
		}
	
		void update_cells() {
			for (int r = 0; r < MAP_ROWS; r++) {
				for (int c = 0; c < MAP_COLUMNS; c++) {

				int neighbours = 0;

				int positions[8][2] = {{-1, -1}, {0, -1}, {1, -1}, 
										{-1, 0},          {1, 0},
										{-1, 1}, {0, 1}, {1, 1}};

				for (int i = 0; i < 8; i++) {
					neighbours += get_cell_state(r + positions[i][0], c + positions[i][1]);
				}	

				cells[r][c].set_next_state(neighbours);

				}
			}

			for (int r = 0; r < MAP_ROWS; r++) {
				for (int c = 0; c < MAP_COLUMNS; c++) {
					cells[r][c].become_next_state();
				}
			}
		}

		int get_cell_state(int row, int column) {
			if (row < 0 || row >= MAP_ROWS || column < 0 || column >= MAP_COLUMNS) {
				return 0;
			}

			return cells[row][column].is_alive();
		}

		void draw_cells() {
			for (int r = 0; r < MAP_ROWS; r++) {
				for (int c = 0; c < MAP_COLUMNS; c++) {
					if (cells[r][c].is_alive()){
						DrawRectangle(c*size, r*size, size, size, BLACK);
					}
				}
			}
		}

		void swap_cell(int row, int column) {
			cells[row][column].swap_state();
		}

	
};




typedef struct Timer {
    double startTime;   // Start time (seconds)
    double lifeTime;    // Lifetime (seconds)
} Timer;

void StartTimer(Timer *timer, double lifetime)
{
    timer->startTime = GetTime();
    timer->lifeTime = lifetime;
}

bool TimerDone(Timer timer)
{
    return GetTime() - timer.startTime >= timer.lifeTime;
}

double GetElapsed(Timer timer)
{
    return GetTime() - timer.startTime;
}

void draw_cursor(int r, int c, int size) {
	DrawRectangleLinesEx({(float)c*size, (float)r*size, (float)size, (float)size}, 5.0, BLUE);
}

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 800;

	int cell_size = screenWidth/MAP_COLUMNS;
	Map map1(cell_size);


	map1.swap_cell(0, 2);
	map1.swap_cell(1, 0);
	map1.swap_cell(1, 2);
	map1.swap_cell(2, 1);
	map1.swap_cell(2, 2);

	int state = 0; 
	// running = 0; paused = 1; editor = 2;

	int cursor_r = 0;
	int cursor_c = 0;

	Timer frame_timer;
	StartTimer(&frame_timer, DELAY);

    InitWindow(screenWidth, screenHeight, "Game of Life C++");
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {

		if (IsKeyPressed(KEY_SPACE)) {
			if (state == 0) {
				state = 1;
			}
			else if (state == 1) {
				state = 0;
			} else {
				map1.swap_cell(cursor_r, cursor_c);
			}

		}

		if (IsKeyPressed(KEY_E)) {
			
			if (state == 2) {
				state = 1;
			} else {
				state = 2;
			}
		}

		if (state == 2) {
			if (IsKeyPressed(KEY_DOWN)) {
				cursor_r += 1;
			}
			if (IsKeyPressed(KEY_UP)) {
				cursor_r -= 1;
			}
			if (IsKeyPressed(KEY_LEFT)) {
				cursor_c -= 1;
			}
			if (IsKeyPressed(KEY_RIGHT)) {
				cursor_c += 1;
			}

			if (cursor_c < 0) {
				cursor_c = 0;
			}

			if (cursor_c >= MAP_COLUMNS) {
				cursor_c = MAP_COLUMNS-1;
			}
			if (cursor_r < 0) {
				cursor_r = 0;
			}

			if (cursor_r >= MAP_ROWS) {
				cursor_r = MAP_ROWS-1;
			}

		}

        BeginDrawing();
            ClearBackground(RAYWHITE);
			map1.draw_cells();


			if (state == 0) {
				DrawText("Running", 10, 10, 25, LIGHTGRAY);
			}
			if (state == 1) {
				DrawText("Paused", 10, 10, 25, LIGHTGRAY);
			}
			if (state == 2) {
				draw_cursor(cursor_r, cursor_c, cell_size);
				DrawText("Editing", 10, 10, 25, LIGHTGRAY);
			}

        EndDrawing();

		/* timer used so that cells are only updated every half a second even though program accepts
		input at 60 fps. */
		if (state == 0 && TimerDone(frame_timer)){
			map1.update_cells();
			StartTimer(&frame_timer, DELAY);
		}

    }

    // De-Initialization
    CloseWindow();

    return 0;
}