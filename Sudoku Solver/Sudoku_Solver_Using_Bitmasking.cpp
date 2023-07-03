#include <iostream>
using namespace std;
#include <algorithm>
#include <vector>

int boxNum(int r, int c)
{
    return (r / 3) * 3 + c / 3;
}

bool isPossible(int num, int r, int c, vector<int> &row, vector<int> &col, vector<int> &box)
{
    int row_check = (row[r] >> num) & 1;
    int col_check = (col[c] >> num) & 1;
    int box_check = (box[boxNum(r, c)] >> num) & 1;

    return (!row_check and !col_check and !box_check);
}

bool solve(int i, int &n, vector<pair<int, int>> &empty, vector<vector<int>> &A, vector<int> &row, vector<int> &col, vector<int> &box)
{
    if (i == n)
        return true;

    int r = empty[i].first;
    int c = empty[i].second;

    for (int num = 1; num <= 9; num++)
    {
        if (isPossible(num, r, c, row, col, box))
        {
            A[r][c] = num;
            row[r] |= 1 << num;
            col[c] |= 1 << num;
            box[boxNum(r, c)] |= 1 << num;

            if (solve(i + 1, n, empty, A, row, col, box))
                return true;

            A[r][c] = 0;
            row[r] &= ~(1 << num);
            col[c] &= ~(1 << num);
            box[boxNum(r, c)] &= ~(1 << num);
        }
    }

    return false;
}

// The input puzzle board is passed as a 2D vector of integers. The empty cells are marked 0.
void sudokuSolver(vector<vector<int>> &puzzle)
{
    vector<pair<int, int>> empty;

    vector<int> row(9, 0);
    vector<int> col(9, 0); // These are used to bitmask all the cells which are not empty in the board in terms of rows and columns.

    vector<int> box(9, 0); // This vector is used to bitmask the element according to the box it is present in (out of all the 9 (3 x 3) boxes).

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (puzzle[i][j] == 0)
                empty.push_back({i, j});
            else
            {
                row[i] |= 1 << puzzle[i][j];
                col[j] |= 1 << puzzle[i][j];
                box[boxNum(i, j)] |= puzzle[i][j];
            }
        }
    }

    int n = empty.size();
    bool ans = solve(0, n, empty, puzzle, row, col, box);
}

int main()
{
    return 0;
}