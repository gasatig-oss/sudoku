// Solver is based on Sudoku Solver for leetcode(problem 37).
// few youtube vidoes were also referred to understand the backtracking solution.
// Some google search to improve backtracking and early pruning.
// few suggestion from chatgpt to improve the code for early pruning
// based on solver code, generator code is written. 
// same solver chatgpt suggestions added to generator.
// got few suggestions from chatgpt and google for grading but could not understant properly
// than better to implement it. so using navive approach for grading based on number of
// prefilled numbers.
// used google to check for reading/writeing from file.
// some base improvements is to move out board as baseclass so that set/clear
// can be only at single place


#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <cstring>

using namespace std;

class SudokuGenerator
{
  private:
    int board[9][9] = {};
    bool rows[9][10] = {};
    bool cols[9][10] = {};
    bool boxes[9][10] = {};
    mt19937 rng;
    
    void clearBoard() {
        memset(board, 0, sizeof(board));
        memset(rows, 0, sizeof(rows));
        memset(cols, 0, sizeof(cols));
        memset(boxes, 0, sizeof(boxes));
    }
    // earlier approach to fill row 0, col 0, box 0.
    // but faced some random issues with it while filling
    // so filling just box 0 randomly as that will be good seed for puzzel
    void initBox() {
      vector<int> boxNums{1,2,3,4,5,6,7,8,9};
      shuffle(boxNums.begin(), boxNums.end(),rng);
      int index = 0;
      for(int row = 0; row < 3; row++) {
        for(int col = 0; col < 3; col++) {
          int num = boxNums[index];
          index++;
          board[row][col] = num;
          rows[row][num] = true;
          cols[col][num] = true;
          boxes[0][num] = true;
        }
      }
    }
    
    bool fillBoard(int index) {
      // this optimizations suggested by chatgpt, earlier it had nested loop to find next empty cell
      // this will do some early pruning as it will start from next cell in each iteration
      // rather than 0,0 of board each time
      // these are the same optimizations as used in solver
      if (index == 81) {
        return true;
      }

      int row = index / 9;
      int col = index % 9;

      if(board[row][col] != 0) {
        return fillBoard(index + 1);
      }

      int box = (row/3) * 3 + (col/3);
      vector<int> validNums;
      for (int num = 1; num <= 9; num++) {
        if(!rows[row][num] && !cols[col][num] && !boxes[box][num]) {
          validNums.push_back(num);
        }
      }

      shuffle(validNums.begin(), validNums.end(), rng);
      for (int num : validNums) {
        board[row][col] = num;
        rows[row][num] = true;
        cols[col][num] = true;
        boxes[box][num] = true;
        if (fillBoard(index + 1)) {
          return true;
        }
        board[row][col] = 0;
        rows[row][num] = false;
        cols[col][num] = false;
        boxes[box][num] = false;
      }
      return false;
    }
    
    void makePuzzel(int blank) {
      uniform_int_distribution<int> dist(0, 8);
      int empty = 0;
      while(empty < blank) {
        int row = dist(rng);
        int col = dist(rng);
        if(board[row][col] != 0) {
          board[row][col] = 0;
          empty++;
        }
      }
    }
    
public:
  SudokuGenerator() {
    random_device rd;
    rng = mt19937(rd());
  }
  
  vector<vector<char>> makeBoard(int empty) {
    clearBoard();
    initBox();
    
    if(!fillBoard(0)) {
      cout << "not a valid board" << "\n";
    }
    
    makePuzzel(empty);
    
    vector<vector<char>> puzzel(9, vector<char>(9, '.'));
    for(int row = 0; row < 9; row++) {
      for(int col = 0; col < 9; col++) {
        if(board[row][col] != 0)
          puzzel[row][col] = board[row][col] + '0';
      }
    }

    return puzzel;
  }

  static void dumpToFile(vector<vector<vector<char>>>& puzzels, string& fileName) {
    ofstream outfile(fileName);
    if(!outfile.is_open()) {
      throw runtime_error("cannot open file " + fileName);
    }
    for (const auto& puzzel : puzzels) {
      for (const auto& row : puzzel) {
        for (char c : row) {
          outfile << c;
        }
        outfile << "\n";
      }
      outfile << "\n";
    }
    outfile.close();
  }
};

class SudokuSolver
{
  private:
    bool rows[9][10] = {};
    bool cols[9][10] = {};
    bool boxes[9][10] = {};
    int prefilled = 0;

    bool solve(vector<vector<char>>& board, int index = 0) {
      // this optimizations suggested by chatgpt, earlier it had nested loop to find next empty cell
      // this will do some early pruning as it will start from next cell in each iteration
      // rather than 0,0 of board each time
      // these are the same optimizations as used in fillBoard

      if (index == 81) {
        return true;
      }

      int row = index / 9;
      int col = index % 9;

      if(board[row][col] != '.') {
        return solve(board, index + 1);
      }

      int box = (row/3) * 3 + (col/3);
      for (int num = 1; num <= 9; num++) {
        if(!rows[row][num] && !cols[col][num] && !boxes[box][num]) {
          board[row][col] = num + '0';
          rows[row][num] = true;
          cols[col][num] = true;
          boxes[box][num] = true;
          if (solve(board, index + 1)) {
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
    
    void clearBoard() {
      prefilled = 0;
      memset(rows, 0, sizeof(rows));
      memset(cols, 0, sizeof(cols));
      memset(boxes, 0, sizeof(boxes));
    }

  public:
    bool solveSudoku(vector<vector<char>>& board) {
      clearBoard();
      for(int row = 0; row < 9; row++) {
        for(int col = 0; col < 9; col++) {
          if (board[row][col] != '.') {
            int box = (row/3) * 3 + (col/3);
            int num = board[row][col] - '0';
            if (rows[row][num] || cols[col][num] || boxes[box][num]) {
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
      return solve(board,0);
    }

    string getGrade() {
      if(prefilled > 40) return "Easy";
      if(prefilled > 35) return "Medium";
      if(prefilled > 30) return "Hard";
      return "Samurai";
    }
    
};

class PuzzleLoader
{
  public:
    static vector<vector<vector<char>>> loadFile(string& fileName) {
      ifstream infile(fileName);

      if (!infile.is_open()) {
        throw runtime_error("Cannot open file!! " + fileName);
      }

      vector<vector<vector<char>>> puzzels;
      vector<vector<char>> puzzel;
      string line;

      while(getline(infile ,line)) {
        if(line.empty()) {
          if(!puzzel.empty()) {
            if(puzzel.size() == 9) {
              puzzels.push_back(puzzel);
            }
            else {
              cout<< "invalid puzzel" << "\n";
            }
            puzzel.clear();
          }
          continue;
        }
        if(line.size() != 9) {
          cout << "Not a valid puzzel input " << line << "\n";
          continue;
        }
        puzzel.emplace_back(line.begin(), line.end());
      }
      
      if(!puzzel.empty()) {
        puzzels.push_back(puzzel);
      }

      return puzzels;
    }
};

void printSolution(vector<vector<char>>& board) {
  for(int row = 0; row < 9; row++) {
    for(int col = 0; col < 9; col++) {
      cout << board[row][col];
    }
    cout << "\n";
  }
  cout << "\n";
}

int main()
{
  string fileName = "generated.txt";
  try {
    SudokuGenerator generator;
    vector<vector<vector<char>>> puzzels;
    for (int i = 0; i < 20; i++) {
      vector<vector<char>> puzzel;
      switch(i%4)
      {
        case 0:
          puzzel = generator.makeBoard(40);
          break;
        case 1:
          puzzel = generator.makeBoard(45);
          break;
        case 2:
          puzzel = generator.makeBoard(50);
          break;
        case 3:
          puzzel = generator.makeBoard(55);
          break;
      }
      puzzels.push_back(puzzel);
    }

    SudokuGenerator::dumpToFile(puzzels, fileName);
  }
  catch (const exception& ex) {
    cout << "Error:    " << ex.what() << "\n";
    return -1;
  }

  try {
    auto puzzles = PuzzleLoader::loadFile(fileName);
    SudokuSolver solver;
    for (size_t i = 0; i < puzzles.size(); i++) {
      cout << "Puzzle number " << i + 1 << "\n";
      
      if(solver.solveSudoku(puzzles[i])) {
        cout << "Grade of puzzel is " << solver.getGrade() << "\n";
        printSolution(puzzles[i]);
      }
      else {
        cout << "there is no valid solution for this puzzel" << "\n";
      }
    }
  }
  catch (const exception& ex) {
    cout << "Error:    " << ex.what() << "\n";
    return -1;
  }

  return 0;
}
