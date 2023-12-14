#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>

using namespace std;

// This function prints the sudoku board.
void printBoard(vector<vector<int>> &board)
{
    for (int i = 0; i < board.size(); i++)
    {
        if (i % 3 == 0 && i != 0)
        {
            cout << "-----------------------" << endl;
        }
        for (int j = 0; j < board[0].size(); j++)
        {
            if (j % 3 == 0 && j != 0)
            {
                cout << " | ";
            }
            if (j == 0)
            {
                cout << board[i][j] << " ";
            }
            else
            {
                cout << board[i][j] << " ";
            }
        }
        cout << endl;
    }
}
// This function finds an empty cell in the sudoku board and return the corresponding row and the column.
vector<int> findEmpty(vector<vector<int>> &board)
{
    for (int i = 0; i < board.size(); i++)
    {
        for (int j = 0; j < board[0].size(); j++)
        {
            if (board[i][j] == 0)
            {
                return {i, j};
            }
        }
    }
    return {};
}
// This function generate a number to be placed inside the sudoku board when an empty point is given. By specifying starting point we can generate numbers above that number.
int generateNum(vector<int> emptyPoint, vector<vector<int>> &board, int startingPt)
{
    bool found1 = true;
    bool found2 = true;
    int num = -1;
    for (int i = startingPt; i <= 9; i++)
    {
        found1 = true;
        found2 = true;
        int generatedNum = i;
        for (int j = 0; j < board[0].size(); j++)
        {
            // Go through the column and row of the empty cell and check whether the generated number is already available.
            if ((board[emptyPoint[0]][j] == generatedNum) || (board[j][emptyPoint[1]] == generatedNum))
            {
                found1 = false;
                break; // If that generated number is already available break from the loop;
            }
        }
        // Check whether the generated number is already available within the 3 x 3 grid corresponding to the empty point.
        int startingRow = emptyPoint[0] / 3;
        int startingCol = emptyPoint[1] / 3;
        for (int j = startingRow * 3; j < startingRow * 3 + 3; j++)
        {
            for (int k = startingCol * 3; k < startingCol * 3 + 3; k++)
            {
                if (board[j][k] == generatedNum)
                {
                    found2 = false;
                    break;
                }
            }
        }

        if (found1 && found2) // has found a number to be placed within the empty cell.
        {
            num = generatedNum;
            break;
        }
    }

    return num; // If we couldn't find a number we'll return -1;
}
// Function to backtrack.
int backtrack(vector<vector<int>> &backtrackingStack, vector<vector<int>> &board)
{

    vector<int> prevEl = backtrackingStack.back();
    if (!backtrackingStack.empty())
    {
        backtrackingStack.pop_back(); // Removing the top most element while backtracking.
    }
    int newNum = generateNum({prevEl[0], prevEl[1]}, board, prevEl[2]); // New number generated to the previos element.
    if (newNum != -1)
    {
        backtrackingStack.push_back({prevEl[0], prevEl[1], newNum});
        board[prevEl[0]][prevEl[1]] = newNum; // update the board;
        return 0;
    }
    else
    {
        board[prevEl[0]][prevEl[1]] = 0;
        return backtrack(backtrackingStack, board);
    }
}

int main()
{
    auto start = chrono::high_resolution_clock::now();

    vector<vector<int>> board;
    // update this board according to the problem. But remember to keep 9 x 9 grid unchanged. I mean don't change it to 18 x 18 grid or some other.
    board = {
        {0, 0, 0, 0, 0, 0, 0, 0, 2},
        {0, 0, 0, 0, 0, 0, 9, 4, 0},
        {0, 0, 3, 0, 0, 0, 0, 0, 5},
        {0, 9, 2, 3, 0, 5, 0, 7, 4},
        {8, 4, 0, 0, 0, 0, 0, 0, 0},
        {0, 6, 7, 0, 9, 8, 0, 0, 0},
        {0, 0, 0, 7, 0, 6, 0, 0, 0},
        {0, 0, 0, 9, 0, 0, 0, 2, 0},
        {4, 0, 8, 5, 0, 0, 3, 6, 0}};

    cout << "---------Initial Sudoku board--------" << endl;

    printBoard(board);
    cout << endl;

    int z = 1;
    bool running = true;
    // Going through the board we will find an empty cell and a number to be placed within that cell, we then push that number to a stack so that when we backtrack we can pop elements from the stack and visit that element.
    vector<vector<int>> backtrackingStack; // first element will be the row number, second element will be column number and the third will be the value.

    while (running)
    {
        vector<int> emptyPoint = findEmpty(board); // Find an empty point in the board.
        if (emptyPoint.size() == 0)
        {
            running = false; // we will be finished when there are no more empty points.
            break;
        }

        int generatedNum = generateNum(emptyPoint, board, 1); // Generate a number to that empty point.

        if (generatedNum != -1)
        {
            // If that generated number is valid we will push the empty element's co-ordinates along with the value and update the board.
            backtrackingStack.push_back({emptyPoint[0], emptyPoint[1], generatedNum});
            board[emptyPoint[0]][emptyPoint[1]] = generatedNum;
        }
        else
        {
            backtrack(backtrackingStack, board);
        }

        cout << endl;
        cout << "********Step " << z++ << "********" << endl;
        printBoard(board);
    }
    // Printing the final sudoku puzzle.
    cout << endl;
    cout << "***FINAL SUDOKU BOARD***" << endl;
    printBoard(board);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Time taken:" << duration.count() << "seconds" << endl;

    return 0;
}