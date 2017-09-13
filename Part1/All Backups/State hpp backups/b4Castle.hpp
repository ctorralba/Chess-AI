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

bool enPassantCheck(const char board[][8], string enPassant, const string opp, const int rank, const int file) {
	bool isEnpassant = false;

	if (rank < 0 || rank > 7 || file < 0 || file > 7) {
		return false;
	}	
	int enPassFile = fileIndex(enPassant[0]);
	int enPassRank = rankIndex(0 + (int)enPassant[1]-48);

	//cout << enPassFile << "," << enPassRank << endl;
	//cout << "file: " << file << ", rank: " << rank << endl;
	if (opp == "White"){
		if (file == enPassFile && rank == enPassRank) {
		//	cout << "Checking board[" << rank + 1 << "][" << file << "]" << endl;
			if (islower(board[rank+1][file])){
				isEnpassant = true;
			}
		}
	}
	else {//black
		if (file == enPassFile && rank == enPassRank) {
			if (isupper(board[rank-1][file])) {
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
bool checkSpot(const int rank, const char file, const char board[][8], const string turn) {
	bool check = false;
	bool stop = false; //for out of bounds
	//file rank (c3)
	//check if black rook || queen to the right top left down
	int rmod, fmod;
	//cout << "b4 Any: " << check << endl;
	//cout << "File: " << file << ", Rank: " << rank << endl;
	rmod = rank;
	fmod = file;
	//cout << "SRmod: " << rmod << ", SFmod: " << fmod << endl;
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
	//	cout << "For i: " << i << ", rmod = " << rmod << ", " << "fmod = " << fmod << endl;
		if (turn == "White") {
		//	cout << "emptyBound = " << emptyBound(board, rmod, fmod) << endl;
		//	cout << "For: " << static_cast<char>(fmod) << "," << rmod << endl;
			if (emptyBound(board, rmod, fmod) != 'w') {
				if (i < 4){
					if (emptyBound(board, rmod, fmod) == 'q'
						|| emptyBound(board, rmod, fmod) == 'r') {
		//				cout << "Rook" << endl;
						check = true;
					}
				}
				else{
					if ((emptyBound(board, rmod, fmod) == 'q'
						|| emptyBound(board, rmod, fmod) == 'b')) {
		//				cout << "bishop" << endl;
						check = true;
					}
				}
			}
		}
		else {
	//		cout << "emptyBound = " << emptyBound(board, rmod, fmod) << endl;
	//		cout << "For: " << static_cast<char>(fmod) << "," << rmod << endl;
			if (emptyBound(board, rmod, fmod) != 'w') {
				if (i < 4){
					if (emptyBound(board, rmod, fmod) == 'Q'
						|| emptyBound(board, rmod, fmod) == 'R') {
					//	cout << "Rook" << endl;
						check = true;
					}
				}
				else{
					if (emptyBound(board, rmod, fmod) == 'Q'
						|| emptyBound(board, rmod, fmod) == 'B') {
					//	cout << "bishop" << endl;
						check = true;
					}
				}
			}
		}
		//then out of range or there's a piece
	}
	//cout << "b4 Knight: " << check << endl;
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
	//cout << "b4 Pawn: " << check << endl;
	if (turn == "White") {
		/*cout << "InChecking" << file << rank << endl;
		cout << "check is @ " << check << endl;*/
		//Pawn jump
		if (rank == 5 && board[1][fileIndex(file)] == 'p') {
			check = true;
		}
		//Pawn diagonals
		if (emptyBound(board, rank+1, file+1) == 'p') {
			check = true;
		}
		if (emptyBound(board, rank + 1, file - 1) == 'p') {
			check = true;
		}
		//pawn straight
		if (emptyBound(board, rank + 1, file) == 'p') {
			check = true;
		}
	}
	else {
		//Pawn jump
		/*cout << "InChecking- " << file << rank << endl;
		cout << "check is @ " << check << endl;*/
		if (rank == 4 && board[6][fileIndex(file)] == 'P') {
			check = true;
		}
		//Pawn diagonals
		if (emptyBound(board, rank - 1, file - 1) == 'P') {
			check = true;
		}
		if (emptyBound(board, rank - 1, file - 1) == 'P') {
			check = true;
		}
		//pawn straight
		if (emptyBound(board, rank - 1, file) == 'P') {
			check = true;
		}
	}

	//cout << "b4 King: " << check << endl;
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
	//cout << "check returns: " << check << endl;
	return check;
}

//Also checks if out of bounds
bool emptyOrOpp(char board[][8], const string opp, const int rank, const char file, int kRank, char kFile, const Mypiece old) {
	bool valid = false;
	bool doCheck = false;
	char temp = '.';
	int rankDex = rankIndex(rank);
	int fileDex = fileIndex(file);
	if (rankDex < 0 || rankDex > 7 || fileDex < 0 || fileDex > 7) {
		return false;
	}
	//cout << old.m_type << "- " << old.m_file << old.m_rank << endl;
	//cout << " is going : " << old.m_type << "- " << file << rank << endl;
	//cout << "ooooooooooo.Oldkfile: " << kFile << ", " << "oldKRank: " << kRank << endl;
	if (opp == "White") {
		if (old.m_type == 'P') {
			if (board[rankDex][fileDex] == '.') {
				doCheck = true;
			}
		}
		else {
			if (board[rankDex][fileDex] == '.' || islower(board[rankDex][fileDex])) {
				doCheck = true;
			}
		}
		if (doCheck) {
			temp = board[rankDex][fileDex];
			board[rankDex][fileDex] = old.m_type;
			board[rankIndex(old.m_rank)][fileIndex(old.m_file)] = '.';
			if (old.m_type == 'K') {
				kRank = rank;
				kFile = file;
			}
		//	cout << "This should be 1 " << !checkSpot(kRank, kFile, board, opp) << endl;
		//	cout << "For: " << file << rank << endl;
		//	cout << "Testing: " << old.m_file << old.m_rank << endl;
		//	cout << "Sending ..." << kRank << "," << kFile << endl;
		//	cout << "!checkspot: " << !checkSpot(kRank, kFile, board, opp) << endl;
			if (!checkSpot(kRank, kFile, board, opp)){
				valid = true;
			}
			board[rankDex][fileDex] = temp;
			board[rankIndex(old.m_rank)][fileIndex(old.m_file)] = old.m_type;
			if (old.m_type == 'K') {
				kRank = old.m_rank;
				kFile = old.m_file;
			}
		}
	}
	else { //black
		if (old.m_type == 'p') {
			if (board[rankDex][fileDex] == '.') {
				doCheck = true;
			}
		}
		else {
			if (board[rankDex][fileDex] == '.' || isupper(board[rankDex][fileDex])) {
				doCheck = true;
			}
		}
		if (doCheck) {
			temp = board[rankDex][fileDex];
			board[rankDex][fileDex] = old.m_type;
			board[rankIndex(old.m_rank)][fileIndex(old.m_file)] = '.';
			if (old.m_type == 'k') {
				kRank = rank;
				kFile = file;
			}
		//	cout << "For: " << file << rank << endl;
		//	cout << "Testing: " << old.m_file << old.m_rank << endl;
		//	cout << "Sending ..." << kRank << "," << kFile << endl;
		//	cout << "!checkspot: " << !checkSpot(kRank, kFile, board, opp) << endl;
			//	cout << "This should be 1 " << !checkSpot(kRank, kFile, board, opp) << endl;
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
	}	
	//cout << "This should be the same as old. Oldkfile: " << kFile << ", " << "oldKRank: " << kRank << endl;
	//true if valid
	/*if (valid) {
	//	cout << "valid" << endl;
	}
	else {
		cout << "not valid" << endl;
	}*/
	return valid;
}

void /*vector<Action>*/ State::actions() { //ACTIONS FUNCTION FOR STATE
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
		board[8 - white[i].m_rank][white[i].m_file - 97] = white[i].m_type;
	}
	for (int i = 0; i < black.size(); i++) {
		//file = black[i].m_file - 97;
		//cout << black[i].m_rank << black[i].m_file-97 << " = " << black[i].m_type << endl;
		board[8 - black[i].m_rank][black[i].m_file - 97] = black[i].m_type;
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
	
	Action valid;
	Mypiece tmp;
	char piece;
	//Used for Rook, Bishop and Queen
	bool enemyEncounter = false;
	int fmodifier = 0; //left and right
	int rmodifier = 0; //up and down
	//

	int turnSize = 0;
	vector<Mypiece> player;
	vector<Action> actionList;

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
					if (emptyOrOpp(board, turn, 4, player[i].m_file, kRank, kFile, player[i])) {
						tmp.setPiece('P', player[i].m_file, player[i].m_rank + 2);
						valid.setAction('P', player[i], tmp);
						actionList.push_back(valid);
					}
				}
			}
			//check jump 1
			//Moving Up
			//If empty
			if (emptyOrOpp(board, turn, player[i].m_rank + 1, player[i].m_file, kRank, kFile, player[i])) {
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
			if (player[i].m_file != 'a') { //this only checks if there is an enemy on diagonal
				if (islower(board[rankIndex(player[i].m_rank + 1)][fileIndex(player[i].m_file - 1)]) ||
					enPassantCheck(board, enPassant, turn, rankIndex(player[i].m_rank + 1), fileIndex(player[i].m_file - 1))) {
					if (enPassantCheck(board, enPassant, turn, rankIndex(player[i].m_rank + 1), fileIndex(player[i].m_file - 1))) {
						tmp.setPiece('P', player[i].m_file - 1, player[i].m_rank + 1);
						valid.setAction('P', player[i], tmp);
						actionList.push_back(valid);
					}
					else {
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
				}
			}
			//diagonal right only if there's opp
			if (player[i].m_file != 'h') {
				if (islower(board[rankIndex(player[i].m_rank + 1)][fileIndex(player[i].m_file + 1)]) ||
					enPassantCheck(board, enPassant, turn, rankIndex(player[i].m_rank + 1), fileIndex(player[i].m_file + 1))) {
					if (enPassantCheck(board, enPassant, turn, rankIndex(player[i].m_rank + 1), fileIndex(player[i].m_file + 1))) {
						tmp.setPiece('P', player[i].m_file + 1, player[i].m_rank + 1);
						valid.setAction('P', player[i], tmp);
						actionList.push_back(valid);
					}
					else {
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
					if (emptyOrOpp(board, turn, 5, player[i].m_file, kRank, kFile, player[i])) {
						tmp.setPiece('p', player[i].m_file, player[i].m_rank - 2);
						valid.setAction('p', player[i], tmp);
						actionList.push_back(valid);
					}
				}
			}
			//check jump 1
			//Moving Down
			if (emptyOrOpp(board, turn, player[i].m_rank - 1, player[i].m_file, kRank, kFile, player[i])) {
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
				if (isupper(board[rankIndex(player[i].m_rank - 1)][fileIndex(player[i].m_file - 1)]) ||
					enPassantCheck(board, enPassant, turn, rankIndex(player[i].m_rank - 1), fileIndex(player[i].m_file - 1))) {
					if (enPassantCheck(board, enPassant, turn, rankIndex(player[i].m_rank - 1), fileIndex(player[i].m_file - 1))) {
						tmp.setPiece('p', player[i].m_file - 1, player[i].m_rank - 1);
						valid.setAction('p', player[i], tmp);
						actionList.push_back(valid);
					}
					else {
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
			}
			//diagonal right only if there's opp
			if (player[i].m_file != 'h') {
				if (isupper(board[rankIndex(player[i].m_rank - 1)][fileIndex(player[i].m_file + 1)]) ||
					enPassantCheck(board, enPassant, turn, rankIndex(player[i].m_rank - 1), fileIndex(player[i].m_file + 1))) {
					if (enPassantCheck(board, enPassant, turn, rankIndex(player[i].m_rank - 1), fileIndex(player[i].m_file + 1))) {
						tmp.setPiece('p', player[i].m_file + 1, player[i].m_rank - 1);
						valid.setAction('p', player[i], tmp);
						actionList.push_back(valid);
					}
					else {
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
				while (emptyOrOpp(board, turn, player[i].m_rank + rmodifier, player[i].m_file + fmodifier, kRank, kFile, player[i]) && !enemyEncounter) {
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
						if (emptyOrOpp(board, turn, player[i].m_rank + k, player[i].m_file + 1, kRank, kFile, player[i])) {
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
						if (emptyOrOpp(board, turn, player[i].m_rank + k, player[i].m_file - 1, kRank, kFile, player[i])) {
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
						if (emptyOrOpp(board, turn, player[i].m_rank + k, player[i].m_file + 2, kRank, kFile, player[i])) {
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
						if (emptyOrOpp(board, turn,player[i].m_rank + k, player[i].m_file - 2, kRank, kFile, player[i])) {
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
					if (!(k==0 && j==0)){
						//cout << "For: " << static_cast<char>(player[i].m_file + j) <<","<< player[i].m_rank + k << endl;
						//cout << "emptyOrOpp: " << emptyOrOpp(board, turn, player[i].m_rank + k, player[i].m_file + j, kRank, kFile) << endl;
						if (emptyOrOpp(board, turn, player[i].m_rank + k, player[i].m_file + j, kRank, kFile, player[i])){
							//if not in check
							//so it should be 0
							//cout << "!checkSpot: " << !checkSpot(player[i].m_rank + k, player[i].m_file + j, board, turn) << endl;
							//cout << "This should be - 0 for g2 and h2" << endl;
							//if (!checkSpot(player[i].m_rank + k, player[i].m_file + j, board, turn)) {
								if (turn == "White") {
									tmp.setPiece('K', player[i].m_file + j, player[i].m_rank + k);
									valid.setAction('K', player[i], tmp);
								}
								else {
									tmp.setPiece('k', player[i].m_file + j, player[i].m_rank + k);
									valid.setAction('k', player[i], tmp);
								}
								actionList.push_back(valid);
							//}
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
	for (int i = 0; i < actionList.size(); i++) {
		cout << actionList[i].original.m_type << "-" << actionList[i].original.m_file << actionList[i].original.m_rank << "->"
			<< actionList[i].move.m_type << "-" << actionList[i].move.m_file << actionList[i].move.m_rank << endl;
	}
	return;
}