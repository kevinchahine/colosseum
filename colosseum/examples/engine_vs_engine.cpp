#include "examples.h"

#include <iostream>

#include <boost/filesystem/path.hpp>

#include <guten/boards/CheckerBoard.h>

#include <uci/commands/command.h>
#include <uci/uci_client.h>

using namespace std;

void engine_vs_engine()
{
	// 1.) --- Launch Each Engine ---
	boost::filesystem::path engine1_path = "/usr/games/stockfish";
	boost::filesystem::path engine2_path = "/usr/games/ethereal-chess";
	
	cout << engine1_path << endl
		<< engine2_path << endl;

	uci::UciClient engine1(engine1_path.string());
	uci::UciClient engine2(engine2_path.string());

	guten::boards::CheckerBoard board;

	cout << "Setting up " << engine1_path << "..." << endl;
	engine1.send_uci();
	engine1.recv_until_uciok();
	engine1.send_isready();
	engine1.recv_until_readyok();
	engine1.send_ucinewgame();
	cout << "done" << endl;

	cout << "Setting up " << engine2_path << "..." << endl;
	engine2.send_uci();
	engine2.recv_until_uciok();
	engine2.send_isready();
	engine2.recv_until_readyok();
	engine2.send_ucinewgame();
	cout << "done" << endl;	

	vector<string> game_moves;
	game_moves.reserve(50);

	uci::go go_param;
	uci::Command cmd;

	while (true) {
		board.print();

		cerr << "\t" << engine1.engine_name() << "'s turn...";

		engine1.send_position("startpos", game_moves);

		go_param.movetime = 1'000;	// search for exactly 1'000 msec
		engine1.send_go(go_param);

		cmd = engine1.recv_until_bestmove();

		if (cmd.front() == "bestmove") {
			cout << "\tBest move is: " << cmd.at(1) << endl;

			game_moves.push_back(cmd.at(1));
		}
		else {
			cout << '\t' << cmd << endl;
		}

		cout << endl;

		// --- ENGINE 2 ---

		board.print();

		cerr << "\t" << engine2.engine_name() << "'s turn...";

		engine2.send_position("startpos", game_moves);

		go_param.movetime = 1'000;	// search for exactly 1'000 msec
		engine2.send_go(go_param);

		cmd = engine2.recv_until_bestmove();

		if (cmd.front() == "bestmove") {
			cout << "\tBest move is: " << cmd.at(1) << endl;

			game_moves.push_back(cmd.at(1));
		}
		else {
			cout << '\t' << cmd << endl;
		}

		cout << endl;
	}
}