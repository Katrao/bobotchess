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
#ifdef _MSC_VER 
#include <intrin.h>
#endif

// For std functions
using namespace std;
// Renaming for readability
typedef int64_t big;
typedef uint64_t u64;
// Format time points (not C++20 format version, backward-compatible)
string formatTimePoint(chrono::system_clock::time_point tp) {
    time_t t = chrono::system_clock::to_time_t(tp);
    tm tms;
    #ifdef _MSC_VER
        localtime_s(&tms, &t);
    #else
        localtime_r(&t, &tms);
    #endif
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tms);
    return string(buffer);
}

// Bitboard utils
#define getBit(bitboard, square) ((bitboard) & (1ULL << (square)))
#define setBit(bitboard, square) ((bitboard) |= (1ULL << (square)))
#define popBit(bitboard, square) ((bitboard) &= ~(1ULL << (square)))
inline int getLSB(u64 bitboard) {
    #ifdef _MSC_VER
        unsigned long index;
        if (_BitScanForward64(&index, bitboard))
            return index;
        else
            return -1;
    #else
        if (bitboard)
            return __builtin_ctzll(bitboard);
        else
            return -1;
    #endif
}
inline int popcount(u64 bitboard) {
    #ifdef _MSC_VER
        return __popcnt64(bitboard);
    #else
        return __builtin_popcountll(bitboard);
    #endif
}
void printBitboard(u64 bitboard) {
    cout << "  +-----------------+" << endl;
    for (int rank = 7; rank >= 0; rank--) {
        cout << rank + 1 << " | ";
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            cout << (getBit(bitboard, square) ? "X " : ". ");
        }
        cout << "|" << endl;
    }
    cout << "  +-----------------+" << endl;
    cout << "    a b c d e f g h" << endl;
    cout << "Bitboard Decimal: " << bitboard << endl;
}

// Enums 
enum Square {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8, no_sq
};
enum Piece {

};

// Piece maps and information
const int pawnMapW[64] = {
            0, 0, 0, 0, 0, 0, 0, 0,
            50, 50, 50, 50, 50, 50, 50, 50,
            10, 10, 20, 30, 30, 20, 10, 10,
            5, 5, 10, 25, 25, 10, 5, 5,
            0, 0, 0, 20, 20, 0, 0, 0,
            5, -5, -10, 0, 0, -10, -5, 5,
            5, 30, 30, -20, -20, 10, 10, 5,
            0, 0, 0, 0, 0, 0, 0, 0
        };
const int pawnMapB[64] = {
            0, 0, 0, 0, 0, 0, 0, 0,
            5, 30, 30, -20, -20, 10, 10, 5,
            5, -5, -10, 0, 0, -10, -5, 5,
            0, 0, 0, 20, 20, 0, 0, 0,
            5, 5, 10, 25, 25, 10, 5, 5,
            10, 10, 20, 30, 30, 20, 10, 10,
            50, 50, 50, 50, 50, 50, 50, 50,
            0, 0, 0, 0, 0, 0, 0, 0
        };

const int knightMapW[64] = {
            -50, -40, -30, -30, -30, -30, -40, -50,
            -40, -20, 0, 0, 0, 0, -20, -40,
            -30, 0, 10, 15, 15, 10, 0, -30,
            -30, 5, 15, 20, 20, 15, 5, -30,
            -30, 0, 15, 20, 20, 15, 0, -30,
            -30, 5, 10, 15, 15, 10, 5, -30,
            -40, -20, 0, 5, 5, 0, -20, -40,
            -50, -40, -30, -30, -30, -30, -40, -50
        };
const int knightMapB[64] = {
            -50, -40, -30, -30, -30, -30, -40, -50,
            -40, -20, 0, 5, 5, 0, -20, -40,
            -30, 5, 10, 15, 15, 10, 5, -30,
            -30, 0, 15, 20, 20, 15, 0, -30,
            -30, 5, 15, 20, 20, 15, 5, -30,
            -30, 0, 10, 15, 15, 10, 0, -30,
            -40, -20, 0, 0, 0, 0, -20, -40,
            -50, -40, -30, -30, -30, -30, -40, -50
        };

const int bishopMapW[64] = {
            -20, -10, -10, -10, -10, -10, -10, -20,
            -10, 0, 0, 0, 0, 0, 0, -10,
            -10, 0, 5, 10, 10, 5, 0, -10,
            -10, 5, 5, 10, 10, 5, 5, -10,
            -10, 0, 10, 10, 10, 10, 0, -10,
            -10, 10, 10, 10, 10, 10, 10, -10,
            -10, 5, 0, 0, 0, 0, 5, -10,
            -20, -10, -10, -10, -10, -10, -10, -20
        };
const int bishopMapB[64] = {
            -20, -10, -10, -10, -10, -10, -10, -20,
            -10, 5, 0, 0, 0, 0, 5, -10,
            -10, 10, 10, 10, 10, 10, 10, -10,
            -10, 0, 10, 10, 10, 10, 0, -10,
            -10, 5, 5, 10, 10, 5, 5, -10,
            -10, 0, 5, 10, 10, 5, 0, -10,
            -10, 0, 0, 0, 0, 0, 0, -10,
            -20, -10, -10, -10, -10, -10, -10, -20
        };

const int rookMapW[64] = {
            0, 0, 0, 0, 0, 0, 0, 0,
            5, 10, 10, 10, 10, 10, 10, 5,
            -5, 0, 0, 0, 0, 0, 0, -5,
            -5, 0, 0, 0, 0, 0, 0, -5,
            -5, 0, 0, 0, 0, 0, 0, -5,
            -5, 0, 0, 0, 0, 0, 0, -5,
            -5, 0, 0, 0, 0, 0, 0, -5,
            0, 0, 0, 5, 5, 0, 0, 0
        };
const int rookMapB[64] = {
            0, 0, 0, 5, 5, 0, 0, 0,
            -5, 0, 0, 0, 0, 0, 0, -5,
            -5, 0, 0, 0, 0, 0, 0, -5,
            -5, 0, 0, 0, 0, 0, 0, -5,
            -5, 0, 0, 0, 0, 0, 0, -5,
            -5, 0, 0, 0, 0, 0, 0, -5,
            5, 10, 10, 10, 10, 10, 10, 5,
            0, 0, 0, 0, 0, 0, 0, 0
        };

const int queenMapW[64] = {
            -20, -10, -10, -5, -5, -10, -10, -20,
            -10, 0, 0, 0, 0, 0, 0, -10,
            -10, 0, 5, 5, 5, 5, 0, -10,
            -5, 0, 5, 5, 5, 5, 0, -5,
            0, 0, 5, 5, 5, 5, 0, -5,
            -10, 5, 5, 5, 5, 5, 0, -10,
            -10, 0, 5, 0, 0, 0, 0, -10,
            -20, -10, -10, -5, -5, -10, -10, -20
        };
const int queenMapB[64] = {
            -20, -10, -10, -5, -5, -10, -10, -20,
            -10, 0, 5, 0, 0, 0, 0, -10,
            -10, 5, 5, 5, 5, 5, 0, -10,
            0, 0, 5, 5, 5, 5, 0, -5,
            -5, 0, 5, 5, 5, 5, 0, -5,
            -10, 0, 5, 5, 5, 5, 0, -10,
            -10, 0, 0, 0, 0, 0, 0, -10,
            -20, -10, -10, -5, -5, -10, -10, -20
        };

const int kingMapWMid[64] = {
            -30, -40, -40, -50, -50, -40, -40, -30,
            -30, -40, -40, -50, -50, -40, -40, -30,
            -30, -40, -40, -50, -50, -40, -40, -30,
            -30, -40, -40, -50, -50, -40, -40, -30,
            -20, -30, -30, -40, -40, -30, -30, -20,
            -10, -20, -20, -20, -20, -20, -20, -10,
            20, 20, 0, 0, 0, 0, 20, 20,
            20, 30, 10, 0, 0, 10, 30, 20
        };
const int kingMapBMid[64] = {
            20, 30, 10, 0, 0, 10, 30, 20,
            20, 20, 0, 0, 0, 0, 20, 20,
            -10, -20, -20, -20, -20, -20, -20, -10,
            -20, -30, -30, -40, -40, -30, -30, -20,
            -30, -40, -40, -50, -50, -40, -40, -30,
            -30, -40, -40, -50, -50, -40, -40, -30,
            -30, -40, -40, -50, -50, -40, -40, -30,
            -30, -40, -40, -50, -50, -40, -40, -30
        };

const int kingMapWEnd[64] = {
            -50, -40, -30, -20, -20, -30, -40, -50,
            -30, -20, -10, 0, 0, -10, -20, -30,
            -30, -10, 20, 30, 30, 20, -10, -30,
            -30, -10, 30, 40, 40, 30, -10, -30,
            -30, -10, 30, 40, 40, 30, -10, -30,
            -30, -10, 20, 30, 30, 20, -10, -30,
            -30, -30, 0, 0, 0, 0, -30, -30,
            -50, -30, -30, -30, -30, -30, -30, -50
        };
const int kingMapBEnd[64] = {
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
    if (p <= 8) return 0;
    if (p >= 9 && p <= 15) return p - 8;  
    if (p >= 17 && p <= 23) return p - 16;
    return 0;
}

int getHashKey(int p) {
    if (p <= 8) return -1;
    if (p >= 9 && p <= 15) return p - 9;
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
    if (getPiece(p1) == 0 || getPiece(p2) == 0) return false;
    return isWhite(p1) == isWhite(p2);
}

bool isOtherColor(int p1, int p2) {
    if (getPiece(p1) == 0 || getPiece(p2) == 0) return false;
    return (isWhite(p1) == isBlack(p2) && isBlack(p1) == isWhite(p2));
}

bool isSlider(int p) {
    if (getPiece(p) == 3 || getPiece(p) == 4 || getPiece(p) == 5) return true;
    return false;
}

class Move {
    public:
        int piece;
        int startPos;
        int endPos;
        int promo;
        int flag;
        Move(int p, int s, int e, int r, int f) {
            piece = p;
            startPos = s;
            endPos = e;
            promo = r;
            flag = f;
        }
        Move(){}
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

mt19937_64 rng(101);

class HashComputer {
    public:
        std::array<std::array<u64, 64>, 12> piecekeys;
        u64 blacksmovekey;
        std::array<u64, 4> castlingkeys;
        std::array<u64, 8> enpassantkeys;

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

        u64 computeHash(u64 b[12], bool btm, int f[8], bool cr[4]) {
            u64 hash = 0;

            for (int p = 0; p < 12; p++) for (int s = 0; s < 64; s++) if (getBit(b[p], s) != 0) hash ^= piecekeys[p][s];
            if (btm) hash ^= blacksmovekey;
            for (int i = 0; i < 4; i++) if (cr[i]) hash ^= castlingkeys[i];
            for (int i = 0; i < 8; i++) if (f[i] == 1) hash ^= enpassantkeys[i];

            return hash;
        }

        u64 updateHash(u64 hash, Move m, MoveCache x, int f[8], int c[4]) {
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
        u64 random64() {
            return rng();
        }
};

struct Board {
    // White bitboards
    u64 wP = 0, wN = 0, wB = 0, wR = 0, wQ = 0, wK = 0;

    // Black bitboards
    u64 bP = 0, bN = 0, bB = 0, bR = 0, bQ = 0, bK = 0;

    // Reference bitboards
    u64 w;
    u64 b;
    u64 full;

    // Game state
    bool whiteToMove = true;
    bool whiteKingMoved = false;
    bool whiteRookAMoved = false;
    bool whiteRookHMoved = false;
    bool blackKingMoved = false;
    bool blackRookAMoved = false;
    bool blackRookHMoved = false;

    // Caches
    u64 currentHash;
    Move lastMove;
    bool prevCastling[6] = {false, false, false, false, false, false};

    // References
    const u64 afile = 0x0101010101010101ULL;
    const u64 hfile = 0x8080808080808080ULL;
    const u64 rank1 = 0x00000000000000FFULL;
    const u64 rank8 = 0xFF00000000000000ULL;

    void loadFEN(const string& fen) {
        wP = wN = wB = wR = wQ = wK = 0ULL;
        bP = bN = bB = bR = bQ = bK = 0ULL;

        int r = 7;
        int f = 0;

        for (char c : fen) {
            if (c == ' ') break;
            int s = f + r * 8;
            if (isdigit(c)) f += c - '0';
            else if (c == '/') {
                r--; 
                f = 0;
            }
            else {
                switch(c) {
                    case 'P': setBit(wP, s); break;
                    case 'N': setBit(wN, s); break;
                    case 'B': setBit(wB, s); break;
                    case 'R': setBit(wR, s); break;
                    case 'Q': setBit(wQ, s); break;
                    case 'K': setBit(wK, s); break;
                    case 'p': setBit(bP, s); break;
                    case 'n': setBit(bN, s); break;
                    case 'b': setBit(bB, s); break;
                    case 'r': setBit(bR, s); break;
                    case 'q': setBit(bQ, s); break;
                    case 'k': setBit(bK, s); break;
                }
                f++;
            }
        }

        w = wP | wN | wB | wR | wQ | wK;
        b = bP | bN | bB | bR | bQ | bK;
        full = w | b;
    }

    void updateRB() {
        w = wP | wN | wB | wR | wQ | wK;
        b = bP | bN | bB | bR | bQ | bK;
        full = w | b;
    }

    MoveCache makeMove(Move m, HashComputer h) {
        u64 tmp = 0ULL;
        u64 wp[6] = {wP, wN, wB, wR, wQ, wK};
        u64 bp[6] = {bP, bN, bB, bR, bQ, bK};
        setBit(tmp, m.endPos);
        int p = 0;
        if (!whiteToMove) for (int i = 0; i < 6; i++) {
            u64 q = wp[i];
            q = q & tmp;
            if (q != 0ULL) p = i + 1;
        } else for (int i = 0; i < 6; i++) {
            u64 q = bp[i];
            q = q & tmp;
            if (q != 0ULL) p = i + 1;
        }
        int sp = getPiece(m.piece);
        MoveCache cache(p, whiteKingMoved, whiteRookAMoved, whiteRookHMoved, blackKingMoved, blackRookAMoved, blackRookHMoved, lastMove);
        if (whiteToMove) {
            if (sp == 1 && m.promo == 0) {wP ^= 1ULL << m.startPos; wP ^= 1ULL << m.endPos;}
            else if (sp == 1) {
                wP ^= 1ULL << m.startPos; 
                if (m.promo == 1) {wN ^= 1ULL << m.endPos;}
                if (m.promo == 2) {wB ^= 1ULL << m.endPos;}
                if (m.promo == 3) {wR ^= 1ULL << m.endPos;}
                if (m.promo == 4) {wQ ^= 1ULL << m.endPos;}
            }
            if (sp == 2) {wN ^= 1ULL << m.startPos; wN ^= 1ULL << m.endPos;}
            if (sp == 3) {wB ^= 1ULL << m.startPos; wB ^= 1ULL << m.endPos;}
            if (sp == 4) {wR ^= 1ULL << m.startPos; wR ^= 1ULL << m.endPos;}
            if (sp == 5) {wQ ^= 1ULL << m.startPos; wQ ^= 1ULL << m.endPos;}
            if (sp == 6) {wK ^= 1ULL << m.startPos; wK ^= 1ULL << m.endPos;}      
            
            if (p != 0) {
                if (p == 1) bP ^= 1ULL << m.endPos;
                if (p == 2) bN ^= 1ULL << m.endPos;
                if (p == 3) bB ^= 1ULL << m.endPos;
                if (p == 4) bR ^= 1ULL << m.endPos;
                if (p == 5) bQ ^= 1ULL << m.endPos;
                if (p == 6) bK ^= 1ULL << m.endPos;
            }
        } else {
            if (sp == 1 && m.promo == 0) {bP ^= 1ULL << m.startPos; bP ^= 1ULL << m.endPos;}
            else if (sp == 1) {
                bP ^= 1ULL << m.startPos; 
                if (m.promo == 1) {bN ^= 1ULL << m.endPos;}
                if (m.promo == 2) {bB ^= 1ULL << m.endPos;}
                if (m.promo == 3) {bR ^= 1ULL << m.endPos;}
                if (m.promo == 4) {bQ ^= 1ULL << m.endPos;}
            }
            if (sp == 2) {bN ^= 1ULL << m.startPos; bN ^= 1ULL << m.endPos;}
            if (sp == 3) {bB ^= 1ULL << m.startPos; bB ^= 1ULL << m.endPos;}
            if (sp == 4) {bR ^= 1ULL << m.startPos; bR ^= 1ULL << m.endPos;}
            if (sp == 5) {bQ ^= 1ULL << m.startPos; bQ ^= 1ULL << m.endPos;}
            if (sp == 6) {bK ^= 1ULL << m.startPos; bK ^= 1ULL << m.endPos;}      
            
            if (p != 0) {
                if (p == 1) wP ^= 1ULL << m.endPos;
                if (p == 2) wN ^= 1ULL << m.endPos;
                if (p == 3) wB ^= 1ULL << m.endPos;
                if (p == 4) wR ^= 1ULL << m.endPos;
                if (p == 5) wQ ^= 1ULL << m.endPos;
                if (p == 6) wK ^= 1ULL << m.endPos;
            }
        }
        if (m.flag == 3) {
            if (whiteToMove) popBit(bP, m.endPos - 8);
            else popBit(wP, m.endPos + 8);
        }
        if (m.flag == 5) {
            if (m.endPos == 2) {wR ^= 1ULL << a1; wR ^= 1ULL << d1;}
            if (m.endPos == 6) {wR ^= 1ULL << h1; wR ^= 1ULL << f1;}
            if (m.endPos == 57) {bR ^= 1ULL << a8; bR ^= 1ULL << d8;}
            if (m.endPos == 62) {bR ^= 1ULL << h8; bR ^= 1ULL << f8;}
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
        updateRB();
        return cache;
    }

    void unmakeMove(Move m, MoveCache c, HashComputer h) {
        int sp = getPiece(m.piece);
        int p = c.x;
        if (!whiteToMove) {
            if (sp == 1 && m.promo == 0) {wP ^= 1ULL << m.startPos; wP ^= 1ULL << m.endPos;}
            else if (sp == 1) {
                wP ^= 1ULL << m.startPos; 
                if (m.promo == 1) {wN ^= 1ULL << m.endPos;}
                if (m.promo == 2) {wB ^= 1ULL << m.endPos;}
                if (m.promo == 3) {wR ^= 1ULL << m.endPos;}
                if (m.promo == 4) {wQ ^= 1ULL << m.endPos;}
            }
            if (sp == 2) {wN ^= 1ULL << m.startPos; wN ^= 1ULL << m.endPos;}
            if (sp == 3) {wB ^= 1ULL << m.startPos; wB ^= 1ULL << m.endPos;}
            if (sp == 4) {wR ^= 1ULL << m.startPos; wR ^= 1ULL << m.endPos;}
            if (sp == 5) {wQ ^= 1ULL << m.startPos; wQ ^= 1ULL << m.endPos;}
            if (sp == 6) {wK ^= 1ULL << m.startPos; wK ^= 1ULL << m.endPos;}      
            
            if (p != 0) {
                if (p == 1) bP ^= 1ULL << m.endPos;
                if (p == 2) bN ^= 1ULL << m.endPos;
                if (p == 3) bB ^= 1ULL << m.endPos;
                if (p == 4) bR ^= 1ULL << m.endPos;
                if (p == 5) bQ ^= 1ULL << m.endPos;
                if (p == 6) bK ^= 1ULL << m.endPos;
            }
        } else {
            if (sp == 1 && m.promo == 0) {bP ^= 1ULL << m.startPos; bP ^= 1ULL << m.endPos;}
            else if (sp == 1) {
                bP ^= 1ULL << m.startPos; 
                if (m.promo == 1) {bN ^= 1ULL << m.endPos;}
                if (m.promo == 2) {bB ^= 1ULL << m.endPos;}
                if (m.promo == 3) {bR ^= 1ULL << m.endPos;}
                if (m.promo == 4) {bQ ^= 1ULL << m.endPos;}
            }
            if (sp == 2) {bN ^= 1ULL << m.startPos; bN ^= 1ULL << m.endPos;}
            if (sp == 3) {bB ^= 1ULL << m.startPos; bB ^= 1ULL << m.endPos;}
            if (sp == 4) {bR ^= 1ULL << m.startPos; bR ^= 1ULL << m.endPos;}
            if (sp == 5) {bQ ^= 1ULL << m.startPos; bQ ^= 1ULL << m.endPos;}
            if (sp == 6) {bK ^= 1ULL << m.startPos; bK ^= 1ULL << m.endPos;}      
            
            if (p != 0) {
                if (p == 1) wP ^= 1ULL << m.endPos;
                if (p == 2) wN ^= 1ULL << m.endPos;
                if (p == 3) wB ^= 1ULL << m.endPos;
                if (p == 4) wR ^= 1ULL << m.endPos;
                if (p == 5) wQ ^= 1ULL << m.endPos;
                if (p == 6) wK ^= 1ULL << m.endPos;
            }
        }
        if (m.flag == 3) {
            if (!whiteToMove) setBit(bP, m.endPos - 8);
            else setBit(wP, m.endPos + 8);
        }
        if (m.flag == 5) {
            if (m.endPos == 2) {wR ^= 1ULL << a1; wR ^= 1ULL << d1;}
            if (m.endPos == 6) {wR ^= 1ULL << h1; wR ^= 1ULL << f1;}
            if (m.endPos == 57) {bR ^= 1ULL << a8; bR ^= 1ULL << d8;}
            if (m.endPos == 62) {bR ^= 1ULL << h8; bR ^= 1ULL << f8;}
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
        updateRB();
    }

    int get(int s) {
        u64 wp[6] = {wP, wN, wB, wR, wQ, wK};
        u64 bp[6] = {bP, bN, bB, bR, bQ, bK};
        for (int i = 0; i < 6; i++) {
            u64 smask = 1ULL << s;
            if (wp[i] & smask) return i + 9;
            if (bp[i] & smask) return i + 17;
        } return 0;
    }
};

struct Generator {
    u64 knightAttacks[64];
    u64 kingAttacks[64];
    vector<u64> bishopAttacks[64];
    vector<u64> rookAttacks[64];
    u64 bishopMasks[64];
    u64 rookMasks[64];
    u64 rookMagics[64] = {
        0x8a80104000800020ULL, 0x140002000100040ULL,  0x2801880a0017001ULL,  0x100081001000420ULL,
        0x200020010080420ULL,  0x3001c0002010008ULL,  0x8480008002000100ULL, 0x2080088004402900ULL,
        0x800098204000ULL,     0x2024401000200040ULL, 0x100802000801000ULL,  0x120800800801000ULL,
        0x208808088000400ULL,  0x2802200800400ULL,    0x2200800100020080ULL, 0x801000060821100ULL,
        0x80044006422000ULL,   0x100808020004000ULL,  0x12000800804000ULL,   0x1000080080000ULL,
        0x20000800400800ULL,   0x2004008008040080ULL, 0x200800800402000ULL,  0x2010008008000400ULL,
        0x2000020008080ULL,    0x20800400808000ULL,   0x800880020004000ULL,  0x8040010002000800ULL,
        0x400801000400080ULL,  0x40000200010080ULL,   0x8000808004000200ULL, 0x8000010020004000ULL,
        0x8000010020004000ULL, 0x8000808004000200ULL, 0x40000200010080ULL,   0x400801000400080ULL,
        0x8040010002000800ULL, 0x800880020004000ULL,  0x20800400808000ULL,   0x2000020008080ULL,
        0x2010008008000400ULL, 0x200800800402000ULL,  0x2004008008040080ULL, 0x20000800400800ULL,
        0x1000080080000ULL,    0x12000800804000ULL,   0x100808020004000ULL,  0x80044006422000ULL,
        0x801000060821100ULL,  0x2200800100020080ULL, 0x2802200800400ULL,    0x208808088000400ULL,
        0x120800800801000ULL,  0x100802000801000ULL,  0x2024401000200040ULL, 0x800098204000ULL,
        0x2080088004402900ULL, 0x8480008002000100ULL, 0x3001c0002010008ULL,  0x200020010080420ULL,
        0x100081001000420ULL,  0x2801880a0017001ULL,  0x140002000100040ULL,  0x8a80104000800020ULL
    };
    u64 bishopMagics[64] = {
        0x40040844404084ULL,   0x2004208a004208ULL,   0x101010101010101ULL,  0x202020202020202ULL,
        0x404040404040404ULL,  0x808080808080808ULL,  0x401004020080100ULL,  0x40040844404084ULL,
        0x2004208a004208ULL,   0x101010101010101ULL,  0x202020202020202ULL,  0x404040404040404ULL,
        0x808080808080808ULL,  0x401004020080100ULL,  0x4002082208080ULL,    0x40810a410820200ULL,
        0x81020400204080ULL,   0x40400804080400ULL,   0x20802004020080ULL,   0x100100200408804ULL,
        0x400208440020100ULL,  0x2044081008080ULL,    0x0408020801080ULL,    0x400408220401080ULL, 
        0x801082004000200ULL,  0x0804010020040100ULL, 0x404004080800400ULL,  0x2000100208040ULL, 
        0x800040100400802ULL,  0x4000820080400ULL,    0x4008080040080ULL,    0x200200040100400ULL,
        0x080800400080401ULL,  0x400040802000100ULL,  0x20080080050800ULL,   0x4040040404040404ULL, 
        0x202020202020202ULL,  0x101010101010101ULL,  0x2004208a004208ULL,   0x40040844404084ULL,
        0x401004020080100ULL,  0x808080808080808ULL,  0x404040404040404ULL,  0x202020202020202ULL,
        0x101010101010101ULL,  0x2004208a004208ULL,   0x40040844404084ULL,   0x401004020080100ULL,
        0x808080808080808ULL,  0x404040404040404ULL,  0x202020202020202ULL,  0x101010101010101ULL,
        0x2004208a004208ULL,   0x40040844404084ULL,
    };
    int bishopShifts[64] = {
        58, 59, 59, 59, 59, 59, 59, 58,
        59, 59, 59, 59, 59, 59, 59, 59,
        59, 59, 57, 57, 57, 57, 59, 59,
        59, 59, 57, 55, 55, 57, 59, 59,
        59, 59, 57, 55, 55, 57, 59, 59,
        59, 59, 57, 57, 57, 57, 59, 59,
        59, 59, 59, 59, 59, 59, 59, 59,
        58, 59, 59, 59, 59, 59, 59, 58
    };
    int rookShifts[64] = {
        52, 53, 53, 53, 53, 53, 53, 52,
        53, 54, 54, 54, 54, 54, 54, 53,
        53, 54, 54, 54, 54, 54, 54, 53,
        53, 54, 54, 54, 54, 54, 54, 53,
        53, 54, 54, 54, 54, 54, 54, 53,
        53, 54, 54, 54, 54, 54, 54, 53,
        53, 54, 54, 54, 54, 54, 54, 53,
        52, 53, 53, 53, 53, 53, 53, 52
    };
    u64 lineBetween[64][64];


    const u64 afile = 0x0101010101010101ULL;
    const u64 hfile = 0x8080808080808080ULL;
    const u64 rank1 = 0x00000000000000FFULL;
    const u64 rank8 = 0xFF00000000000000ULL;

    void initLeapers() {
        for (int s = 0; s < 64; s++) {
            u64 nmoves = 0ULL; 
            if ((s % 8 < 7) && (s / 8 < 6)) setBit(nmoves, s + 17);
            if ((s % 8 < 6) && (s / 8 < 7)) setBit(nmoves, s + 10);
            if ((s % 8 < 6) && (s / 8 > 0)) setBit(nmoves, s - 6);
            if ((s % 8 < 7) && (s / 8 > 1)) setBit(nmoves, s - 15);
            if ((s % 8 > 0) && (s / 8 > 1)) setBit(nmoves, s - 17);
            if ((s % 8 > 1) && (s / 8 > 0)) setBit(nmoves, s - 10);
            if ((s % 8 > 1) && (s / 8 < 7)) setBit(nmoves, s + 6);
            if ((s % 8 > 0) && (s / 8 < 6)) setBit(nmoves, s + 15);
            knightAttacks[s] = nmoves;

            u64 kmoves = 0ULL;
            int koffsets[8] = {8, -8, 1, -1, 7, -7, 9, -9};
            for (int offset : koffsets) {
                int t = s + offset;
                if (t >= 0 && t <= 63 && abs(t % 8 - s % 8) <= 1) setBit(kmoves, t); 
            }
            kingAttacks[s] = kmoves;
        }
    }

    void initMagic() {
        cout << "\n\n>>>> RUNNING THE CORRECT initMagic() VERSION 2.0 <<<<\n\n" << endl;
        for (int s = 0; s < 64; s++) {
            cout << "       Processing square " << s << endl;
            u64 rmask = 0ULL;
            int tr = s / 8;
            int tf = s % 8;
            for (int r = tr + 1; r < 7; r++) rmask |= (1ULL << (r * 8 + tf));
            for (int r = tr - 1; r > 0; r--) rmask |= (1ULL << (r * 8 + tf));
            for (int f = tf + 1; f < 7; f++) rmask |= (1ULL << (tr * 8 + f));
            for (int f = tf - 1; f > 0; f--) rmask |= (1ULL << (tr * 8 + f));
            rookMasks[s] = rmask;
            u64 bmask = 0ULL;
            for (int r = tr + 1, f = tf + 1; r < 7 && f < 7; r++, f++) bmask |= (1ULL << (r * 8 + f));
            for (int r = tr + 1, f = tf - 1; r < 7 && f > 0; r++, f--) bmask |= (1ULL << (r * 8 + f));
            for (int r = tr - 1, f = tf + 1; r > 0 && f < 7; r--, f++) bmask |= (1ULL << (r * 8 + f));
            for (int r = tr - 1, f = tf - 1; r > 0 && f > 0; r--, f--) bmask |= (1ULL << (r * 8 + f));
            bishopMasks[s] = bmask; 
            int bcount = popcount(bishopMasks[s]);
            int ebcount = 64 - bishopShifts[s];
            if (bcount != ebcount) {
                 cout << "[ERROR] Mismatch on square " << s << ": ";
                cout << "Mask popcount is " << bcount << ", but shift implies " << ebcount << endl;
            }
            int rcount = popcount(rookMasks[s]);
            bishopAttacks[s].resize(1 << bcount);
            rookAttacks[s].resize(1 << rcount);
            for (int i = 0; i < (1 << bcount); i++) {
                u64 blockers = 0ULL;
                u64 tmpmsk = bmask;
                for (int j = 0; j < bcount; j++) {
                    int bpos = getLSB(tmpmsk);
                    if (i & (1 << j)) {
                        setBit(blockers, bpos);
                    }
                    popBit(tmpmsk, bpos);
                }
                u64 mindex = (blockers * bishopMagics[s]) >> bishopShifts[s];
                if (mindex >= bishopAttacks[s].size()) {
                    cout << "[ERROR] FATAL: BISHOP INDEX OUT OF BOUNDS" << endl;
                    cout << "Square: " << s << endl;
                    cout << "Calculated Index: " << mindex << endl;
                    cout << "Table Size: " << bishopAttacks[s].size() << endl;
                    exit(1); 
                }
                u64 attacks = 0ULL;
                int r, f;
                for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++) { setBit(attacks, r * 8 + f); if (getBit(blockers, r * 8 + f)) break; }
                for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--) { setBit(attacks, r * 8 + f); if (getBit(blockers, r * 8 + f)) break; }
                for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++) { setBit(attacks, r * 8 + f); if (getBit(blockers, r * 8 + f)) break; }
                for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--) { setBit(attacks, r * 8 + f); if (getBit(blockers, r * 8 + f)) break; }
                bishopAttacks[s][mindex] = attacks;
            }
            for (int i = 0; i < (1 << rcount); i++) {
                u64 blockers = 0ULL;
                u64 tmpmsk = rmask;
                for (int j = 0; j < rcount; j++) {
                    int bpos = getLSB(tmpmsk);
                    if (i & (1 << j)) {
                        setBit(blockers, bpos);
                    }
                    popBit(tmpmsk, bpos);
                }
                u64 mindex = (blockers * rookMagics[s]) >> rookShifts[s];
                u64 attacks = 0ULL;
                int r, f;
                for (r = tr + 1; r <= 7; r++) { setBit(attacks, r * 8 + tf); if (getBit(blockers, r * 8 + tf)) break; }
                for (r = tr - 1; r >= 0; r--) { setBit(attacks, r * 8 + tf); if (getBit(blockers, r * 8 + tf)) break; }
                for (f = tf + 1; f <= 7; f++) { setBit(attacks, tr * 8 + f); if (getBit(blockers, tr * 8 + f)) break; }
                for (f = tf - 1; f >= 0; f--) { setBit(attacks, tr * 8 + f); if (getBit(blockers, tr * 8 + f)) break; }
                rookAttacks[s][mindex] = attacks;
            }
        }
    }

    u64 getBishopAttacks(int square, u64 blockers) {
        blockers &= bishopMasks[square];
        u64 index = (blockers * bishopMagics[square]) >> bishopShifts[square];
        return bishopAttacks[square][index];
    }

    u64 getRookAttacks(int square, u64 blockers) {
        blockers &= rookMasks[square];
        u64 index = (blockers * rookMagics[square]) >> rookShifts[square];
        return rookAttacks[square][index];
    }

    void initLineBetween() {
        for (int s1 = 0; s1 < 64; ++s1) {
            for (int s2 = 0; s2 < 64; ++s2) {
                lineBetween[s1][s2] = 0ULL;
                if (s1 == s2) continue;
                u64 full = 0ULL;
                if (getRookAttacks(s1, 0) & (1ULL << s2)) {
                    full = (getRookAttacks(s1, 0) & getRookAttacks(s2, 0)) | (1ULL << s1) | (1ULL << s2);
                } else if (getBishopAttacks(s1, 0) & (1ULL << s2)) {
                    full = (getBishopAttacks(s1, 0) & getBishopAttacks(s2, 0)) | (1ULL << s1) | (1ULL << s2);
                }
                u64 btmask = getRookAttacks(s1, 1ULL << s2) & getRookAttacks(s2, 1ULL << s1);
                if (!btmask) { 
                    btmask = getBishopAttacks(s1, 1ULL << s2) & getBishopAttacks(s2, 1ULL << s1);
                }

                lineBetween[s1][s2] = btmask;
            }
        }
    }

    void initGen() {
        cout << "Initializing lookup tables... " << endl;
        cout << "   Initializing leapers... " << endl;
        initLeapers();
        cout << "   Initializing magic bitboards... " << endl;
        initMagic();
        cout << "   Initializing square rays... " << endl;
        initLineBetween();
        cout << "Lookup tables populated. " << endl;
    }

    bool isSquareAttacked(Board& b, int s, bool white) {
        u64 victim_sq = 1ULL << s;
        if (white) {
            if ((((b.wP & ~afile) << 7) | ((b.wP & ~hfile) << 9)) & victim_sq) return true;
            if (knightAttacks[s] & b.wN) return true;
            if (kingAttacks[s] & b.wK) return true;
            if (getBishopAttacks(s, b.full) & (b.wB | b.wQ)) return true;
            if (getRookAttacks(s, b.full) & (b.wR | b.wQ)) return true;
        } else {
            if ((((b.bP & ~hfile) >> 7) | ((b.bP & ~afile) >> 9)) & victim_sq) return true;
            if (knightAttacks[s] & b.bN) return true;
            if (kingAttacks[s] & b.bK) return true;
            if (getBishopAttacks(s, b.full) & (b.bB | b.bQ)) return true;
            if (getRookAttacks(s, b.full) & (b.bR | b.bQ)) return true;
        }
        return false;
    }
    
    bool isSquareAttackedRange(Board& b, int s, int e, bool white) {
        for (int i = s; i < e + 1; i++) if (isSquareAttacked(b, i, white)) return true;
        return false;
    }

    bool isLegal(Board& b, Move m, HashComputer h) {
        MoveCache c = b.makeMove(m, h);
        u64 k = !b.whiteToMove ? b.wK : b.bK;
        bool incheck = isSquareAttacked(b, getLSB(k), b.whiteToMove);
        b.unmakeMove(m, c, h);
        return !incheck;
    }

    u64 aTo(Board& b, int s, bool white) {
        u64 attackers = 0ULL;
        u64 f = b.full;
        if (white) {
            u64 pattackers = (((1ULL << s) & ~afile) >> 9) | (((1ULL << s) & ~hfile) >> 7);
            attackers |= pattackers & b.wP;

            attackers |= knightAttacks[s] & b.wN;

            attackers |= getBishopAttacks(s, f) & (b.wB | b.wQ);

            attackers |= getRookAttacks(s, f) & (b.wR | b.wQ);

            attackers |= kingAttacks[s] & b.wK;
        } else {
            u64 pattackers = (((1ULL << s) & ~hfile) << 9) | (((1ULL << s) & ~afile) << 7);
            attackers |= pattackers & b.bP;

            attackers |= knightAttacks[s] & b.bN;

            attackers |= getBishopAttacks(s, f) & (b.bB | b.bQ);

            attackers |= getRookAttacks(s, f) & (b.bR | b.bQ);

            attackers |= kingAttacks[s] & b.bK;
        }
        return attackers;
    }

    void generateKingMoves(vector<Move>& list, Board& b, int s, u64 msk) {
        u64 tmp = kingAttacks[s];
        u64 temp = b.whiteToMove ? b.w : b.b;
        tmp = tmp & ~temp;
        tmp &= msk;
        int k = b.whiteToMove ? 14 : 22;
        int ct = popcount(tmp);
        for (int i = 0; i < ct; i++) {
            int e = getLSB(tmp);  
            if (!isSquareAttacked(b, e, !b.whiteToMove)) list.push_back(Move(k, s, e, 0, 0));
            popBit(tmp, e);
        }
        u64 mask1 = 7ULL << 1;
        u64 mask2 = 3ULL << 5;
        u64 mask3 = 7ULL << 57;
        u64 mask4 = 3ULL << 61;
        if (
            !b.whiteKingMoved && !b.whiteRookAMoved &&
            !(mask1 & b.full) &&
            !isSquareAttackedRange(b, 2, 4, false) &&
            b.whiteToMove
        ) list.push_back(Move(14, 4, 2 , 0, 5));
        if (
            !b.whiteKingMoved && !b.whiteRookHMoved &&
            !(mask2 & b.full) &&
            !isSquareAttackedRange(b, 4, 6, false) &&
            b.whiteToMove
        ) list.push_back(Move(14, 4, 6 , 0, 5));
        if (
            !b.blackKingMoved && !b.blackRookAMoved &&
            !(mask3 & b.full) &&
            !isSquareAttackedRange(b, 58, 60, true) &&
            !b.whiteToMove
        ) list.push_back(Move(22, 60, 58 , 0, 5));
        if (
            !b.blackKingMoved && !b.blackRookHMoved &&
            !(mask4 & b.full) &&
            !isSquareAttackedRange(b, 60, 62, true) &&
            !b.whiteToMove
        ) list.push_back(Move(22, 60, 62 , 0, 5));
    }

    void generateKnightMoves(vector<Move>& list, Board& b, u64 msk) {
        u64 knights = b.whiteToMove ? b.wN : b.bN;
        int ct = popcount(knights);
        int p = b.whiteToMove ? 10 : 18;
        for (int i = 0; i < ct; i++) {    
            int s = getLSB(knights);
            popBit(knights, s);
            u64 tmp = knightAttacks[s];
            u64 temp = b.whiteToMove ? b.w : b.b;
            tmp = tmp & ~temp;
            tmp &= msk;
            int cnt = popcount(tmp);
            for (int j = 0; j < cnt; j++) {
                int e = getLSB(tmp);  
                list.push_back(Move(p, s, e, 0, 0));
                popBit(tmp, e);
            }
        }
    }

    void generatePawnMoves(vector<Move>& list, Board& b, u64 msk) {
        u64 pawns = b.whiteToMove ? b.wP : b.bP;
        u64 other = b.whiteToMove ? b.b : b.w;

        u64 tpawns = b.whiteToMove ? b.wP << 8 : b.bP >> 8;
        u64 dpawns = b.whiteToMove ? b.wP << 16 : b.bP >> 16;
        u64 cpawns1 = b.whiteToMove ? (b.wP & ~afile) << 7 : (b.bP & ~hfile) >> 7;
        u64 cpawns2 = b.whiteToMove ? (b.wP & ~hfile) << 9 : (b.bP & ~afile) >> 9;

        u64 smask = ~b.full;
        u64 cmask = other;

        u64 t = tpawns & smask & msk;
        u64 ddst;
        if (b.whiteToMove) {
            ddst = (t & (rank1 << 16)) << 8; 
        } else {
            ddst = (t & (rank8 >> 16)) >> 8;
        }
        u64 d = ddst & smask & msk;
        u64 c1 = cpawns1 & cmask & msk;
        u64 c2 = cpawns2 & cmask & msk;

        int k = b.whiteToMove ? getLSB(b.wK) : getLSB(b.bK);
        int p = b.whiteToMove ? 9 : 17;
        int offset = b.whiteToMove? 8 : -8;
        int coffset1 = b.whiteToMove ? 7 : -7;
        int coffset2 = b.whiteToMove ? 9 : -9;

        int ct = popcount(t);
        for (int i = 0; i < ct; i++) {
            int e = getLSB(t);
            popBit(t, e);
            int s = e - offset;
            u64 fr = b.whiteToMove ? rank8 : rank1;
            u64 emask = 1ULL << e;
            if (!(fr & emask)) list.push_back(Move(p, s, e, 0, 0));
            else for (int j = 1; j < 5; j++) list.push_back(Move(p, s, e, j, 4));
        }

        ct = popcount(d);
        for (int i = 0; i < ct; i++) {
            int e = getLSB(d);
            popBit(d, e);
            int s = e - 2 * offset;
            list.push_back(Move(p, s, e, 0, 2));
        }

        ct = popcount(c1);
        for (int i = 0; i < ct; i++) {
            int e = getLSB(c1);
            popBit(c1, e);
            int s = e - coffset1;
            u64 fr = b.whiteToMove ? rank8 : rank1;
            u64 emask = 1ULL << e;
            if (!(fr & emask & other)) list.push_back(Move(p, s, e, 0, 0));
            else for (int j = 1; j < 5; j++) list.push_back(Move(p, s, e, j, 4));
        }

        ct = popcount(c2);
        for (int i = 0; i < ct; i++) {
            int e = getLSB(c2);
            popBit(c2, e);
            int s = e - coffset2;
            u64 fr = b.whiteToMove ? rank8 : rank1;
            u64 emask = 1ULL << e;
            if (!(fr & emask & other)) list.push_back(Move(p, s, e, 0, 0));
            else for (int j = 1; j < 5; j++) list.push_back(Move(p, s, e, j, 4));
        }

        if (b.lastMove.flag == 2) {
            u64 eptmp1 = 1ULL << (b.lastMove.endPos - 1);
            eptmp1 &= ~hfile;
            u64 eptmp2 = 1ULL << (b.lastMove.endPos + 1);
            eptmp2 &= ~afile;
            u64 eptmp = eptmp1 | eptmp2;
            u64 ep = eptmp & pawns;
            ep &= msk;
            ct = popcount(ep);
            for (int i = 0; i < ct; i++) {
                int s = getLSB(ep);
                popBit(ep, s);
                int e = b.whiteToMove ? b.lastMove.endPos + 8 : b.lastMove.endPos - 8;
                if (!(b.full & (1ULL << e))) list.push_back(Move(p, s, e, 0, 3));
            }
        }
    }

    void generateRookMoves(vector<Move>& list, Board& b, u64 msk) {
        u64 rooks = b.whiteToMove ? b.wR : b.bR;
        int p = b.whiteToMove ? 12 : 20;
        u64 friendly = b.whiteToMove ? b.w : b.b;

        while (rooks) {
            int s = getLSB(rooks);
            popBit(rooks, s);
            u64 blockers = b.full & rookMasks[s];
            int index = (blockers * rookMagics[s]) >> rookShifts[s];
            u64 attacks = rookAttacks[s][index];
            attacks &= ~friendly;
            attacks &= msk;
            while (attacks) {
                int e = getLSB(attacks);
                popBit(attacks, e);
                list.push_back(Move(p, s, e, 0, 0));
            }
        }
    }

    void generateBishopMoves(vector<Move>& list, Board& b, u64 msk) {
        u64 bishops = b.whiteToMove ? b.wB : b.bB;
        int p = b.whiteToMove ? 11 : 19;
        u64 friendly = b.whiteToMove ? b.w : b.b;

        while (bishops) {
            int s = getLSB(bishops);
            popBit(bishops, s);
            u64 blockers = b.full & bishopMasks[s];
            int index = (blockers * bishopMagics[s]) >> bishopShifts[s];
            u64 attacks = bishopAttacks[s][index];
            attacks &= ~friendly;
            attacks &= msk;
            while (attacks) {
                int e = getLSB(attacks);
                popBit(attacks, e);
                list.push_back(Move(p, s, e, 0, 0));
            }
        }
    }

    void generateQueenMoves(vector<Move>& list, Board& b, u64 msk) {
        u64 bishops = b.whiteToMove ? b.wQ : b.bQ;
        u64 rooks = b.whiteToMove ? b.wQ : b.bQ;
        int p = b.whiteToMove ? 13 : 21;
        u64 friendly = b.whiteToMove ? b.w : b.b;

        while (bishops) {
            int s = getLSB(bishops);
            popBit(bishops, s);
            u64 blockers = b.full & bishopMasks[s];
            int index = (blockers * bishopMagics[s]) >> bishopShifts[s];
            u64 attacks = bishopAttacks[s][index];
            attacks &= ~friendly;
            attacks &= msk;
            while (attacks) {
                int e = getLSB(attacks);
                popBit(attacks, e);
                list.push_back(Move(p, s, e, 0, 0));
            }
        }

        while (rooks) {
            int s = getLSB(rooks);
            popBit(rooks, s);
            u64 blockers = b.full & rookMasks[s];
            int index = (blockers * rookMagics[s]) >> rookShifts[s];
            u64 attacks = rookAttacks[s][index];
            attacks &= ~friendly;
            attacks &= msk;
            while (attacks) {
                int e = getLSB(attacks);
                popBit(attacks, e);
                list.push_back(Move(p, s, e, 0, 0));
            }
        }
    }

    vector<Move> generateMoves(Board& b, HashComputer h) {
        vector<Move> moves;
        moves.reserve(256);

        u64 kb = b.whiteToMove ? b.wK : b.bK;
        int k = getLSB(kb);
        u64 cb = aTo(b, k, !b.whiteToMove);
        int cnum = popcount(cb);
        u64 pinned = 0ULL;
        array<u64, 64> pinrays = {0};
        u64 erl = b.whiteToMove ? (b.bR | b.bQ) : (b.wR | b.wQ);
        u64 ebl = b.whiteToMove ? (b.bB | b.bQ) : (b.wB | b.wQ);
    
        u64 prl = getRookAttacks(k, 0) & erl;
        while (prl) {
            int ps = getLSB(prl);
            popBit(prl, ps);
            u64 btray = lineBetween[k][ps];
            u64 pray = btray & b.full;
            if (popcount(pray) == 1) {
                if (pray & (b.whiteToMove ? b.w : b.b)) {
                    int psq = getLSB(pray);
                    setBit(pinned, psq);
                    pinrays[psq] = btray | (1ULL << ps);
                }
            }
        }
        u64 pbl = getBishopAttacks(k, 0) & ebl;
        while (pbl) {
            int ps = getLSB(pbl);
            popBit(pbl, ps);
            u64 btray = lineBetween[k][ps];
            u64 pray = btray & b.full;
            if (popcount(pray) == 1) {
                int psq = getLSB(pray);
                setBit(pinned, psq);
                pinrays[psq] = btray | (1ULL << ps);
            }
        }

        u64 movemask = ~0ULL;
        if (cnum > 1) generateKingMoves(moves, b, k, movemask);
        else {
            if (cnum == 1) {
                int cs = getLSB(cb);
                movemask = lineBetween[k][cs] | (1ULL << cs);
            }
            u64 unpinned = ~pinned;
            generatePawnMoves(moves, b, movemask & unpinned);
            generateKnightMoves(moves, b, movemask & unpinned);
            generateBishopMoves(moves, b, movemask & unpinned);
            generateRookMoves(moves, b, movemask & unpinned);
            generateQueenMoves(moves, b, movemask & unpinned);
            generateKingMoves(moves, b, k, ~0ULL);

            u64 pc = pinned;
            while (pc) {
                int s = getLSB(pc);
                popBit(pc, s);
                Board temp = b;
                u64 pmask = 1ULL << s;
                u64 pmmask = movemask & pinrays[s];
                if (b.whiteToMove) { temp.wP &= pmask; temp.wN &= pmask; temp.wB &= pmask; temp.wR &= pmask; temp.wQ &= pmask; } 
                else { temp.bP &= pmask; temp.bN &= pmask; temp.bB &= pmask; temp.bR &= pmask; temp.bQ &= pmask; }
                temp.updateRB();
                generatePawnMoves(moves, temp, pmmask);
                generateKnightMoves(moves, temp, pmmask);
                generateBishopMoves(moves, temp, pmmask);
                generateRookMoves(moves, temp, pmmask);
                generateQueenMoves(moves, temp, pmmask);
            }
        }
        return moves;
    }

    void generateKingCaptures(vector<Move>& list, Board& b, int s) {
        u64 tmp = kingAttacks[s];
        u64 temp = b.whiteToMove ? b.w : b.b;
        u64 other = b.whiteToMove ? b.b : b.w;
        tmp = tmp & ~temp & other;
        int k = b.whiteToMove ? 14 : 22;
        int ct = popcount(tmp);
        for (int i = 0; i < ct; i++) {
            int e = getLSB(tmp);  
            list.push_back(Move(k, s, e, 0, 0));
            popBit(tmp, e);
        }
        u64 mask1 = 7ULL << 1;
        u64 mask2 = 3ULL << 5;
        u64 mask3 = 7ULL << 57;
        u64 mask4 = 3ULL << 61;
    }

    void generateKnightCaptures(vector<Move>& list, Board& b) {
        u64 knights = b.whiteToMove ? b.wN : b.bN;
        u64 other = b.whiteToMove ? b.b : b.w;
        int ct = popcount(knights);
        int p = b.whiteToMove ? 10 : 18;
        for (int i = 0; i < ct; i++) {    
            int s = getLSB(knights);
            popBit(knights, s);
            u64 tmp = knightAttacks[s];
            u64 temp = b.whiteToMove ? b.w : b.b;
            tmp = tmp & ~temp & other;
            int cnt = popcount(tmp);
            for (int i = 0; i < cnt; i++) {
                int e = getLSB(tmp);  
                list.push_back(Move(p, s, e, 0, 0));
                popBit(tmp, e);
            }
        }
    }

    void generatePawnCaptures(vector<Move>& list, Board& b) {
        u64 pawns = b.whiteToMove ? b.wP : b.bP;
        u64 other = b.whiteToMove ? b.b : b.w;
        
        u64 cpawns1 = b.whiteToMove ? (b.wP & ~afile) << 7 : (b.bP & ~hfile) >> 7;
        u64 cpawns2 = b.whiteToMove ? (b.wP & ~hfile) << 9 : (b.bP & ~afile) >> 9;

        u64 drmask = b.whiteToMove? rank1 << 8 : rank8 >> 8;
        u64 cmask = other;

        u64 c1 = cpawns1 & cmask;
        u64 c2 = cpawns2 & cmask;

        int k = b.whiteToMove ? getLSB(b.wK) : getLSB(b.bK);
        int p = b.whiteToMove ? 9 : 17;
        int offset = b.whiteToMove? 8 : -8;
        int coffset1 = b.whiteToMove ? 7 : -7;
        int coffset2 = b.whiteToMove ? 9 : -9;

        int ct = popcount(c1);
        for (int i = 0; i < ct; i++) {
            int e = getLSB(c1);
            popBit(c1, e);
            int s = e - coffset1;
            u64 fr = b.whiteToMove ? rank8 : rank1;
            u64 emask = 1ULL << e;
            if (!(emask & other)) continue;
            if (!(fr & emask)) list.push_back(Move(p, s, e, 0, 0));
            else for (int j = 1; j < 5; j++) list.push_back(Move(p, s, e, j, 4));
        }

        ct = popcount(c2);
        for (int i = 0; i < ct; i++) {
            int e = getLSB(c2);
            popBit(c2, e);
            int s = e - coffset2;
            u64 fr = b.whiteToMove ? rank8 : rank1;
            u64 emask = 1ULL << e;
            if (!(emask & other)) continue;
            if (!(fr & emask)) list.push_back(Move(p, s, e, 0, 0));
            else for (int j = 1; j < 5; j++) list.push_back(Move(p, s, e, j, 4));
        }

        if (b.lastMove.flag == 2) {
            u64 eptmp1 = 1ULL << (b.lastMove.endPos - 1);
            eptmp1 &= ~hfile;
            u64 eptmp2 = 1ULL << (b.lastMove.endPos + 1);
            eptmp2 &= ~afile;
            u64 eptmp = eptmp1 | eptmp2;
            u64 ep = eptmp & pawns;
            ct = popcount(ep);
            for (int i = 0; i < ct; i++) {
                int s = getLSB(ep);
                popBit(ep, s);
                int e = b.whiteToMove ? b.lastMove.endPos + 8 : b.lastMove.endPos - 8;
                if (!(b.full & (1ULL << e))) list.push_back(Move(p, s, e, 0, 3));
            }
        }
    }

    void generateRookCaptures(vector<Move>& list, Board& b) {
        u64 rooks = b.whiteToMove ? b.wR : b.bR;
        int p = b.whiteToMove ? 12 : 20;
        u64 friendly = b.whiteToMove ? b.w : b.b;
        u64 other = b.whiteToMove ? b.b : b.w;

        while (rooks) {
            int s = getLSB(rooks);
            popBit(rooks, s);
            u64 blockers = b.full & rookMasks[s];
            int index = (blockers * rookMagics[s]) >> rookShifts[s];
            u64 attacks = rookAttacks[s][index];
            attacks &= ~friendly;
            attacks &= other;
            while (attacks) {
                int e = getLSB(attacks);
                popBit(attacks, e);
                list.push_back(Move(p, s, e, 0, 0));
            }
        }
    }

    void generateBishopCaptures(vector<Move>& list, Board& b) {
        u64 bishops = b.whiteToMove ? b.wB : b.bB;
        int p = b.whiteToMove ? 11 : 19;
        u64 friendly = b.whiteToMove ? b.w : b.b;
        u64 other = b.whiteToMove ? b.b : b.w;

        while (bishops) {
            int s = getLSB(bishops);
            popBit(bishops, s);
            u64 blockers = b.full & bishopMasks[s];
            int index = (blockers * bishopMagics[s]) >> bishopShifts[s];
            u64 attacks = bishopAttacks[s][index];
            attacks &= ~friendly;
            attacks &= other;
            while (attacks) {
                int e = getLSB(attacks);
                popBit(attacks, e);
                list.push_back(Move(p, s, e, 0, 0));
            }
        }
    }

    void generateQueenCaptures(vector<Move>& list, Board& b) {
        u64 bishops = b.whiteToMove ? b.wQ : b.bQ;
        u64 rooks = b.whiteToMove ? b.wQ : b.bQ;
        int p = b.whiteToMove ? 13 : 21;
        u64 friendly = b.whiteToMove ? b.w : b.b;
        u64 other = b.whiteToMove ? b.b : b.w;

        while (bishops) {
            int s = getLSB(bishops);
            popBit(bishops, s);
            u64 blockers = b.full & bishopMasks[s];
            int index = (blockers * bishopMagics[s]) >> bishopShifts[s];
            u64 attacks = bishopAttacks[s][index];
            attacks &= ~friendly;
            attacks &= other;

            while (attacks) {
                int e = getLSB(attacks);
                popBit(attacks, e);
                list.push_back(Move(p, s, e, 0, 0));
            }
        }

        while (rooks) {
            int s = getLSB(rooks);
            popBit(rooks, s);
            u64 blockers = b.full & rookMasks[s];
            int index = (blockers * rookMagics[s]) >> rookShifts[s];
            u64 attacks = rookAttacks[s][index];
            attacks &= ~friendly;
            attacks &= other;
            while (attacks) {
                int e = getLSB(attacks);
                popBit(attacks, e);
                list.push_back(Move(p, s, e, 0, 0));
            }
        }
    }

    vector<Move> generateCaptures(Board& b, HashComputer h) {
        vector<Move> plmoves;
        vector<Move> moves;
        u64 k = b.whiteToMove ? b.wK : b.bK;
        generateKingCaptures(plmoves, b, getLSB(k));
        generateQueenCaptures(plmoves, b);
        generateBishopCaptures(plmoves, b);
        generateRookCaptures(plmoves, b);
        generateKnightCaptures(plmoves, b);
        generatePawnCaptures(plmoves, b);
        for (Move move : plmoves) {
            if (isLegal(b, move, h)) moves.push_back(move);
        }
        return moves;
    }
};

struct Evaluator {
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

    float eval(Board& b) {
        int whiteEval = 0;
        int blackEval = 0;
        int whitep[6] = {0, 0, 0, 0, 0, 0};
        int blackp[6] = {0, 0, 0, 0, 0, 0};
        int wpos = 0;
        int bpos = 0;

        u64 wp[6] = {b.wP, b.wN, b.wB, b.wR, b.wQ, b.wK};
        u64 bp[6] = {b.bP, b.bN, b.bB, b.bR, b.bQ, b.bK};

        for (int i = 0; i < 6; i++) {
            whiteEval += popcount(wp[i]) * pieceValues[i];
            whitep[i] += popcount(wp[i]) * pieceValues[i];
            blackEval += popcount(bp[i]) * pieceValues[i];
            blackp[i] += popcount(bp[i]) * pieceValues[i];

            u64 tmp = wp[i];
            while (tmp) {
                int s = getLSB(tmp);
                popBit(tmp, s);
                if (i == 0) wpos += pawnMapW[s];
                else if (i == 1) wpos += knightMapW[s];
                else if (i == 2) wpos += bishopMapW[s];
                else if (i == 3) wpos += rookMapW[s];
                else if (i == 4) wpos += queenMapW[s];
                else if (i == 5) {
                    if (whitep[4] == 0 && whitep[3] == 0 && blackp[4] == 0 && blackp[3] == 0) wpos += kingMapWEnd[s];
                    else wpos += kingMapWMid[s];
                }
            }

            tmp = bp[i];
            while (tmp) {
                int s = getLSB(tmp);
                popBit(tmp, s);
                if (i == 0) bpos += pawnMapB[s];
                else if (i == 1) bpos += knightMapB[s];
                else if (i == 2) bpos += bishopMapB[s];
                else if (i == 3) bpos += rookMapB[s];
                else if (i == 4) bpos += queenMapB[s];
                else if (i == 5) {
                    if (whitep[4] == 0 && whitep[3] == 0 && blackp[4] == 0 && blackp[3] == 0) bpos += kingMapBEnd[s];
                    else bpos += kingMapBMid[s];
                }
            }
        }
        return squeeze(whiteEval + blackEval);
    }

    float evalPers(Board &b, bool white) {
        int pers = white ? 1 : -1;
        return eval(b) * pers;
    }
};

struct Game {
    Board board;
    Generator generator;
    HashComputer comp;
    Evaluator eval;
};

void perft(Game& g, int depth, u64& nodes) {
    if (depth == 0) {nodes++; return;}
    vector<Move> moves = g.generator.generateMoves(g.board, g.comp);
    for (Move& move : moves) {
        MoveCache cache = g.board.makeMove(move, g.comp);
        perft(g, depth - 1, nodes);
        g.board.unmakeMove(move, cache, g.comp);
    }
}

int perft(int depth) {
    auto start = chrono::steady_clock::now();
    auto sthr = chrono::system_clock::now();
    cout << "Execution started at " << formatTimePoint(sthr) << ". " << endl << endl;
    string sp = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    string tstp = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1";
    Game game;
    game.board.loadFEN(sp);
    cout << "Generator leapers initializing..." << endl;
    game.generator.initLeapers();
    cout << "Leapers finished." << endl;
    cout << "Generator magics initializing..." << endl;
    game.generator.initMagic();
    cout << "Magics finished." << endl;
    cout << "Board initialized with FEN. Running PERFT. " << endl;
    
    u64 ct = 0;
    perft(game, depth, ct);
    auto end = chrono::steady_clock::now();
    auto edhr = chrono::system_clock::now();
    cout << "PERFT finished with depth " << to_string(depth) << "." << endl;
    cout << "Result: " + to_string(ct) << endl << endl;
    chrono::duration<double> elapsed = end - start;
    cout << "Execution finished at " << formatTimePoint(edhr) << ". " << endl;
    cout << "Time elapsed: " << elapsed.count() << " seconds." << endl;
    return 0;
}

void perftsplit(Game& g, int depth, u64& nodes) {
    u64 totalNodes = 0;
    cout << "--- Perft Split for Depth " << depth << " ---" << endl;

    vector<Move> moves = g.generator.generateMoves(g.board, g.comp);

    for (Move& move : moves) {
        u64 nodes_for_this_move = 0;
        MoveCache cache = g.board.makeMove(move, g.comp);
        bool wasWhitesTurn = !g.board.whiteToMove;
        perft(g, depth - 1, nodes_for_this_move);
        totalNodes += nodes_for_this_move;
        cout << move.srep() << ": " << nodes_for_this_move << endl;
        g.board.unmakeMove(move, cache, g.comp);
        nodes += nodes_for_this_move;
    }
    cout << "--- Perft Ended for Depth " << depth << " ---" << endl;
    cout << "Total Nodes: " << totalNodes << endl;
}

int perftsplit(int depth) {
    auto start = chrono::steady_clock::now();
    auto sthr = chrono::system_clock::now();
    cout << "Execution started at " << formatTimePoint(sthr) << ". " << endl << endl;
    string sp = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    string tstp = "7k/8/8/1p1p4/2P5/8/8/7K w - - 0 1";
    Game game;
    game.board.loadFEN(sp);
    game.generator.initGen();
    cout << "Board initialized with FEN. Running PERFT. " << endl;
    game.board.makeMove(Move(9, 14, 30, 0, 2), game.comp);
    game.board.makeMove(Move(17, 48, 40, 0, 2), game.comp);
    printBitboard(game.board.full);
    
    u64 ct = 0;
    perftsplit(game, depth, ct);
    auto end = chrono::steady_clock::now();
    auto edhr = chrono::system_clock::now();
    cout << "PERFT finished with depth " << to_string(depth) << "." << endl;
    cout << "Result: " + to_string(ct) << endl << endl;
    chrono::duration<double> elapsed = end - start;
    cout << "Execution finished at " << formatTimePoint(edhr) << ". " << endl;
    cout << "Time elapsed: " << elapsed.count() << " seconds." << endl;
    return 0;
}

int main() {
    return perftsplit(1);
}
