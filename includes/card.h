#ifndef MEMO_CARD_H
#define MEMO_CARD_H

#include <fstream>
#include <vector>

#define DAY_IN_SECONDS 86400 // 24 * 60 * 60

// Super Memo 2 algorithm
void sm2(unsigned int &n, unsigned int grade, unsigned int &interval,
         float &easiness_factor);

class Card {
private:
  std::string ask_;
  std::string res_;
  float easiness_;
  unsigned int n_;
  unsigned int nbSeen_;
  unsigned int lastSeen_;
  unsigned int usrGrade_;
  unsigned int interval_;

public:
  // Constructor
  Card(std::string ask, std::string res, float diff = 2.5);

  // Getters
  unsigned int getRepetition() const { return n_; };
  float getEasiness() const { return easiness_; };
  unsigned int getLastSeen() const { return lastSeen_; };
  unsigned int getNbSeen() const { return nbSeen_; };
  unsigned int getUserGrade() const { return usrGrade_; };
  unsigned int getInterval() const { return interval_; };
  std::string getAsk() const { return ask_; };
  std::string getRes() const { return res_; };

  // Setters
  void setUserGrade(unsigned int grade) { usrGrade_ = grade; };
  void setLastSeen(unsigned int lastSeen) { lastSeen_ = lastSeen; };
  void see() {
    nbSeen_++;
    if (usrGrade_ >= 3)
      n_++;
  };

  void save(std::ofstream &os) const;
  void load(std::ifstream &is);

  void update();
};

class Deck {
private:
  std::vector<Card> cards_;
  std::vector<Card> cardsToSave_;
  unsigned int nbCards_;
  std::string deckName_;

public:
  // Constructor
  Deck();
  explicit Deck(std::string name);

  // Getters
  std::vector<Card> getCards() const { return cards_; };
  unsigned int getNbCards() const { return nbCards_; };
  Card *getCardByNb(unsigned int nb) { return &cards_[nb]; };
  std::string getDeckName() const { return deckName_; };

  std::vector<std::string> getQuestions() const;

  void addCard(const Card &card);
  void delCard(unsigned int carbNb);
  void save(std::ofstream &os) const;
  void load(std::ifstream &is);

  void update();
};

#endif // MEMO_CARD_H
