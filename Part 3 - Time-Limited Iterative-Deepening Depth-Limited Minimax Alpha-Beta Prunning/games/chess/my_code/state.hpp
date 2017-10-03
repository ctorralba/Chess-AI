//Christopher Torralba
#include <sstream>
#include <ctime>
#include "../ai.hpp"
void Mypiece::setPiece(const char type, const char file, const int rank) {
	m_type = type;
	m_file = file;
	m_rank = rank;
	return;
}

bool Action:: operator==(const Action &other) const {
	bool isEqual = false;
	if (m_type == other.m_type) {
		if (original.m_type == other.original.m_type &&
			original.m_file == other.original.m_file &&
			original.m_rank == other.original.m_rank &&
			move.m_type == other.move.m_type &&
			move.m_file == other.move.m_file &&
			move.m_rank == other.move.m_rank) {
			isEqual = true;
		}
	}
	return isEqual;
}
//converts a rank to an index
int rankIndex(int rank) {
	return (8 - rank);
}
//converts a file to an index
int fileIndex(char file) {
	int fileNum = file - 97;
	return fileNum;
}

bool enemySpace(const char board[][8], const string opp, const int rankIndex, const int fileIndex) {
	bool enemy = false;
	if (opp == "White") {
		if (islower(board[rankIndex][fileIndex])) {
			enemy = true;
		}
	}
	else {
		if (isupper(board[rankIndex][fileIndex])) {
			enemy = true;
		}
	}
	return enemy;
}

void Action::setAction(const char type, const Mypiece start, const Mypiece end) {
	m_type = type;
	original = start;
	move = end;
	return;
}

bool enPassantCheck(const char board[][8], string enPassant, const string opp, const int rank, const int file) {
	bool isEnpassant = false;

	if (rank < 0 || rank > 7 || file < 0 || file > 7) {
		return false;
	}
	int enPassFile = fileIndex(enPassant[0]);
	int enPassRank = rankIndex(0 + (int)enPassant[1] - 48);

	if (opp == "White") {
		if (file == enPassFile && rank == enPassRank) {
			if (islower(board[rank + 1][file])) {
				isEnpassant = true;
			}
		}
	}
	else {//black
		if (file == enPassFile && rank == enPassRank) {
			if (isupper(board[rank - 1][file])) {
				isEnpassant = true;
			}
		}
	}
	return isEnpassant;
}

char emptyBound(const char board[][8], const int rank, const char file) {
	char emptyBound = 'w';
	int rankdex = rankIndex(rank);
	int filedex = fileIndex(file);
	if (rankdex < 0 || rankdex > 7 || filedex < 0 || filedex > 7) {
		return emptyBound;
	}
	if (board[rankdex][filedex] == '.') {
		emptyBound = 'e';
	}
	else {
		emptyBound = board[rankdex][filedex];
	}
	return emptyBound;
}

//this works in reverse order of moves.
//this spot checks potential spots of attack before getting attack
//rank and file has to be the same as turn
bool checkSpot(const int rank, const char file, const char board[][8], const string turn) {
	bool check = false;
	bool stop = false; //for out of bounds
					   //file rank (c3)
					   //check if black rook || queen to the right top left down
	int rmod, fmod;
	rmod = rank;
	fmod = file;
	for (int i = 0; i < 8; i++) {
		rmod = rank;
		fmod = file;
		switch (i) {
		case 0: fmod++; //start at right
			break;
		case 1: rmod++; //start at top
			break;
		case 2: fmod--; //start at left
			break;
		case 3: rmod--; //start at bottom
			break; //bishop
		case 4: fmod++; //start at top right
			rmod++;
			break;
		case 5: fmod--; //start at top left
			rmod++;
			break;
		case 6: fmod--; //start at bottom left
			rmod--;
			break;
		case 7: fmod++; //start at bottom right
			rmod--;
		}
		//empty and inbounds if there is a piece check the piece
		while (emptyBound(board, rmod, fmod) == 'e') {
			switch (i) {
			case 0:
				fmod++;
				break;
			case 1:
				rmod++;
				break;
			case 2:
				fmod--;
				break;
			case 3:
				rmod--;
				break;
			case 4: //bishop
				rmod++;
				fmod++;
				break;
			case 5:
				rmod++;
				fmod--;
				break;
			case 6:
				rmod--;
				fmod--;
				break;
			case 7:
				rmod--;
				fmod++;
				break;
			}
		}
		if (turn == "White") {
			if (emptyBound(board, rmod, fmod) != 'w') {
				if (i < 4) {
					if (emptyBound(board, rmod, fmod) == 'q'
						|| emptyBound(board, rmod, fmod) == 'r') {
						check = true;
					}
				}
				else {
					if ((emptyBound(board, rmod, fmod) == 'q'
						|| emptyBound(board, rmod, fmod) == 'b')) {
						check = true;
					}
				}
			}
		}
		else {
			if (emptyBound(board, rmod, fmod) != 'w') {
				if (i < 4) {
					if (emptyBound(board, rmod, fmod) == 'Q'
						|| emptyBound(board, rmod, fmod) == 'R') {
						check = true;
					}
				}
				else {
					if (emptyBound(board, rmod, fmod) == 'Q'
						|| emptyBound(board, rmod, fmod) == 'B') {
						check = true;
					}
				}
			}
		}
		//then out of range or there's a piece
	}
	//Reverse checking knights
	for (int i = -2; i <= 2; i++) {
		if (i != 0) {
			if (i % 2 == 0) {
				if (turn == "White") {
					if (emptyBound(board, rank + i, file + 1) == 'n') {
						check = true;
					}
					if (emptyBound(board, rank + i, file - 1) == 'n') {
						check = true;
					}
				}
				else {
					if (emptyBound(board, rank + i, file + 1) == 'N') {
						check = true;
					}
					if (emptyBound(board, rank + i, file - 1) == 'N') {
						check = true;
					}
				}
			}
			else {
				if (turn == "White") {
					if (emptyBound(board, rank + i, file + 2) == 'n') {
						check = true;
					}
					if (emptyBound(board, rank + i, file - 2) == 'n') {
						check = true;
					}
				}
				else {
					if (emptyBound(board, rank + i, file + 2) == 'N') {
						check = true;
					}
					if (emptyBound(board, rank + i, file - 2) == 'N') {
						check = true;
					}
				}
			}
		}
	}

	//pawn checking
	if (turn == "White") {
		//Pawn jump
		if (rank == 5 && board[1][fileIndex(file)] == 'p') {
			check = true;
		}
		//Pawn diagonals
		if (emptyBound(board, rank + 1, file + 1) == 'p') {
			check = true;
		}
		if (emptyBound(board, rank + 1, file - 1) == 'p') {
			check = true;
		}
	}
	else {
		//Pawn jump
		if (rank == 4 && board[6][fileIndex(file)] == 'P') {
			check = true;
		}
		//Pawn diagonals
		if (emptyBound(board, rank - 1, file - 1) == 'P') {
			check = true;
		}
		if (emptyBound(board, rank - 1, file + 1) == 'P') {
			check = true;
		}
	}
	//king checking
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1;j++) {
			if (!(i == 0 && j == 0)) {
				if (turn == "White") {
					if (emptyBound(board, rank + i, file + j) == 'k') {
						check = true;
					}
				}
				else {
					if (emptyBound(board, rank + i, file + j) == 'K') {
						check = true;
					}
				}
			}
		}
	}
	return check;
}

bool kingCheck(char board[][8], int kRank, char kFile, const Mypiece old, const int rank, const char file, const string opp) {
	char temp = '.';
	int rankDex = rankIndex(rank);
	int fileDex = fileIndex(file);
	bool valid = false;

	temp = board[rankDex][fileDex];
	board[rankDex][fileDex] = old.m_type;
	board[rankIndex(old.m_rank)][fileIndex(old.m_file)] = '.';
	if (old.m_type == 'K') {
		kRank = rank;
		kFile = file;
	}
	if (!checkSpot(kRank, kFile, board, opp)) {
		valid = true;
	}
	board[rankDex][fileDex] = temp;
	board[rankIndex(old.m_rank)][fileIndex(old.m_file)] = old.m_type;
	if (old.m_type == 'K') {
		kRank = old.m_rank;
		kFile = old.m_file;
	}
	return valid;
}

//Also checks if out of bounds
bool emptyOrOpp(char board[][8], const string opp, const int rank, const char file, int kRank, char kFile, const Mypiece old, const string enPassant) {
	bool valid = false;
	bool doCheck = false;
	char temp = '.';
	int rankDex = rankIndex(rank);
	int fileDex = fileIndex(file);
	int enPassFile = 0;
	int enPassRank = 0;
	bool qrb = false;
	if (enPassant != "-"){
		enPassFile = fileIndex(enPassant[0]);
		enPassRank = rankIndex(0 + (int)enPassant[1] - 48);
	}
	if (rankDex < 0 || rankDex > 7 || fileDex < 0 || fileDex > 7) {
		return false;
	}
	if (opp == "White") {
		if (old.m_type == 'P') {
			if (old.m_file == file) {//going straight check only if empty
				if (board[rankDex][fileDex] == '.') {
					doCheck = true;
				}
			}
			else if (old.m_file + 1 == file || old.m_file - 1 == file) {
				if (islower(board[rankDex][fileDex])) {
					doCheck = true;
				}
				if (enPassant != "-") {
					if (enPassFile == fileDex && enPassRank == rankDex) {
						if (board[rankDex][fileDex] == '.') {
							if (islower(board[rankDex + 1][fileDex])) {
								doCheck = true;
							}
						}
					}
				}
			}
		}
		else {
			if (board[rankDex][fileDex] == '.' || islower(board[rankDex][fileDex])) {
				doCheck = true;
				qrb = true;
			}
			if (isupper(board[rankDex][fileDex])) {
				return false;
			}
		}
		if (doCheck && !(old.m_type == 'Q' || old.m_type == 'R' || old.m_type == 'B')) {
			temp = board[rankDex][fileDex];
			board[rankDex][fileDex] = old.m_type;
			board[rankIndex(old.m_rank)][fileIndex(old.m_file)] = '.';
			if (old.m_type == 'K') {
				kRank = rank;
				kFile = file;
			}
			if (!checkSpot(kRank, kFile, board, opp)) {
				valid = true;
			}
			board[rankDex][fileDex] = temp;
			board[rankIndex(old.m_rank)][fileIndex(old.m_file)] = old.m_type;
			if (old.m_type == 'K') {
				kRank = old.m_rank;
				kFile = old.m_file;
			}
		}else if (old.m_type == 'Q' || old.m_type == 'R' || old.m_type == 'B' && qrb) {
			valid = true;
		}
	}
	else { //black
		if (old.m_type == 'p') {
			if (old.m_file == file) {//going straight check only if empty
				if (board[rankDex][fileDex] == '.') {
					doCheck = true;
				}
			}
			else if (old.m_file + 1 == file || old.m_file - 1 == file) {
				if (isupper(board[rankDex][fileDex])) {
					doCheck = true;
				}
				if (enPassant != "-") {
					if (enPassFile == fileDex && enPassRank == rankDex){
						if (board[rankDex][fileDex] == '.') {
							if (isupper(board[rankDex - 1][fileDex])) {
								doCheck = true;
							}
						}
					}
				}
			}
		}
		else {
			if (board[rankDex][fileDex] == '.' || isupper(board[rankDex][fileDex])) {
				doCheck = true;
				qrb = true;
			}
			if (islower(board[rankDex][fileDex])) {
				return false;
			}
		}
		if (doCheck && !(old.m_type == 'q' || old.m_type == 'r' || old.m_type == 'b')) {
			temp = board[rankDex][fileDex];
			board[rankDex][fileDex] = old.m_type;
			board[rankIndex(old.m_rank)][fileIndex(old.m_file)] = '.';
			if (old.m_type == 'k') {
				kRank = rank;
				kFile = file;
			}
			if (!checkSpot(kRank, kFile, board, opp)) {
				valid = true;
			}
			board[rankDex][fileDex] = temp;
			board[rankIndex(old.m_rank)][fileIndex(old.m_file)] = old.m_type;
			if (old.m_type == 'k') {
				kRank = old.m_rank;
				kFile = old.m_file;
			}
		}
		else if (old.m_type == 'q' || old.m_type == 'r' || old.m_type == 'b' && qrb) {
			valid = true;
		}
	}
	return valid;
}

void State::actions(vector<Action>& actionList) { //ACTIONS FUNCTION FOR STATE
												 //generating all moves for state and putting in vector actionList
												 //Generating current state for reading:
	char board[8][8];
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			board[i][j] = '.';
		}
	}

	for (int i = 0; i < white.size(); i++) {
		//file = player[i].m_file - 97;
		board[8 - white[i].m_rank][white[i].m_file - 97] = white[i].m_type;
	}
	for (int i = 0; i < black.size(); i++) {
		//file = black[i].m_file - 97;
		board[8 - black[i].m_rank][black[i].m_file - 97] = black[i].m_type;
	}

	//00 01 02 03 04
	//10 11 12 13 14
	//20 21 22 23 24
	//30 31 32 33
	//7

	Action valid;
	Mypiece tmp;
	char piece;
	//Used for Rook, Bishop and Queen
	bool enemyEncounter = false;
	int fmodifier = 0; //left and right
	int rmodifier = 0; //up and down

	int turnSize = 0;
	vector<Mypiece> player;
	bool oneCastle;
	bool third;

	if (turn == "White") {
		turnSize = white.size();
		player = white;
	}
	else {
		turnSize = black.size();
		player = black;
	}
	for (int i = 0; i < turnSize; i++) {
		//for all the pieces check where they go
		switch (player[i].m_type) { //P R N B Q K
		case ('P'):
			//call pawn movement function
			if (player[i].m_rank == 2) {
				//then check if there's an object at position 3 & 4
				//if any color at +2 then pawn can only move 1
				//if any color at +1 then pawn can't move at all
				//if there's a black piece at right and left diagonal then valid
				//board[rank][file]
				//check jump 2
				if (board[5][fileIndex(player[i].m_file)] == '.') {
					if (emptyOrOpp(board, turn, 4, player[i].m_file, kRank, kFile, player[i], enPassant)) {
						tmp.setPiece('P', player[i].m_file, player[i].m_rank + 2);
						valid.setAction('P', player[i], tmp);
						actionList.push_back(valid);
					}
				}
			}
			//check jump 1
			//Moving Up
			//If empty
			if (emptyOrOpp(board, turn, player[i].m_rank + 1, player[i].m_file, kRank, kFile, player[i], enPassant)) {
				if (player[i].m_rank != 7) {
					tmp.setPiece('P', player[i].m_file, player[i].m_rank + 1);
					valid.setAction('P', player[i], tmp);
					actionList.push_back(valid);
				}
				else {
					tmp.setPiece('Q', player[i].m_file, player[i].m_rank + 1);
					valid.setAction('P', player[i], tmp);
					actionList.push_back(valid);
					tmp.setPiece('R', player[i].m_file, player[i].m_rank + 1);
					valid.setAction('P', player[i], tmp);
					actionList.push_back(valid);
					tmp.setPiece('N', player[i].m_file, player[i].m_rank + 1);
					valid.setAction('P', player[i], tmp);
					actionList.push_back(valid);
					tmp.setPiece('B', player[i].m_file, player[i].m_rank + 1);
					valid.setAction('P', player[i], tmp);
					actionList.push_back(valid);
				}
			}
			//check if diagonal left
			if (emptyOrOpp(board, turn, player[i].m_rank + 1, player[i].m_file - 1, kRank, kFile, player[i], enPassant)) {
				if (islower(board[rankIndex(player[i].m_rank + 1)][fileIndex(player[i].m_file - 1)])) {
					if (player[i].m_rank != 7) {
						tmp.setPiece('P', player[i].m_file - 1, player[i].m_rank + 1);
						valid.setAction('P', player[i], tmp);
						actionList.push_back(valid);
					}
					else { //Promotion Diagonal Left
						tmp.setPiece('Q', player[i].m_file - 1, player[i].m_rank + 1);
						valid.setAction('P', player[i], tmp);
						actionList.push_back(valid);
						tmp.setPiece('R', player[i].m_file - 1, player[i].m_rank + 1);
						valid.setAction('P', player[i], tmp);
						actionList.push_back(valid);
						tmp.setPiece('N', player[i].m_file - 1, player[i].m_rank + 1);
						valid.setAction('P', player[i], tmp);
						actionList.push_back(valid);
						tmp.setPiece('B', player[i].m_file - 1, player[i].m_rank + 1);
						valid.setAction('P', player[i], tmp);
						actionList.push_back(valid);
					}
				}
				else {
					if (enPassantCheck(board, enPassant, turn, rankIndex(player[i].m_rank + 1), fileIndex(player[i].m_file - 1))) {
						tmp.setPiece('P', player[i].m_file - 1, player[i].m_rank + 1);
						valid.setAction('P', player[i], tmp);
						actionList.push_back(valid);
					}
				}
			}
			//diagonal right only if there's opp
			if (emptyOrOpp(board, turn, player[i].m_rank + 1, player[i].m_file + 1, kRank, kFile, player[i], enPassant)) {
				if (islower(board[rankIndex(player[i].m_rank + 1)][fileIndex(player[i].m_file + 1)])) {
					if (player[i].m_rank != 7) {
						tmp.setPiece('P', player[i].m_file + 1, player[i].m_rank + 1);
						valid.setAction('P', player[i], tmp);
						actionList.push_back(valid);
					}
					else { //Promotion Diagonal Left
						tmp.setPiece('Q', player[i].m_file + 1, player[i].m_rank + 1);
						valid.setAction('P', player[i], tmp);
						actionList.push_back(valid);
						tmp.setPiece('R', player[i].m_file + 1, player[i].m_rank + 1);
						valid.setAction('P', player[i], tmp);
						actionList.push_back(valid);
						tmp.setPiece('N', player[i].m_file + 1, player[i].m_rank + 1);
						valid.setAction('P', player[i], tmp);
						actionList.push_back(valid);
						tmp.setPiece('B', player[i].m_file + 1, player[i].m_rank + 1);
						valid.setAction('P', player[i], tmp);
						actionList.push_back(valid);
					}
				}
				else {
					if (enPassantCheck(board, enPassant, turn, rankIndex(player[i].m_rank + 1), fileIndex(player[i].m_file + 1))) {
						tmp.setPiece('P', player[i].m_file + 1, player[i].m_rank + 1);
						valid.setAction('P', player[i], tmp);
						actionList.push_back(valid);
					}
				}
			}
			break;
		case('p'):
			if (player[i].m_rank == 7) {
				//then check if there's an object at position 3 & 4
				//if any color at +2 then pawn can only move 1
				//if any color at +1 then pawn can't move at all
				//if there's a black piece at right and left diagonal then valid
				//board[rank][file]
				//check jump 2
				if (board[2][fileIndex(player[i].m_file)] == '.') {
					if (emptyOrOpp(board, turn, 5, player[i].m_file, kRank, kFile, player[i], enPassant)) {
						tmp.setPiece('p', player[i].m_file, player[i].m_rank - 2);
						valid.setAction('p', player[i], tmp);
						actionList.push_back(valid);
					}
				}
			}
			//check jump 1
			//Moving Down
			if (emptyOrOpp(board, turn, player[i].m_rank - 1, player[i].m_file, kRank, kFile, player[i], enPassant)) {
				if (player[i].m_rank != 2) {
					tmp.setPiece('p', player[i].m_file, player[i].m_rank - 1);
					valid.setAction('p', player[i], tmp);
					actionList.push_back(valid);
				}
				else {
					tmp.setPiece('q', player[i].m_file, player[i].m_rank - 1);
					valid.setAction('p', player[i], tmp);
					actionList.push_back(valid);
					tmp.setPiece('r', player[i].m_file, player[i].m_rank - 1);
					valid.setAction('p', player[i], tmp);
					actionList.push_back(valid);
					tmp.setPiece('n', player[i].m_file, player[i].m_rank - 1);
					valid.setAction('p', player[i], tmp);
					actionList.push_back(valid);
					tmp.setPiece('b', player[i].m_file, player[i].m_rank - 1);
					valid.setAction('p', player[i], tmp);
					actionList.push_back(valid);
				}
			}
			//check if diagonal left
			if (emptyOrOpp(board, turn, player[i].m_rank - 1, player[i].m_file - 1, kRank, kFile, player[i], enPassant)) {
				if (isupper(board[rankIndex(player[i].m_rank - 1)][fileIndex(player[i].m_file - 1)])) {
					if (player[i].m_rank != 2) {
						tmp.setPiece('p', player[i].m_file - 1, player[i].m_rank - 1);
						valid.setAction('p', player[i], tmp);
						actionList.push_back(valid);
					}
					else { //Promotion Diagonal Left
						tmp.setPiece('q', player[i].m_file - 1, player[i].m_rank - 1);
						valid.setAction('p', player[i], tmp);
						actionList.push_back(valid);
						tmp.setPiece('r', player[i].m_file - 1, player[i].m_rank - 1);
						valid.setAction('p', player[i], tmp);
						actionList.push_back(valid);
						tmp.setPiece('n', player[i].m_file - 1, player[i].m_rank - 1);
						valid.setAction('p', player[i], tmp);
						actionList.push_back(valid);
						tmp.setPiece('b', player[i].m_file - 1, player[i].m_rank - 1);
						valid.setAction('p', player[i], tmp);
						actionList.push_back(valid);
					}
				}
				else {
					if (enPassantCheck(board, enPassant, turn, rankIndex(player[i].m_rank - 1), fileIndex(player[i].m_file - 1))) {
						tmp.setPiece('p', player[i].m_file - 1, player[i].m_rank - 1);
						valid.setAction('p', player[i], tmp);
						actionList.push_back(valid);
					}
				}
			}
			//diagonal right only if there's opp
			if (emptyOrOpp(board, turn, player[i].m_rank - 1, player[i].m_file + 1, kRank, kFile, player[i], enPassant)) {
				if (isupper(board[rankIndex(player[i].m_rank - 1)][fileIndex(player[i].m_file + 1)])) {
					if (player[i].m_rank != 2) {
						tmp.setPiece('p', player[i].m_file + 1, player[i].m_rank - 1);
						valid.setAction('p', player[i], tmp);
						actionList.push_back(valid);
					}
					else { //Promotion Diagonal Left
						tmp.setPiece('q', player[i].m_file + 1, player[i].m_rank - 1);
						valid.setAction('p', player[i], tmp);
						actionList.push_back(valid);
						tmp.setPiece('r', player[i].m_file + 1, player[i].m_rank - 1);
						valid.setAction('p', player[i], tmp);
						actionList.push_back(valid);
						tmp.setPiece('n', player[i].m_file + 1, player[i].m_rank - 1);
						valid.setAction('p', player[i], tmp);
						actionList.push_back(valid);
						tmp.setPiece('b', player[i].m_file + 1, player[i].m_rank - 1);
						valid.setAction('p', player[i], tmp);
						actionList.push_back(valid);
					}
				}
				else {
					if (enPassantCheck(board, enPassant, turn, rankIndex(player[i].m_rank - 1), fileIndex(player[i].m_file + 1))) {
						tmp.setPiece('p', player[i].m_file + 1, player[i].m_rank - 1);
						valid.setAction('p', player[i], tmp);
						actionList.push_back(valid);
					}
				}
			}
			break;
		case('R'): //cases for QUEEN, BISHOP AND ROOK
		case('B'):
		case('Q'):
		case('r'):
		case('b'):
		case('q'):
			piece = player[i].m_type;
			int pieceDirections;
			if (piece == 'Q' || piece == 'q') {
				pieceDirections = 8;
			}
			else {
				pieceDirections = 4;
			}
			for (int k = 0; k < pieceDirections; k++) {
				enemyEncounter = false;
				switch (k) {
				case 0://if queen set all these off
					   //starting right for rook
					if (piece == 'R' || piece == 'r') {
						fmodifier = 1;
						rmodifier = 0;
					}
					//starting top right for bishop
					else {
						fmodifier = 1;
						rmodifier = 1;
					}
					break;
				case 1:
					//starting left for rook
					if (piece == 'R' || piece == 'r') {
						fmodifier = -1;
						rmodifier = 0;
					}
					//starting top left for bishop
					else {
						fmodifier = -1;
						rmodifier = 1;
					}
					break;
				case 2:
					//starting up for rook
					if (piece == 'R' || piece == 'r') {
						rmodifier = 1;
						fmodifier = 0;
					}
					//starting bottom left for bishop
					else {
						fmodifier = -1;
						rmodifier = -1;
					}
					break;
				case 3:
					//starting down for rook
					if (piece == 'R' || piece == 'r') {
						rmodifier = -1;
						fmodifier = 0;
					}
					else {
						//starting bottom right for bishop
						fmodifier = 1;
						rmodifier = -1;
					}
					break;
				case 4: //Queen's Moveset::she already did the bishop's moves
					fmodifier = 1;
					rmodifier = 0;
					break;
				case 5:
					fmodifier = -1;
					rmodifier = 0;
					break;
				case 6:
					rmodifier = 1;
					fmodifier = 0;
					break;
				case 7:
					rmodifier = -1;
					fmodifier = 0;
					break;
				}

				while (emptyOrOpp(board, turn, player[i].m_rank + rmodifier, player[i].m_file + fmodifier, kRank, kFile, player[i], enPassant) && !enemyEncounter) {
					if (kingCheck(board, kRank, kFile, player[i], player[i].m_rank + rmodifier, player[i].m_file + fmodifier, turn)) {
						if (turn == "White") {
							if (piece == 'R') {
								tmp.setPiece('R', player[i].m_file + fmodifier, player[i].m_rank + rmodifier);
								valid.setAction('R', player[i], tmp);
							}
							if (player[i].m_type == 'Q') {
								tmp.setPiece('Q', player[i].m_file + fmodifier, player[i].m_rank + rmodifier);
								valid.setAction('Q', player[i], tmp);
							}
						}
						else {
							if (piece == 'r') {
								tmp.setPiece('r', player[i].m_file + fmodifier, player[i].m_rank + rmodifier);
								valid.setAction('r', player[i], tmp);
							}
							if (player[i].m_type == 'q') {
								tmp.setPiece('q', player[i].m_file + fmodifier, player[i].m_rank + rmodifier);
								valid.setAction('q', player[i], tmp);
							}
						}
						if (turn == "White") {
							if (piece == 'B') {
								tmp.setPiece('B', player[i].m_file + fmodifier, player[i].m_rank + rmodifier);
								valid.setAction('B', player[i], tmp);
							}
							if (player[i].m_type == 'Q') {
								tmp.setPiece('Q', player[i].m_file + fmodifier, player[i].m_rank + rmodifier);
								valid.setAction('Q', player[i], tmp);
							}
						}
						else {
							if (piece == 'b') {
								tmp.setPiece('b', player[i].m_file + fmodifier, player[i].m_rank + rmodifier);
								valid.setAction('b', player[i], tmp);
							}
							if (player[i].m_type == 'q') {
								tmp.setPiece('q', player[i].m_file + fmodifier, player[i].m_rank + rmodifier);
								valid.setAction('q', player[i], tmp);
							}
						}
						actionList.push_back(valid);
					}
				

					if (enemySpace(board, turn, rankIndex(player[i].m_rank + rmodifier), fileIndex(player[i].m_file + fmodifier))) {
						enemyEncounter = true;
					}
					switch (k) {
					case 0: //keep adding to file
						if (piece == 'R' || piece == 'r') {
							fmodifier++;
						}
						else {
							fmodifier++;
							rmodifier++;
						}
						break;
					case 1: //keep subtracting from file
						if (piece == 'R' || piece == 'r') {
							fmodifier--;
						}
						else {
							fmodifier--;
							rmodifier++;
						}
						break;
					case 2: //keep adding to rank
						if (piece == 'R' || piece == 'r') {
							rmodifier++;
						}
						else {
							rmodifier--;
							fmodifier--;
						}
						break;
					case 3: //keep sub from rank
						if (piece == 'R' || piece == 'r') {
							rmodifier--;
						}
						else {
							rmodifier--;
							fmodifier++;
						}
						break;
					case 4:
						fmodifier++;
						break;
					case 5:
						fmodifier--;
						break;
					case 6:
						rmodifier++;
						break;
					case 7:
						rmodifier--;
						break;
					}
				}
			}
			break;
		case ('N'): //KNIGHT
		case ('n'):
			//he can move 2 up 1 over right		[rank+2][file+1]
			//he can move 2 up 1 over left		[rank+2][file-1]
			//he can move 1 up and 2 left	    [rank+1][file-2]
			//he can move 1 up and 2 right		[rank+1][file+2]
			//he can move 1 down 2 over right	[rank-1][file+2]
			//he can move 1 down 2 over left	[rank-1][file-2]
			//he can move 2 down 1 over right	[rank-2][file+1]
			//he can move 2 down 1 over left	[rank-2][file-1]
			for (int k = -2; k <= 2; k++) {
				if (k != 0) {
					if (k % 2 == 0) {
						if (emptyOrOpp(board, turn, player[i].m_rank + k, player[i].m_file + 1, kRank, kFile, player[i], enPassant)) {
							if (turn == "White") {
								tmp.setPiece('N', player[i].m_file + 1, player[i].m_rank + k);
								valid.setAction('N', player[i], tmp);
							}
							else {
								tmp.setPiece('n', player[i].m_file + 1, player[i].m_rank + k);
								valid.setAction('n', player[i], tmp);
							}
							actionList.push_back(valid);
						}
						if (emptyOrOpp(board, turn, player[i].m_rank + k, player[i].m_file - 1, kRank, kFile, player[i], enPassant)) {
							if (turn == "White") {
								tmp.setPiece('N', player[i].m_file - 1, player[i].m_rank + k);
								valid.setAction('N', player[i], tmp);
							}
							else {
								tmp.setPiece('n', player[i].m_file - 1, player[i].m_rank + k);
								valid.setAction('n', player[i], tmp);
							}
							actionList.push_back(valid);
						}
					}
					else {
						if (emptyOrOpp(board, turn, player[i].m_rank + k, player[i].m_file + 2, kRank, kFile, player[i], enPassant)) {
							if (turn == "White") {
								tmp.setPiece('N', player[i].m_file + 2, player[i].m_rank + k);
								valid.setAction('N', player[i], tmp);
							}
							else {
								tmp.setPiece('n', player[i].m_file + 2, player[i].m_rank + k);
								valid.setAction('n', player[i], tmp);
							}
							actionList.push_back(valid);
						}
						if (emptyOrOpp(board, turn, player[i].m_rank + k, player[i].m_file - 2, kRank, kFile, player[i], enPassant)) {
							if (turn == "White") {
								tmp.setPiece('N', player[i].m_file - 2, player[i].m_rank + k);
								valid.setAction('N', player[i], tmp);
							}
							else {
								tmp.setPiece('n', player[i].m_file - 2, player[i].m_rank + k);
								valid.setAction('n', player[i], tmp);
							}
							actionList.push_back(valid);
						}
					}
				}
			}
			break;
		case ('K'):
		case ('k'):
			//check for the new spot
			//remove the old spot
			//use that to check

			//from opp vector 
			for (int k = -1; k <= 1; k++) {
				for (int j = -1; j <= 1; j++) {
					if (!(k == 0 && j == 0)) {
						if (emptyOrOpp(board, turn, player[i].m_rank + k, player[i].m_file + j, kRank, kFile, player[i], enPassant)) {
							if (turn == "White") {
								tmp.setPiece('K', player[i].m_file + j, player[i].m_rank + k);
								valid.setAction('K', player[i], tmp);
							}
							else {
								tmp.setPiece('k', player[i].m_file + j, player[i].m_rank + k);
								valid.setAction('k', player[i], tmp);
							}
							actionList.push_back(valid);
						}
					}
				}
			}

			//Castling
			//moves 2 steps both directions
			oneCastle = false;
			int rank, boardRow, empBoardF1, empBoardF2;
			char through, into, kingC;
			third = false;
			for (int k = 0; k < 4; k++) {
				if (k == 0 && castleK) {
					oneCastle = true;
					rank = 1;
					boardRow = 7;
					empBoardF1 = 5;
					empBoardF2 = 6;
					through = 'f';
					into = 'g';
					kingC = 'K';
					third = true;
				}
				else {
					if (k == 0) {
						oneCastle = false;
					}
				}
				if (k == 1 && castleQ) {
					oneCastle = true;
					rank = 1;
					boardRow = 7;
					empBoardF1 = 3;
					empBoardF2 = 2;
					through = 'd';
					into = 'c';
					kingC = 'K';
					if (board[7][1] == '.') {
						third = true;
					}
					else {
						third = false;
					}
				}
				else {
					if (k == 1) {
						oneCastle = false;
					}
				}
				if (k == 2 && castlek) {
					oneCastle = true;
					rank = 8;
					boardRow = 0;
					empBoardF1 = 5;
					empBoardF2 = 6;
					through = 'f';
					into = 'g';
					kingC = 'k';
					third = true;
				}
				else {
					if (k == 2) {
						oneCastle = false;
					}
				}
				if (k == 3 && castleq) {
					oneCastle = true;
					rank = 8;
					boardRow = 0;
					empBoardF1 = 3;
					empBoardF2 = 2;
					through = 'd';
					into = 'c';
					kingC = 'k';
					if (board[0][1] == '.') {
						third = true;
					}
					else {
						third = false;
					}
				}
				else {
					if (k == 3) {
						oneCastle = false;
					}
				}
				if (oneCastle) {
					if (board[boardRow][empBoardF1] == '.' && board[boardRow][empBoardF2] == '.' && third) {
						//This is done in cascade of ifs to prevent loading all ways space can be checked
						//Since this iterates through all reverse actions for the king of how he can be attacked
						if (k == 3) {
						}
						if (!checkSpot(rank, 'e', board, turn)) { //king check
							if (k == 3) {
							}
							if (!checkSpot(rank, through, board, turn)) { //through check
								if (k == 3) {
								}
								if (!checkSpot(rank, into, board, turn)) { //jump to check
									if (k == 3) {
									}
									//Represented by king moving to the right 2
									tmp.setPiece(kingC, into, rank);
									valid.setAction(kingC, player[i], tmp);
									actionList.push_back(valid);
								}
							}
						}
					}
				}
			}
			break;
		default:
			cout << "Error: Unspecified Type being called to valid actions." << endl;
			break;
		}
	}
	return;
}

Action convertAction(string color, string type, string oldFile, int oldRank, string newFile, int newRank) {
	Action newAct;
	Mypiece oldPiece, newPiece;
	char convertType;
	if (color == "White") {
		if (type == "Pawn") {
			convertType = 'P';
		}
		else if (type == "Bishop") {
			convertType = 'B';
		}
		else if (type == "Rook") {
			convertType = 'R';
		}
		else if (type == "Knight") {
			convertType = 'N';
		}
		else if (type == "Queen") {
			convertType = 'Q';
		}
		else if (type == "King"){
			convertType = 'K';
		}
	}
	else {
		if (type == "Pawn") {
			convertType = 'p';
		}
		else if (type == "Bishop") {
			convertType = 'b';
		}
		else if (type == "Rook") {
			convertType = 'r';
		}
		else if (type == "Knight") {
			convertType = 'n';
		}
		else if (type == "Queen") {
			convertType = 'q';
		}
		else if (type == "King") {
			convertType = 'k';
		}
	}

	//converting files to char
	oldPiece.setPiece(convertType, oldFile[0], oldRank);
	newPiece.setPiece(convertType, newFile[0], newRank);
	newAct.setAction(convertType, oldPiece, newPiece);
	return newAct;
}

string Mypiece::convertType() {
	string name = "None";
	switch (m_type) {
	case 'P':
	case 'p':
		name = "Pawn";
		break;
	case 'B':
	case 'b':
		name = "Bishop";
		break;
	case 'Q':
	case 'q':
		name = "Queen";
		break;
	case 'K':
	case 'k':
		name = "King";
		break;
	case 'N':
	case 'n':
		name = "Knight";
		break;
	case 'R':
	case 'r':
		name = "Rook";
		break;
	}
	return name;
}


void displayAction(Action A) {
	cout << A.original.m_type << ":" << A.original.m_file << A.original.m_rank << "-> "
		<< A.move.m_type << ":" << A.move.m_file << A.move.m_rank;
	return;
}

void displayState(const State S) {
	cout << "---------------" << endl;
	cout << "Board: " << endl;
	char board[8][8];
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			board[i][j] = '.';
		}
	}
	//Displaying Board
	for (int i = 0; i < S.white.size(); i++) {
		//file = player[i].m_file - 97;
		board[8 - S.white[i].m_rank][S.white[i].m_file - 97] = S.white[i].m_type;
	}
	for (int i = 0; i < S.black.size(); i++) {
		//file = black[i].m_file - 97;
		board[8 - S.black[i].m_rank][S.black[i].m_file - 97] = S.black[i].m_type;
	}
	for (int i = 0; i < 8; i++) {
		cout << 8 - i << " ";
		for (int j = 0; j < 8; j++) {
			cout << board[i][j] << " ";
		}
		cout << endl;
	}
	cout << "  a b c d e f g h" << endl;
	/*for (int i = 0; i < white.size(); i++) {
		cout << white[i].m_type << ": " << white[i].m_file << white[i].m_rank << endl;
	}
	for (int i = 0; i < white.size(); i++) {
		cout << black[i].m_type << ": " << black[i].m_file << black[i].m_rank << endl;
	}*/
	cout << "Turn: " << S.turn << endl;
	cout << "En Passant: " << S.enPassant << endl;
	cout << "Action Applied: " << S.actionApplied.original.m_type << ": " << S.actionApplied.original.m_file << S.actionApplied.original.m_rank << "->";
	cout << S.actionApplied.move.m_file << S.actionApplied.move.m_rank << endl;
	cout << "Castling (KQkq): " << S.castleK << S.castleQ << S.castlek << S.castleq << endl;

	cout << "Max: " << S.max << endl;
	cout << "King position: " << S.kFile << S.kRank << endl;
	cout << "Fifty Move Rule (0->100): " << S.fiftyMoveRule << endl;
	if (S.last8Moves.size() == 8) {
		cout << "Last 8 Moves: " << endl;
		for (int i = 0; i < 8; i++){
			cout << i << ". ";
			displayAction(S.last8Moves[i]);
			cout << ", ";
		}
		cout << endl;
	}
	cout << "---------------" << endl;
	return;
}

State result(const State S, const Action A) {
	State result;
	result = S;
	//Adding the action to the last 8 actions
	
	if (result.last8Moves.size() >= 8) {

		result.last8Moves.pop_back();
		result.last8Moves.insert(result.last8Moves.begin(), A);
	}

	//This is reset later if capture or pawn moves
	result.fiftyMoveRule++;

	switch (A.original.m_type) {
	case 'K':
		//White King
		result.castleK = false;
		result.castleQ = false;
		break;
	case 'k':
		//Black King
		result.castlek = false;
		result.castleq = false;
		break;
	case 'R':
		if (A.original.m_file == 'a' && A.original.m_rank == 1) {
			result.castleQ = false;
		}
		if (A.original.m_file == 'h' && A.original.m_rank == 1) {
			result.castleK = false;
		}
		break;
	case 'r':
		if (A.original.m_file == 'a' && A.original.m_rank == 8) {
			result.castleq = false;
		}
		if (A.original.m_file == 'h' && A.original.m_rank == 8) {
			result.castlek = false;
		}
		break;
	}

	bool enPassant = false;
	result.enPassant.clear();
	stringstream ss;
	if (A.original.m_type == 'P' && A.original.m_rank == 2 && A.move.m_rank == 4) {
		ss << A.move.m_file;
		ss >> result.enPassant;
		result.enPassant = result.enPassant + "3";
		enPassant = true;
		result.fiftyMoveRule = 0;
	}
	if (A.original.m_type == 'p' && A.original.m_rank == 7 && A.move.m_rank == 5) {
		ss << A.move.m_file;
		ss >> result.enPassant;
		result.enPassant = result.enPassant + "6";
		enPassant = true;
		result.fiftyMoveRule = 0;
	}
	if (!enPassant) {
		enPassant = "-";
	}

	if (A.original.m_type != A.move.m_type) {
		//Promotion
		if (S.turn == "White") {
			for (int i = 0; i < result.white.size(); i++) {
				if (result.white[i].m_type == 'P' && result.white[i].m_file == A.original.m_file &&
					result.white[i].m_rank == A.original.m_rank) {
					result.white[i].m_type = A.move.m_type;
					result.fiftyMoveRule = 0;
				}
			}
		}
		if (S.turn == "Black") {
			for (int i = 0; i < result.black.size(); i++) {
				if (result.black[i].m_type == 'p' && result.black[i].m_file == A.original.m_file &&
					result.black[i].m_rank == A.original.m_rank) {
					result.black[i].m_type = A.move.m_type;
					result.fiftyMoveRule = 0;
				}
			}
		}
	}

	//Moving
	int removeIndex;
	bool found = false;
	if (S.turn == "White") {
		//First check if there is an opp on the square the object is moving to
		for (int i = 0; i < result.black.size(); i++) {
			if (result.black[i].m_file == A.move.m_file && result.black[i].m_rank == A.move.m_rank) {
				removeIndex = i;
				found = true;
				break;
			}
		}
		if (found){ //If there is an opp delete
		 result.black.erase(result.black.begin() + removeIndex);
		 result.fiftyMoveRule = 0;
		}
		//move the piece with the original rank and file to the new one
		for (int i = 0; i < result.white.size(); i++) {
			if (result.white[i].m_file == A.original.m_file && result.white[i].m_rank == A.original.m_rank) {
				result.white[i].m_file = A.move.m_file;
				result.white[i].m_rank = A.move.m_rank;
				break;
			}
		}
	}
	else { //black
		for (int i = 0; i < result.white.size(); i++) {
			if (result.white[i].m_file == A.move.m_file && result.white[i].m_rank == A.move.m_rank) {
				removeIndex = i;
				found = true;
				break;
			}
		}
		if (found){
			result.white.erase(result.white.begin() + removeIndex);
			result.fiftyMoveRule = 0;
		}
		for (int i = 0; i < result.black.size(); i++) {
			if (result.black[i].m_file == A.original.m_file && result.black[i].m_rank == A.original.m_rank) {
				result.black[i].m_file = A.move.m_file;
				result.black[i].m_rank = A.move.m_rank;
				break;
			}
		}
	}

	//replace kRank & kFile
	if (S.turn == "White"){
		for (int j = 0; j < result.black.size(); j++) {
			if (result.black[j].m_type == 'k') {
				result.kRank = result.black[j].m_rank;
				result.kFile = result.black[j].m_file;
				break;
			}
		}
	}
	else {
		for (int j = 0; j < result.white.size(); j++) {
			if (result.white[j].m_type == 'K') {
				result.kRank = result.white[j].m_rank;
				result.kFile = result.white[j].m_file;
				break;
			}
		}
	}

	S.turn == "White" ? result.turn = "Black" : result.turn = "White";
	result.actionApplied = A;
	return result;
}


int evaluate(State S) {
	int eval = 0;
	if (S.max == "White") {
		//Count white pieces
		for (int i = 0; i < S.white.size();i++) {
			switch (S.white[i].m_type) {
			case 'P':
				eval++;
				break;
			case 'N' :
			case 'B':
				eval = eval + 3;
				break;
			case 'R' :
				eval = eval + 5;
				break;
			case 'Q' :
				eval = eval + 9;
				break;
			}
		}
		//subtract black pieces
		for (int i = 0; i < S.black.size();i++) {
			switch (S.black[i].m_type) {
			case 'p':
				eval--;
				break;
			case 'n':
			case 'b':
				eval = eval - 3;
				break;
			case 'r':
				eval = eval - 5;
				break;
			case 'q':
				eval = eval - 9;
				break;
			}
		}
	}
	else {
		//count Black pieces
		for (int i = 0; i < S.black.size();i++) {
			switch (S.black[i].m_type) {
			case 'p':
				eval++;
				break;
			case 'n' :
			case 'b' :
				eval = eval + 3;
				break;
			case 'r' :
				eval = eval + 5;
				break;
			case 'q' :
				eval = eval + 9;
				break;
			}
		}
		for (int i = 0; i < S.white.size();i++) {
			switch (S.white[i].m_type) {
			case 'P':
				eval--;
				break;
			case 'N':
			case 'B':
				eval = eval - 3;
				break;
			case 'R':
				eval = eval - 5;
				break;
			case 'Q':
				eval = eval - 9;
				break;
			}
		}
	}
	return eval;
}

//if white true
//else black false
//determines the color of the square a piece is on
bool color(char file, int rank) {
	bool colorSpace = false;
	int newFile = fileIndex(file);
	newFile = newFile + 1; //starts at 0

	if ((rank + newFile) % 2 == 0) {
		colorSpace = false;
	}
	else {
		colorSpace = true;
	}
	return colorSpace;
}

bool State::draw() {
	bool draw = false;
	//Simplified three fold repetition

	if (last8Moves.size() == 8) {
		if (last8Moves[0] == last8Moves[4] && last8Moves[1] == last8Moves[5]
			&& last8Moves[2] == last8Moves[6] && last8Moves[3] == last8Moves[7]){
			draw = true;
		}
	}

	//Stalemate (if player at move has no move and not in check)
	//he would have no valid action -- Precondition before this call was made.

	//50 move rule
	if (fiftyMoveRule == 100) {
		draw = true;
	}
	//Insufficient Material
	//check if black and white pieces are <= 2
	if (black.size() <= 2 && white.size() <= 2) {
		//King vs King
		if (black.size() == 1 && white.size() == 1) {
			draw = true;
		}
		int wIndex, bIndex;
		if (turn == "White") {
			if (white.size() == 1 && black.size() == 2) {
				//check if king vs king and bishop
				if (black[0].m_type == 'b' || black[1].m_type == 'b') {
					draw = true;
				}
				//check if king vs king and Knight
				if (black[0].m_type == 'n' || black[1].m_type == 'n') {
					draw = true;
				}
			}
			//color of bishop is the same
			if (white.size() == 2 && black.size() == 2) {
				wIndex = 0;
				bIndex = 0;
				for (int i = 0; i < white.size(); i++) {
					if (white[i].m_type == 'B') {
						wIndex = i;
					}
				}
				for (int i = 0; i < black.size(); i++) {
					if (black[i].m_type == 'b') {
						bIndex = i;
					}
				}
				if (color(white[wIndex].m_file, white[wIndex].m_rank) ==
					color(black[bIndex].m_file, black[bIndex].m_rank)) {
					draw = true;
				}
			}
		}
		else {
			if (black.size() == 1 && white.size() == 2) {
				//check if king vs king and bishop
				if (white[0].m_type == 'b' || white[1].m_type == 'b') {
					draw = true;
				}
				//check if king vs king and Knight
				if (white[0].m_type == 'n' || white[1].m_type == 'n') {
					draw = true;
				}
			}
			//color of bishop is the same
			if (black.size() == 2 && white.size() == 2) {
				wIndex = 0;
				bIndex = 0;
				for (int i = 0; i < black.size(); i++) {
					if (black[i].m_type == 'b') {
						bIndex = i;
					}
				}
				for (int i = 0; i < white.size(); i++) {
					if (white[i].m_type == 'B') {
						wIndex = i;
					}
				}
				if (color(black[bIndex].m_file, black[bIndex].m_rank) ==
					color(white[wIndex].m_file, white[wIndex].m_rank)) {
					draw = true;
				}
			}
		}
	}
	return draw;
}


Action ABDLM(State S, int counter, int& alpha, int& beta) {
	Action bestAction;
	bestAction = S.actionApplied;
	if (counter == 0) { //Base Case
		bestAction.utilityVal = evaluate(S);
		if (S.max == S.turn) { //Max player
			alpha = bestAction.utilityVal;
		}
		else { //Min player
			beta = bestAction.utilityVal;
		}
	}
	else {
		//Generating the frontier for this state.
		vector<Action> actionList;
		S.actions(actionList);
		//calculating if player can win
		//determine the other players king position
		//if one of the actions goes to the king position then current player wins.
		char oppkFile;
		int oppkRank;
		//if actionlist is empty or there's a draw
		if (!(S.draw() || actionList.empty())){ //if state is a draw or win don't recurse
			Action frontier;
			Action eval;
			if (S.turn == S.max) {
				//if max player
				//Return the max of all possible actions
				for (int i = 0; i < actionList.size(); i++) {
					frontier = actionList[i];
					int alphaState = alpha;
					int betaState = beta;
					eval = ABDLM(result(S, actionList[i]), counter - 1, alpha, beta);
					//once the evaluated state returns: return its value to the frontier state.
					//But DON'T COPY over the returned action to the frontier actions
					frontier.utilityVal = eval.utilityVal;
				//	cout << "Max got frontier value" << endl;
					alpha = alphaState;
					beta = betaState;
					if (bestAction.utilityVal == -9999 || (frontier.utilityVal < beta && frontier.utilityVal > alpha)) {
						//best action is set to the first thing in the frontier.
						//if the state value is greater than the best action then replace.
						bestAction = frontier;
						alpha = frontier.utilityVal;
					}
					else {
						//not the first eval state and not within bounds
						//Check if it prunes
						//Fail High
						if (frontier.utilityVal >= beta) {
							bestAction = frontier;
							//exit frontier
							//cout << "MAX PRUNE" << endl;
							break;
						}
						else {
							//don't update alpha or beta but choose the max action
							if (bestAction.utilityVal < frontier.utilityVal) {
							//	cout << "FL" << endl;
								bestAction = frontier;
							}
							//else nothing needs to be done
						}
					}
				}
			}
			else {
				//obtain the min for all actions
				for (int i = 0; i < actionList.size(); i++) {
					frontier = actionList[i];
					eval = ABDLM(result(S, actionList[i]), counter - 1, alpha, beta);
					frontier.utilityVal = eval.utilityVal;
					//cout << "Min got frontier value" << endl;
					if (bestAction.utilityVal == -9999 || (frontier.utilityVal < beta && frontier.utilityVal > alpha)) {
						//If the best action's value is greater than the frontier's value
						//then replace the bestAction with the state of the frontier.
						bestAction.utilityVal = frontier.utilityVal;
						beta = frontier.utilityVal;
					}
					else {
						//not the first eval state and not within bounds
						//Check if it prunes
						//Fail Low
						if (frontier.utilityVal <= alpha) {
							bestAction.utilityVal = frontier.utilityVal;
							break;
						}
						else {
							//don't update alpha or beta but choose the max action
							if (bestAction.utilityVal > frontier.utilityVal) {
							//cout << "FH" << endl;
								bestAction.utilityVal = frontier.utilityVal;
							}
							//else nothing needs to be done
						}
					}
				}
			}
		}
		else{
		//Returns:
		//handles cases where there are no actions
		//when the actionlist is empty someone lost.
		//this is used to find nodes that don't have an action afterwards
			bestAction.utilityVal = evaluate(S);
			if (actionList.empty()){
				//--differentiating stalemate (no actions)
				char board[8][8];
				for (int i = 0; i < 8; i++) {
					for (int j = 0; j < 8; j++) {
						board[i][j] = '.';
					}
				}
				for (int i = 0; i < S.white.size(); i++) {
					board[8 - S.white[i].m_rank][S.white[i].m_file - 97] = S.white[i].m_type;
				}
				for (int i = 0; i < S.black.size(); i++) {
					board[8 - S.black[i].m_rank][S.black[i].m_file - 97] = S.black[i].m_type;
				}
				if (checkSpot(S.kRank, S.kFile, board, S.turn)){
					if (S.max == S.turn) { //loss
						bestAction.utilityVal = bestAction.utilityVal - 15;
					}
					else { //win
						bestAction.utilityVal = bestAction.utilityVal + 10;
					}
				}
				else {
					//Stalemate Draw
					bestAction.utilityVal = bestAction.utilityVal - 11;
				}
			}
			else{ //It would be a draw
			//	cout << "There's a draw" << endl;
				bestAction.utilityVal = bestAction.utilityVal - 11;
			}

			//updating alpha and beta for the return test case
			if (S.max == S.turn) { //Max player
				alpha = bestAction.utilityVal;
			}
			else { //Min player
				beta = bestAction.utilityVal;
			}
		}
	}
	return bestAction;
}


//finalDepth has to be >= 1 to obtain a real action
//wantedTime is in ns
Action IDM(State S, int finalDepth, const double wantedTime) {
	int counter = 0;
	bool terminal = false;
	Action act;
	S.max = S.turn;
	int alpha, beta;
	clock_t begin = clock();
	while (counter+1 <= finalDepth){
		counter++;
		alpha = -10000;
		beta = 10000;
		act = ABDLM(S, counter, alpha, beta);
		clock_t end = clock();
		double time_spent = double(end - begin) / CLOCKS_PER_SEC;
		time_spent = time_spent * 1000000000; //conversion from sec to ns to microsec
		if (time_spent >= wantedTime) {
			cout << "Broke out of IDM @ " << counter << endl;
			break;
		}
	}
	return act;
}
