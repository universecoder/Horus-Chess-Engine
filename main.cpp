#include "chessboard.hpp"
#include "move.hpp"
#include "data.hpp"
#define empty -1

static chessboard * theBoard = 0;

void dumpAll() {
	if (theBoard != 0) {
		theBoard->printGame();
		theBoard->printBoard();
	}
}

int main() {
    initHash();
    setUpDebugging();

    cout << "feature done=0" << endl;
    cout.setf(ios::unitbuf);

    chessboard b;
    theBoard = &b;
    int engineSide = empty;
    bool isConsoleMode = true;

    string command;

    while (true) {
        if (isConsoleMode) {
            cout << "# Your move/command: ";
            flush(cout);
        }
        cin >> command;
        cout << "# received command: " << command << endl;

        if ( command == "" ) continue; // in console mode, an empty command should not terminate the engine

        if ( command == "quit" ) break;

        if ( command == "xboard" ) {
            cout << endl;
            isConsoleMode = false;
        }

        else if ( command == "protover" ) {
            int temp;
            cin >> temp;
            if ( temp >= 2 ) {
                cout << "feature ping=1 time=0 sigint=0 sigterm=0 usermove=1 done=1" << endl;
            }
        }

        else if ( command == "ping" ) {
            int param;
            cin >> param;
            cout << "# received parameter: " << param << endl;
            cout << "pong " << param << endl;
        }

        else if ( command == "force" ) {
            engineSide = empty;
        }

        else if ( command == "new" ) {
            b.resetToInitialPosition();
            engineSide = black;
        }

        else if ( command == "go" ) {
            engineSide = b.side;
        }

        else if ( command == "usermove" ) {
            string param;
            cin >> param;
            cout << "# received parameter: " << param << endl;
            Move move = b.parseMoveFromString(param);
            if (move.isNull()) {
                cout << "Error (illegal move): " << param << endl;
            } else {
                b.playMove(move);
                b.printMinimalBoard();
            }
        }

        else if ( command == "accepted" ) {
            string temp;
            cin >> temp;
            continue;
        }

        else if ( command == "selftest" ) {
            samplePerftTest();
        }
        
        else {
            cout << "# ignoring unknown command: " << command << endl;
            continue;
        }

        if (engineSide == b.side) {
            cout << "# calculating engine move ..." << endl;
            Move move = b.findMove();
            cout << "move ";
            move.printMove(b.side);
            cout << endl;
            b.playMove(move);
            b.printMinimalBoard();
        }
    }

    return 0;
}

