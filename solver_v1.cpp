// This is based on Sudoku Solver for leetcode. This is accepted as solution for https://leetcode.com/problems/sudoku-solver/description/

class Solution {
public:
    bool rows[9][10] = {};
    bool cols[9][10] = {};
    bool boxes[9][10] = {};

    bool solve(vector<vector<char>>& board) {
        for(int row_itr = 0; row_itr < 9; row_itr++)
        {
            for(int col_itr = 0; col_itr < 9; col_itr++)
            {
                if(board[row_itr][col_itr] == '.')
                {
                    for (int num = 1; num <= 9; num++)
                    {
                        int box = (row_itr/3) * 3 + (col_itr/3);
                        if (!rows[row_itr][num] && !cols[col_itr][num] && !boxes[box][num])
                        {
                            board[row_itr][col_itr] = num + '0';
                            rows[row_itr][num] = true;
                            cols[col_itr][num] = true;
                            boxes[box][num] = true;
                            if (solve(board))
                            {
                                return true;
                            }
                            board[row_itr][col_itr] = '.';
                            rows[row_itr][num] = false;
                            cols[col_itr][num] = false;
                            boxes[box][num] = false;
                        }
                    }
                    return false;
                }
            }
        }
        return true;
    }
    void solveSudoku(vector<vector<char>>& board) {
        int box = 0;
        int num = 0;
        for(int row_itr = 0; row_itr < 9; row_itr++)
        {
            for(int col_itr = 0; col_itr < 9; col_itr++)
            {
                 if (board[row_itr][col_itr] != '.')
                 {
                    num = board[row_itr][col_itr] - '0';
                    box = ((row_itr/3) * 3) + (col_itr/3);
                    rows[row_itr][num] = true;
                    cols[col_itr][num] = true;
                    boxes[box][num] = true;
                 }
            }
        }
        solve(board);
    }
};
