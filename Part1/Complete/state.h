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
};

struct State {
	vector<Mypiece> white; //holds information about white
	vector<Mypiece> black; //holds information about black
	string turn; //in this state whose turn is it.
	string enPassant = "-";
	bool castleK = false;
	bool castleQ = false;
	bool castlek = false;
	bool castleq = false;
	
	int kRank = 1;
	char kFile = 'a';

	//The state holds information about the board
	//player defines if it's a black or white that's moving
	void /*vector<Action>*/ actions(vector<Action>& actionList);
};

#include "state.hpp"
#endif