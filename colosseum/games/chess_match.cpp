#pragma once

#include "chess_match.h"

#include <forge/core/GameState.h>

#include <algorithm>

using namespace std;

namespace games
{
	// Determines whether `move` is legal at `position`
	bool isMoveLegal(const forge::Position& position, const forge::Move& move)
	{
		forge::MoveGenerator2 movegen;

		forge::MoveList movelist = movegen.generate(position);

		bool isFound = false;

		for (const forge::MovePositionPair& pair : movelist) {
			if (pair.move == move) {
				isFound = true;
				break;
			}
		}

		return isFound;
	}

	// ------------------------------------------------------------------------

	void ChessMatch::launchWhiteEngine(const boost::filesystem::path& filename)
	{
		_whiteEngine.launch(filename);

		_whiteEngine.send_uci();
		_whiteEngine.recv_until_uciok();
		_whiteEngine.send_isready();
		_whiteEngine.recv_until_readyok();
	}

	void ChessMatch::launchBlackEngine(const boost::filesystem::path& filename)
	{
		_blackEngine.launch(filename);

		_blackEngine.send_uci();
		_blackEngine.recv_until_uciok();
		_blackEngine.send_isready();
		_blackEngine.recv_until_readyok();
	}

	forge::GameState ChessMatch::play(const uci::commands::go& whites_params, const uci::commands::go& blacks_params)
	{
		forge::GameState state;

		// --- Clock ---

		_clock.synchronize(
			chrono::milliseconds(whites_params.wtime.value_or(22)),
			chrono::milliseconds(blacks_params.btime.value_or(22)),
			chrono::milliseconds(whites_params.winc.value_or(22)),
			chrono::milliseconds(blacks_params.binc.value_or(22))
		);

		forge::Position position;
		position.setupNewGame();

		_whiteEngine.send_ucinewgame();
		_blackEngine.send_ucinewgame();

		while (true)
		{
			// --- 1.) Who's turn is it? ---
			uci::engine& currEngine = (position.isWhitesTurn() ? _whiteEngine : _blackEngine);
			uci::commands::go currGo = (position.isWhitesTurn() ? whites_params : blacks_params);

			// --- Set clock ---
			//currGo.winc = _clock.get_whites_increment().count();
			//currGo.wtime = chrono::duration_cast<chrono::milliseconds>(_clock.get_white_timer().expires_from_now()).count();
			//currGo.binc = _clock.get_blacks_increment().count();
			//currGo.btime = chrono::duration_cast<chrono::milliseconds>(_clock.get_black_timer().expires_from_now()).count();

			_clock.resume();

			uci::commands::position pos_cmd;
			pos_cmd.pos = position.toFEN();
			currEngine.send_position(pos_cmd);
			currEngine.send_go(currGo);

			const uci::commands::bestmove& bestmove = currEngine.recv_until_bestmove();

			_clock.stop();

			forge::Move move = bestmove.move;

			// --- Validate move. Make sure its legal. ---
			if (!isMoveLegal(position, move)) {
				cout << termcolor::red << "Move: " << bestmove << " is illegal!!!" << termcolor::white << endl;

				state(_history);

				break;
			}

			// --- Apply move ---
			position.move<forge::pieces::Piece>(move);

			_history.push_back(forge::MovePositionPair(move, position));

			this->_viewPtr->highlight(move.from());
			this->_viewPtr->highlight(move.to());
			this->_clock = _clock;
			this->_viewPtr->show(position);

			// --- Is it game over yet? ---
			state(_history);

			if (state.isGameOver()) {
				break;
			}
		} // while (true)	

		return state;
	}

	forge::GameState ChessMatch::play(const uci::commands::go& params)
	{
		return this->play(params, params);
	}
} // namespace games
