//Bashir Abdullah
//My ship sails card game.
#include <iostream>
#include <vector>
#include <sstream>
#include <string> 
#include <cstdlib>
#include <ctime>

using namespace std;

// represents card
struct Card {
    int rank;   // Stores rank of card, 2-Two,.., 10-Ten, 11-Jack, 12-Queen, 13-King, 14-Ace
    char suit;  // Stores suit of card, S-Spades, C-Clubs, H-Hearts, D-Diamonds
    
    Card();     // Default constuctor, creates invalid card
    Card(int r, char s); // Constructor, creates and initialized card
};

// Default constructor
Card::Card() {
    rank = 0;
    suit = 0;
}

// Parametrized contructor
Card::Card(int r, char s) {
    rank = r;
    suit = s;
}

// Abstract player class
class Player {
protected:
    vector<Card> hand;      // Stores cards og current player
    string name;            // Stores name of this player
public:
    Player(string n);       

    // Returns name of player
    string getName() { return name; };

    // add card into hand
    void addCard(Card card);

    // Select card, player want to replace
    // This function is different for human player and computer player
    virtual Card selectCardToReplace() = 0;

    // Print cards for player
    void printHand(ostream& os);

    // Checks player wins
    // return true, if wins
    bool check();
};

// Class for human player
class HumanPlayer : public Player {
    // reference to vector of entire player
    // it's needed to use reference, because other player may not be yet added
    vector<Player*>& players;
public:    
    HumanPlayer(string n, vector<Player*>& allPlayers);
    virtual Card selectCardToReplace();
};

// class for computer player
class ComputerPlayer : public Player {
    // find first card with specified suit
    int findCard(char suit);
    // convert suit to index
    char getSuit(int suitIndex);
public:
    ComputerPlayer(string n);
    virtual Card selectCardToReplace();
};

// overloaded operator for printing card
ostream& operator<<(ostream& os, const Card& card) {
    os << card.suit;

    switch (card.rank)
    {
        case 11:
            os << "J";
            break;
        case 12:
            os << "Q";
            break;
        case 13:
            os << "K";
            break;
        case 14:
            os << "A";
            break;
        default:
            os << card.rank;
            break;
    }    
    return os;
}

// Base constructor for Player class
// Initialized name
Player::Player(string n) : name(n) {
}

// Add card into vector of cards
void Player::addCard(Card card) {
    hand.push_back(card);
}

// Print cards in pllayer hands
void Player::printHand(ostream& os) {    
    char c = 'a';  // stores number to choice this card

    // loop throught all cards in player hand
    for (Card card : hand) {
        cout << "(" << c << ") " << card << "   ";
        c++;
    }
}

// Check player wins
bool Player::check() {

    // to initialize your four counters before the loop
    int countSpades = 0;
    int countHearts = 0;
    int countClubs = 0;
    int countDiamonds = 0;

    // loop throught all cards in player hand
    for (Card card : hand)
        // looking at the suit of each card and increasing the counter for that suit.
        switch (card.suit) {
            case 'S':
                countSpades++;
                break;
            case 'C':
                countClubs++;
                break;
            case 'D':
                countDiamonds++;
                break;
            case 'H':
                countHearts++;
                break;
        }

    // player is winner, if one of counter reach 7
    return countSpades == 7 || countHearts == 7 || countClubs == 7 || countDiamonds==7;
}

// Creates Human player
HumanPlayer::HumanPlayer(string n, vector<Player*>& allPlayers) : Player(n), players(allPlayers) {

}

// Selection card for human player
Card HumanPlayer::selectCardToReplace() {

    char c;

    // print hand of human player
    cout << "  ";
    printHand(cout);
    cout << endl << endl;

    do
    {
        cout << "Which one to replace? ";
        // ask user to enter
        cin >> c;

        // check, if user wants to cheat
        if (c == '?') {
            // print app players, excepting current one
            for (size_t i = 0; i < players.size(); i++) {
                // skip current player
                if (players[i] != this)
                {
                    // print name of palyer and hand
                    cout << players[i]->getName() << "  ";
                    players[i]->printHand(cout);
                    cout << endl;
                }
            }
        }
        // loop until user chooses card
    } while (c<'a' || c>'g');
    cout << endl;

    // get index of card
    int selected = c - 'a';
    // store card in order to return
    Card card = hand[selected];
    // remove card from hand
    hand.erase(hand.begin() + selected);

    // return card to replace
    return card;
}

ComputerPlayer::ComputerPlayer(string n) : Player(n) {
}

// find first card with specified suit
int ComputerPlayer::findCard(char suit) {
    int index = -1;

    for (size_t i = 0; i < hand.size(); i++) {
        if (hand[i].suit == suit)
            return i;
    }
    return index;
}

// convert suit to index
char ComputerPlayer::getSuit(int suitIndex) {
    switch (suitIndex) {
        case 0:
            return 'S';
            break;
        case 1:
            return 'C';
            break;
        case 2:
            return 'D';
            break;
        case 3:
            return 'H';
            break;
        default:
            return -1;
            break;
    }
}

Card ComputerPlayer::selectCardToReplace() {

    int counts[4];
    // to initialize your four counters before the loop
    for (int i = 0; i < 4; i++)
        counts[i] = 0;

    // loop throught all cards in player hand
    for (Card card : hand)
        // looking at the suit of each card and increasing the counter for that suit.
        switch (card.suit) {
        case 'S':
            counts[0]++;
            break;
        case 'C':
            counts[1]++;
            break;
        case 'D':
            counts[2]++;
            break;
        case 'H':
            counts[3]++;
            break;
    }

    int selectedSuit = -1;
    //find suit that has minimal occurence, but has more than 0 
    for (int i = 0; i < 4; i++) {
        if (counts[i]>0) {
            // check it's first suit
            if (selectedSuit == -1)
                selectedSuit = i;
            // check current suit occurrs
            else if (counts[selectedSuit]>counts[i])
                selectedSuit = i;       
        }
    }
    
    int selected = findCard(getSuit(selectedSuit));
        
    Card card = hand[selected];
    hand.erase(hand.begin() + selected);
    return card;
}

// Creates new deck, 
// assumes deck is empty
void createDeck(vector<Card>& deck)
{
    // loop through all ranks fropm Two till Ace
    for (int i = 2; i <= 14; i++) {
        // Add all 4 suits for certain rank
        deck.push_back(Card(i, 'D'));
        deck.push_back(Card(i, 'C'));
        deck.push_back(Card(i, 'H'));
        deck.push_back(Card(i, 'S'));
    }
}

// Deal deck
// assumes there is not too much players
// (7 players are max)
void deal(vector<Player*>& players) {

    // Create new decl
    vector<Card> deck;
    createDeck(deck);

    // Deal until each user has 7 cards
    for (int i = 0; i < 7; i++) {
        // loop through all players
        for (Player* player : players) {
            // pick random card from deck
            int picked = rand() % deck.size();
            // deal this card to player
            player->addCard(deck[picked]);
            // remove dealed card from deck
            deck.erase(deck.begin() + picked);
        }
    }
}

int main() {
   
    srand((unsigned int)time(NULL));

    // Create players
    vector<Player*> players;
    players.push_back(new HumanPlayer("Player 0", players));
    players.push_back(new ComputerPlayer("Player 1"));
    players.push_back(new ComputerPlayer("Player 2"));
    players.push_back(new ComputerPlayer("Player 3"));
    players.push_back(new ComputerPlayer("Player 4"));

    deal(players);

    // Wil be true, if there is any winner
    bool winnerFound = false;

    do
    {
        // Make one turn
        for (size_t i = 0; i < players.size(); i++) {
            // Current player selects card to replace, and remove from their hand
            Card card = players[i]->selectCardToReplace();

            // calculate indexs of next player, wil be 0 for last player
            int next = (i + 1) % players.size();

            // add card to next player
            players[next]->addCard(card);

            // print hint about replaced card
            cout << "Hint: " << players[i]->getName() << " passes " << card << " to " << players[next]->getName() << endl;
        }
        cout << endl;

        // Find winner, loop through all players
        for (size_t i = 0; i < players.size(); i++) {
            // Check player wins
            if (players[i]->check()) {
                // winner found
                winnerFound = true;
                // print info abut winner
                cout << "My Ship Sails! --- " << players[i]->getName() << " Won..." << endl;
            }
        }

    } while (!winnerFound);

    // print final distribution
    cout << endl << "[Final card distributions]" << endl;
    for (size_t i = 0; i < players.size(); i++) {
        cout << players[i]->getName() << "  ";
        players[i]->printHand(cout);
        cout << endl;
    }

    return 0;
}
