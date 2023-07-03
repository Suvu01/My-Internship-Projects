#include <iostream>
#include <windows.h>
using namespace std;
#include <algorithm>
#include <vector>

#include <cstdlib>
#include <time.h>

/*
Color codes used in the program:

1 : Blue
2 : Green
3 : Cyan
4 : Red
5 : Purple
6 : Yellow(dark)
7 : Default white
8 : Grey
9 : Bright blue
10 : Bright green
11 : Bright cyan
12 : Bright red
13 : pink
14 : yellow
15 : Bright white

*/

HANDLE col = GetStdHandle(STD_OUTPUT_HANDLE);

class PromptTheUser
{
private:
    float sum;

public:
    PromptTheUser()
    {
        cout << "Enter the starting amount (in $): ";
        float amount;
        cin >> amount;

        cout << endl;

        setInitialAmount(amount);
    }

    inline void setInitialAmount(float amount);
    inline float getSum();
    inline void modify(float net_addition);
    inline void myAmount();
};

class Blackjack
{
public:
    int d_sum, my_sum;
    vector<int> deck;
    vector<int> myCards, dealer;

    int bet;

    Blackjack(vector<int> myCards, vector<int> dealer, vector<int> &cards)
    {
        d_sum = my_sum = 0;

        deck = cards;
        this->myCards = myCards;
        this->dealer = dealer;
    }

    // Set the bet in each game / turn:
    inline void placeBet();
    inline int getBet();

    // Push cards to myCards and dealer:
    inline void pushMyCard(int val);
    inline void pushDealerCard(int val);

    // Sum of my cards and cards of the dealer:
    inline int MySum();
    inline int DealerSum();

    // Game Status:
    inline void JackPot();
    inline void Win();
    inline void Lost();
    inline void Push();

    // Displaying cards:
    inline void DisplayMyCards();
    inline void DisplayDealerCards();

    // Prompt to ask to play a new game:
    inline bool NewGame();

    // Ask the user whether to pick an extra card or not:
    inline bool ExtraCard();

    // Checking if the the decks contain Ace(s):
    inline bool aceInMyCards();
    inline bool aceInDealer();

    // Removing Ace(s) for better positions:
    inline void removeMyAce();
    inline void removeDealerAce();
};

int main()
{
    srand(time(0));
    vector<int> cards = {11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10};
    int N = 12;

    // The dealer will decide whether to pick a card or not.
    vector<int> probability;
    for (int i = 0; i < 100; i++)
        probability.push_back(i % 2);

    // Prompt the user to enter the starting amount.
    SetConsoleTextAttribute(col, 10);
    PromptTheUser me;
    SetConsoleTextAttribute(col, 15);

    while (me.getSum() > 0)
    {
        SetConsoleTextAttribute(col, 10);
        Blackjack B({}, {}, cards);
        B.placeBet(); // Prompt the user to set a bet in the first round.
        int bet = B.getBet();
        SetConsoleTextAttribute(col, 15);

        me.modify(-bet); // Deduct the bet from the original amount.

        B.pushMyCard(cards[rand() % N]);
        B.pushMyCard(cards[rand() % N]);

        B.pushDealerCard(cards[rand() % N]);
        B.pushDealerCard(cards[rand() % N]);

        // If the user gets the sum 21 in the first two moves:
        if (B.MySum() == 21)
        {
            if (B.DealerSum() == 21)
            {
                B.Push(), B.DisplayMyCards(), B.DisplayDealerCards();

                me.modify(bet);
                continue;
            }
            else
            {
                B.JackPot(), B.DisplayMyCards(), B.DisplayDealerCards();

                me.modify(bet + 1.5 * bet);
                continue;
            }
        }

        B.DisplayMyCards();

        SetConsoleTextAttribute(col, 11);
        cout << "One of the cards of the dealer is " << B.dealer[0] << "." << endl
             << endl;
        SetConsoleTextAttribute(col, 15);

        // While the sum is less than or equal to 21 and user wishes to draw an extra card:
        bool lost = false;
        while (!lost)
        {
            bool ask = B.ExtraCard();
            if (!ask)
                break;

            B.pushMyCard(cards[rand() % N]);
            while (B.my_sum > 21 and B.aceInMyCards())
            {
                B.removeMyAce();
                B.my_sum -= 10;
            }
            B.DisplayMyCards();

            if (B.my_sum > 21)
            {
                B.DisplayDealerCards(), B.DisplayMyCards(), B.Lost();

                lost = true;

                me.myAmount();
                break;
            }
        }

        // If the user loses the game, he has two choices, either to quit or continue playing:
        if (lost)
        {
            if (me.getSum() == 0)
            {
                cout << "Thanks for playing. You go home with $" << me.getSum() << ". Visit again." << endl
                     << endl;
                break;
            }
            bool ask = B.NewGame();
            if (ask)
                continue;

            SetConsoleTextAttribute(col, 10);
            cout << "Thanks for playing. You go home with $" << me.getSum() << ". Visit again." << endl
                 << endl;
            SetConsoleTextAttribute(col, 15);

            break;
        }

        // Now it is the dealer's turn:
        bool win = false;
        while (true)
        {
            if (B.d_sum <= 11)
                B.pushDealerCard(cards[rand() % N]);
            else
            {
                int prob = probability[rand() % 100];
                if (prob == 0 or B.d_sum >= 20)
                    break;

                B.pushDealerCard(cards[rand() % N]);
                while (B.d_sum > 21 and B.aceInDealer())
                {
                    B.removeDealerAce();
                    B.d_sum -= 10;
                }
                if (B.d_sum > 21)
                {
                    B.DisplayDealerCards(), B.DisplayMyCards(), B.Win();

                    me.modify(2 * bet), me.myAmount();

                    win = true;
                    break;
                }
            }
        }

        // If the user wins because the dealer gets a sum greater than 21, he / she can choose to continue playing or quit:
        if (win)
        {
            bool ask = B.NewGame();
            if (ask)
                continue;

            SetConsoleTextAttribute(col, 10);
            cout << "Thanks for playing. You go home with $" << me.getSum() << ". Visit again." << endl
                 << endl;
            SetConsoleTextAttribute(col, 15);

            break;
        }

        // If both the user and the dealer have a sum less than or equal to 21:
        B.DisplayDealerCards(), B.DisplayMyCards();

        if (B.my_sum > B.d_sum)
        {
            B.Win();
            me.modify(2 * bet), me.myAmount();
        }

        else if (B.my_sum == B.d_sum)
        {
            B.Push(), me.modify(bet), me.myAmount();
        }

        else
        {
            B.Lost();
            me.myAmount();
        }

        // Ask the user for a new game:
        bool ask = B.NewGame();
        if (ask)
            continue;

        SetConsoleTextAttribute(col, 10);
        cout << "Thanks for playing. You go home with $" << me.getSum() << ". Visit again." << endl
             << endl;
        SetConsoleTextAttribute(col, 15);

        break;
    }
    return 0;
}

// Member Functions for an object of class PromptTheUser:
void PromptTheUser::setInitialAmount(float amount)
{
    sum = amount;
}

float PromptTheUser::getSum()
{
    return sum;
}

void PromptTheUser::modify(float net_addition)
{
    sum = sum + net_addition;
}

void PromptTheUser::myAmount()
{
    SetConsoleTextAttribute(col, 13);

    cout << "You have $" << sum << " left." << endl
         << endl;

    SetConsoleTextAttribute(col, 15);
}

// Member Functions for an object of class Blackjack:
void Blackjack::placeBet()
{
    cout << "Enter the amount you wish to bet (in $): ";
    int amount;
    cin >> amount;

    cout << endl;

    bet = amount;
}

int Blackjack::getBet()
{
    return bet;
}

// Push cards to myCards and dealer:
void Blackjack::pushMyCard(int val)
{
    myCards.push_back(val);
    my_sum += val;
}

void Blackjack::pushDealerCard(int val)
{
    dealer.push_back(val);
    d_sum += val;
}

// Sum of my cards and the cards of the dealer:
int Blackjack::MySum()
{
    return my_sum;
}

int Blackjack::DealerSum()
{
    return d_sum;
}

// Game Status:
void Blackjack::JackPot()
{
    SetConsoleTextAttribute(col, 10);
    cout << "Blackjack! You Won The Game!" << endl
         << endl;

    SetConsoleTextAttribute(col, 15);
}

void Blackjack::Win()
{
    SetConsoleTextAttribute(col, 10);

    cout << "Congrats! You Won The Game!" << endl
         << endl;

    SetConsoleTextAttribute(col, 15);
}

void Blackjack::Lost()
{
    SetConsoleTextAttribute(col, 10);

    cout << "Dealer Wins!" << endl
         << endl;

    SetConsoleTextAttribute(col, 15);
}

void Blackjack::Push()
{
    SetConsoleTextAttribute(col, 10);

    cout << "Push! The Game is a Draw." << endl
         << endl;

    SetConsoleTextAttribute(col, 15);
}

// Displaying cards:
void Blackjack::DisplayMyCards()
{
    SetConsoleTextAttribute(col, 14);

    cout << "Your cards are given below:" << endl;
    cout << "[";
    for (int i = 0; i < myCards.size() - 1; i++)
        cout << myCards[i] << ", ";
    cout << myCards[myCards.size() - 1];
    cout << "]";
    cout << endl;
    cout << "The sum of your cards is " << my_sum << "." << endl
         << endl;

    SetConsoleTextAttribute(col, 15);
}

void Blackjack::DisplayDealerCards()
{
    SetConsoleTextAttribute(col, 11);

    cout << "The dealer's cards are given below:" << endl;
    cout << "[";
    for (int i = 0; i < dealer.size() - 1; i++)
        cout << dealer[i] << ", ";
    cout << dealer[dealer.size() - 1];
    cout << "]";
    cout << endl;
    cout << "The sum of the dealer's cards is " << d_sum << "." << endl
         << endl;

    SetConsoleTextAttribute(col, 15);
}

// Prompt to ask to play a new game:
bool Blackjack::NewGame()
{
    SetConsoleTextAttribute(col, 13);

    char ask;
    cout << "Do you wish to continue ? (Y / N) " << endl;
    cin >> ask;

    SetConsoleTextAttribute(col, 15);

    cout << endl;

    if (ask == 'Y')
        return true;
    return false;
}

// Ask the user whether to pick an extra card or not:
bool Blackjack::ExtraCard()
{
    SetConsoleTextAttribute(col, 13);

    char ask;
    cout << "Do you wish to pick an extra card ? (Y / N) " << endl;
    cin >> ask;

    SetConsoleTextAttribute(col, 15);

    cout << endl;

    if (ask == 'Y')
        return true;
    return false;
}

// Checking if the the decks contain Ace(s):
bool Blackjack::aceInMyCards()
{
    int n = myCards.size();
    for (int i = 0; i < n; i++)
        if (myCards[i] == 11)
            return true;

    return false;
}

bool Blackjack::aceInDealer()
{
    int n = dealer.size();
    for (int i = 0; i < n; i++)
        if (dealer[i] == 11)
            return true;

    return false;
}

// Removing Ace(s) for better positions:
void Blackjack::removeMyAce()
{
    int n = myCards.size();
    for (int i = 0; i < n; i++)
    {
        if (myCards[i] == 11)
        {
            myCards[i] = 1;
            return;
        }
    }
}

void Blackjack::removeDealerAce()
{
    int n = dealer.size();
    for (int i = 0; i < n; i++)
    {
        if (dealer[i] == 11)
        {
            dealer[i] = 1;
            return;
        }
    }
}