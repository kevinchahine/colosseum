#include "text_view.h"

#include <guten/boards/CheckerBoard.h>
#include <guten/draw/DrawFunctions.h>

#include <string>
#include <sstream>

using namespace std;

namespace views
{
	guten::core::Matrix drawLeftPanel(
		const TextView& view,
		const guten::Size& size,
		const forge::Position& position,
		const forge::clock& clock,
		bool isWhite)
	{
		guten::core::Matrix panel{ size };

		guten::draw::rectangle(panel, panel.origin(), panel.size());

		guten::draw::putText(panel, (isWhite ? "White" : "Black"), guten::Point{ 2, 2 });

		// --- Pieces ---
		const forge::Board& b = position.board();
		forge::BitBoard pieces = (isWhite ? b.whites() : b.blacks());

		forge::BitBoard ourQueens = b.queens() & pieces;
		forge::BitBoard ourRooks = b.rooks() & pieces;
		forge::BitBoard ourBishops = b.bishops() & pieces;
		forge::BitBoard ourKnights = b.knights() & pieces;
		forge::BitBoard ourPawns = b.pawns() & pieces;

		guten::boards::CheckerBoard cb;

		termcolor::ColorFBG color((isWhite ? cb.lightPiece : cb.darkPiece), cb.lightCell);

		guten::draw::putText(panel, string(ourQueens.count(), 'Q'), guten::Point{ 4, 3 }, color);
		guten::draw::putText(panel, string(ourRooks.count(), 'R'), guten::Point{ 5, 3 }, color);
		guten::draw::putText(panel, string(ourBishops.count(), 'B'), guten::Point{ 6, 3 }, color);
		guten::draw::putText(panel, string(ourKnights.count(), 'N'), guten::Point{ 7, 3 }, color);
		guten::draw::putText(panel, string(ourPawns.count(), 'P'), guten::Point{ 8, 3 }, color);

		// --- Clock ---

		stringstream ss;
		ss << (isWhite ? clock.get_white_timer() : clock.get_black_timer());

		guten::draw::putText(panel, ss.str(), guten::Point{ 10, 3 });

		return panel;
	}

	void TextView::show(const forge::Position& position)
	{
		// --- Create Checker Board ---
		guten::boards::CheckerBoard cb;

		const forge::Board& b = position.board();

		forge::BitBoard whites = b.whites();
		forge::BitBoard blacks = b.blacks();

		cb.placePieces('K', b.kings() & whites, true);
		cb.placePieces('Q', b.queens() & whites, true);
		cb.placePieces('R', b.rooks() & whites, true);
		cb.placePieces('B', b.bishops() & whites, true);
		cb.placePieces('N', b.knights() & whites, true);
		cb.placePieces('p', b.pawns() & whites, true);

		cb.placePieces('K', b.kings() & blacks, false);
		cb.placePieces('Q', b.queens() & blacks, false);
		cb.placePieces('R', b.rooks() & blacks, false);
		cb.placePieces('B', b.bishops() & blacks, false);
		cb.placePieces('N', b.knights() & blacks, false);
		cb.placePieces('p', b.pawns() & blacks, false);

		cb.highlight(this->_highlights);

		guten::core::Matrix boardMat = cb.draw();

		// --- Draw Left Side Panel ---

		guten::core::Matrix blacksPanel = drawLeftPanel(*this, guten::Size{ boardMat.nRows() / 2, 20 }, position, _clock, false);
		guten::core::Matrix whitesPanel = drawLeftPanel(*this, guten::Size{ boardMat.nRows() / 2, 20 }, position, _clock, true);

		guten::core::Matrix leftPanel(boardMat.nRows(), 20);
		blacksPanel.copyTo(leftPanel, guten::Point{ 0, 0 });
		whitesPanel.copyTo(leftPanel, guten::Point{ leftPanel.nRows() / 2, 0 });

		//guten::draw::rectangle(
		//	leftPanel,
		//	guten::Point{ 0, 0 },
		//	guten::Point{ 0, 0 } + leftPanel.size());
		
		// --- Put it all together ---

		int nRows = boardMat.nRows();
		int nCols = boardMat.nCols() + leftPanel.nCols();

		guten::core::Matrix mat{ nRows, nCols };

		leftPanel.copyTo(mat, guten::Point{ 0, 0 });
		boardMat.copyTo(mat, guten::Point{ 0, leftPanel.nCols() });

		mat.print();

		this->_highlights = forge::BitBoard();	// clear highlights
	}
} // anmespace views