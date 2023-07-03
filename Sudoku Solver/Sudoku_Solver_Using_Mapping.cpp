#include <iostream>
using namespace std;
#include <algorithm>
#include <vector>

class Solution
{
public:
    int row[9][9] = {0}, col[9][9] = {0};
    int box[3][3][9] = {0};

    bool solve(int i, vector<vector<char>> &A)
    {
        if (i == 81)
            return true;

        int r = i / 9;
        int c = i % 9;

        if (A[r][c] != '.')
            return solve(i + 1, A);

        for (int num = 1; num <= 9; num++)
        {
            if (row[r][num - 1] or col[c][num - 1] or box[r / 3][c / 3][num - 1])
                continue;

            A[r][c] = num + 48;
            row[r][num - 1] = col[c][num - 1] = box[r / 3][c / 3][num - 1] = 1;

            if (solve(i + 1, A))
                return true;

            A[r][c] = '.';
            row[r][num - 1] = col[c][num - 1] = box[r / 3][c / 3][num - 1] = 0;
        }
        return false;
    }

    void solveSudoku(vector<vector<char>> &board)
    {
        ios::sync_with_stdio(false);
        cin.tie(nullptr);
        cout.tie(nullptr);

        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                if (board[i][j] == '.')
                    continue;

                int num = board[i][j] - 48;
                row[i][num - 1] = col[j][num - 1] = box[i / 3][j / 3][num - 1] = 1;
            }
        }
        solve(0, board);
    }
};

int main()
{
    return 0;
}