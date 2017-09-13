//Christopher Torralba

void Mypiece :: setPiece(const char type, const char file, const int rank){
	m_type = type;
	m_file = file;
	m_rank = rank;
	return;
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

//Also checks if out of bounds
bool emptyOrOpp(const char board[][8], const string opp, const int rankIndex, const int fileIndex) {
	bool valid = false;
	if (rankIndex < 0 || rankIndex > 7 || fileIndex < 0 || fileIndex > 7) {
		return false;
	}
  if (opp == "White"){
	  if (board[rankIndex][fileIndex] == '.' || islower(board[rankIndex][fileIndex])) {
		  valid = true;
	}
  }
  else { //black
	  if (board[rankIndex][fileIndex] == '.' || isupper(board[rankIndex][fileIndex])) {
		  valid = true;
	}
  }
  //true if valid
  return valid;
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

void Action :: setAction(const char type, const Mypiece start, const Mypiece end) {
	m_type = type;
	original = start;
	move = end;
	return;
}


void /*vector<Action>*/ State:: actions() { //ACTIONS FUNCTION FOR STATE
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
		//cout << 8-white[i].m_rank << white[i].m_file-97 << " = " << white[i].m_type << endl;
		board[8-white[i].m_rank][white[i].m_file-97] = white[i].m_type;
	}
	for (int i = 0; i < black.size(); i++) {
		//file = black[i].m_file - 97;
		//cout << black[i].m_rank << black[i].m_file-97 << " = " << black[i].m_type << endl;
		board[8-black[i].m_rank][black[i].m_file-97] = black[i].m_type;
	}

	//00 01 02 03 04
	//10 11 12 13 14
	//20 21 22 23 24
	//30 31 32 33
	//7
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			cout << board[i][j];
		}
		cout << endl;
	}
	vector<Action> actionList;
	Action valid;
	Mypiece tmp;
	char piece;
	bool enPassant = false;
	//Used for Rook, Bishop and Queen
	bool enemyEncounter = false;
	int fmodifier = 0; //left and right
	int rmodifier = 0; //up and down
	//

	//setting the vector to check
	int turnSize = 0;
	vector<Mypiece> player;
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
				if (emptyOrOpp(board, turn, 4, fileIndex(player[i].m_file))){
					tmp.setPiece('P', player[i].m_file, player[i].m_rank + 2);
					valid.setAction('P', player[i], tmp);
					actionList.push_back(valid);
					enPassant = true;
				}
			}
			//check jump 1
			//Moving Up
			if (emptyOrOpp(board, turn, rankIndex(player[i].m_rank + 1), fileIndex(player[i].m_file))){
				if (player[i].m_rank != 7){
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
			if (player[i].m_file != 'a') { //this only checks if there is an enemy on diagonal
				if (islower(board[rankIndex(player[i].m_rank+1)][fileIndex(player[i].m_file-1)])) {
					if (player[i].m_rank != 7) {
						tmp.setPiece('P', player[i].m_file - 1, player[i].m_rank + 1);
						valid.setAction('P', player[i], tmp);
						actionList.push_back(valid);
					}
					else { //Promotion Diagonal Left
						tmp.setPiece('Q', player[i].m_file-1, player[i].m_rank + 1);
						valid.setAction('P', player[i], tmp);
						actionList.push_back(valid);
						tmp.setPiece('R', player[i].m_file-1, player[i].m_rank + 1);
						valid.setAction('P', player[i], tmp);
						actionList.push_back(valid);
						tmp.setPiece('N', player[i].m_file-1, player[i].m_rank + 1);
						valid.setAction('P', player[i], tmp);
						actionList.push_back(valid);
						tmp.setPiece('B', player[i].m_file-1, player[i].m_rank + 1);
						valid.setAction('P', player[i], tmp);
						actionList.push_back(valid);
					}
				}
			}
			//diagonal right only if there's opp
			if (player[i].m_file != 'h') {
				if (islower(board[rankIndex(player[i].m_rank+1)][fileIndex(player[i].m_file+1)])) {
					if (player[i].m_rank != 7) {
						tmp.setPiece('P', player[i].m_file + 1, player[i].m_rank + 1);
						valid.setAction('P', player[i], tmp);
						actionList.push_back(valid);
					}
					else {//Promtion diagonal right
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
			}
			//if (player[i].m_rank == 5 && enpassant) {
			//}
			break;
		case('p'):
			if (player[i].m_rank == 7) {
				//then check if there's an object at position 3 & 4
				//if any color at +2 then pawn can only move 1
				//if any color at +1 then pawn can't move at all
				//if there's a black piece at right and left diagonal then valid
				//board[rank][file]
				//check jump 2
				if (emptyOrOpp(board, turn, 5, fileIndex(player[i].m_file))) {
					tmp.setPiece('p', player[i].m_file, player[i].m_rank + 2);
					valid.setAction('p', player[i], tmp);
					actionList.push_back(valid);
					enPassant = true;
				}
			}
			//check jump 1
			//Moving Up
			if (emptyOrOpp(board, turn, rankIndex(player[i].m_rank - 1), fileIndex(player[i].m_file))) {
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
			if (player[i].m_file != 'a') { //this only checks if there is an enemy on diagonal
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
			}
			//diagonal right only if there's opp
			if (player[i].m_file != 'h') {
				if (isupper(board[rankIndex(player[i].m_rank - 1)][fileIndex(player[i].m_file + 1)])) {
					if (player[i].m_rank != 2) {
						tmp.setPiece('p', player[i].m_file + 1, player[i].m_rank - 1);
						valid.setAction('p', player[i], tmp);
						actionList.push_back(valid);
					}
					else {//Promtion diagonal right
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
			}
			//if (player[i].m_rank == 4 && enpassant) {
			//}
			break;
		case('R'): //cases for QUEEN, BISHOP AND ROOK
		case('B'):
		case('Q'):
		case('r'):
		case('b'):
		case('q'):
			piece = player[i].m_type;
			for (int k = 0; k < 4; k++) {
				enemyEncounter = false;
				switch (k) {
				case 0://if queen set all these off
					//starting right for rook
					if (piece == 'R' || piece == 'r'){
						fmodifier = 1;
						rmodifier = 0;
					}
					//starting top right for bishop
					else{
						fmodifier = 1;
						rmodifier = 0;
					}
					break;
				case 1:
					//starting left for rook
					if (piece == 'R' || piece == 'r'){
						fmodifier = -1;
						rmodifier = 0;
					}
					//starting top left for bishop
					else{
						fmodifier = 1;
						rmodifier = 0;
					}
					break;
				case 2:
					//starting up for rook
					if (piece == 'R' || piece == 'r'){
						rmodifier = 1;
						fmodifier = 0;
					}
					//starting bottom left for bishop
					else{
						fmodifier = 1;
						rmodifier = 0;
					}
					break;
				case 3:
					//starting down for rook
					if (piece == 'R' || piece == 'r'){
						rmodifier = -1;
						fmodifier = 0;
					}
					else{
					//starting bottom right for bishop
						fmodifier = 1;
						rmodifier = 0;
					}
					break;
				}
				while (emptyOrOpp(board, turn, rankIndex(player[i].m_rank + rmodifier), fileIndex(player[i].m_file + fmodifier)) && !enemyEncounter) {
						if (turn == "White"){
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
							if (piece == 'r'){
								tmp.setPiece('r', player[i].m_file + fmodifier, player[i].m_rank + rmodifier);
								valid.setAction('r', player[i], tmp);
							}
							if (player[i].m_type == 'q') {
								tmp.setPiece('q', player[i].m_file + fmodifier, player[i].m_rank + rmodifier);
								valid.setAction('q', player[i], tmp);
							}
						}
						actionList.push_back(valid);
						if (turn == "White"){
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
							if (piece == 'b'){
								tmp.setPiece('b', player[i].m_file + fmodifier, player[i].m_rank + rmodifier);
								valid.setAction('b', player[i], tmp);
							}
							if (player[i].m_type == 'q') {
								tmp.setPiece('q', player[i].m_file + fmodifier, player[i].m_rank + rmodifier);
								valid.setAction('q', player[i], tmp);
							}
						}
						actionList.push_back(valid);
					if (enemySpace(board, turn, rankIndex(player[i].m_rank + rmodifier), fileIndex(player[i].m_file + fmodifier))) {
						enemyEncounter = true;
					}
					switch (k) {
					case 0: //keep adding to file
						if (piece == 'R' || piece == 'r'){
							fmodifier++;
						}
						else {
							fmodifier++;
							rmodifier++;
						}
						break;
					case 1: //keep subtracting from file
						if (piece == 'R' || piece == 'r'){
							fmodifier--;
						}
						else {
							fmodifier--;
							rmodifier++;
						}
						break;
					case 2: //keep adding to rank
						if (piece == 'R' || piece == 'r'){
							rmodifier++;
						}
						else {
							rmodifier--;
							fmodifier--;
						}
						break;
					case 3: //keep sub from rank
						if (piece == 'R' || piece == 'r'){
							rmodifier--;
						}
						else {
							rmodifier--;
							fmodifier++;
						}
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
						if (emptyOrOpp(board, turn, rankIndex(player[i].m_rank + k), fileIndex(player[i].m_file + 1))) {
							if (turn == "White"){
								tmp.setPiece('N', player[i].m_file + 1, player[i].m_rank + k);
								valid.setAction('N', player[i], tmp);
							}
							else {
								tmp.setPiece('n', player[i].m_file + 1, player[i].m_rank + k);
								valid.setAction('n', player[i], tmp);
							}
							actionList.push_back(valid);
						}
						if (emptyOrOpp(board, turn, rankIndex(player[i].m_rank + k), fileIndex(player[i].m_file - 1))) {
							if (turn == "White"){
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
						if (emptyOrOpp(board, turn, rankIndex(player[i].m_rank + k), fileIndex(player[i].m_file + 2))) {
							if (turn == "White"){
								tmp.setPiece('N', player[i].m_file + 2, player[i].m_rank + k);
								valid.setAction('N', player[i], tmp);
							}
							else {
								tmp.setPiece('n', player[i].m_file + 2, player[i].m_rank + k);
								valid.setAction('n', player[i], tmp);
							}
							actionList.push_back(valid);
						}
						if (emptyOrOpp(board, turn, rankIndex(player[i].m_rank + k), fileIndex(player[i].m_file - 2))) {
							if (turn == "White"){
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
			for (int k = -1; k <= 1; k++){
				for (int j = -1; j <= 1; j++){
					if (emptyOrOpp(board, turn, rankIndex(player[i].m_rank+k), fileIndex(player[i].m_file+j))){
						if (turn == "White"){
							tmp.setPiece('K', player[i].m_file+j, player[i].m_rank+k);
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
			break;
		default:
			cout << "Error: Unspecified Type being called to valid actions." << endl;
		break;
		}
	}
	for (int i = 0; i < actionList.size(); i++) {
		cout << actionList[i].original.m_type << "-" << actionList[i].original.m_file << actionList[i].original.m_rank << "->"
			<< actionList[i].move.m_type << "-" << actionList[i].move.m_file << actionList[i].move.m_rank << endl;
	}
	return;
}