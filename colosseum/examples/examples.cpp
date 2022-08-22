#include "examples.h"
#include "games/games.hpp"
#include "views/views.hpp"
#include "source/elo_rating.h"
#include "source/globals.h"

#include <algorithm>
#include <iostream>

#include <boost/filesystem/path.hpp>

#include <forge/core/Position.h>

#include <guten/boards/CheckerBoard.h>

#include <uci/commands/commands.hpp>
#include <uci/engine.h>

using namespace std;

const boost::filesystem::path& whiteEnginePath = globals::stockfishPath;
const boost::filesystem::path& blackEnginePath = globals::lc0Path;

void engine_vs_engine()
{
	// 1.) --- Launch Each Engine ---
	boost::filesystem::path whiteEnginePath = globals::stockfishPath;
	boost::filesystem::path blackEnginePath = globals::lc0Path;

	cout << whiteEnginePath << endl
		<< blackEnginePath << endl;

	uci::engine engine1(whiteEnginePath);
	uci::engine engine2(blackEnginePath);

	cout << "Setting up " << whiteEnginePath << "..." << endl;
	engine1.send_uci();
	engine1.recv_until_uciok();
	engine1.send_isready();
	engine1.recv_until_readyok();
	engine1.send_ucinewgame();
	cout << "done" << endl;

	cout << "Setting up " << blackEnginePath << "..." << endl;
	engine2.send_uci();
	engine2.recv_until_uciok();
	engine2.send_isready();
	engine2.recv_until_readyok();
	engine2.send_ucinewgame();
	cout << "done" << endl;

	forge::Position pos;
	pos.setupNewGame();

	vector<string> game_moves;
	game_moves.reserve(50);

	uci::commands::go go_param;
	uci::commands::command cmd;

	guten::boards::CheckerBoard cb = pos.board().getCheckerBoard();
	cb.print();

	while (true) {
		cerr << "\t" << engine1.engine_name() << "'s turn...";

		uci::commands::position pos_cmd;
		pos_cmd.pos = pos.toFEN();
		engine1.send_position(pos_cmd);

		go_param.movetime = 3'000;	// search for exactly 1'000 msec
		engine1.send_go(go_param);

		uci::commands::bestmove best = engine1.recv_until_bestmove();
		cout << "Best move command: " << cmd << endl;

		forge::Move bestmove = best.move;

		cout << "\tBest move is: " << bestmove << endl;

		game_moves.push_back(bestmove.toLAN());

		pos.move<forge::pieces::Piece>(bestmove);

		{
			guten::boards::CheckerBoard cb = pos.board().getCheckerBoard();
			cb.highlight(bestmove.from().row(), bestmove.from().col());
			cb.highlight(bestmove.to().row(), bestmove.to().col());
			cb.print();
		}

		cout << "--------------------------------------------" << endl;

		// --- ENGINE 2 ---

		cerr << "\t" << engine2.engine_name() << "'s turn...";

		pos_cmd.pos = pos.toFEN();
		engine2.send_position(pos_cmd);
		cout << "currpos: " << pos << endl;

		go_param.movetime = 3'000;	// search for exactly 1'000 msec
		engine2.send_go(go_param);

		best = engine2.recv_until_bestmove();
		cout << "Best move command: " << cmd << endl;

		bestmove = best.move;

		cout << "\tBest move is: " << bestmove << endl;

		game_moves.push_back(bestmove.toLAN());

		pos.move<forge::pieces::Piece>(bestmove);

		{
			guten::boards::CheckerBoard cb = pos.board().getCheckerBoard();
			cb.highlight(bestmove.from().row(), bestmove.from().col());
			cb.highlight(bestmove.to().row(), bestmove.to().col());
			cb.print();
		}

		cout << "--------------------------------------------" << endl;
	}
}

void elo_rating()
{
	EloRating player1;
	EloRating player2;
	EloRating player3;

	cout << player1 << endl
		<< player2 << endl
		<< player3 << endl
		<< endl;

	// --- 1 beats 2 ---
	EloRating::update(player1, 1.0, player2, 0.0);

	cout << player1 << endl
		<< player2 << endl
		<< player3 << endl
		<< endl;

	// --- 2 beats 3 ---
	EloRating::update(player2, 1.0, player3, 0.0);

	cout << player1 << endl
		<< player2 << endl
		<< player3 << endl
		<< endl;

	// --- 1 beats 3 ---
	EloRating::update(player1, 1.0, player3, 0.0);

	cout << player1 << endl
		<< player2 << endl
		<< player3 << endl
		<< endl;
}

void chess_match()
{
	games::ChessMatch match;
	match.launchWhiteEngine(whiteEnginePath);
	match.launchBlackEngine(blackEnginePath);
	match.makeView<views::TextView>();

	uci::commands::go goParams;
	goParams.movetime = 1'000;
	goParams.wtime = chrono::duration_cast<chrono::milliseconds>(chrono::minutes(5)).count();
	goParams.winc = chrono::duration_cast<chrono::milliseconds>(chrono::seconds(5)).count();
	goParams.btime = chrono::duration_cast<chrono::milliseconds>(chrono::minutes(5)).count();
	goParams.binc = chrono::duration_cast<chrono::milliseconds>(chrono::seconds(5)).count();

	match.play(goParams);
}

void round_robin()
{
	RoundRobin tournament;

	tournament.makeView<views::NoView>();
	//tournament.makeView<views::TextView>();

	tournament.saveFile = "C:/Users/kchah/Code/Projects/AI/colosseum/colosseum/data/roundRobin.json";

	tournament.load();

	tournament.setNRounds(100);
	uci::commands::go go;
	go.movetime = 1000;	//  msec search time
	go.btime = 1000;
	go.wtime = 1000;
	go.depth = 22;

	// !!! Do not call this and tournament.load() together !!!
	// Otherwise engines will be added more than once
	tournament.go() = go;
	for (const auto& path : globals::enginePaths) {
		tournament.players().emplace_back(path);
	}

	tournament.play();

	tournament.save();
}

