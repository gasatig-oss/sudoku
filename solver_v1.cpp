// This is based on Sudoku Solver for leetcode. This is accepted as solution for https://leetcode.com/problems/sudoku-solver/description/

class Solution {
public:

    bool isValid(vector<vector<char>>&board, int row, int col, char num)
    {
        for(int i = 0; i < 9; i++)
        {
            if (board[row][i] == num) return false;
            if (board[i][col] == num) return false;
            if (board[3*(row/3) + i/3][3*(col/3) + i%3] == num) return false;

        }
        return true;

    }
    bool solver_rec(vector<vector<char>>& board, int row, int col)
    {
        if (row == 9) return true;

        int rowNext = row;
        int colNext = col + 1;

        if (colNext == 9)
        {
            rowNext = rowNext + 1;
            colNext = 0;
        }

        if (board[row][col] != '.')
        {
            return solver_rec(board, rowNext, colNext);
        }

        for (char num = '1'; num <= '9'; num++)
        {
            if(isValid(board, row, col, num))
            {
                board[row][col] = num;
                if(solver_rec(board, rowNext, colNext))
                {
                    return true;
                }
                board[row][col] = '.';
            }
        }

        return false;

    }
    void solveSudoku(vector<vector<char>>& board) {
        solver_rec(board, 0, 0);
    }
};
