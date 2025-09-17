// This is based on Sudoku Solver for leetcode. This is accepted as solution for https://leetcode.com/problems/sudoku-solver/description/

class Solution {
public:

    bool isValid(vector<vector<char>>&board, int row, int col, int num)
    {
        char c_num = num + '0';
        for(int i = 0; i < 9; i++)
        {
            if (board[row][i] == c_num) return false;
            if (board[i][col] == c_num) return false;
            if (board[3*(row/3) + i/3][3*(col/3) + i%3] == c_num) return false;

        }
        return true;
    }

    bool solver_rec(vector<vector<char>>& board)
    {
        for(int row = 0; row < 9; row++)
        {
            for(int col = 0; col < 9; col++)
            {
                if(board[row][col] == '.')
                {
                    for (int num = 1; num <= 9; num++)
                    {
                        if(isValid(board, row, col, num))
                        {
                            board[row][col] = num + '0';
                            if(solver_rec(board))
                            {
                                return true;
                            }
                            else
                            {
                                board[row][col] = '.';
                            }
                        }
                    }
                    return false;
                }
            }
        }
        return true;
    }
    void solveSudoku(vector<vector<char>>& board) {
        solver_rec(board);
    }
};
