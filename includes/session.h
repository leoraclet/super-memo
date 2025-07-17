#ifndef MEMO_SESSION_H
#define MEMO_SESSION_H

#include <vector>
#include <string>
#include <ctime>

#include "card.h"


class Session {
private:
    std::vector<Deck> allDecks_;
    unsigned int nbDecks_;
    time_t startTime_;
public:
    // Constructor
    Session();

    // Getters
    Deck* getDeckByNb(unsigned int nb) {return &allDecks_[nb];};
    unsigned int getNbDecks() const {return nbDecks_;};
    std::vector<std::string> getDeckNames() const;
    tm* getStartTime() const;

    void addDeck(Deck &deck);
    void delDeck(unsigned int deckNb);
    void start();
    void save() const;
    void load();

    void update();
};


#endif //MEMO_SESSION_H
