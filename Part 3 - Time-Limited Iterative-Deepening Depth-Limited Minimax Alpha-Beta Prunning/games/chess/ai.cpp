// AI
// This is where you build your AI

#include "ai.hpp"
#include "my_code/state.h"
#include <stdio.h>
#include <ctype.h>
#include <string>

// You can add #includes here for your AI.

namespace cpp_client
{

namespace chess
{

/// <summary>
/// This returns your AI's name to the game server.
/// Replace the string name.
/// </summary>
/// <returns>The name of your AI.</returns>
std::string AI::get_name() const
{
    // REPLACE WITH YOUR TEAM NAME!
    return "ChristopherTorralba";
}

/// <summary>
/// This is automatically called when the game first starts, once the game objects are created
/// </summary>
void AI::start()
{
    // This is a good place to initialize any variables
	srand(time(NULL));
}

/// <summary>
/// This is automatically called the game (or anything in it) updates
/// </summary>
void AI::game_updated()
{
    // If a function you call triggers an update this will be called before it returns.
}

/// <summary>
/// This is automatically called when the game ends.
/// </summary>
/// <param name="won">true if you won, false otherwise</param>
/// <param name="reason">An explanation for why you either won or lost</param>
void AI::ended(bool won, const std::string& reason)
{
    // You can do any cleanup of your AI here.  The program ends when this function returns.
}

/// <summary>
/// This is called every time it is this AI.player's turn.
/// </summary>
/// <returns>Represents if you want to end your turn. True means end your turn, False means to keep your turn going and re-call this function.</returns>
bool AI::run_turn()
{
    // Here is where you'll want to code your AI.

    // We've provided sample code that:
    //    1) prints the board to the console
    //    2) prints the opponent's last move to the console
    //    3) prints how much time remaining this AI has to calculate moves
    //    4) makes a random (and probably invalid) move.

    // 1) print the board to the console
    print_current_board();

    // 2) print the opponent's last move to the console
   if(game->moves.size() > 0)
    {
       std::cout << "Opponent's Last Move: '" << game->moves[game->moves.size() - 1]->san << "'" << std::endl;
   }
	
	// 3) print how much time remaining this AI has to calculate moves
    std::cout << "Time Remaining: " << player->time_remaining << " ns" << std::endl;
	
	// 4) Making a move
    State S;
	S.turn = player->color;
	S.max = S.turn;
	
	//Getting the last 8 moves (if any)
	Action prevMove;
	if (game->moves.size() >= 8) {
		for (int i = 1; i <= 8; i++) {
			prevMove = convertAction(game->moves[game->moves.size() - i]->piece->owner->color,
				game->moves[game->moves.size() - i]->piece->type, game->moves[game->moves.size() - i]->from_file, game->moves[game->moves.size() - i]->from_rank,
				game->moves[game->moves.size() - i]->to_file, game->moves[game->moves.size() - i]->to_rank);
			S.last8Moves.push_back(prevMove);
		}
	}

	//Getting the last move
	if (game->moves.size() != 0) {
		prevMove = convertAction(game->moves[game->moves.size()-1]->piece->owner->color,
			game->moves[game->moves.size() - 1]->piece->type, game->moves[game->moves.size() - 1]->from_file, game->moves[game->moves.size() - 1]->from_rank,
			game->moves[game->moves.size() - 1]->to_file, game->moves[game->moves.size() - 1]->to_rank);
		S.actionApplied = prevMove;
	}
	
	//reading in board
	std::string fen = game->fen;
	//Pieces places turn castling ability enPassant movement hc fm
	//fen = "8/6Pk/1P3K2/5N1p/p6P/PN6/8/6N1 b - - 2 136";
	// cout << fen << endl;
	int rank = 8;
	int pass = 0;
	char file = 'a';
	int spaceCount = 0;
	int wRank, bRank;
	char wFile, bFile;
	string halfturns;
	Mypiece piece;
	for (int i = 0; i < fen.size(); i++) {
		if (spaceCount == 0){
			if (isdigit(fen[i])) {
				file = file + (int)fen[i]- 48;
			}
			if (fen[i] == '/') {
				rank--;
				file = 'a';
			}
			if (isalpha(fen[i])) {
				if (S.turn == "Black" && fen[i] == 'k'){
					bFile = file;
					bRank = rank;
				}
				if (S.turn == "White" && fen[i] == 'K') {
					wFile = file;
					wRank = rank;
				}
				//std::cout << file << rank << ": " << fen[i] << std::endl; //-- Display all active pieces
				if (isupper(fen[i])){
					piece.setPiece(fen[i], file, rank);
					S.white.push_back(piece); //Make a piece and push into white
				}
				else{
					piece.setPiece(fen[i], file, rank);
					S.black.push_back(piece);//Make a piece and push into black
				}
				file++;
			}
		}
		if (spaceCount == 2) {
			if (fen[i] == 'K') {
				S.castleK = true;
			}
			if (fen[i] == 'Q') {
				S.castleQ = true;
			}
			if (fen[i] == 'k') {
				S.castlek = true;
			}
			if (fen[i] == 'q') {
				S.castleq = true;
			}
		}
		if (spaceCount == 3) {
			if (pass == 0){
				S.enPassant = fen[i];
				pass++;
			}
			else{
				S.enPassant = S.enPassant + fen[i];
				pass = 0;
			}
		}
		if (spaceCount == 4) {
			halfturns = fen[i];
			if (isspace(fen[i + 1])) {
				break;
			}
			else {
				halfturns = halfturns + fen[i+1];
				break;
			}
		}
		if (isspace(fen[i])) {
			spaceCount++;
		}
	}
	S.fiftyMoveRule = atoi(halfturns.c_str());
	//0 = reset
	//100 = fifty move rule
	
	if (S.turn == "White") {
		S.kFile = wFile;
		S.kRank = wRank;
	}
	else {
		S.kFile = bFile;
		S.kRank = bRank;
	}
	//States are specified
	//Now testing for valid actions

	vector<Action> actionList;
	actionList.clear();


	//Min time difference testing vs my own AI -> 5.0E+07
	//Max time difference testing vs my own AI -> 4.0E+08
	//With this setup it exits around 4 to 5 depth against the difficulty of my own AI
	/////////////////////////////////
	//Call to IDM with timelimit and ABDLM
	///////////////////////////////////
	double wantedTime = 50000000;
	Action validAct = IDM(S, 5, wantedTime);

	cout << "IDM used: " << endl;
	displayAction(validAct);
	cout << endl;
	
	//find the piece
	chess::Piece random_piece;

	string oFile(1, validAct.original.m_file);
	for (int i = 0; i < player->pieces.size(); i++) {
		if (player->pieces[i]->type == validAct.original.convertType()) {
			//pieces have to have different rank && file
			if (player->pieces[i]->rank == validAct.original.m_rank &&
				player->pieces[i]->file == oFile) {
				random_piece = player->pieces[i];
			}
		}
	}
		
	std::string idm_move_file(1, validAct.move.m_file);
	int idm_move_rank = validAct.move.m_rank;

	if (validAct.original.m_type == validAct.move.m_type) {
		random_piece->move(idm_move_file, idm_move_rank);
	}
	else {
		std::string promotion = validAct.move.convertType();
		random_piece->move(idm_move_file, idm_move_rank, promotion);
	}
	return true;
}

/// <summary>
///  Prints the current board using pretty ASCII art
/// </summary>
/// <remarks>
/// Note: you can delete this function if you wish
/// </remarks>
void AI::print_current_board()
{
    for(int rank = 9; rank >= -1; rank--)
    {
        std::string str = "";
        if(rank == 9 || rank == 0) // then the top or bottom of the board
        {
            str = "   +------------------------+";
        }
        else if(rank == -1) // then show the ranks
        {
            str = "     a  b  c  d  e  f  g  h";
        }
        else // board
        {
            str += " ";
            str += std::to_string(rank);
            str += " |";
            // fill in all the files with pieces at the current rank
            for(int file_offset = 0; file_offset < 8; file_offset++)
            {
                std::string file(1, 'a' + file_offset); // start at a, with with file offset increasing the char;
                chess::Piece current_piece = nullptr;
                for(const auto& piece : game->pieces)
                {
                    if(piece->file == file && piece->rank == rank) // then we found the piece at (file, rank)
                    {
                        current_piece = piece;
                        break;
                    }
                }

                char code = '.'; // default "no piece";
                if(current_piece != nullptr)
                {
                    code = current_piece->type[0];

                    if(current_piece->type == "Knight") // 'K' is for "King", we use 'N' for "Knights"
                    {
                        code = 'N';
                    }

                    if(current_piece->owner->id == "1") // the second player (black) is lower case. Otherwise it's upppercase already
                    {
                        code = tolower(code);
                    }
                }

                str += " ";
                str += code;
                str += " ";
            }

            str += "|";
        }

        std::cout << str << std::endl;
    }
}

// You can add additional methods here for your AI to call

} // chess

} // cpp_client
