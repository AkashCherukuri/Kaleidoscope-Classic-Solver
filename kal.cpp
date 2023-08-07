#include <iostream>
#include <vector>
#include <list>
#include <assert.h>

using namespace std;

#define vi vector<int>
#define vvi vector< vector <int> >

#define EMP -1
#define RED 1
#define BLA 2
#define BLU 3
#define YEL 4

vvi trg;

class Block{
    public:
    vvi br, by;
    int sm, sn;
    Block(int m, int n, vvi br_in, vvi by_in){
        sm = m;
        sn = n;
        br = br_in;
        by = by_in;
    }

    int get_cell(int x, int y, int ori){
        assert(ori<8);
        int rot = ori%4, flp = ori/4;
        int it=y, jt=x, i, j;
        if(rot == 0){
            i = it;
            j = jt;
        }
        else if(rot == 1){
            i = jt; 
            j = sn-1-it; 
        }
        else if(rot == 2){
            i = sm-1-it;
            j = sn-1-jt;
        }
        else if(rot == 3){
            i = sm-1-jt;
            j = it;
        }
        if(flp == 0){
            //  Use the Red-Black side of the piece
            return br[i][j];
        }
        else{
            // Use Blue-Yellow-Black side of the piece
            return by[i][j];
        }
    }

	// Check if this block fits the trg without conflicts on board
	// Assume that sanity checks for the piece to be within the bounds of the board are done
	bool check_fit(int i, int j, int ori, const vvi &board){
		int rot = ori%4, flp = ori/4;
		bool fit = true;
		for(int ip=0; ip<sm; ip++){
			for(int jp=0; jp<sn; jp++){
				if(flp == 0){
					if(br[ip][jp] != EMP){
						if(rot == 0){
							if(br[ip][jp] != trg[i+ip][j+jp] || board[i+ip][j+jp] != EMP){
								fit = false;
								break;
							}
						}
						else if(rot == 1){
							if(br[ip][jp] != trg[i+jp][j+sm-1-ip] || board[i+jp][j+sm-1-ip] != EMP){
								fit = false;
								break;
							}
						}
						else if(rot == 2){
							if(br[ip][jp] != trg[i+sm-1-ip][j+sn-1-jp] || board[i+sm-1-ip][j+sn-1-jp] != EMP){
								fit = false;
								break;
							}
						}
						else if(rot == 3){
							if(br[ip][jp] != trg[i+sn-1-jp][j+ip] || board[i+sn-1-jp][j+ip] != EMP){
								fit = false;
								break;
							}
						}
					}
				}
				else{
					if(by[ip][jp] != EMP){
						if(rot == 0){
							if(by[ip][jp] != trg[i+ip][j+jp] || board[i+ip][j+jp] != EMP){
								fit = false;
								break;
							}
						}
						else if(rot == 1){
							if(by[ip][jp] != trg[i+jp][j+sm-1-ip] || board[i+jp][j+sm-1-ip] != EMP){
								fit = false;
								break;
							}
						}
						else if(rot == 2){
							if(by[ip][jp] != trg[i+sm-1-ip][j+sn-1-jp] || board[i+sm-1-ip][j+sn-1-jp] != EMP){
								fit = false;
								break;
							}
						}
						else if(rot == 3){
							if(by[ip][jp] != trg[i+sn-1-jp][j+ip] || board[i+sn-1-jp][j+ip] != EMP){
								fit = false;
								break;
							}
						}
					}
				}
			}
			if(!fit){
				break;
			}
		}
		return fit;
	}

	void set_in_board(int i, int j, int ori, vvi &board, int idx){
		int rot = ori%4, flp = ori/4;
		for(int ip=0; ip<sm; ip++){
			for(int jp=0; jp<sn; jp++){
				if(flp == 0){
					if(br[ip][jp] != EMP){
						if(rot == 0){
							board[i+ip][j+jp] = idx;
						}
						else if(rot == 1){
							board[i+jp][j+sm-1-ip] = idx;
						}
						else if(rot == 2){
							board[i+sm-1-ip][j+sn-1-jp] = idx;
						}
						else if(rot == 3){
							board[i+sn-1-jp][j+ip] = idx;
						}
					}
				}
				else{
					if(by[ip][jp] != EMP){
						if(rot == 0){
							board[i+ip][j+jp] = idx;
						}
						else if(rot == 1){
							board[i+jp][j+sm-1-ip] = idx;
						}
						else if(rot == 2){
							board[i+sm-1-ip][j+sn-1-jp] = idx;
						}
						else if(rot == 3){
							board[i+sn-1-jp][j+ip] = idx;
						}
					}
				}
			}
		}
	}

	void remove_from_board(int i, int j, int ori, vvi &board){
		int rot = ori%4, flp = ori/4;
		for(int ip=0; ip<sm; ip++){
			for(int jp=0; jp<sn; jp++){
				if(flp == 0){
					if(br[ip][jp] != EMP){
						if(rot == 0){
							board[i+ip][j+jp] = EMP;
						}
						else if(rot == 1){
							board[i+jp][j+sm-1-ip] = EMP;
						}
						else if(rot == 2){
							board[i+sm-1-ip][j+sn-1-jp] = EMP;
						}
						else if(rot == 3){
							board[i+sn-1-jp][j+ip] = EMP;
						}
					}
				}
				else{
					if(by[ip][jp] != EMP){
						if(rot == 0){
							board[i+ip][j+jp] = EMP;
						}
						else if(rot == 1){
							board[i+jp][j+sm-1-ip] = EMP;
						}
						else if(rot == 2){
							board[i+sm-1-ip][j+sn-1-jp] = EMP;
						}
						else if(rot == 3){
							board[i+sn-1-jp][j+ip] = EMP;
						}
					}
				}
			}
		}
	}
};

list<Block> block_l;

void init_blocks(){
    block_l.clear();

    // Line Block
    vvi line1({{BLA, RED, BLA, RED, BLA, RED, BLA, RED}});
    vvi line2({{BLU, BLA, YEL, BLA, BLU, BLA, YEL, BLA}}); 
    Block line(1, 8, line1, line2);
    block_l.push_back(line);

    // L-Family
    vvi l1r({{BLA, EMP},
            {RED, EMP},
            {BLA, RED}});
    vvi l1b({{EMP, BLU},
            {EMP, BLA},
            {BLA, YEL}});
    Block L1(3, 2, l1r, l1b);
    block_l.push_back(L1);

    vvi l2r({{RED, EMP},
            {BLA, EMP},
            {RED, BLA}});
    vvi l2b({{EMP, YEL},
            {EMP, BLA},
            {BLA, BLU}});
    Block L2(3, 2, l2r, l2b);
    block_l.push_back(L2);

    vvi l3r({{EMP, BLA},
            {EMP, RED},
            {RED, BLA}});
    vvi l3b({{BLA, EMP},
            {BLU, EMP},
            {BLA, YEL}});
    Block L3(3, 2, l3r, l3b);
    block_l.push_back(L3);

    vvi l4r({{EMP, RED},
            {EMP, BLA},
            {BLA, RED}});
    vvi l4b({{BLA, EMP},
            {YEL, EMP},
            {BLA, BLU}});
    Block L4(3, 2, l4r, l4b);
    block_l.push_back(L4);

    // T-Blocks
    vvi t1r({{RED, EMP},
            {BLA, RED},
            {RED, EMP}});
    vvi t1b({{EMP, BLA},
            {BLA, BLU},
            {EMP, BLA}});
    Block T1(3, 2, t1r, t1b);
    block_l.push_back(T1);

    vvi t2r({{BLA, EMP},
            {RED, BLA},
            {BLA, EMP}});
    vvi t2b({{EMP, YEL},
            {YEL, BLA},
            {EMP, BLU}});
    Block T2(3, 2, t2r, t2b);
    block_l.push_back(T2);

    // Z-Blocks
    vvi z1r({{EMP, RED},
            {RED, BLA},
            {BLA, EMP}});
    vvi z1b({{BLU, EMP},
            {BLA, YEL},
            {EMP, BLA}});
    Block Z1(3, 2, z1r, z1b);
    block_l.push_back(Z1);

    vvi z2r({{BLA, EMP},
            {RED, BLA},
            {EMP, RED}});
    vvi z2b({{EMP, YEL},
            {BLU, BLA},
            {BLA, EMP}});
    Block Z2(3, 2, z2r, z2b);
    block_l.push_back(Z2);

    // Mid Line
    vvi m1r({{RED, BLA, RED, BLA}});
    vvi m1b({{BLA, YEL, BLA, BLU}});
    Block M(1, 4, m1r, m1b);
    block_l.push_back(M);

    // V Blocks
    vvi v1r({{RED, EMP},
            {BLA, RED}});
    vvi v1b({{EMP, BLU},
            {YEL, BLA}});
    Block v1(2, 2, v1r, v1b);
    block_l.push_back(v1);

    vvi v2r({{BLA, EMP},
            {RED, BLA}});
    vvi v2b({{EMP, BLA},
            {BLA, YEL}});
    Block v2(2, 2, v2r, v2b);
    block_l.push_back(v2);

    // O Block
    vvi or1({{RED, BLA},
            {BLA, RED}});
    vvi ob1({{YEL, BLA},
            {BLA, BLU}});
    Block O(2, 2, or1, ob1);
    block_l.push_back(O);

    // Short Line
    vvi s1r({{RED, BLA, RED}});
    vvi s1b({{YEL, BLA, BLU}});
    Block s1(1, 3, s1r, s1b);
    block_l.push_back(s1);
    
    vvi s2r({{BLA, RED, BLA}});
    vvi s2b({{BLA, YEL, BLA}});
    Block s2(1, 3, s2r, s2b);
    block_l.push_back(s2);

    // Duo
    vvi dr({{BLA, RED}});
    vvi db({{BLA, BLU}});
    Block D(1, 2, dr, db);
    block_l.push_back(D);

    // Cells
    vvi c1r({{RED}});
    vvi c1b({{BLA}});
    Block c1(1, 1, c1r, c1b);
    block_l.push_back(c1);

    vvi c2r({{BLA}});
    vvi c2b({{BLU}});
    Block c2(1, 1, c2r, c2b);
    block_l.push_back(c2);
}

bool solve(vvi &board, int idx){
	// cout << idx << endl;
	if(idx == block_l.size())
		return true;

    // Get an iterator to the block we are trying to fit now
    auto bl = next(block_l.begin(), idx);

	// Try to brute force fit the block in the given board, with all orientations
	for(int ori=0; ori<8; ori++){
		int rot = ori%4, flp = ori/4;
		// Get the bounds for iteration to make it a little more efficient
		// These bounds are INCLUSIVE of the iteration!
		int imax, jmax;
		if(rot%2 == 0){
			imax = 8-(bl->sm);
			jmax = 8-(bl->sn);
		}
		else{
			imax = 8-(bl->sn);
			jmax = 8-(bl->sm);
		}

		for(int i=0; i<=imax; i++){
			for(int j=0; j<=jmax; j++){
				// Check if this block fits in the board
				if(bl->check_fit(i, j, ori, board)){
					// cout << idx << " fit at (" << i << "," << j << "); ori: " << ori  << endl; 
					// The block fits!
					// 1. Set the block into the board
					bl->set_in_board(i, j, ori, board, idx);
					// for(int i=0; i<8; i++){
					// 	for(int j=0; j<8; j++){
					// 		cout << board[i][j] << " ";
					// 	}
					// 	cout << endl;
					// }
					// 2. Increase idx, recurse and check if a solution is found
					if(solve(board, idx+1)){
						return true;
					}
					// 3. If solution is not found, remove the block from board and continue
					else{
						bl->remove_from_board(i, j, ori, board);
					}
				}
			}
		}
	}
	return false;
}

int main(){
    // ios_base::sync_with_stdio(false);
    // cin.tie(NULL);

    // Initialise the blocks
    init_blocks();
    
    // Get the board that needs to be solved for from in.txt

    // Initialise solved board
    vvi sol(8, vi(8, EMP));

	trg = vvi(8, vi(8));

	for(int i=0; i<8; i++){
		for(int j=0; j<8; j++){
			cin >> trg[i][j];
		}
	}
	
	bool soln = solve(sol, 0);
	if(soln)
		cout << "Solved!" << endl;
	else
		cout << "Womp Womp" << endl;
		
	for(int i=0; i<8; i++){
		for(int j=0; j<8; j++){
			cout << sol[i][j] << "\t";
		}
		cout << endl;
	}
 
    return 0;
}