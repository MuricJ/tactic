#include <iostream>
#include <bitset>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <random>
#include <array>
#include <random>
#include <chrono>


/*
TODO/ideas:
    - seperate unsafe and safe boards by making a safe board that inherits from board
    - make a monte carlo agent class
    - make an enum class to define board states (won, lost)
    - add seperate Move classes for different types of boards to prevent collisions and unexpected behaviour
      with casting or some conversion frome one to the other with checks
    - test entire MiniBoard class thoroughly
    - implement Board in terms of MiniBoards

*/

enum class BoardState {WON_0, WON_1, DRAWN, IN_PROGRESS, UNKNOWN};
enum class CellState {TAKEN_0, TAKEN_1, EMPTY, UNKNOWN};

// BOARD EVALUATION FUNCIONS
const std::bitset<9> hori1(0b111000000);
const std::bitset<9> hori2(0b000111000);
const std::bitset<9> hori3(0b000000111);
const std::bitset<9> vert1(0b100100100);
const std::bitset<9> vert2(0b010010010);
const std::bitset<9> vert3(0b001001001);
const std::bitset<9> diag1(0b100010001);
const std::bitset<9> diag2(0b001010100);

bool checkWinPatterns(const std::bitset<9> cells){
    return (cells & hori1) == hori1 || (cells & hori2) == hori2 || (cells & hori3) == hori3 || // check horizonatl wins
    (cells & vert1) == vert1 || (cells & vert2) == vert2 || (cells & vert3) == vert3        || // check vertical wins
    (cells & diag1) == diag1 || (cells & diag2) == diag2;                                      // check diagonal wins
}


const std::string BOARD_ASCII = 
" 0 │ 1 │ 2  ║  9 │ 10 │ 11  ║  18 │ 19 │ 20 \n"
"───┼───┼─── ║ ───┼───┼─── ║ ───┼───┼─── \n"
" 3 │ 4 │ 5  ║  12 │ 13 │ 14  ║  21 │ 22 │ 23 \n"
"───┼───┼─── ║ ───┼───┼─── ║ ───┼───┼─── \n"
" 6 │ 7 │ 8  ║  15 │ 16 │ 17  ║  24 │ 25 │ 26 \n"
"════════════╬═════════════╬════════════ \n"
" 27 │ 28 │ 29  ║  36 │ 37 │ 38  ║  45 │ 46 │ 47 \n"
"───┼───┼─── ║ ───┼───┼─── ║ ───┼───┼─── \n"
" 30 │ 31 │ 32  ║  39 │ 40 │ 41  ║  48 │ 49 │ 50 \n"
"───┼───┼─── ║ ───┼───┼─── ║ ───┼───┼─── \n"
" 33 │ 34 │ 35  ║  42 │ 43 │ 44  ║  51 │ 52 │ 53 \n"
"════════════╬═════════════╬════════════ \n"
" 54 │ 55 │ 56  ║  63 │ 64 │ 65  ║  72 │ 73 │ 74 \n"
"───┼───┼─── ║ ───┼───┼─── ║ ───┼───┼─── \n"
" 57 │ 58 │ 59  ║  66 │ 67 │ 68  ║  75 │ 76 │ 77 \n"
"───┼───┼─── ║ ───┼───┼─── ║ ───┼───┼─── \n"
" 60 │ 61 │ 62  ║  69 │ 70 │ 71  ║  78 │ 79 │ 80 \n";

struct MiniMove{
    MiniMove(int8_t id, bool player);
    int8_t move_id;
    bool player;
};

MiniMove::MiniMove(int8_t id, bool player_to_move){
    move_id = id;
    player = player_to_move;
}

/*
IMPORTANT: If the board is not empty, then move_list must
           contain at least the last move!
*/


const static std::string MINI_BOARD_ASCII = 
    " 0 │ 1 │ 2 \n"
    "───┼───┼───\n"
    " 3 │ 4 │ 5 \n"
    "───┼───┼───\n"
    " 6 │ 7 │ 8 ";

class MiniBoard {
public:
    MiniBoard();
    MiniBoard(const MiniBoard &to_copy);
    MiniBoard(const std::string &position_str);

    void PlayMove(MiniMove to_move);
    bool PlayMoveSafe(MiniMove to_move);
    bool ClearCell(int n);
    void Reset();

    std::vector<MiniMove> AvailableMoves();
    BoardState State();
    std::string PositionString();
    CellState Cell(int n);

    void PrintBoard();

protected:
    std::bitset<9> base_cells0; 
    std::bitset<9> base_cells1;
    BoardState state;
    int move_number;
};

MiniBoard::MiniBoard(){
    base_cells0.reset();
    base_cells1.reset();
    move_number = 0;
    state = BoardState::IN_PROGRESS;
 }

MiniBoard::MiniBoard(const std::string &position_str){
    int c=0, num_moves=0;
    for (auto it=position_str.begin(); it != position_str.end(); it++){
        if (*it == 'X'){
            base_cells0[c] = 1;
            num_moves++;
        } else if (*it == 'O'){
            base_cells1[c] = 1;
            num_moves++;
        }
        c++;
    }
    move_number = num_moves;
}

MiniBoard::MiniBoard(const MiniBoard &to_copy){
    base_cells0 = to_copy.base_cells0;
    base_cells1 = to_copy.base_cells1;
    move_number = to_copy.move_number;
    state = to_copy.state;
}

void MiniBoard::PrintBoard(){
    int p;
    std::string to_print(MINI_BOARD_ASCII);
    for (int i=0; i<9; i++){
        p = to_print.find(" " + std::to_string(i) + " ");
        if (base_cells0[p] == true){
            to_print.replace(p, 1, "X");
        } else if (base_cells1[p] == 1){
            to_print.replace(p, 1, "O");
        } else {
            to_print.replace(p, 1, " ");
        }
    }
    std::cout << to_print;
}

BoardState MiniBoard::State(){
    if (state != BoardState::UNKNOWN) return state;
    else if (checkWinPatterns(base_cells0)) return BoardState::WON_0;
    else if (checkWinPatterns(base_cells1)) return BoardState::WON_1;
    else if (base_cells0.count() + base_cells1.count() == 9) return BoardState::DRAWN;
    else return BoardState::IN_PROGRESS;
}

bool MiniBoard::PlayMoveSafe(MiniMove to_move){
    if (this->State() == BoardState::IN_PROGRESS &&
        base_cells0[to_move.move_id] == 0 &&
        base_cells1[to_move.move_id] == 0)
        { // replace? below with this->PlayMove(to_move)
        if (to_move.player == 0){
            base_cells0[to_move.move_id] = 1;
        } else {
            base_cells1[to_move.move_id] = 1;
        }
        move_number++;
        state = BoardState::UNKNOWN;
        return true;
    } else {
        return false;
    }
}

void MiniBoard::PlayMove(MiniMove to_move){
    if (to_move.player == 0){
        base_cells0[to_move.move_id] = 1;
    } else {
        base_cells1[to_move.move_id] = 1;
    }
    move_number++;
    state = BoardState::UNKNOWN;
}

bool MiniBoard::ClearCell(int n){
    bool result = base_cells0[n] | base_cells1[n];
    base_cells0[n] = 0;
    base_cells1[n] = 0;
    if (result) {
        move_number--;
        state = BoardState::UNKNOWN;
    }
    return result;
}

/* 
bool MiniBoard::UndoMoves(int n){
    if (n > move_list.size()){
        return false;
    } else {
        MiniMove last_move(0, 0);
        for (int i=0; i<n; i++){
            last_move = move_list.back();
            if (last_move.player == 0){
                base_cells0.set(last_move.move_id, false);
            } else {
                base_cells1.set(last_move.move_id, false);
            }
            move_number--;
        }
        state = BoardState::UNKNOWN;
        return true;
    }
}
*/
void MiniBoard::Reset(){
    base_cells0.reset();
    base_cells1.reset();
    move_number = 0;
    state = BoardState::IN_PROGRESS;
}

std::vector<MiniMove> MiniBoard::AvailableMoves(){
    std::bitset<9> occupied = base_cells0 | base_cells1;
    std::vector<MiniMove> avail;
    bool next_player = (move_number % 2);
    for (int i=0; i<9; i++){
        if (occupied[i] == 0) {
            avail.push_back(MiniMove(i, next_player));
        }
    }
    return avail;
}

std::string MiniBoard::PositionString(){
    std::string position;
    for (int i=0; i<9; i++){
        if (base_cells0[i]){
            position += "X";
        } else if (base_cells1[i]){
            position += "O";
        } else {
            position += "-";
        }
    }
    return position;
}

CellState MiniBoard::Cell(int n){
    if (base_cells0[n] == 1){
        return CellState::TAKEN_0;
    } else if (base_cells1[n] == 1){
        return CellState::TAKEN_1;
    } else{
        return CellState::EMPTY;
    }
}

struct Move{
    Move(int8_t id, int8_t board, bool plr);
    Move(MiniMove mini_move, int8_t board);
    MiniMove EquivalentMiniMove();
    
    int8_t move_id;
    int8_t board_num;
    bool player;
};

Move::Move(int8_t id, int8_t board, bool player_to_move){
    move_id = id;
    player = player_to_move;
    board_num = board;
}
Move::Move(MiniMove mini_move, int8_t board){
    move_id = mini_move.move_id;
    player = mini_move.player;
    board_num = board;
}

MiniMove Move::EquivalentMiniMove(){
    return MiniMove(move_id % 9, player);
}

class Board{
public:
    Board();
    Board(const Board &old_board);
    Board(const std::string &state_string, const Move &last_move);

    std::string State_String();
    std::vector<Move> AvailableMoves();
    BoardState State();
    int MoveNumber();

    void PlayMove(const Move &to_play); // WARNING: unsafe - this method will not check if a move is valid
    bool PlayMoveSafe(const Move &to_play);
    bool UndoMoves(int n=1);
    void Reset();

private:
    std::array<MiniBoard, 9> mini_boards;
    std::vector<Move> move_list;
    int move_num;
};

Board::Board(){
    for (int i=0; i<9; i++){
        mini_boards[i].Reset();
    }
    move_num = 0;
    move_list.clear();
}

void Board::Reset(){
    for (int i=0; i<9; i++){
        mini_boards[i].Reset();
    }
    move_num = 0;
    move_list.clear(); 
}

Board::Board(const Board &old_board){
    mini_boards = old_board.mini_boards;
    move_num = old_board.move_num;
    move_list = old_board.move_list;
}

// todo add check if position is reachable with normal play? or dont?
Board::Board(const std::string &state_string, const Move &last_move){ // the board must have at least one move to use this constructor
    for (int i=0; i<9; i++){
        mini_boards[i].Reset();
    }
    move_list.clear();
    move_list.push_back(last_move);
    int c = 0;
    move_num = 0;
    for (auto it=state_string.cbegin(); it != state_string.cend(); it++){
        if (*it == 'X') {
            mini_boards[c/9].PlayMove(MiniMove(c%9, 0));
            move_num++;
        }
        else if (*it == 'O') {
            mini_boards[c/9].PlayMove(MiniMove(c%9, 1));
            move_num++;
        }
        c++;
    }
    if (move_num == 0){
        throw std::invalid_argument("Board must have at least one move played");
    }
}

std::string Board::State_String(){
    std::string state;
    CellState current;
    for (int i=0; i<81; i++){
        current = mini_boards[i/9].Cell(i%9);
        switch (current){
            case CellState::TAKEN_0:
               state.push_back('X');
               break;
            case CellState::TAKEN_1:
                state.push_back('O');
                break;
            default:
                state.push_back('-');
        }
    }
    return state;
}

std::vector<Move> Board::AvailableMoves(){
    std::vector<Move> available;
    std::vector<MiniMove> tmp;
    bool next_player = (bool) ((move_num) % 2);
    int next_board = (move_list.size() > 0) ? move_list.back().move_id % 9 : -1; 
    BoardState nextboard_state = (next_board == -1) ? BoardState::DRAWN : mini_boards[next_board].State();
    if (move_num > 0 && nextboard_state == BoardState::IN_PROGRESS){
        tmp = mini_boards[next_board].AvailableMoves();
        for (auto it: tmp){
            available.push_back(Move(it.move_id, next_board, next_player));
        }
    } else {
        for (int i=0; i<9; i++){
            if (mini_boards[i].State() == BoardState::IN_PROGRESS){
                tmp = mini_boards[i].AvailableMoves();
                for (auto it: tmp){
                    available.push_back(Move(it.move_id, i, next_player));
                }
            }
        }
    }
    return available;
}

int Board::MoveNumber(){
    return move_num;
}

void Board::PlayMove(const Move &to_play){
    mini_boards[to_play.board_num].PlayMove(MiniMove(to_play.move_id, to_play.player));
    move_list.push_back(to_play);
    move_num++;
}

bool Board::PlayMoveSafe(const Move &to_move){
    if ((to_move.player != (bool) ((move_num) % 2)) ||                               // check if it's incorrect player's turn
       ((mini_boards[to_move.board_num].Cell(to_move.move_id)) != CellState::EMPTY) || // check if board occupied
       (move_list.size() > 0 && (move_list.back().move_id != to_move.board_num)) &&  mini_boards[move_list.back().move_id].State() == BoardState::IN_PROGRESS)    // check if the mini-board is correct
    { 
        return false;
    }
    PlayMove(to_move);
    return true;
}

bool Board::UndoMoves(int n){
    if (n > move_num){
        return false;
    }
    Move current(-1, -1, 0);
    for (int i=0; i<n; i++){
        current = move_list.back();
        mini_boards[current.board_num].ClearCell(current.move_id);
        move_list.pop_back();
    }
    move_num -= n;
    return true;
}

BoardState Board::State(){
    std::bitset<9> cells0;
    std::bitset<9> cells1;
    int done_cells = 0;
    for (int i=0; i<9; i++){
        switch(mini_boards[i].State()){
            case BoardState::WON_0:
                cells0[i] = 1;
                done_cells++;
                break;
            case BoardState::WON_1:
                cells1[i] = 1;
                done_cells++;;
                break;
            case BoardState::DRAWN:
                done_cells++;
                break;
        }
    }
    if (checkWinPatterns(cells0)) return BoardState::WON_0;
    else if (checkWinPatterns(cells1)) return BoardState::WON_1;
    else if (done_cells == 9) return BoardState::DRAWN;
    else return BoardState::IN_PROGRESS;
}

void printBoard(Board to_print){
    std::string print_str = BOARD_ASCII;
    std::string board_str = to_print.State_String();
    int p;
    for (int i=0; i<81; i++){
        p = print_str.find(" " + std::to_string(i) + " ");
        print_str.replace(p+1, 1, (board_str[i] == '-') ? " " :  std::string(1, board_str[i]));
        if (i > 9){
            print_str.erase(p+2, 1);
        }
    }
    std::cout << print_str;
}

std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
int randm(int a, int b){
    std::uniform_int_distribution<int> distribution(a,b);
    return distribution(generator);
}

double monteCarloStaticEval(Board game_board, int games, unsigned int seed){
    std::default_random_engine generator(seed);
    Board eval_board = game_board;
    int win0=0, win1=0, draw=0, tmp;
    std::vector<Move> moves;
    for (int i=0; i<games; i++){
        while (eval_board.State() == BoardState::IN_PROGRESS){
            moves = eval_board.AvailableMoves();
            eval_board.PlayMove(moves[generator() % moves.size()]); // generator() % n introduces a bias, not uniform!
        }
        switch (eval_board.State()){
            case BoardState::WON_0:
                win0++;
                break;
            case BoardState::WON_1:
                win1++;
                break;
            case BoardState::DRAWN:
                draw++;
                break;
        }
        eval_board = game_board;
    }
    return ((double)(win0-win1))/draw;

}

double miniMax(Board &eval_board, int depth, double alpha, double beta, double (*staticEvalFn)(Board)){
    //printBoard(eval_board);
    //std::cout <<"\n\n";
    if (eval_board.State() == BoardState::DRAWN) return 0;
    else if (eval_board.State() == BoardState::WON_0) return std::numeric_limits<double>::infinity();
    else if (eval_board.State() == BoardState::WON_1) return -std::numeric_limits<double>::infinity();
    else if (depth == 0) return staticEvalFn(eval_board);

    double value;
    
    if (eval_board.MoveNumber() % 2 == 0){
        value = -std::numeric_limits<double>::infinity();
        for (auto it: eval_board.AvailableMoves()){
            eval_board.PlayMove(it);
            value = std::max(value, miniMax(eval_board, depth-1, alpha, beta, staticEvalFn));
            alpha = std::max(alpha, value);
            eval_board.UndoMoves();
            if (value >= beta) break;
        }
        return value;
    } else {
        value = std::numeric_limits<double>::infinity();
        for (auto it: eval_board.AvailableMoves()){
            eval_board.PlayMove(it);
            value = std::min(value, miniMax(eval_board, depth-1, alpha, beta, staticEvalFn));
            beta = std::min(beta, value);
            eval_board.UndoMoves();
            if (value <= alpha) break;
        }
        return value;

    }



}

double MTWrapper(Board x){
    return monteCarloStaticEval(x, 50, generator());
}

int main(){
    std::string a = "----X----------------------------------------------------------------------------";
    Board b(a, Move(4,0,0));
    std::cout << std::flush;
    printBoard(b);


    double inff = std::numeric_limits<double>::infinity();
    Board old = b;
    while (old.AvailableMoves().size() > 0){
        double m=inff, tmp;
        Move t(0,0,0);
        int ind=-1;

        for (auto it: old.AvailableMoves()){
            b = old;
            b.PlayMove(it);
            tmp = miniMax(b, 4, -inff, inff, MTWrapper);
            if (tmp < m){
                m = tmp;
                t = it;
            }
            std::cout << "Move: " << it.board_num*8+it.move_id << " Eval: " << tmp << std::endl;

        }
        old.PlayMove(t);
        printBoard(old);
        int i1, i2;
        std::cin >> i1 >> i2;
        if (i1 == -1){
            old.UndoMoves(2);
            continue;
        }
        old.PlayMove(Move(i2, i1, 0));
        printBoard(old);
    }
    

    return 0;
}