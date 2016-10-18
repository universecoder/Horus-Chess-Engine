#ifndef __CHESSBOARD__
#define __CHESSBOARD__

#include "data.hpp"
#include "move.hpp"

class chessboard {
private:
	
	// Basic Board Representation and Data
	int board[BOARDSIZE];
	int plies, moves;

	int actualPlies;

	int fiftyMoveRule, threeFold;
	bool whiteCastlePerms[2];
	bool blackCastlePerms[2];
	int enPassantSquare[2]; // enPassantSquare[white] => white can capture it
	
	// Makes generating the moves efficient, we only have to iterate over the piece list rather 64 squares
	// PieceList - an array of integer sets
	// Also makes checking the legality of the moves easier
	unordered_set <int> pieceList[13];
	
	// Maintain the position hashes for the three fold repetition rule
	ULL uniqueKey;

	// Maintain flag indicating whether king of moving side is in check
	bool inCheck;

	// Maintain set of pieces currently pinned to king of moving side
	ULL pins;
	
	// To store moves while we are searching the game tree 

	vector < array<bool, 4> > castleList;
	vector < pair<int, int> > enPassSqList;
	vector <ULL> keyList;
	
	// To store the moves actually played and the position hash during that time(for the fifty move rule)
	vector < pair<Move, ULL> > moveHistory;
	
	// Member Functions:
	void initEmptyBoard();

	void fenSetup(string &fen);

	void initPieceList();
	void initUniqueKey();

	bool isSquareSafe(int square, int Side);
	bool isSquareAttacked(int square, int Side);
	bool isMoveValid(Move &move);
	void addMove(Move &move, vector <Move> &moveList);
	int staticEval();
	int negamax(int depth);
	int alphaBeta(int alpha, int beta, int depth);

	enum {
		MAX_GAME_MOVES = 1000
	};

	Move game[MAX_GAME_MOVES];

#ifndef NDEBUG
	bool suppressValidityCheck;
#endif /* NDEBUG */



public:
	chessboard();
	chessboard(string &fen);
	void resetToFEN(string &fen);
	void resetToInitialPosition();
	void playMove(Move &move);
	void undoMove(Move &move);
	void printBoard();
	void printMinimalBoard();
	void printGame();
	void generateAllMoves(vector <Move> &moveList);
	ULL perft(int depth);

	bool isValid();

	static ULL hashList[13][64];
	static ULL whiteCastleHash[2];
	static ULL blackCastleHash[2];
	static ULL sideHash[2];
	static ULL enPassantHash[64];
	
	void test();

	int side;
	
	Move findMove();
	
	Move parseMoveFromString(string m);
	ULL getPins();
	int kingSquare(int side);
};

#endif
