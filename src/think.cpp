/*
Horus - A Chess Engine created for learning Game AI techniques
Copyright (C) 2016 Pranav Deshpande

This file is a part of Horus.

Horus is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

Horus is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Horus. If not, see <http://www.gnu.org/licenses/>.
*/

#include "chessboard.hpp"

// Piece square tables for the evaluation function - Taken from https://chessprogramming.wikispaces.com/Simplified+evaluation+function
int pTable[] = {
	 0,  0,  0,  0,  0,  0,  0,  0,
	 5, 10, 10,-20,-20, 10, 10,  5,
	 5, -5,-10,  0,  0,-10, -5,  5,
	 0,  0,  0, 20, 20,  0,  0,  0,
	 5,  5, 10, 25, 25, 10,  5,  5,
	10, 10, 20, 30, 30, 20, 10, 10,
	50, 50, 50, 50, 50, 50, 50, 50,
	 0,  0,  0,  0,  0,  0,  0,  0
};

int nTable[] = {
	-50,-40,-30,-30,-30,-30,-40,-50,
	-40,-20,  0,  5,  5,  0,-20,-40,
	-30,  5, 10, 15, 15, 10,  5,-30,
	-30,  0, 15, 20, 20, 15,  0,-30,
	-30,  5, 15, 20, 20, 15,  5,-30,
	-30,  0, 10, 15, 15, 10,  0,-30,
	-40,-20,  0,  0,  0,  0,-20,-40,
	-50,-40,-30,-30,-30,-30,-40,-50
};

int bTable[] = {
	-20,-10,-10,-10,-10,-10,-10,-20,
	-10,  5,  0,  0,  0,  0,  5,-10,
	-10, 10, 10, 10, 10, 10, 10,-10,
	-10,  0, 10, 10, 10, 10,  0,-10,
	-10,  5,  5, 10, 10,  5,  5,-10,
	-10,  0,  5, 10, 10,  5,  0,-10,
	-10,  0,  0,  0,  0,  0,  0,-10,
	-20,-10,-10,-10,-10,-10,-10,-20
};

int rTable[] = {
	 0,  0,  0,  5,  5,  0,  0,  0
	-5,  0,  0,  0,  0,  0,  0, -5,
	-5,  0,  0,  0,  0,  0,  0, -5,
	-5,  0,  0,  0,  0,  0,  0, -5,
	-5,  0,  0,  0,  0,  0,  0, -5,
	-5,  0,  0,  0,  0,  0,  0, -5,
	 5, 10, 10, 10, 10, 10, 10,  5,
	 0,  0,  0,  0,  0,  0,  0,  0
};

int qTable[] = {
	-20,-10,-10, -5, -5,-10,-10,-20
	-10,  0,  5,  0,  0,  0,  0,-10,
	-10,  5,  5,  5,  5,  5,  0,-10,
	  0,  0,  5,  5,  5,  5,  0, -5,
	 -5,  0,  5,  5,  5,  5,  0, -5,
	-10,  0,  5,  5,  5,  5,  0,-10,
	-10,  0,  0,  0,  0,  0,  0,-10,
	-20,-10,-10, -5, -5,-10,-10,-20
};

// Changed the table a little bit
// So that it encourages queen side castling
int kTableMiddleGame[] = {
	 20, 20, 30,  0,  0, 10, 30, 20,
	 20, 20,  0,  0,  0,  0, 20, 20,
	-10,-20,-20,-20,-20,-20,-20,-10,
	-20,-30,-30,-40,-40,-30,-30,-20,
	-30,-40,-40,-50,-50,-40,-40,-30,
	-30,-40,-40,-50,-50,-40,-40,-30,
	-30,-40,-40,-50,-50,-40,-40,-30,
	-30,-40,-40,-50,-50,-40,-40,-30
};

int kTableEndGame[] = {
	-50,-30,-30,-30,-30,-30,-30,-50,
	-30,-30,  0,  0,  0,  0,-30,-30,
	-30,-10, 20, 30, 30, 20,-10,-30,
	-30,-10, 30, 40, 40, 30,-10,-30,
	-30,-10, 30, 40, 40, 30,-10,-30,
	-30,-10, 20, 30, 30, 20,-10,-30,
	-30,-20,-10,  0,  0,-10,-20,-30,
	 50,-40,-30,-20,-20,-30,-40,-50
};

int mirror[] = {
	56, 57, 58, 59, 60, 61, 62, 63,
	48, 49, 50, 51, 52, 53, 54, 55,
	40, 41, 42, 43, 44, 45, 46, 47,
	32, 33, 34, 35, 36, 37, 38, 39,
	24, 25, 26, 27, 28, 29, 30, 31,
	16, 17, 18, 19, 20, 21, 22, 23,
	 8,  9, 10, 11, 12, 13, 14, 15,
	 0,  1,  2,  3,  4,  5,  6,  7
};

int chessboard::staticEval() {

	int pieceVals[] = {EM, 100, 320, 330, 500, 900, 20000, -100, -320, -330, -500, -900, -20000};
	int pieceColours[] = {OB, white, white, white, white, white, white, black, black, black, black, black, black};
	int val = 0, square, *kTable;
	int num = 0;
	int numQueens = pieceList[wq].size() + pieceList[bq].size();

	for(int piece = wn; piece <= wr; piece++) num += pieceList[piece].size();
	for(int piece = bn; piece <= br; piece++) num += pieceList[piece].size();

	if( num <= 6 && numQueens == 0 ) kTable = kTableEndGame;
	else kTable = kTableMiddleGame;

	for(int piece = wp; piece <= bk; piece++) {
		val += ( pieceList[piece].size() * pieceVals[piece] );

		for(unordered_set<int>::iterator it = pieceList[piece].begin(); it != pieceList[piece].end(); it++) {
			if ( pieceColours[piece] == white ) square = board120[*it];
			else square = mirror[ board120[*it] ];

			switch(piece) {
				case wp: val += pTable[square]; break;
				case wn: val += nTable[square]; break;
				case wb: val += bTable[square]; break;
				case wr: val += rTable[square]; break;
				case wq: val += qTable[square]; break;
				case wk: val += kTable[square]; break;

				case bp: val -= pTable[square]; break;
				case bn: val -= nTable[square]; break;
				case bb: val -= bTable[square]; break;
				case br: val -= rTable[square]; break;
				case bq: val -= qTable[square]; break;
				case bk: val -= kTable[square]; break;
			}
		}
	}

	if ( side == white ) return val;
	else return -val;
}

int chessboard::negamax(int depth) {

	if ( depth == 0 ) return staticEval();

	updateInCheck();
	vector <Move> childNodes;
	generateAllMoves(childNodes);

	// If no legal moves are possible, then we check for stalemate or checkmate
	if ( childNodes.begin() == childNodes.end() ) {
		if ( inCheck ) return -infinity; // Checkmate
		else return 0; // Stalemate
	}

	int bestValue = -infinity;

	for(vector<Move>::iterator it = childNodes.begin(); it != childNodes.end(); it++) {

		Move move = *it;
		playMove(move);
		int val = -negamax(depth - 1);
		undoMove(move);

		if ( val > bestValue ) bestValue = val;
	}

	return bestValue;
}

// Fail-Hard AlphaBeta Pruning
int chessboard::alphaBeta(int alpha, int beta, int depth, int distToRoot) {

	if ( depth == 0 ) return staticEval();
	if ( checkDraw() ) return 0;

	updateInCheck();
	vector<Move> childNodes;
	generateAllMoves(childNodes);

	// If no legal moves are possible, then we check for stalemate or checkmate
	if ( childNodes.begin() == childNodes.end() ) {
		if ( inCheck ) return (-infinity + distToRoot); // Checkmate
		else return 0; // Stalemate
	}

	orderMoves(childNodes);
	for(vector<Move>::iterator it = childNodes.begin(); it != childNodes.end(); it++) {

		Move move = *it;
		playMove(move);
		int val = -alphaBeta(-beta, -alpha, depth - 1, distToRoot + 1);
		undoMove(move);

		if ( val >= beta ) return beta;

		if ( val > alpha ) alpha = val;
	}

	return alpha;
}

Move chessboard::findMove() {

	updateInCheck();
	vector<Move> moveList;
	generateAllMoves(moveList);

	Move bestMove;
	int maxScore = -infinity;

    orderMoves(moveList);
	for(vector<Move>::iterator it = moveList.begin(); it!= moveList.end(); it++) {

		Move move = *it;

		playMove(move);
		int score = -alphaBeta(-infinity, infinity, 3, 1);
		undoMove(move);

		if ( score > maxScore ) {
			bestMove = move;
			maxScore = score;
		}
	}

	return bestMove;
}
