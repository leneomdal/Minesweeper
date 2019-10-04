#include "MinesweeperWindow.h"

MinesweeperWindow::MinesweeperWindow(Point xy, int width, int height, int mines, const string& title) :
	Graph_lib::Window(xy, width * cellSize, height*cellSize, title), width(width), height(height), mines(mines), 
	gameEndBackground(Point{ Width() / 2 - 60, Height() / 2 - 40 }, Point{ Width() / 2 + 60 ,Height() / 2 }), 
	gameEndText(Point{ Width() / 2 - 60 + 3, Height() / 2 - 40 * 3 / 10 }, " "), tilesLeft{ height*width - mines}
	//Initialiser medlemsvariabler, bruker også konstruktøren til Windowsklassen
{
	// Legg til alle tiles på vinduet
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			int y = i* cellSize,
				x = j * cellSize;
			tiles.push_back(new Tile{ Point{x, y}, cellSize, cb_click });
			attach(tiles.back());
		}
	}

	//Legg til miner på tilfeldige posisjoner
	int i = 0;
	while (i < mines) {
		int pos = rand() % tiles.size();
		if (!tiles[pos].isMine) {
			tiles[pos].isMine = true;
			++i;
		}
	}

	// Fjern window reskalering
	resizable(nullptr);
	size_range(x_max(), y_max(), x_max(), y_max());
}

int MinesweeperWindow::countMines(vector<Point> points) const {
	int numMines = 0;
	for (Point p : points) {
		
		if (at(p).isMine) {
			++numMines;
		}
	}
	return numMines;
};
vector<Point> MinesweeperWindow::adjacentPoints(Point xy) const {
	vector<Point> points;
	for (int di = -1; di <= 1; ++di) {
		for (int dj = -1; dj <= 1; ++dj) {
			if (di == 0 && dj == 0) {
				continue;
			}

			Point neighbour{ xy.x + di * cellSize,xy.y + dj * cellSize };
			if (inRange(neighbour)) {
				points.push_back(neighbour);
			}
		}
	}

	return points;
}

void MinesweeperWindow::openTile(Point xy) {

	Tile& tile = at(xy);
	if (tile.state == Cell::closed) {
		tile.open();
		--tilesLeft;
		if (tilesLeft == 0) {
			gameWon();
		}
		if (!tile.isMine) {
			vector<Point> points = adjacentPoints(xy);
			int numMines = countMines(points);
			if (numMines > 0) {
				tile.setAdjMines(numMines);
			}
			else {
				for (Point p : points) {
					openTile(p);
				}
			}
			
		}
		if (tile.isMine) {
			gameLost();
		}
	}

}

void MinesweeperWindow::flagTile(Point xy) {
	Tile& tile = at(xy);                                              // hvorfor &????
	switch (tile.state)
	{
	case(Cell::closed):
		tile.flag();
		break;
	case(Cell::flagged):
		tile.flag();
		break;
	default:
		break;
	}
	
}

//Kaller opentile ved venstreklikk og flagTile ved høyreklikk
void MinesweeperWindow::cb_click(Address, Address pw)
{
	Point xy{ Fl::event_x(),Fl::event_y() };
	MouseButton mb = static_cast<MouseButton>(Fl::event_button());
	auto& win = reference_to<MinesweeperWindow>(pw);

	if (!win.inRange(xy)) {
		return;
	}

	switch (mb) {
	case MouseButton::left:
		win.openTile(xy);
		break;
	case MouseButton::right:
		win.flagTile(xy);
		break;
	}
	win.flush();
}

void MinesweeperWindow::gameLost()
{
	gameIsLost = true;
	for (Tile* tile : tiles) {            //// hvorfor må denne være en pointer?
		if (tile->isMine) {
			tile->open();
		}
	}
	gameEndWindow("GAME OVER", Color::dark_red);
}

void MinesweeperWindow::gameWon()
{
	for (Tile* tile : tiles) {
		if (tile->state == Cell::closed) {
			tile->flag();
		}
	}
	gameEndWindow("GAME WON", Color::dark_green);
}

void MinesweeperWindow::gameEndWindow(string s, Color c)
{
	gameEndBackground.set_color(c);
	gameEndBackground.set_fill_color(c);
	gameEndText.set_label(s);
	gameEndText.set_fill_color(Color::black);
	gameEndText.set_font_size(20);

	attach(gameEndBackground);
	attach(gameEndText);
}

