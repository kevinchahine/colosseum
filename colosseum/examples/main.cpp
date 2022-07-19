#include <iostream>
#include <string>

#include "examples/examples.h"

using namespace std;

int main(int argc, const char ** argv)
{
	cout << "=== Colosseum ===" << endl;

	for (int i = 0; i < argc; i++) {
		cout << '\t' << argv[i] << endl;
	}

	engine_vs_engine();

	
	
	return 0;
}