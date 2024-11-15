#include <iostream>
#include <vector>
#include <ctime>
#include <random> 
#include <thread>
#include <chrono>
#include <algorithm>

using namespace std;

class Card {
public:
    int value;
    string name;

    Card(int v, string n) : value(v), name(n) {}
};

class Deck {
private:
    vector<Card> cards;
    const vector<string> names = { "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
    const vector<int> values = { 6, 7, 8, 9, 10, 10, 10, 10, 11 };

public:
    Deck() {
        initializeDeck();
        shuffleDeck();
    }

    void initializeDeck() {
        cards.clear();
        for (int i = 0; i < names.size(); i++) {
            for (int j = 0; j < 4; j++) {
                cards.emplace_back(values[i], names[i]);
            }
        }
    }

    void shuffleDeck() {
        random_device rd;
        mt19937 g(rd());
        shuffle(cards.begin(), cards.end(), g);
    }

    Card drawCard() {
        if (cards.empty()) {
            cout << "The deck is empty! Shuffling a new deck..." << endl;
            initializeDeck();
            shuffleDeck();
        }
        Card drawnCard = cards.back();
        cards.pop_back();
        return drawnCard;
    }

    int cardsLeft() const {
        return cards.size();
    }
};

class Player {
public:
    int score = 0;

    void drawCard(Deck& deck, const string& playerName) {
        Card card = deck.drawCard();
        score += card.value;
        cout << playerName << " drew a " << card.name << ". " << playerName << "'s score is now " << score << "." << endl;
    }

    void resetScore() {
        score = 0;
    }

    bool isBusted() const {
        return score > 21;
    }

    bool isBlackjack() const {
        return score == 21;
    }
};

void playGame(Deck& deck) {
    Player player, dealer;

    player.resetScore();
    dealer.resetScore();

    cout << "\nStarting a new game! Cards left in the deck: " << deck.cardsLeft() << endl;

    cout << "Dealing cards in 5 seconds..." << endl;
    this_thread::sleep_for(chrono::seconds(5));

    player.drawCard(deck, "You");
    player.drawCard(deck, "You");

    dealer.drawCard(deck, "Dealer");
    dealer.drawCard(deck, "Dealer");

    if (player.isBlackjack()) {
        cout << "Blackjack! You win!" << endl;
        return;
    }

    char choice;
    while (true) {
        cout << "Do you want to draw another card? (y/n): ";
        cin >> choice;

        if (choice == 'y') {
            player.drawCard(deck, "You");
            if (player.isBusted()) {
                cout << "You busted! Dealer wins." << endl;
                return;
            }
            if (player.isBlackjack()) {
                cout << "You win!" << endl;
                return;
            }
        } else {
            break;
        }
    }

    while (dealer.score < 17) {
        dealer.drawCard(deck, "Dealer");
        if (dealer.isBusted()) {
            cout << "Dealer busted! You win!" << endl;
            return;
        }
    }

    cout << "\nYour final score: " << player.score << endl;
    cout << "Dealer's final score: " << dealer.score << endl;

    if (player.score > dealer.score) {
        cout << "You win!" << endl;
    } else if (player.score < dealer.score) {
        cout << "Dealer wins!" << endl;
    } else {
        cout << "It's a tie!" << endl;
    }
}

int main() {
    Deck deck;
    char playAgain;

    do {
        playGame(deck);

        cout << "\nDo you want to play again? (y/n): ";
        cin >> playAgain;

        if (deck.cardsLeft() == 0) {
            deck.initializeDeck();
            deck.shuffleDeck();
            cout << "Deck has been reshuffled!" << endl;
        }
    } while (playAgain == 'y');

    cout << "\nThank you for playing! Goodbye!" << endl;
    return 0;
}
