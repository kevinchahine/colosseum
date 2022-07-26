#include "examples.h"

#include <iostream>

#include <boost/filesystem/path.hpp>

#include <forge/core/Position.h>

#include <guten/boards/CheckerBoard.h>

#include <uci/commands/command.h>
#include <uci/uci_client.h>

using namespace std;

void engine_vs_engine()
{
	// 1.) --- Launch Each Engine ---
	boost::filesystem::path engine1_path =
		//R"dil(/usr/games/stockfish)dil";
		R"dil(C:\Users\kchah\Saved Games\stockfish_15\stockfish_15.exe)dil";

	boost::filesystem::path engine2_path =
		//R"dil(/usr/games/ethereal-chess)dil";
		R"dil(C:\Users\kchah\Saved Games\lc0\lc0.exe)dil";
		//engine1_path;

	cout << engine1_path << endl
		<< engine2_path << endl;
	
	uci::UciClient engine1(engine1_path.string());
	uci::UciClient engine2(engine2_path.string());
	
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

	forge::Position pos;
	pos.setupNewGame();

	vector<string> game_moves;
	game_moves.reserve(50);
	
	uci::go go_param;
	uci::Command cmd;
	
	vector<forge::Move> moves;

	guten::boards::CheckerBoard cb = pos.board().getCheckerBoard();
	cb.print();

	while (true) {
		cerr << "\t" << engine1.engine_name() << "'s turn...";
	
		engine1.send_position(pos.toFEN());
		//engine1.send_position("startpos", game_moves);
		cout << "currpos ^^^:" << pos << endl;

		go_param.movetime = 3'000;	// search for exactly 1'000 msec
		engine1.send_go(go_param);
	
		cmd = engine1.recv_until_bestmove();
		cout << "Best move command: " << cmd << endl;

		if (cmd.is_bestmove()) {
			forge::Move bestmove = cmd.at(1);

			cout << "\tBest move is: " << bestmove << endl;
			
			game_moves.push_back(cmd.at(1));

			pos.move<forge::pieces::Piece>(bestmove);

			guten::boards::CheckerBoard cb = pos.board().getCheckerBoard();
			cb.highlight(bestmove.from().row(), bestmove.from().col());
			cb.highlight(bestmove.to().row(), bestmove.to().col());
			cb.print();
		}
		else {
			cout << '\t' << "Error: bestmove not received: " << cmd << endl;
		}
	
		cout << "--------------------------------------------" << endl;
	
		// --- ENGINE 2 ---
	
		cerr << "\t" << engine2.engine_name() << "'s turn...";
	
		engine2.send_position(pos.toFEN());
		//engine2.send_position("startpos", game_moves);
		cout << "currpos: " << pos << endl;

		go_param.movetime = 3'000;	// search for exactly 1'000 msec
		engine2.send_go(go_param);
	
		cmd = engine2.recv_until_bestmove();
		cout << "Best move command: " << cmd << endl;

		if (cmd.is_bestmove()) {
			forge::Move bestmove = cmd.at(1);

			cout << "\tBest move is: " << bestmove << endl;
	
			game_moves.push_back(cmd.at(1));

			pos.move<forge::pieces::Piece>(bestmove);

			guten::boards::CheckerBoard cb = pos.board().getCheckerBoard();
			cb.highlight(bestmove.from().row(), bestmove.from().col());
			cb.highlight(bestmove.to().row(), bestmove.to().col());
			cb.print();
		}
		else {
			cout << '\t' << "Error: bestmove not received: " << cmd << endl;
		}
	
		cout << "--------------------------------------------" << endl;
	}
}