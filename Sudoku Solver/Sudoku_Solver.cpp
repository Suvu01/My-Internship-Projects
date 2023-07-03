#include <iostream>
using namespace std;
#include <algorithm>
#include <vector>

bool isPossible(int num, int row, int col, vector<vector<char>> &A)
{
    char n = num + 48;
    for (int i = 0; i < 9; i++)
        if (i != row and A[i][col] == n)
            return false;
    for (int j = 0; j < 9; j++)
        if (j != col and A[row][j] == n)
            return false;

    int start_row = (row / 3) * 3;
    int start_col = (col / 3) * 3;

    for (int i = start_row; i < 3 + start_row; i++)
    {
        for (int j = start_col; j < 3 + start_col; j++)
        {
            if (A[i][j] == n)
            {
                if (i == row and j == col)
                    continue;
                else
                    return false;
            }
        }
    }

    return true;
}

bool solve(int i, int &n, vector<vector<char>> &A, vector<vector<int>> &empty)
{
    if (i == n)
        return true;

    int row = empty[i][0];
    int col = empty[i][1];

    for (int num = 1; num <= 9; num++)
    {
        if (isPossible(num, row, col, A))
        {
            A[row][col] = num + 48;
            if (solve(i + 1, n, A, empty))
                return true;

            else
                A[row][col] = '.';
        }
    }

    return false;
}

void sudokuSolver(vector<vector<char>> &puzzle)
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    vector<vector<int>> empty;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (puzzle[i][j] == '.')
                empty.push_back({i, j});
        }
    }

    int n = empty.size();
    solve(0, n, puzzle, empty);
}

int main()
{
    return 0;
}