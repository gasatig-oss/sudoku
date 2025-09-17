#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;

class SudokuSolver
{
  private:
    bool rows[9][10] = {};
    bool cols[9][10] = {};
    bool boxes[9][10] = {};
    int prefilled = 0;

    bool solve(vector<vector<char>>& board)
    {
      for(int row = 0; row < 9; row++)
      {
        for(int col = 0; col < 9; col++)
        {
          if(board[row][col] == '.')
          {
            for (int num = 1; num < 10; num++)
            {
              int box = (row/3) * 3 + col/3;
              if(!rows[row][num] && !cols[col][num] && !boxes[box][num])
              {
                board[row][col] = num + '0';
                rows[row][num] = true;
                cols[col][num] = true;
                boxes[box][num] = true;
                if (solve(board))
                {
                  return true;
                }
                board[row][col] = '.';
                rows[row][num] = false;
                cols[col][num] = false;
                boxes[box][num] = false;
              }
            }
            return false;
          }
        }
      }
      return true;
    }
    
    void clearMap()
    {
      prefilled = 0;
      for(int i = 0; i < 9; i++)
      {
        fill(begin(rows[i]), end(rows[i]), false);
        fill(begin(cols[i]), end(cols[i]), false);
        fill(begin(boxes[i]), end(boxes[i]), false);
      }
    }

  public:
    bool solveSudoku(vector<vector<char>>& board)
    {
      clearMap();
      for(int row = 0; row < 9; row++)
      {
        for(int col = 0; col < 9; col++)
        {
          if (board[row][col] != '.')
          {
            int box = (row/3) * 3 + (col/3);
            int num = board[row][col] - '0';
            if (rows[row][num] || cols[col][num] || boxes[box][num])
            {
              cout << "number already in some row/col/box  " << num << " \n";
              cout << row << " " << col << " " << box << " " << " \n";
              return false;
            }
            rows[row][num] = true;
            cols[col][num] = true;
            boxes[box][num] = true;
            prefilled++;
          }
        }
      }
      return solve(board);
    }
    
    string getGrade()
    {
      if(prefilled > 40) return "Easy";
      if(prefilled > 35) return "Medium";
      if(prefilled > 30) return "Hard";
      return "Samurai";
    }
};


class PuzzleLoader
{
  public:
    static vector<vector<vector<char>>> loadFile(string& fileName)
    {
      ifstream infile(fileName);

      if (!infile.is_open())
      {
        throw runtime_error("Cannot open file!! " + fileName);
      }

      vector<vector<vector<char>>> puzzels;
      vector<vector<char>> puzzel;
      string line;

      while(getline(infile ,line))
      {
        if(line.empty())
        {
          if(!puzzel.empty())
          {
            if(puzzel.size() == 9)
            {
              puzzels.push_back(puzzel);
            }
            else
            {
              cout<< "invalid puzzel" << "\n";
            }
            puzzel.clear();
          }
          continue;
        }
        if(line.size() != 9)
        {
          cout << "Not a valid puzzel input " << line << "\n";
          continue;
        }
        puzzel.emplace_back(line.begin(), line.end());
      }
      
      if(!puzzel.empty())
      {
        puzzels.push_back(puzzel);
      }

      return puzzels;
    }
};

void printSolution(vector<vector<char>>& board)
{
  for(int row = 0; row < 9; row++)
  {
    for(int col = 0; col < 9; col++)
    {
      cout << board[row][col];
    }
    cout << "\n";
  }
  cout << "\n";
}

int main()
{
  string fileName = "sudoku.txt";
  try
  {
    auto puzzles = PuzzleLoader::loadFile(fileName);
    SudokuSolver solver;
    for (size_t i = 0; i < puzzles.size(); i++)
    {
      cout << "Puzzle number " << i + 1 << "\n";
      if(solver.solveSudoku(puzzles[i]))
      {
        cout << "Grade of puzzel is " << solver.getGrade() << "\n";
        printSolution(puzzles[i]);
      }
      else
      {
        cout << "there is no valid solution for this puzzel" << "\n";
      }
      
    }
  }
  catch (const exception ex)
  {
    cout << "Error:    " << ex.what() << "\n";
    return -1;
  }
  return 0;
}
