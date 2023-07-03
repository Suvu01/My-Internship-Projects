#include <iostream>
using namespace std;
#include <algorithm>
#include <vector>

#include <cstdlib>
#include <time.h>

#define lines 5

bool randomNumber(vector<vector<int>> &board, vector<int> &randoms)
{
    vector<pair<int, int>> cells;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (board[i][j] == 0)
                cells.push_back({i, j});
        }
    }

    int C = cells.size();
    if (C == 0)
        return false;

    pair<int, int> cell = cells[rand() % C];
    int random_row = cell.first;
    int random_col = cell.second;

    int random_number = randoms[rand() % 10];
    board[random_row][random_col] = (random_number);

    return true;
}

void displayBoard(vector<vector<int>> &board, int &score)
{
    for (int row = 0; row < 4; row++)
    {
        cout << "[";

        for (int col = 0; col < 3; col++)
        {
            printf("%3d,   ", board[row][col]);
        }

        printf("%3d  ]", board[row][3]);

        cout << endl
             << endl;
    }
    cout << "Score: " << score << endl;
    for (int i = 0; i < lines; i++)
        cout << endl;
}

void merge(vector<int> &ans, vector<int> &nums, int n, int &score)
{
    int i = 0, j = 1;
    while (j < n)
    {
        if (nums[i] == nums[j])
        {
            score += 2 * nums[i];
            ans.push_back(nums[i] + nums[j]);
            i += 2;
            j += 2;
        }

        else
        {
            ans.push_back(nums[i]);
            i++;
            j++;
        }
    }

    while (i < n)
        ans.push_back(nums[i++]);
}

void operateUp(vector<vector<int>> &board, int &score)
{
    for (int col = 0; col < 4; col++)
    {
        vector<int> v;
        for (int row = 0; row < 4; row++)
            if (board[row][col] != 0)
                v.push_back(board[row][col]);

        int n = v.size();
        vector<int> ans;
        merge(ans, v, n, score);

        int N = ans.size();
        for (int row = 0; row < N; row++)
            board[row][col] = ans[row];
        for (int row = N; row < 4; row++)
            board[row][col] = 0;
    }
}

void operateDown(vector<vector<int>> &board, int &score)
{
    for (int col = 0; col < 4; col++)
    {
        vector<int> v;
        for (int row = 3; row >= 0; row--)
            if (board[row][col] != 0)
                v.push_back(board[row][col]);

        int n = v.size();
        vector<int> ans;
        merge(ans, v, n, score);

        int N = ans.size();
        for (int row = 0; row < N; row++)
            board[3 - row][col] = ans[row];
        for (int row = N; row < 4; row++)
            board[3 - row][col] = 0;
    }
}

void operateLeft(vector<vector<int>> &board, int &score)
{
    for (int row = 0; row < 4; row++)
    {
        vector<int> v;
        for (int col = 0; col < 4; col++)
            if (board[row][col] != 0)
                v.push_back(board[row][col]);

        int n = v.size();
        vector<int> ans;
        merge(ans, v, n, score);

        int N = ans.size();
        for (int col = 0; col < N; col++)
            board[row][col] = ans[col];
        for (int col = N; col < 4; col++)
            board[row][col] = 0;
    }
}

void operateRight(vector<vector<int>> &board, int &score)
{
    for (int row = 0; row < 4; row++)
    {
        vector<int> v;
        for (int col = 3; col >= 0; col--)
            if (board[row][col] != 0)
                v.push_back(board[row][col]);

        int n = v.size();
        vector<int> ans;
        merge(ans, v, n, score);

        int N = ans.size();
        for (int col = 0; col < N; col++)
            board[row][3 - col] = ans[col];
        for (int col = N; col < 4; col++)
            board[row][3 - col] = 0;
    }
}

int main()
{
    srand(time(0));

    vector<int> randoms = {4, 2, 2, 2, 2, 2, 2, 2, 2, 2};
    vector<vector<int>> board(4, vector<int>(4, 0));

    int score = 0;

    bool check = true;
    check = randomNumber(board, randoms);
    check = randomNumber(board, randoms);
    displayBoard(board, score);

    while (true)
    {
        string x;
        cout << "ENTER  YOUR  CHOICE: ";
        getline(cin, x);

        for (int i = 0; i < lines; i++)
            cout << endl;

        if (!(x == "W" or x == "A" or x == "S" or x == "D"))
            continue;

        if (x == "W")
            operateUp(board, score);
        else if (x == "A")
            operateLeft(board, score);
        else if (x == "S")
            operateDown(board, score);
        else
            operateRight(board, score);

        check = randomNumber(board, randoms);
        displayBoard(board, score);

        if (!check)
        {
            cout << "GAME OVER" << endl;
            cout << "Final Score: " << score << endl
                 << endl;
            break;
        }
    }

    return 0;
}
