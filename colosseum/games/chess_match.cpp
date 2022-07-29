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

	void ChessMatch::play(const uci::go& whites_params, const uci::go& blacks_params)
	{
		// --- Clock ---

		_clock.synchronize(
			chrono::milliseconds(whites_params.wtime.value_or(22)),
			chrono::milliseconds(blacks_params.btime.value_or(22)),
			chrono::milliseconds(whites_params.winc.value_or(22)),
			chrono::milliseconds(blacks_params.binc.value_or(22))
		);

		cout << "clock: " << _clock << endl;
		cin.get();

		forge::Position position;
		position.setupNewGame();

		_whiteEngine.send_ucinewgame();
		_blackEngine.send_ucinewgame();

		while (true)
		{
			// --- 1.) Who's turn is it? ---
			uci::UciClient& currEngine = (position.isWhitesTurn() ? _whiteEngine : _blackEngine);
			uci::go currGo = (position.isWhitesTurn() ? whites_params : blacks_params);

			// --- Set clock ---
			currGo.winc = _clock.get_whites_increment().count();
			currGo.wtime = chrono::duration_cast<chrono::milliseconds>(_clock.get_white_timer().expires_from_now()).count();
			currGo.binc = _clock.get_blacks_increment().count();
			currGo.btime = chrono::duration_cast<chrono::milliseconds>(_clock.get_black_timer().expires_from_now()).count();

			cerr << currEngine.engine_name() << "'s turn...";

			_clock.resume();

			currEngine.send_position(position.toFEN());
			currEngine.send_go(currGo);

			const uci::Command & cmd = currEngine.recv_until_bestmove();

			_clock.stop();

			if (cmd.is_bestmove()) {
				forge::Move bestmove = cmd.at(1);

				cout << "\tBest move is: " << bestmove << endl;

				// --- Validate move. Make sure its legal. ---
				if (!isMoveLegal(position, bestmove)) {
					cout << "Move is illegal!!!" << endl;

					break;
				}

				// --- Apply move ---
				position.move<forge::pieces::Piece>(bestmove);

				_history.push_back(forge::MovePositionPair(cmd.at(1), position));

				this->_viewPtr->highlight(bestmove.from());
				this->_viewPtr->highlight(bestmove.to());
				this->_clock = _clock;
				this->_viewPtr->show(position);
			}
			else {
				cout << '\t' << "Error: bestmove not received: " << cmd << endl;
			}

			// --- Is it game over yet? ---
			forge::GameState state;
			state(_history);

			cout << state << endl;
			if (state.isGameOver()) {
				cout << state << endl;

				break;
			}
		} // while (true)	
	}

	void ChessMatch::play(const uci::go& params)
	{
		this->play(params, params);
	}
} // namespace games
