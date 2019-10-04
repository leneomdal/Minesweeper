#include "MinesweeperWindow.h"

int main()
{
	srand(static_cast<unsigned int>(time(nullptr)));
	Fl::background(200, 200, 200);
	constexpr int width = 20;
	constexpr int height = 17;
	constexpr int mines = 15;

	Point startPoint{ 200,250 };
	MinesweeperWindow mw{ startPoint, width, height, mines, "Lene's Minesweeper ;)" };
	return gui_main();

}
