#include "session.h"


Session::Session()
{
    nbDecks_ = 0;
    startTime_ = time(nullptr);
}

tm* Session::getStartTime() const
{
    return gmtime(&startTime_);
}

void Session::load()
{
    std::ifstream is;
    is.open("decks_db.txt");

    is >> nbDecks_;
    is.ignore();

    for (unsigned int i = 0; i < nbDecks_; i++)
    {
        Deck new_deck = Deck();
        new_deck.delCard(0);
        new_deck.load(is);
        allDecks_.push_back(new_deck);
    }

    is.close();
}

void Session::save() const
{
    std::ofstream os;
    os.open("decks_db.txt");

    os << nbDecks_ << std::endl;
    for (unsigned int i = 0; i < nbDecks_; i++)
        allDecks_[i].save(os);

    os.close();
}

void Session::start()
{
    for (int i = 0; i < nbDecks_; ++i) {
        allDecks_[i].update();
    }
}

void Session::addDeck(Deck &deck)
{
    nbDecks_++;
    allDecks_.push_back(deck);
}

std::vector<std::string> Session::getDeckNames() const
{
    std::vector<std::string> items;

    for (unsigned int i = 0; i < nbDecks_; ++i)
        items.push_back(allDecks_[i].getDeckName());

    return items;
}

void Session::delDeck(unsigned int deckNb)
{
    allDecks_.erase(allDecks_.begin() + deckNb);
    nbDecks_--;
}

void Session::update()
{

}
