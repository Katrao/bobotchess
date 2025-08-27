#include <string>
#include <list>
#include <iostream>
#include <vector>
#include <cctype>
#include <thread>
#include <chrono>
#include <ctime>
#include <cstdint>
#include <random>
#include <array>
#include <cstdlib>
#include <algorithm>

using namespace std;

bool findc(char c, char carr[], int size) {
    for (int i = 0; i < size; i++) {
        if (carr[i] == c) return true;
    }
    return false;
}

int indexc(char c, char carr[], int size) {
    for (int i = 0; i < size; i++) {
        if (carr[i] == c) return i;
    }
    return -1;
}

bool findint(int c, int carr[], int size) {
    for (int i = 0; i < size; i++) if (carr[i] == c) return true;
    return false;
}

int indexint(int c, int carr[], int size) {
    for (int i = 0; i < size; i++) if (carr[i] == c) return i;
    return -1;
}

int indexstr(string c, string carr[], int size) {
    for (int i = 0; i < size; i++) if (carr[i] == c) return i;
    return -1;
}

string formatTimePoint(chrono::system_clock::time_point tp) {
    time_t t = chrono::system_clock::to_time_t(tp);
    tm tms;
    localtime_s(&tms, &t);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tms);
    return string(buffer);
}

class Board;
class Move;
class Generator;
class Evaluator;

class Piece {
    public:
        int none = 0;
        int pawn = 1;
        int knight = 2;
        int bishop = 3;
        int rook = 4;
        int queen = 5;
        int king = 6;

        int white = 8;
        int black = 16;

        bool whiteBool = true;
        bool blackBool = false;

        int pawnMapW[64] = {
            0, 0, 0, 0, 0, 0, 0, 0,
            50, 50, 50, 50, 50, 50, 50, 50,
            10, 10, 20, 30, 30, 20, 10, 10,
            5, 5, 10, 25, 25, 10, 5, 5,
            0, 0, 0, 20, 20, 0, 0, 0,
            5, -5, -10, 0, 0, -10, -5, 5,
            5, 30, 30, -20, -20, 10, 10, 5,
            0, 0, 0, 0, 0, 0, 0, 0
        };
        int pawnMapB[64] = {
            0, 0, 0, 0, 0, 0, 0, 0,
            5, 30, 30, -20, -20, 10, 10, 5,
            5, -5, -10, 0, 0, -10, -5, 5,
            0, 0, 0, 20, 20, 0, 0, 0,
            5, 5, 10, 25, 25, 10, 5, 5,
            10, 10, 20, 30, 30, 20, 10, 10,
            50, 50, 50, 50, 50, 50, 50, 50,
            0, 0, 0, 0, 0, 0, 0, 0
        };

        int knightMapW[64] = {
            -50, -40, -30, -30, -30, -30, -40, -50,
            -40, -20, 0, 0, 0, 0, -20, -40,
            -30, 0, 10, 15, 15, 10, 0, -30,
            -30, 5, 15, 20, 20, 15, 5, -30,
            -30, 0, 15, 20, 20, 15, 0, -30,
            -30, 5, 10, 15, 15, 10, 5, -30,
            -40, -20, 0, 5, 5, 0, -20, -40,
            -50, -40, -30, -30, -30, -30, -40, -50
        };
        int knightMapB[64] = {
            -50, -40, -30, -30, -30, -30, -40, -50,
            -40, -20, 0, 5, 5, 0, -20, -40,
            -30, 5, 10, 15, 15, 10, 5, -30,
            -30, 0, 15, 20, 20, 15, 0, -30,
            -30, 5, 15, 20, 20, 15, 5, -30,
            -30, 0, 10, 15, 15, 10, 0, -30,
            -40, -20, 0, 0, 0, 0, -20, -40,
            -50, -40, -30, -30, -30, -30, -40, -50
        };

        int bishopMapW[64] = {
            -20, -10, -10, -10, -10, -10, -10, -20,
            -10, 0, 0, 0, 0, 0, 0, -10,
            -10, 0, 5, 10, 10, 5, 0, -10,
            -10, 5, 5, 10, 10, 5, 5, -10,
            -10, 0, 10, 10, 10, 10, 0, -10,
            -10, 10, 10, 10, 10, 10, 10, -10,
            -10, 5, 0, 0, 0, 0, 5, -10,
            -20, -10, -10, -10, -10, -10, -10, -20
        };
        int bishopMapB[64] = {
            -20, -10, -10, -10, -10, -10, -10, -20,
            -10, 5, 0, 0, 0, 0, 5, -10,
            -10, 10, 10, 10, 10, 10, 10, -10,
            -10, 0, 10, 10, 10, 10, 0, -10,
            -10, 5, 5, 10, 10, 5, 5, -10,
            -10, 0, 5, 10, 10, 5, 0, -10,
            -10, 0, 0, 0, 0, 0, 0, -10,
            -20, -10, -10, -10, -10, -10, -10, -20
        };

        int rookMapW[64] = {
            0, 0, 0, 0, 0, 0, 0, 0,
            5, 10, 10, 10, 10, 10, 10, 5,
            -5, 0, 0, 0, 0, 0, 0, -5,
            -5, 0, 0, 0, 0, 0, 0, -5,
            -5, 0, 0, 0, 0, 0, 0, -5,
            -5, 0, 0, 0, 0, 0, 0, -5,
            -5, 0, 0, 0, 0, 0, 0, -5,
            0, 0, 0, 5, 5, 0, 0, 0
        };
        int rookMapB[64] = {
            0, 0, 0, 5, 5, 0, 0, 0,
            -5, 0, 0, 0, 0, 0, 0, -5,
            -5, 0, 0, 0, 0, 0, 0, -5,
            -5, 0, 0, 0, 0, 0, 0, -5,
            -5, 0, 0, 0, 0, 0, 0, -5,
            -5, 0, 0, 0, 0, 0, 0, -5,
            5, 10, 10, 10, 10, 10, 10, 5,
            0, 0, 0, 0, 0, 0, 0, 0
        };

        int queenMapW[64] = {
            -20, -10, -10, -5, -5, -10, -10, -20,
            -10, 0, 0, 0, 0, 0, 0, -10,
            -10, 0, 5, 5, 5, 5, 0, -10,
            -5, 0, 5, 5, 5, 5, 0, -5,
            0, 0, 5, 5, 5, 5, 0, -5,
            -10, 5, 5, 5, 5, 5, 0, -10,
            -10, 0, 5, 0, 0, 0, 0, -10,
            -20, -10, -10, -5, -5, -10, -10, -20
        };
        int queenMapB[64] = {
            -20, -10, -10, -5, -5, -10, -10, -20,
            -10, 0, 5, 0, 0, 0, 0, -10,
            -10, 5, 5, 5, 5, 5, 0, -10,
            0, 0, 5, 5, 5, 5, 0, -5,
            -5, 0, 5, 5, 5, 5, 0, -5,
            -10, 0, 5, 5, 5, 5, 0, -10,
            -10, 0, 0, 0, 0, 0, 0, -10,
            -20, -10, -10, -5, -5, -10, -10, -20
        };

        int kingMapWMid[64] = {
            -30, -40, -40, -50, -50, -40, -40, -30,
            -30, -40, -40, -50, -50, -40, -40, -30,
            -30, -40, -40, -50, -50, -40, -40, -30,
            -30, -40, -40, -50, -50, -40, -40, -30,
            -20, -30, -30, -40, -40, -30, -30, -20,
            -10, -20, -20, -20, -20, -20, -20, -10,
            20, 20, 0, 0, 0, 0, 20, 20,
            20, 30, 10, 0, 0, 10, 30, 20
        };
        int kingMapBMid[64] = {
            20, 30, 10, 0, 0, 10, 30, 20,
            20, 20, 0, 0, 0, 0, 20, 20,
            -10, -20, -20, -20, -20, -20, -20, -10,
            -20, -30, -30, -40, -40, -30, -30, -20,
            -30, -40, -40, -50, -50, -40, -40, -30,
            -30, -40, -40, -50, -50, -40, -40, -30,
            -30, -40, -40, -50, -50, -40, -40, -30,
            -30, -40, -40, -50, -50, -40, -40, -30
        };

        int kingMapWEnd[64] = {
            -50, -40, -30, -20, -20, -30, -40, -50,
            -30, -20, -10, 0, 0, -10, -20, -30,
            -30, -10, 20, 30, 30, 20, -10, -30,
            -30, -10, 30, 40, 40, 30, -10, -30,
            -30, -10, 30, 40, 40, 30, -10, -30,
            -30, -10, 20, 30, 30, 20, -10, -30,
            -30, -30, 0, 0, 0, 0, -30, -30,
            -50, -30, -30, -30, -30, -30, -30, -50
        };
        int kingMapBEnd[64] = {
            -50, -30, -30, -30, -30, -30, -30, -50,
            -30, -30, 0, 0, 0, 0, -30, -30,
            -30, -10, 20, 30, 30, 20, -10, -30,
            -30, -10, 30, 40, 40, 30, -10, -30,
            -30, -10, 30, 40, 40, 30, -10, -30,
            -30, -10, 20, 30, 30, 20, -10, -30,
            -30, -20, -10, 0, 0, -10, -20, -30,
            -50, -40, -30, -20, -20, -30, -40, -50
        };

        int getPiece(int p) {
            if (p <= 8) return none;
            if (p >= 9 && p <= 15) return p - 8;  
            if (p >= 17 && p <= 23) return p - 16;
            return none;
        }

        int getHashKey(int p) {
            if (p <= 8) return -1;
            if (p >= 9 && p <= 15) return p - 8;
            if (p >= 17 && p <= 23) return p - 11;
            return -1;
        }

        bool isWhite(int p) {
            return (p >= 9 && p <= 15);
        }
        
        bool isBlack(int p) {
            return (p >= 17 && p <= 23);
        }

        bool isSameColor(int p1, int p2) {
            if (getPiece(p1) == none || getPiece(p2) == none) return false;
            return isWhite(p1) == isWhite(p2);
        }

        bool isOtherColor(int p1, int p2) {
            if (getPiece(p1) == none || getPiece(p2) == none) return false;
            return (isWhite(p1) == isBlack(p2) && isBlack(p1) == isWhite(p2));
        }
};

class Move {
    public:
        int piece;
        int startPos;
        int endPos;
        int promo;
        int flag;
        // Constructor
        Move(int p, int s, int e, int r, int f) {
            piece = p;
            startPos = s;
            endPos = e;
            promo = r;
            flag = f;
        }
        // Default constructor
        Move(){}


        // Equality
        bool moveEq(Move m) {
            int mp = m.piece;
            int ms = m.startPos;
            int me = m.endPos;
            int mr = m.promo;
            int mf = m.flag;
            if (
                mp == piece &&
                ms == startPos &&
                me == endPos &&
                mr == promo &&
                mf == flag
            ) return true;
            return false;
        }

        bool same(Move m) {
            if (m.startPos == startPos && m.endPos == endPos) return true;
            return false;
        }
        
        // String rep
        string files[9] = {"a", "b", "c", "d", "e", "f", "g", "h", " "};
        string nums[8] = {"1", "2", "3", "4", "5", "6", "7", "8"};
        string srep() {
            int sx = startPos % 8;
            int sy = startPos / 8;
            int tx = endPos % 8;
            int ty = endPos / 8;
            string result = "";
            result = files[sx] + nums[sy] + files[tx] + nums[ty] /*+ " (" + nums[sx] + nums[sy] + nums[tx] + nums[ty] + ")"*/;
            return result;
        }

        // User input
        void in(int a, int b, int c, int d) {
            startPos = a + b * 8;
            endPos = c + d * 8;
        }
};

class MoveCache {
    public:
    int x;
    bool whiteKingMoved = false;
    bool whiteRookAMoved = false;
    bool whiteRookHMoved = false;
    bool blackKingMoved = false;
    bool blackRookAMoved = false;
    bool blackRookHMoved = false;
    Move prevMove;

    MoveCache(int y, bool a, bool b, bool c, bool d, bool e, bool f, Move m) {
        x = y;
        whiteKingMoved = a;
        whiteRookAMoved = b;
        whiteRookHMoved = c;
        blackKingMoved = d;
        blackRookAMoved = e;
        blackRookHMoved = f;
        prevMove = m;
    }

    MoveCache(){}
};

class SearchCache {
    public:
        float eval;
        Move move;
        int reached;
        int flag;
        int depth;
    SearchCache(float e, Move m, int r, int f, int d) {
        eval = e;
        move = m;
        reached = r;
        flag = f;
        depth = d;
    }
    SearchCache(){}
};

class TTEntry {
    public:
        uint64_t hash;
        SearchCache cache;

        TTEntry(uint64_t h, SearchCache c) {
            hash = h;
            cache = c;
        }
        TTEntry(){}

        bool eq(uint64_t h){
            return (h == hash);
        }
};

mt19937_64 rng(101);

class HashComputer {
    public:
        std::array<std::array<uint64_t, 64>, 12> piecekeys;
        uint64_t blacksmovekey;
        std::array<uint64_t, 4> castlingkeys;
        std::array<uint64_t, 8> enpassantkeys;

        Piece p;

        HashComputer() {
            for (int i = 0; i < 12; ++i) {
                for (int j = 0; j < 64; ++j) {
                    piecekeys[i][j] = random64();
                }
            }

            blacksmovekey = random64();

            for (int i = 0; i < 4; ++i) {
                castlingkeys[i] = random64();
            }

            for (int i = 0; i < 8; ++i) {
                enpassantkeys[i] = random64();
            }
        }

        uint64_t computeHash(int b[64], bool btm, int f[8], bool cr[4]) {
            uint64_t hash = 0;

            for (int s = 0; s < 64; s++) if (p.getPiece(b[s]) != 0) hash ^= piecekeys[p.getHashKey(b[s])][s];
            if (btm) hash ^= blacksmovekey;
            for (int i = 0; i < 4; i++) if (cr[i]) hash ^= castlingkeys[i];
            for (int i = 0; i < 8; i++) if (f[i] == 1) hash ^= enpassantkeys[i];

            return hash;
        }

        uint64_t updateHash(uint64_t hash, Move m, MoveCache x, int f[8], int c[4]) {
            hash ^= blacksmovekey;

            hash ^= piecekeys[m.piece][m.startPos];
            if (x.x != 0) hash ^= piecekeys[x.x][m.endPos];
            hash ^= piecekeys[m.piece][m.endPos];

            for (int i = 0; i < 8; i++) if (f[i] == 1) hash ^= enpassantkeys[i];
            if (m.flag == 2) hash ^= enpassantkeys[m.endPos % 8];
            for (int i = 0; i < 4; i++) if (c[i] == 1) hash ^= castlingkeys[i];

            return hash;
        }

    private:
        uint64_t random64() {
            return rng();
        }
};

class TranspositionTable {
    private:
        static const size_t tsize = 1048576;
        vector<TTEntry> table;
    public:
        void store(uint64_t hash, SearchCache cache) {table[hash % tsize] = TTEntry(hash, cache);}
        bool probe(uint64_t hash) {
            TTEntry e = table[hash % tsize];
            if (e.hash == hash) return true;
            return false;
        }
        SearchCache get(uint64_t hash) {
            TTEntry e = table[hash % tsize];
            return e.cache;
        }
        TranspositionTable() {
            table.resize(tsize); 
            cout << "[DEBUG] Transposition Table created with size: " << table.size() << endl;
        }
};

class Evaluator {
    public:
        Piece p;

        int pawnValue = 100;
        int knightValue = 300;
        int bishopValue = 310;
        int rookValue = 500;
        int queenValue = 900;
        int bishopPairValue = 700;
        int rookPairValue = 1050;
        int kingValue = 7500;

        int pieceValues[6] = {pawnValue, knightValue, bishopValue, rookValue, queenValue, kingValue};

        int checkValue = 110;
        int threatValue = 80;

        float squeeze(float n) {return n / 50;}

        float eval(Board &b);
        float evalPers(Board &b, bool white);
};

class Board {
        public: 
            // Board
            int board[64] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            
            // References
            char numbers[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
            char pieces[12] = {'k', 'K', 'q', 'Q', 'p', 'P', 'n', 'N', 'b', 'B', 'r', 'R'};
            char piecesW[6] = {'P', 'N', 'B', 'R', 'Q', 'K'};
            char piecesB[6] = {'p', 'n', 'b', 'r', 'q', 'k'};
            char files[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
            string filesStr[8] = {"a", "b", "c", "d", "e", "f", "g", "h"};
            int dirOffsets[8] = {8, -8, 1, -1, 7, -7, 9, -9};

            // Game state
            bool whiteToMove = true;
            bool whiteKingMoved = false;
            bool whiteRookAMoved = false;
            bool whiteRookHMoved = false;
            bool blackKingMoved = false;
            bool blackRookAMoved = false;
            bool blackRookHMoved = false;

            // Caches
            uint64_t currentHash;
            HashComputer h;
            Move lastMove;
            bool prevCastling[6] = {false, false, false, false, false, false};
            int whiteKingPos = 4;
            int blackKingPos = 60;
            string startFEN;

            // Classes
            Piece p;
            TranspositionTable tt;
            Evaluator eval;

            void checkCache() {
                if (board[0] != p.rook + p.white) whiteRookAMoved = true;
                if (board[4] != p.king + p.white) whiteKingMoved = true;
                if (board[7] != p.rook + p.white) whiteRookHMoved = true;
                if (board[56] != p.rook + p.black) blackRookAMoved = true;
                if (board[60] != p.king + p.black) blackKingMoved = true;
                if (board[63] != p.rook + p.black) blackRookHMoved = true;
                for (int i = 0; i < 64; i++) {
                    if (p.getPiece(board[i]) == p.king) {
                        if (p.isWhite(board[i])) whiteKingPos = i;
                        if (p.isBlack(board[i])) blackKingPos = i;
                    }
                }
            }

            // Load FEN notation
            void loadFEN(const string& fen) {
                int rank = 7; 
                int file = 0;
                for (char c : fen) {
                    if (c == ' ') break;
                    if (isdigit(c)) {
                        file += c - '0'; 
                    } else if (findc(c, pieces, 12)) {
                        int idx = rank * 8 + file;
                        if (findc(c, piecesW, 6)) board[idx] = indexc(c, piecesW, 6) + 9;
                        else board[idx] = indexc(c, piecesB, 6) + 17;
                        file++;
                    } else if (c == '/') {
                        rank--;
                        file = 0;
                    }
                }
                checkCache();
                int f[8] = {0, 0, 0, 0, 0, 0, 0, 0};
                bool q[4] = {true, true, true, true};
                currentHash = h.computeHash(board, !whiteToMove, f, q);
            }

            // Generate FEN notation
            string toFEN() {
                string fen = "";
                for (int rank = 7; rank >= 0; rank--) {
                    int empty = 0;
                    for (int file = 0; file < 8; file++) {
                        int sq = rank * 8 + file;
                        int piece = board[sq];
                        if (piece == 0) {
                            empty++;
                        } else {
                            if (empty > 0) {
                                fen += to_string(empty);
                                empty = 0;
                            }
                            if (p.isWhite(piece)) {
                                fen += piecesW[p.getPiece(piece) - 1];
                            } else {
                                fen += piecesB[p.getPiece(piece) - 1];
                            }
                        }
                    }
                    if (empty > 0) {
                        fen += to_string(empty);
                    }
                    if (rank > 0) {
                        fen += "/";
                    }
                }
                fen += whiteToMove ? " w" : " b";
                fen += " - - 0 1";
                return fen;
            }
           
            
            // Constructor
            Board(string fen) {
                loadFEN(fen);
                cout << "[DEBUG] Board created with fen " << fen << ". Initial hash: " << currentHash << endl;
            }

            MoveCache makeMove(Move m) {
                MoveCache cache(
                    board[m.endPos],
                    whiteKingMoved,
                    whiteRookAMoved,
                    whiteRookHMoved,
                    blackKingMoved,
                    blackRookAMoved,
                    blackRookHMoved,
                    lastMove
                );
                int x = board[m.endPos];
                board[m.endPos] = m.piece;
                board[m.startPos] = 0;
                if (p.getPiece(m.piece) == p.king) {
                    if (p.isWhite(m.piece)) whiteKingPos = m.endPos;
                    else blackKingPos = m.endPos;
                }
                if (m.promo != 0) board[m.endPos] += m.promo;
                if (m.flag == 4) {
                    if (p.isWhite(m.piece)) board[m.endPos - 8] = 0;
                    else board[m.endPos + 8] = 0;                 
                }
                if (m.flag == 5) {
                    if (m.endPos == 2) {board[3] = board[0]; board[0] = 0;}
                    if (m.endPos == 6) {board[5] = board[7]; board[7] = 0;}
                    if (m.endPos == 58) {board[59] = board[56]; board[56] = 0;}
                    if (m.endPos == 62) {board[61] = board[63]; board[63] = 0;}
                }
                whiteToMove = !whiteToMove;
                bool temp[6] = {whiteKingMoved, whiteRookAMoved, whiteRookHMoved, blackKingMoved, blackRookAMoved, blackRookHMoved};
                bool q[4] = {whiteKingMoved && whiteRookAMoved, whiteKingMoved && whiteRookHMoved, blackKingMoved && blackRookAMoved, blackKingMoved && blackRookHMoved};
                if (m.startPos == 4) whiteKingMoved = true;
                if (m.startPos == 0) whiteRookAMoved = true;
                if (m.startPos == 7) whiteRookHMoved = true;
                if (m.startPos == 60) blackKingMoved = true;
                if (m.startPos == 56) blackRookAMoved = true;
                if (m.startPos == 63) blackRookHMoved = true;
                if (m.endPos == 0) whiteRookAMoved = true;
                if (m.endPos == 7) whiteRookHMoved = true;
                if (m.endPos == 56) blackRookAMoved = true;
                if (m.endPos == 63) blackRookHMoved = true;
                int f[8] = {0, 0, 0, 0, 0, 0, 0, 0};
                if (lastMove.flag == 2) f[lastMove.endPos % 8] = 1;
                int c[6] = {0, 0, 0, 0, 0, 0};
                bool qr[4] = {whiteKingMoved && whiteRookAMoved, whiteKingMoved && whiteRookHMoved, blackKingMoved && blackRookAMoved, blackKingMoved && blackRookHMoved};
                for (int i = 0; i < 4; i++) {
                    if (q[i] != qr[i]) c[i] = 1;
                }
                lastMove = m;
                
                currentHash = h.updateHash(currentHash, m, cache, f, c);
                return cache;
            }
      
            void unmakeMove(Move m, MoveCache c) {
                board[m.endPos] = c.x;
                board[m.startPos] = m.piece;
                if (p.getPiece(m.piece) == p.king) {
                    if (p.isWhite(m.piece)) whiteKingPos = m.startPos;
                    else blackKingPos = m.startPos;
                }
                if (m.flag == 4) {
                    if (p.isWhite(m.piece)) board[m.endPos - 8] = p.pawn + p.black;
                    else board[m.endPos + 8] = p.pawn + p.white;
                }
                if (m.flag == 5) {
                    if (m.endPos == 2) {board[0] = board[3]; board[3] = 0;}
                    if (m.endPos == 6) {board[7] = board[5]; board[5] = 0;}
                    if (m.endPos == 58) {board[56] = board[59]; board[59] = 0;}
                    if (m.endPos == 62) {board[63] = board[61]; board[61] = 0;}
                }
                whiteToMove = !whiteToMove;
                bool temp[6] = {whiteKingMoved, whiteRookAMoved, whiteRookHMoved, blackKingMoved, blackRookAMoved, blackRookHMoved};
                bool q[4] = {whiteKingMoved && whiteRookAMoved, whiteKingMoved && whiteRookHMoved, blackKingMoved && blackRookAMoved, blackKingMoved && blackRookHMoved};
                whiteKingMoved = c.whiteKingMoved;
                whiteRookAMoved = c.whiteRookAMoved;
                whiteRookHMoved = c.whiteRookHMoved;
                blackKingMoved = c.blackKingMoved;
                blackRookAMoved = c.blackRookAMoved;
                blackRookHMoved = c.blackRookHMoved;
                int f[8] = {0, 0, 0, 0, 0, 0, 0, 0};
                if (lastMove.flag == 2) f[lastMove.endPos % 8] = 1;
                int cr[6] = {0, 0, 0, 0, 0, 0};
                bool qr[4] = {whiteKingMoved && whiteRookAMoved, whiteKingMoved && whiteRookHMoved, blackKingMoved && blackRookAMoved, blackKingMoved && blackRookHMoved};
                for (int i = 0; i < 4; i++) {
                    if (q[i] != qr[i]) cr[i] = 1;
                }
                currentHash = h.updateHash(currentHash, m, c, f, cr);
                lastMove = c.prevMove;
            }

            /*   
            MoveCache makeMove(Move m) {
                            // 1. Save the state BEFORE any changes are made.
                            MoveCache cache(
                                board[m.endPos],
                                whiteKingMoved,
                                whiteRookAMoved,
                                whiteRookHMoved,
                                blackKingMoved,
                                blackRookAMoved,
                                blackRookHMoved,
                                lastMove
                            );

                            // 2. Apply all changes to the board and state.
                            board[m.startPos] = 0;
                            board[m.endPos] = m.piece;
                            if (m.promo != 0) {
                                board[m.endPos] += m.promo;
                            }

                            if (m.flag == 4) { // En passant capture
                                if (p.isWhite(m.piece)) board[m.endPos - 8] = 0;
                                else board[m.endPos + 8] = 0;
                            } else if (m.flag == 5) { // Castling move
                                if (m.endPos == 2) { board[3] = board[0]; board[0] = 0; }
                                if (m.endPos == 6) { board[5] = board[7]; board[7] = 0; }
                                if (m.endPos == 58) { board[59] = board[56]; board[56] = 0; }
                                if (m.endPos == 62) { board[61] = board[63]; board[63] = 0; }
                            }
                            
                            whiteToMove = !whiteToMove;
                            lastMove = m;

                            if (p.getPiece(m.piece) == p.king) {
                                if (p.isWhite(m.piece)) whiteKingPos = m.endPos;
                                else blackKingPos = m.endPos;
                            }

                            // Update castling rights (for both moving and capturing)
                            if (m.startPos == 4) whiteKingMoved = true;
                            if (m.startPos == 0 || m.endPos == 0) whiteRookAMoved = true;
                            if (m.startPos == 7 || m.endPos == 7) whiteRookHMoved = true;
                            if (m.startPos == 60) blackKingMoved = true;
                            if (m.startPos == 56 || m.endPos == 56) blackRookAMoved = true;
                            if (m.startPos == 63 || m.endPos == 63) blackRookHMoved = true;

                            // (Your Zobrist hash update logic would go here)

                            return cache;
                        }
                 
            void unmakeMove(Move m, MoveCache c) {
                // 1. Restore the simple state variables first.
                whiteToMove = !whiteToMove;
                lastMove = c.prevMove;

                // 2. Restore all castling rights from the cache.
                whiteKingMoved = c.whiteKingMoved;
                whiteRookAMoved = c.whiteRookAMoved;
                whiteRookHMoved = c.whiteRookHMoved;
                blackKingMoved = c.blackKingMoved;
                blackRookAMoved = c.blackRookAMoved;
                blackRookHMoved = c.blackRookHMoved;

                // 3. Restore the piece positions on the board.
                board[m.startPos] = m.piece;  // Put the moving piece back.
                board[m.endPos] = c.x;        // Put the captured piece back (or 0 if empty).

                // 4. Update the king's cached position.
                if (p.getPiece(m.piece) == p.king) {
                    if (p.isWhite(m.piece)) whiteKingPos = m.startPos;
                    else blackKingPos = m.startPos;
                }

                // 5. Undo special move piece placements.
                if (m.flag == 4) { // En Passant capture
                    if (p.isWhite(m.piece)) board[m.endPos - 8] = p.pawn + p.black;
                    else board[m.endPos + 8] = p.pawn + p.white;
                } else if (m.flag == 5) { // Castling
                    // Undo the rook's move
                    if (m.endPos == 2) { board[0] = board[3]; board[3] = 0; } // White Q-side
                    if (m.endPos == 6) { board[7] = board[5]; board[5] = 0; } // White K-side
                    if (m.endPos == 58) { board[56] = board[59]; board[59] = 0; } // Black Q-side
                    if (m.endPos == 62) { board[63] = board[61]; board[61] = 0; } // Black K-side
                }
}
*/
            
            void updateKingPos(Move m) {
            if (p.isWhite(m.piece)) whiteKingPos = m.endPos;
            else blackKingPos = m.endPos;
        }

            /*
        bool isSquareAttacked(int square, bool byWhite) {
            int direction = byWhite ? -1 : 1;
            int sx = square % 8;
            int sy = square / 8;

            vector<int> pawnOffsets = byWhite ? vector<int>{-9, -7} : vector<int>{7, 9};
            for (int offset : pawnOffsets) {
                int target = square + offset;
                if (target >= 0 && target < 64 && abs(target % 8 - sx) == 1) {
                    int piece = board[target];
                    if (p.getPiece(piece) == p.pawn && p.isWhite(piece) == byWhite)
                        return true;
                }
            }

            int knightOffsets[] = {15, 17, 10, 6, -15, -17, -10, -6};
            for (int offset : knightOffsets) {
                int target = square + offset;
                if (target >= 0 && target < 64) {
                    int piece = board[target];
                    if (p.getPiece(piece) == p.knight && p.isWhite(piece) == byWhite)
                        return true;
                }
            }

            int bdx[4] = { 1,-1, 1,-1}, bdy[4] = { 1, 1,-1,-1};
            for (int k = 0; k < 4; ++k) {
                int x = sx, y = sy;
                while (true) {
                    x += bdx[k]; y += bdy[k];
                    if (x < 0 || x > 7 || y < 0 || y > 7) break;
                    int t = x + 8*y, pc = board[t];
                    if (pc) { if (p.isWhite(pc)==byWhite) { int pt=p.getPiece(pc); if (pt==p.bishop||pt==p.queen) return true; } break; }
                }
            }
            int rdx[4] = { 0, 0, 1,-1}, rdy[4] = { 1,-1, 0, 0};
            for (int k = 0; k < 4; ++k) {
                int x = sx, y = sy;
                while (true) {
                    x += rdx[k]; y += rdy[k];
                    if (x < 0 || x > 7 || y < 0 || y > 7) break;
                    int t = x + 8*y, pc = board[t];
                    if (pc) { if (p.isWhite(pc)==byWhite) { int pt=p.getPiece(pc); if (pt==p.rook||pt==p.queen) return true; } break; }
                }
            }

            int kingOffsets[] = {-9, -8, -7, -1, 1, 7, 8, 9};
            for (int offset : kingOffsets) {
                int target = square + offset;
                int tx = target % 8;
                int ty = target / 8;
                if (target >= 0 && target < 64) {
                    int piece = board[target];
                    if (abs(tx - sx) > 1) continue;
                    if (abs(ty - sy) > 1) continue;
                    if (p.getPiece(piece) == p.king && p.isWhite(piece) == byWhite)
                        return true;
                }
            }

            return false;
        }
        */
        
            bool isSquareAttacked(int square, bool byWhite, int b[64]) {
                int pawnAttackOffset1 = byWhite ? -9 : 7;
                int pawnAttackOffset2 = byWhite ? -7 : 9;
                int startFile = square % 8;
                if ((square + pawnAttackOffset1) >= 0 && (square + pawnAttackOffset1) < 64) {
                    int attackerSquare = square + pawnAttackOffset1;
                    int attackerFile = attackerSquare % 8;
                    if (abs(startFile - attackerFile) == 1) {
                        int piece = b[attackerSquare];
                        if (p.getPiece(piece) == p.pawn && p.isWhite(piece) == byWhite) return true;
                    }
                }
                if ((square + pawnAttackOffset2) >= 0 && (square + pawnAttackOffset2) < 64) {
                    int attackerSquare = square + pawnAttackOffset2;
                    int attackerFile = attackerSquare % 8;
                    if (abs(startFile - attackerFile) == 1) {
                        int piece = b[attackerSquare];
                        if (p.getPiece(piece) == p.pawn && p.isWhite(piece) == byWhite) return true;
                    }
                }
                int knightOffsets[] = {-17, -15, -10, -6, 6, 10, 15, 17};
                for (int offset : knightOffsets) {
                    int target = square + offset;
                    if (target >= 0 && target < 64) {
                        int file_diff = abs((square % 8) - (target % 8));
                        if (file_diff > 2) continue;
                        int piece = b[target];
                        if (p.getPiece(piece) == p.knight && p.isWhite(piece) == byWhite) return true;
                    }
                }
                int dirOffsets[] = {8, -8, 1, -1, 7, -7, 9, -9};
                for (int i = 0; i < 8; i++) {
                    int offset = dirOffsets[i];
                    int target = square;
                    for (int j = 0; j < 8; j++) {
                        target += offset;
                        if (target < 0 || target >= 64) break;
                        int file_diff = abs((target % 8) - ((target - offset) % 8));
                        if (file_diff > 1) break;
                        int pieceOnTarget = b[target];
                        if (pieceOnTarget != 0) {
                            if (p.isWhite(pieceOnTarget) == byWhite) {
                                int pieceType = p.getPiece(pieceOnTarget);
                                if ((i < 4 && (pieceType == p.rook || pieceType == p.queen)) ||
                                    (i >= 4 && (pieceType == p.bishop || pieceType == p.queen))) {
                                    return true;
                                }
                            }
                            break;
                        }
                    }
                }
                int kingOffsets[] = {-9, -8, -7, -1, 1, 7, 8, 9};
                for (int offset : kingOffsets) {
                    int target = square + offset;
                    if (target >= 0 && target < 64) {
                        int file_diff = abs((square % 8) - (target % 8));
                        if (file_diff > 1) continue;

                        int piece = b[target];
                        if (p.getPiece(piece) == p.king && p.isWhite(piece) == byWhite) return true;
                    }
                }

                return false;
            }
            
            bool isSquareAttackedRange(int start, int end, bool byWhite) {
                bool result = false;
                for (int i = start; i < end + 1; i++) if (isSquareAttacked(i, byWhite, board)) result = true;
                return result;
            }
            
            bool isLegal(Move move) {
                int temp[64];
                copy(board, board + 64, temp);
                int tp = temp[move.endPos];
                if (p.isSameColor(tp, move.piece)) return false;
                temp[move.startPos] = 0;
                temp[move.endPos] = move.piece;
                if (move.promo != 0) temp[move.endPos] += move.promo;
                if (move.flag == 4) {
                    if (p.isWhite(move.piece)) temp[move.endPos - 8] = 0;
                    else temp[move.endPos + 8] = 0;                 
                }
                if (move.flag == 5) {
                    if (move.endPos == 2) {temp[3] = temp[0]; temp[0] = 0;}
                    if (move.endPos == 6) {temp[5] = temp[7]; temp[7] = 0;}
                    if (move.endPos == 58) {temp[59] = temp[56]; temp[56] = 0;}
                    if (move.endPos == 62) {temp[61] = temp[63]; temp[63] = 0;}
                }
                int kingPos = -1;
                int king = whiteToMove? p.white + p.king : p.black + p.king;
                for (int i = 0; i < 64; i++) if (temp[i] == king) kingPos = i;
                if (kingPos == -1) return false;
                bool inCheck = isSquareAttacked(kingPos, !whiteToMove, temp);
                return !inCheck;
            }

            int get(int s) {
            if (0 > s || s > 63) return -1;
            return board[s];
        }

            void srep() {
                cout << endl << "-------------------" << endl;
                for (int i = 0; i < 8; i++) {
                    string s = to_string(8 - i);
                    s += " ";
                    for (int j = 7; j >= 0; j--) {
                        int sq = 63 - (j + i * 8);
                        int t = board[sq];
                        if (p.isWhite(t)) s += piecesW[p.getPiece(t) - 1];
                        else if (p.isBlack(t)) s += piecesB[p.getPiece(t) - 1];
                        else s += '-';
                        s += ' ';
                    }
                    s += "+";
                    cout << s << endl;
                }
                cout << "- a b c d e f g h -" << endl << endl;
            }

            vector<Move> generateMoves();
            vector<Move> generateCaptures();
};

class Generator {
    public:
        Piece piece;

        vector<Move> generateSlidingMoves(Board& b, int p, int s) {
            vector<Move> moves;
            int start = 0;
            int end = 8;
            int sx = s % 8;
            int sy = s / 8;
            int t = s;
            if (piece.getPiece(p) == piece.rook) end = 4;
            else if (piece.getPiece(p) == piece.bishop) start = 4;
            for (int dindex = start; dindex < end; dindex++) {
                t = s;
                int tx = sx;
                int ty = sy;
                int ptx = tx;
                int i = 0;
                while (0 <= tx && tx < 8 && 0 <= ty && ty < 8) {
                    t += b.dirOffsets[dindex];
                    if (0 > t || t > 63) break;
                    tx = t % 8;
                    ty = t / 8;
                    int q = t - b.dirOffsets[dindex];
                    int f = abs((t % 8) - (q % 8));
                    if (f > 1) {
                        break;
                    }
                    if (piece.isSameColor(p, b.board[t])) break;
                    moves.push_back(Move(p, s, t, 0, 0));
                    if (piece.isOtherColor(p, b.board[t])) break;
                    ptx = tx;
                    i++;
                }
            }
            return moves;
        }

        vector<Move> generateKnightMoves(Board& b, int p, int s) {
            vector<Move> moves;
            int x = s % 8;
            int y = s / 8;
            const int dx[8] = {2, 2, -2, -2, 1, 1, -1, -1};
            const int dy[8] = {1, -1, 1, -1, 2, -2, 2, -2};
            for (int i = 0; i < 8; i++) {
                int tx = x + dx[i];
                int ty = y + dy[i];
                if (tx < 0 || tx > 7 || ty < 0 || ty > 7) continue;
                int t = tx + 8 * ty;
                if (t < 0 || t > 63) continue;
                if (!piece.isSameColor(p, b.board[t])) {
                    moves.push_back(Move(p, s, t, 0, 0));
                }
            }
            return moves;
        }

        vector<Move> generatePawnMoves(Board& b, int p, int s) {
            vector<Move> moves;
            int rank = s / 8;
            int file = s % 8;
            int direction = b.whiteToMove ? 1 : -1;
            int startRank = b.whiteToMove ? 1 : 6;
            int promotionRank = b.whiteToMove ? 7 : 0;
            int t = s + (8 * direction);
            if (t >= 0 && t < 64 && b.board[t] == 0) {
                if (rank == promotionRank - direction) {
                    moves.push_back(Move(p, s, t, 1, 1));
                    moves.push_back(Move(p, s, t, 2, 1));
                    moves.push_back(Move(p, s, t, 3, 1));
                    moves.push_back(Move(p, s, t, 4, 1));
                } else {
                    moves.push_back(Move(p, s, t, 0, 0));
                }
                if (rank == startRank) {
                    int d = s + (16 * direction);
                    if (b.board[d] == 0) {
                        moves.push_back(Move(p, s, d, 0, 2));
                    }
                }
            }
            int coff[] = {-1, 1};
            for (int offset : coff) {
                int c = s + (8 * direction) + offset;
                if (c < 0 || c >= 64) continue;
                if (abs(file - (c % 8)) != 1) continue;
                if (b.board[c] != 0 && !piece.isSameColor(p, b.board[c])) {
                    if (rank == promotionRank - direction) { 
                        moves.push_back(Move(p, s, c, 1, 3));
                        moves.push_back(Move(p, s, c, 2, 3));
                        moves.push_back(Move(p, s, c, 3, 3));
                        moves.push_back(Move(p, s, c, 4, 3));
                    } else {
                        moves.push_back(Move(p, s, c, 0, 3));
                    }
                }
            }
            if (b.lastMove.flag == 2) {
                int ept = b.lastMove.endPos;
                int epr = (b.whiteToMove) ? 4 : 3;
                if (abs(s % 8 - ept % 8) == 1 && (int) s / 8 == epr) {
                    int t = ept + (8 * direction);
                    moves.push_back(Move(p, s, t, 0, 4));
                }
            }
            return moves;
        }

        vector<Move> generateKingMoves(Board& b, int p, int s) {
            vector<Move> moves;
            for (int dindex = 0; dindex < 8; dindex++) {
                int sx = s % 8;
                int sy = s / 8;
                int dir = b.dirOffsets[dindex];
                int t = s + dir;
                int tx = t % 8;
                int ty = t / 8;
                bool nowrap = true;
                if (abs(tx - sx) != 1 && abs(tx - sx) != 0) nowrap = false;
                if (0 <= t && t <= 63 && nowrap) if (!piece.isSameColor(p, b.board[t])) moves.push_back(Move(p, s, t, 0, 0));
            }
            if (
                !b.whiteKingMoved && !b.whiteRookAMoved &&
                b.board[1] == 0 && b.board[2] == 0 && b.board[3] == 0 &&
                !b.isSquareAttackedRange(2, 4, false) &&
                b.whiteToMove
            ) moves.push_back(Move(p, 4, 2 , 0, 5));
            if (
                !b.whiteKingMoved && !b.whiteRookHMoved &&
                b.board[5] == 0 && b.board[6] == 0 &&
                !b.isSquareAttackedRange(4, 6, false) &&
                b.whiteToMove
            ) moves.push_back(Move(p, 4, 6 , 0, 5));
            if (
                !b.blackKingMoved && !b.blackRookAMoved &&
                b.board[57] == 0 && b.board[58] == 0 && b.board[59] == 0 &&
                !b.isSquareAttackedRange(58, 60, true) &&
                !b.whiteToMove
            ) moves.push_back(Move(p, 60, 58 , 0, 5));
            if (
                !b.blackKingMoved && !b.blackRookHMoved &&
                b.board[61] == 0 && b.board[62] == 0 &&
                !b.isSquareAttackedRange(60, 62, true) &&
                !b.whiteToMove
            ) moves.push_back(Move(p, 60, 62 , 0, 5));

            return moves;
        }

        vector<Move> generateSlidingCaptures(Board& b, int p, int s) {
            vector<Move> moves;
            int start = 0;
            int end = 8;
            int sx = s % 8;
            int sy = s / 8;
            int t = s;
            if (piece.getPiece(p) == piece.rook) end = 4;
            else if (piece.getPiece(p) == piece.bishop) start = 4;
            for (int dindex = start; dindex < end; dindex++) {
                t = s;
                int tx = sx;
                int ty = sy;
                int ptx = tx;
                int i = 0;
                while (0 <= tx && tx < 8 && 0 <= ty && ty < 8) {
                    t += b.dirOffsets[dindex];
                    if (0 > t || t > 63) break;
                    tx = t % 8;
                    ty = t / 8;
                    int q = t - b.dirOffsets[dindex];
                    int f = abs((t % 8) - (q % 8));
                    if (f > 1) {
                        break;
                    }
                    if (piece.isSameColor(p, b.board[t])) break;
                    if (piece.isOtherColor(p, b.board[t])) {
                        moves.push_back(Move(p, s, t, 0, 0));
                        break;
                    }
                    ptx = tx;
                    i++;
                }
            }
            return moves;
        }

        vector<Move> generateKnightCaptures(Board& b, int p, int s) {
            vector<Move> moves;
            int x = s % 8;
            int y = s / 8;
            const int dx[8] = {2, 2, -2, -2, 1, 1, -1, -1};
            const int dy[8] = {1, -1, 1, -1, 2, -2, 2, -2};
            for (int i = 0; i < 8; i++) {
                int tx = x + dx[i];
                int ty = y + dy[i];
                if (tx < 0 || tx > 7 || ty < 0 || ty > 7) continue;
                int t = tx + 8 * ty;
                if (t < 0 || t > 63) continue;
                if (!piece.isSameColor(p, b.board[t]) && piece.getPiece(b.board[t]) != 0) {
                    moves.push_back(Move(p, s, t, 0, 0));
                }
            }
            return moves;
        }

        vector<Move> generatePawnCaptures(Board& b, int p, int s) {
            vector<Move> moves;
            int rank = s / 8;
            int file = s % 8;
            int direction = b.whiteToMove ? 1 : -1;
            int startRank = b.whiteToMove ? 1 : 6;
            int promotionRank = b.whiteToMove ? 7 : 0;
            int t = s + (8 * direction);
            int coff[] = {-1, 1};
            for (int offset : coff) {
                int c = s + (8 * direction) + offset;
                if (c < 0 || c >= 64) continue;
                if (abs(file - (c % 8)) != 1) continue;
                if (b.board[c] != 0 && !piece.isSameColor(p, b.board[c])) {
                    if (rank == promotionRank - direction) { 
                        moves.push_back(Move(p, s, c, 1, 3));
                        moves.push_back(Move(p, s, c, 2, 3));
                        moves.push_back(Move(p, s, c, 3, 3));
                        moves.push_back(Move(p, s, c, 4, 3));
                    } else {
                        moves.push_back(Move(p, s, c, 0, 3));
                    }
                }
            }
            if (b.lastMove.flag == 2) {
                int ept = b.lastMove.endPos;
                int epr = (b.whiteToMove) ? 4 : 3;
                if (abs(s % 8 - ept % 8) == 1 && (int) s / 8 == epr) {
                    int t = ept + (8 * direction);
                    moves.push_back(Move(p, s, t, 0, 4));
                }
            }
            return moves;
        }

        vector<Move> generateKingCaptures(Board& b, int p, int s) {
            vector<Move> moves;
            for (int dindex = 0; dindex < 8; dindex++) {
                int sx = s % 8;
                int sy = s / 8;
                int dir = b.dirOffsets[dindex];
                int t = s + dir;
                int tx = t % 8;
                int ty = t / 8;
                bool nowrap = true;
                if (abs(tx - sx) != 1 && abs(tx - sx) != 0) nowrap = false;
                if (0 <= t && t <= 63 && nowrap) if (!piece.isSameColor(p, b.board[t]) && piece.getPiece(b.board[t]) != 0) moves.push_back(Move(p, s, t, 0, 0));
            }
            return moves;
        }
};

vector<Move> Board::generateMoves() {
    vector<Move> plmoves;
    vector<Move> moves;
    Generator generator;
    for (int i = 0; i < 64; i++) {
        int tp = board[i];
        if (p.getPiece(tp) != 0 && p.isWhite(tp) == whiteToMove) {
            int pt = p.getPiece(tp);
            vector<Move> cmoves;
            if (pt == p.bishop || pt == p.rook || pt == p.queen) cmoves = generator.generateSlidingMoves(*this, tp, i);
            else if (pt == p.knight) cmoves = generator.generateKnightMoves(*this, tp, i);
            else if (pt == p.pawn) cmoves = generator.generatePawnMoves(*this, tp, i);
            else if (pt == p.king) cmoves = generator.generateKingMoves(*this, tp, i);
            plmoves.insert(plmoves.end(), cmoves.begin(), cmoves.end());
        }
    }
    for (Move move : plmoves) {
        if (isLegal(move)) moves.push_back(move);
    }
    return moves;
}

vector<Move> Board::generateCaptures() {
    vector<Move> plmoves;
    vector<Move> moves;
    Generator generator;
    for (int i = 0; i < 64; i++) {
        int tp = board[i];
        if (p.getPiece(tp) != 0 && p.isWhite(tp) == whiteToMove) {
            int pt = p.getPiece(tp);
            vector<Move> cmoves;
            if (pt == p.bishop || pt == p.rook || pt == p.queen) cmoves = generator.generateSlidingCaptures(*this, tp, i);
            else if (pt == p.knight) cmoves = generator.generateKnightCaptures(*this, tp, i);
            else if (pt == p.pawn) cmoves = generator.generatePawnCaptures(*this, tp, i);
            else if (pt == p.king) cmoves = generator.generateKingCaptures(*this, tp, i);
            plmoves.insert(plmoves.end(), cmoves.begin(), cmoves.end());
        }
    }
    for (Move move : plmoves) {
        if (isLegal(move)) moves.push_back(move);
    }
    return moves;
}

float Evaluator::eval(Board& b) {
    int whiteEval = 0;
    int blackEval = 0;
    int whitep[6] = {0, 0, 0, 0, 0, 0};
    int blackp[6] = {0, 0, 0, 0, 0, 0};
    int wpos = 0;
    int bpos = 0;

    for (int i = 0; i < 64; i++) {
        int t = b.board[i];
        int pt = p.getPiece(t);
        if (p.isWhite(t)) {
               if (pt == 1) whitep[0] += 1; wpos += p.pawnMapW[i];
               if (pt == 2) whitep[1] += 1; wpos += p.knightMapW[i];
               if (pt == 3) whitep[2] += 1; wpos += p.bishopMapW[i];
               if (pt == 4) whitep[3] += 1; wpos += p.rookMapW[i];
               if (pt == 5) whitep[4] += 1; wpos += p.queenMapW[i];
        } else if (p.isBlack(t)) {
            if (pt == 1) blackp[0] += 1; bpos += p.pawnMapB[i];
            if (pt == 2) blackp[1] += 1; bpos += p.knightMapB[i];
            if (pt == 3) blackp[2] += 1; bpos += p.bishopMapB[i];
            if (pt == 4) blackp[3] += 1; bpos += p.rookMapB[i];
            if (pt == 5) blackp[4] += 1; bpos += p.queenMapB[i];
        }
    }
    for (int i = 0; i < 64; i++) {
        int t = b.board[i];
        int pt = p.getPiece(t);
        if (pt == 6) {
            if (p.isWhite(t)) {
                if (whitep[4] == 0 && whitep[3] == 0 && blackp[4] == 0 && blackp[3] == 0) wpos += p.kingMapWMid[i];
                else wpos += p.kingMapWEnd[i];
            } else if (p.isBlack(t)) {
                if (whitep[4] == 0 && whitep[3] == 0 && blackp[4] == 0 && blackp[3] == 0) bpos += p.kingMapBMid[i];
                else bpos += p.kingMapBEnd[i];
            }
        }
    }

    for (int i = 0; i < 6; i++) {
       if (i == 2 && whitep[2] == 2) whiteEval += bishopPairValue;
        else if (i == 3 && whitep[3] == 2) whiteEval += rookPairValue;
        else whiteEval += whitep[i] * pieceValues[i];

        if (i == 2 && blackp[2] == 2) blackEval += bishopPairValue;
        else if (i == 3 && blackp[3] == 2) blackEval += rookPairValue;
        else blackEval += blackp[i] * pieceValues[i];
    }

    whiteEval += wpos / 2;
    blackEval += bpos / 2;

    return squeeze(whiteEval + blackEval);
}

float Evaluator::evalPers(Board& b, bool white) {
    int pers = white ? 1 : -1;
    return eval(b) * pers;
}

int scoreMove(const Move& m, Board& b) {
    int cp = b.board[m.endPos];
    int score = 0;
    if (cp > 8) {
        int vval = b.eval.pieceValues[b.p.getPiece(cp) - 1];
        int aval = b.eval.pieceValues[b.p.getPiece(m.piece) - 1];
        score += 10 * vval - aval;
    }
    if (m.promo != 0) {
        score += b.eval.pieceValues[m.piece + m.promo];
    }
    if (b.p.isSameColor(cp, m.piece)) return -INFINITY;
    return score;
}

void orderMoves(vector<Move>& moves, Board& b) {
    sort(moves.begin(), moves.end(), 
        [&b](const Move& x, const Move& y) { return scoreMove(x, b) > scoreMove(y, b); }
    );
}

void perft(Board& b, int depth, long long& nodes);

void perftsplit(Board& b, int depth) {
    long long totalNodes = 0;
    cout << "--- Perft Split for Depth " << depth << " ---" << endl;

    vector<Move> moves = b.generateMoves();

    for (Move& move : moves) {
        long long nodes_for_this_move = 0;
        MoveCache cache = b.makeMove(move);

        bool wasWhitesTurn = !b.whiteToMove;
        int kingPos = wasWhitesTurn ? b.whiteKingPos : b.blackKingPos;

        if (!b.isSquareAttacked(kingPos, b.whiteToMove, b.board)) {
            perft(b, depth - 1, nodes_for_this_move);
            totalNodes += nodes_for_this_move;
            cout << move.srep() << ": " << nodes_for_this_move << endl;
        }
        
        b.unmakeMove(move, cache);
    }
    cout << "---------------------------------" << endl;
    cout << "Total Nodes: " << totalNodes << endl;
}

void perft(Board& b, int depth, long long& nodes) {
    if (depth == 0) {
        nodes++;
        return;
    }

    vector<Move> moves = b.generateMoves();

    for (const Move& move : moves) {
        MoveCache cache = b.makeMove(move);

        bool wasWhitesTurn = !b.whiteToMove;
        int kingPos = wasWhitesTurn ? b.whiteKingPos : b.blackKingPos;
        
        if (!b.isSquareAttacked(kingPos, b.whiteToMove, b.board)) {
            perft(b, depth - 1, nodes);
        }
        
        b.unmakeMove(move, cache);
    }
}

SearchCache searchCaptures(float alpha, float beta, Board& b, int depth) {
    if (b.tt.probe(b.currentHash)) {auto c = b.tt.get(b.currentHash); if (c.depth >= depth) return c;}
    float oAlpha = alpha;
    float eval = b.eval.evalPers(b, b.whiteToMove);
    if (depth == 0) return SearchCache(eval, Move(-1, -1, -1, -1, -1), 0, 0, 0);
    if (eval >= beta) return SearchCache(beta, Move(-1, -1, -1, -1, -1), 0, 0, depth);
    alpha = max(alpha, eval);
    vector<Move> moves = b.generateCaptures();
    orderMoves(moves, b);
    if (moves.size() == 0) return SearchCache(eval, Move(-1, -1, -1, -1, -1), 0, 0, depth);
    Move bestMove = moves[0];
    for (Move move : moves) {
        MoveCache x = b.makeMove(move);
        SearchCache cache = searchCaptures(-beta, -alpha, b, depth - 1);
        cache.eval = -cache.eval;
        b.unmakeMove(move, x);
        if (cache.eval >= beta) {
            b.tt.store(b.currentHash, cache);
            return SearchCache(beta, move, 0, 2, depth);
        }
        if (alpha < cache.eval) {
            alpha = cache.eval;
            bestMove = move;
        }
    }
    int flag;
    if (alpha <= oAlpha) flag = 1;
    else if (alpha >= beta) flag = 2;
    else flag = 0;
    SearchCache result(alpha, bestMove, 0, flag, depth);
    b.tt.store(b.currentHash, result);
    return result;
}

SearchCache search(int depth, float alpha, float beta, Board& b) {
    if (b.tt.probe(b.currentHash)) {auto c = b.tt.get(b.currentHash); if (c.depth >= depth) return c;}
    float oAlpha = alpha;
    if (depth == 0) return searchCaptures(alpha, beta, b, 10);
    vector<Move> moves = b.generateMoves();
    orderMoves(moves, b);
    if (moves.size() == 0) {
        int kingpos = (b.whiteToMove) ? b.whiteKingPos : b.blackKingPos;
        if (b.isSquareAttacked(kingpos, !b.whiteToMove, b.board)) return SearchCache(-INFINITY, Move(-1, -1, -1, -1, -1), 0, -1, depth);
        return SearchCache(0, Move(-1, -1, -1, -1, -1), 0, -2, depth);
    }
    Move bestMove = moves[0];
    for (Move move : moves) {
        MoveCache x = b.makeMove(move);
        int kingpos = (!b.whiteToMove) ? b.whiteKingPos : b.blackKingPos;
        SearchCache cache = search(depth - 1, -beta, -alpha, b);
        cache.eval = -cache.eval;
        b.unmakeMove(move, x);
        if (cache.eval >= beta) {
            b.tt.store(b.currentHash, cache);
            return SearchCache(beta, move, 0, 2, depth);
        }
        if (alpha < cache.eval) {
            alpha = cache.eval;
            bestMove = move;
        }
    }
    int flag;
    if (alpha <= oAlpha) flag = 1;
    else if (alpha >= beta) flag = 2;
    else flag = 0;
    SearchCache result(alpha, bestMove, 0, flag, depth);
    b.tt.store(b.currentHash, result);
    return result;
}

int perft(int depth) {
    auto start = chrono::steady_clock::now();
    auto sthr = chrono::system_clock::now();
    cout << "Execution started at " << formatTimePoint(sthr) << ". " << endl << endl;
    string sp = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    string tstp = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1";
    Board testboard(sp);
    cout << "Board initialized with FEN. Running PERFT. " << endl;
    
    long long ct = 0;
    perft(testboard, depth, ct);
    auto end = chrono::steady_clock::now();
    auto edhr = chrono::system_clock::now();
    cout << "PERFT finished with depth " << to_string(depth) << "." << endl;
    cout << "Result: " + to_string(ct) << endl << endl;
    chrono::duration<double> elapsed = end - start;
    cout << "Execution finished at " << formatTimePoint(edhr) << ". " << endl;
    cout << "Time elapsed: " << elapsed.count() << " seconds." << endl;
    return 0;
}

int perftsplit(int depth) {
    auto start = chrono::steady_clock::now();
    auto sthr = chrono::system_clock::now();
    cout << "Execution started at " << formatTimePoint(sthr) << ". " << endl << endl;
    string sp = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    string tstp = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1";
    Board testboard(sp);
    cout << "Board initialized with FEN. Running PERFT. " << endl;

    perftsplit(testboard, depth);
    auto end = chrono::steady_clock::now();
    auto edhr = chrono::system_clock::now();
    cout << "PERFT finished with depth " << to_string(depth) << "." << endl;
    chrono::duration<double> elapsed = end - start;
    cout << "Execution finished at " << formatTimePoint(edhr) << ". " << endl;
    cout << "Time elapsed: " << elapsed.count() << " seconds." << endl;
    return 0;
}

int tocd(string f, Board& b) {
    return indexstr(f, b.filesStr, 8);
}

int game(int ply) {
    string sp = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    string tstp = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1";
    Board game(sp);

    std::this_thread::sleep_for(std::chrono::seconds(5));
    system("cls");

    while (true) {
        bool moveFound = false;
        game.srep();
        cout << "Your move. " << endl;
        int b, d;
        string a, c;
        Move move;
        cin >> a >> b >> c >> d;
        int ai = tocd(a, game);
        int ci = tocd(c, game);
        move.in(ai, b - 1, ci, d - 1);
        vector<Move> moves = game.generateMoves();
        orderMoves(moves, game);
        for (Move m : moves) { 
            if (move.same(m)) {
                game.makeMove(m); 
                moveFound = true;
                break;
            }
        }
        if (!moveFound) {
            system("cls");
            cout << "Invalid move. Please enter a valid move. " << endl;
            continue;
        }
        SearchCache m = search(ply, -INFINITY, INFINITY, game);
        MoveCache u = game.makeMove(m.move);
        system("cls");
        cout << "Bot's eval: " << to_string(m.eval) << ". Bot played: " << m.move.srep() << endl;
        cout << "FEN: " << game.toFEN() << endl;
    }

    return 0;
}

int main() {
    return perftsplit(4);
}
