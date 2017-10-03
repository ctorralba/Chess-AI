//Christopher Torralba
//Contains the board state (Information about piece placement)

#ifndef STATE_H
#define STATE_H

#include <iostream>
#include <vector>
using namespace std;

struct Mypiece {
	char m_type; //P,B,Q,K,N,R or p b q k n r
	char m_file; //column a b c d e f g h
	short m_rank; //row 1 2 3 4 5 6 7 8

	//default constructor --INLINE--
	Mypiece() : m_type('P'), m_file('A'), m_rank(1) {}
	
	void setPiece(const char type, const char file, const int rank);
	string convertType();

};

struct Action {
	//type 
	char m_type;
	//1st pos
	Mypiece original;
	//2nd pos
	Mypiece move;
	void setAction(const char m_type, const Mypiece start, const Mypiece end);
	//specify if promoted, etc.

	int utilityVal = -9999; //arbritary small value that heuristic can't go past

	bool operator==(const Action &other) const;
};

struct State {
	vector<Mypiece> white; //holds information about white
	vector<Mypiece> black; //holds information about black
	string turn; //in this state whose turn is it.
	Action actionApplied; //Last move made by opponent

	//Special Cases:
	string enPassant = "-";
	//Castling:
	bool castleK = false;
	bool castleQ = false;
	bool castlek = false;
	bool castleq = false;
	//IDM:
	string max = "None";
	//King position for current player:
	int kRank = 1;
	char kFile = 'a';

	//For draws
	vector<Action> last8Moves;
	int fiftyMoveRule = 50;


	//The state holds information about the board
	//player defines if it's a black or white that's moving
	void actions(vector<Action>& actionList);

	/*checks following conditions:
		- Three fold repetition (simplified)
		- Stalemate (Precondition before call)
		- 50 move Rule
		- Insufficient Material
	*/
	bool draw();
};

#include "state.hpp"
#endif