#include <cmath>
#include <string>

#include "card.h"

void sm2(unsigned int &n, unsigned int grade, unsigned int &interval,
         float &easiness_factor) {
  // Correct response
  if (grade >= 3) {
    if (n == 0)
      interval = 1;
    else if (n == 1)
      interval = 6;
    else
      interval = (int)roundf((float)interval * easiness_factor);
    n++;
  }
  // Incorrect response
  else {
    n = 0;
    interval = 1;
  }

  easiness_factor += (float)((0.1 - (5 - grade)) * (0.08 + (5 - grade) * 0.02));
  if (easiness_factor < 1.3) {
    easiness_factor = 1.3;
  }
}

Card::Card(std::string ask, std::string res, float diff) {
  ask_ = std::move(ask);
  res_ = std::move(res);
  easiness_ = diff;
  nbSeen_ = 0;
  lastSeen_ = 0;
  usrGrade_ = 0;
  n_ = 0;
  interval_ = 0;
}

void Card::update() { sm2(n_, usrGrade_, interval_, easiness_); }

void Card::save(std::ofstream &os) const {
  os << ask_ << std::endl;
  os << res_ << std::endl;
  os << easiness_ << std::endl;
  os << nbSeen_ << std::endl;
  os << lastSeen_ << std::endl;
  os << usrGrade_ << std::endl;
  os << n_ << std::endl;
  os << interval_ << std::endl;
  os << std::endl;
}

void Card::load(std::ifstream &is) {
  std::getline(is, ask_);
  std::getline(is, res_);
  is >> easiness_;
  is.ignore();
  is >> nbSeen_;
  is.ignore();
  is >> lastSeen_;
  is.ignore();
  is >> usrGrade_;
  is.ignore();
  is >> n_;
  is.ignore();
  is >> interval_;
  is.ignore();
  is.ignore();
}

Deck::Deck() {
  nbCards_ = 0;
  deckName_ = "Default";
  cards_.emplace_back("Test", "test");
  cardsToSave_.emplace_back("Test", "test");
  nbCards_++;
}

Deck::Deck(std::string name) {
  nbCards_ = 0;
  deckName_ = std::move(name);
  cards_.emplace_back("Test", "test");
  cardsToSave_.emplace_back("Test", "test");
  nbCards_++;
}

void Deck::addCard(const Card &card) {
  cards_.push_back(card);
  cardsToSave_.push_back(card);
  nbCards_++;
}

void Deck::save(std::ofstream &os) const {
  os << deckName_ << std::endl;

  if (cardsToSave_[0].getAsk() == "Test" && cardsToSave_[0].getRes() == "test")
    os << cardsToSave_.size() - 1 << std::endl;
  else
    os << cardsToSave_.size() << std::endl;

  bool has_ben_updated = false;

  for (const auto &i : cardsToSave_) {
    if (i.getAsk() == "Test" && i.getRes() == "test")
      continue;
    for (const auto &j : cards_) {
      if (i.getAsk() == j.getAsk()) {
        if (j.getLastSeen() > i.getLastSeen()) {
          has_ben_updated = true;
          j.save(os);
        }
      }
    }
    if (!has_ben_updated)
      i.save(os);

    has_ben_updated = false;
  }
}

void Deck::load(std::ifstream &is) {
  std::getline(is, deckName_);
  is >> nbCards_;
  is.ignore();

  for (unsigned int i = 0; i < nbCards_; i++) {
    Card new_card("None", "None");
    new_card.load(is);
    cards_.push_back(new_card);
    cardsToSave_.push_back(new_card);
  }
}

void Deck::delCard(unsigned int cardNb) {
  cards_.erase(cards_.begin() + cardNb);
  cardsToSave_.erase(cardsToSave_.begin() + cardNb);
  nbCards_--;
}

std::vector<std::string> Deck::getQuestions() const {
  std::vector<std::string> items;
  items.reserve(cards_.size());

  for (const auto &i : cards_)
    items.push_back(i.getAsk());

  return items;
}

void Deck::update() {
  std::vector<unsigned int> to_delete;

  for (unsigned int i = 0; i < nbCards_; ++i) {
    unsigned int t = time(nullptr);
    if (cards_[i].getLastSeen() == 0)
      cards_[i].setLastSeen(t);

    if (cards_[i].getLastSeen() + DAY_IN_SECONDS * cards_[i].getInterval() > t)
      to_delete.push_back(i);
  }

  for (const auto &j : to_delete) {
    cards_.erase(cards_.begin() + j);
    nbCards_--;
  }
}
