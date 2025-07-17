// C++
#include <iostream>
// Dear ImGui
#include "imgui.h"
#include "imgui-SFML.h"
#include "imgui-style.h"
// SFML
#include "SFML/Graphics.hpp"
// Local
#include "card.h"
#include "session.h"


int main()
{
    // Whether to show or not cards window
    bool is_open = false;

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Memo", sf::Style::Fullscreen);
    // Initialize Dear ImGui
    if (!ImGui::SFML::Init(window)) return 1;
    setImGuiStyle();

    // Cards
    Session session;
    session.load();
    session.start();

    // Needed
    Deck blank_deck = Deck();

    // Colors for buttons
    ImVec4 RED = ImVec4{0.8f, 0.1f, 0.1f, 1.0f};
    ImVec4 GREEN = ImVec4{0.1f, 0.8f, 0.1f, 1.0f};
    ImVec4 BLUE = ImVec4{0.1f, 0.1f, 0.8f, 1.0f};
    ImVec4 BLACK = ImVec4{0.0f, 0.0f, 0.0f, 1.0f};

    // Timer
    sf::Clock clock;

    // Window main loop
    while (window.isOpen())
    {
        ImGui::SFML::Update(window, clock.restart());
        sf::Event event{};

        while (window.pollEvent(event))
        {
            // Process events in ImGui windows
            ImGui::SFML::ProcessEvent(window, event);

            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyReleased:
                    switch (event.key.code) {
                        // Close window when pressing ESC
                        case sf::Keyboard::Escape:
                            window.close();
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

        // =================================================================================== //
        // Graphical User Interface (GUI) for CARDS
        // =================================================================================== //

        // Static variables for ImGUI to print card and decks details
        static int currQuestion = 0;
        static int currDeck = 0;

        Deck *cards;
        Card *c;

        // Handle cases where there is no deck yet
        if (session.getNbDecks() == 0)
            cards = &blank_deck;
        else
            cards = session.getDeckByNb(currDeck);

        c = cards->getCardByNb(currQuestion);
        static std::string question = c->getAsk();

        // ImGui window
        if (is_open) {
            ImGui::Begin("Deck - Current card", &is_open,ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize);
            ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.9f);
            /*
             * Card details
             */
            if (cards->getNbCards() != 0) {
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Details").x) * 0.5f);
                ImGui::Text("Details");
                ImGui::Separator();
                ImGui::NewLine();
                ImGui::Text("Easiness: %1.1f", c->getEasiness());
                ImGui::Text("Number of correct repetition: %d", c->getRepetition());
                ImGui::Text("Last Seen: %d days ago", (time(nullptr) - c->getLastSeen()) / DAY_IN_SECONDS);
                ImGui::Text("Number of times seen: %d", c->getNbSeen());
                ImGui::Text("Interval: %d", c->getInterval());
                ImGui::Text("Last user grade: %d", c->getUserGrade());
                ImGui::NewLine();
            }
            ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Current Card").x) * 0.5f);
            ImGui::Text("Current Card");
            ImGui::Separator();
            ImGui::NewLine();
            if (cards->getNbCards() == 0)
                question = "No cards for today !";
            if (ImGui::Button(question.c_str(), ImVec2(ImGui::GetWindowSize().x * 0.9f, 100.0f)))
            {
                if (cards->getNbCards() != 0) {
                    if (question == c->getAsk())
                        question = c->getRes();
                    else
                        question = c->getAsk();
                }
            }
            ImGui::NewLine();
            static int userGrade = 0;
            ImGui::Text("Mark your memory: ");
            ImGui::SliderInt("##grade", &userGrade, 0, 5);
            ImGui::NewLine();
            ImGui::PushStyleColor(ImGuiCol_Button, BLACK);
            if (ImGui::Button("Mark !", ImVec2(ImGui::GetWindowSize().x * 0.9f, 40.0f)))
            {
                if (cards->getNbCards() != 0)
                {
                    c->setUserGrade(userGrade);
                    c->setLastSeen(time(nullptr));
                    c->see();
                    c->update();
                }
            }
            ImGui::PopStyleColor(1);
            ImGui::NewLine();
            static int nextQuestion = 1;
            ImGui::Text("Next question: ");
            std::vector<std::string> questions = cards->getQuestions();
            const char* preview_q = "";
            if (nextQuestion < cards->getNbCards())
                preview_q = questions[nextQuestion].c_str();
            else if (!questions.empty())
                preview_q = questions[currQuestion].c_str();

            if (ImGui::BeginCombo("##type", preview_q)) {
                for (int i = 0; i < questions.size(); ++i) {
                    const bool isSelected = (nextQuestion == i);
                    if (ImGui::Selectable(questions[i].c_str(), isSelected)) {
                        nextQuestion = i;
                    }
                    // Set the initial focus when opening the combo
                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::NewLine();
            ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Card Actions").x) * 0.5f);
            ImGui::Text("Card Actions");
            ImGui::Separator();
            ImGui::NewLine();
            ImGui::PushStyleColor(ImGuiCol_Button, BLUE);
            if (ImGui::Button("Create New Card", ImVec2(ImGui::GetWindowSize().x * 0.9f, 40.0f)))
                ImGui::OpenPopup("NewCard");

            if (ImGui::BeginPopup("NewCard")) {
                // Create new card
                static int difficulty = 0;
                static char q[100];
                static char a[100];
                ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.9f);
                ImGui::Text("Difficulty: ");
                ImGui::SliderInt("##popupdiff", &difficulty, 0, 5);
                ImGui::NewLine();
                ImGui::Text("Question: ");
                ImGui::InputText("##q", q, IM_ARRAYSIZE(q));
                ImGui::NewLine();
                ImGui::Text("Answer: ");
                ImGui::InputText("##a", a, IM_ARRAYSIZE(a));
                ImGui::NewLine();
                ImGui::PushStyleColor(ImGuiCol_Button, BLACK);
                if (ImGui::Button("Create", ImVec2(ImGui::GetWindowSize().x * 0.9f, 40.0f))) {
                    Card newCard(q, a);
                    cards->addCard(newCard);
                    ImGui::CloseCurrentPopup();
                    q[0] = '\0';
                    a[0] = '\0';
                    difficulty = 0;
                }
                ImGui::PopStyleColor(1);
                ImGui::PopItemWidth();
                ImGui::EndPopup();
            }
            ImGui::PopStyleColor(1);
            ImGui::NewLine();
            ImGui::PushStyleColor(ImGuiCol_Button, GREEN);
            if (ImGui::Button("Next Card", ImVec2(ImGui::GetWindowSize().x * 0.9f, 40.0f))) {
                if ((unsigned int) nextQuestion != cards->getNbCards() && cards->getNbCards() != 0)
                {
                    c = cards->getCardByNb(nextQuestion);
                    question = c->getAsk();
                    currQuestion = nextQuestion;
                    nextQuestion++;
                } else {
                    ImGui::OpenPopup("no_more_cards");
                }
            }
            if (ImGui::BeginPopup("no_more_cards")) {
                ImGui::PushStyleColor(ImGuiCol_Button, BLACK);
                if (ImGui::Button("No more cards !", ImVec2(ImGui::GetWindowSize().x * 0.9f, 40.0f)))
                    ImGui::CloseCurrentPopup();
                ImGui::PopStyleColor(1);
                ImGui::EndPopup();
            }
            ImGui::PopStyleColor(1);
            ImGui::NewLine();
            ImGui::PushStyleColor(ImGuiCol_Button, RED);
            if (ImGui::Button("Delete Card", ImVec2(ImGui::GetWindowSize().x * 0.9f, 40.0f)))
            {
                if (cards->getNbCards() > 1)
                {
                    cards->delCard(currQuestion);
                    if ((unsigned int) currQuestion == cards->getNbCards() && currQuestion != 0)
                        currQuestion--;
                    if ((unsigned int) nextQuestion != cards->getNbCards() - 1)
                        nextQuestion = currQuestion + 1;

                    c = cards->getCardByNb(currQuestion);
                    question = c->getAsk();
                }
            }
            ImGui::PopStyleColor(1);
            ImGui::End();
        }

        // =================================================================================== //
        // Graphical User Interface (GUI) for DECKS
        // =================================================================================== //

        ImGui::Begin("Session", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize);
        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.9f);
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Details").x) * 0.5f);
        ImGui::Text("Details");
        ImGui::Separator();
        ImGui::NewLine();
        tm *time_now = session.getStartTime();
        ImGui::Text("Number of decks: %d", session.getNbDecks());
        ImGui::Text("Current day: %d", time_now->tm_mday);
        ImGui::NewLine();
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Current Deck").x) * 0.5f);
        ImGui::Text("Current Deck");
        ImGui::Separator();
        ImGui::NewLine();
        static std::string deck = cards->getDeckName();
        ImGui::Button(deck.c_str(), ImVec2(ImGui::GetWindowSize().x * 0.9f, 100.0f));
        ImGui::NewLine();
        static int nextDeck = 1;
        ImGui::PushStyleColor(ImGuiCol_Button,  BLACK);
        if (ImGui::Button("Next Deck", ImVec2(ImGui::GetWindowSize().x * 0.9f, 40.0f)))
        {
            if ((unsigned int)nextDeck < session.getNbDecks()) {
                cards = session.getDeckByNb(nextDeck);
                deck = cards->getDeckName();
                currDeck = nextDeck;
                nextDeck++;
                currQuestion = 0;
            }
        }
        ImGui::PopStyleColor(1);
        ImGui::NewLine();
        ImGui::Text("Next deck: ");
        std::vector<std::string> decks = session.getDeckNames();
        const char* preview_d = "";
        if (nextDeck < session.getNbDecks())
            preview_d = decks[nextDeck].c_str();
        else if (!decks.empty())
            preview_d = decks[currDeck].c_str();

        if (ImGui::BeginCombo("##decks", preview_d))
        {
            for (int j = 0; j < decks.size(); ++j)
            {
                const bool isSelected = (nextDeck == j);
                if (ImGui::Selectable(decks[j].c_str(), isSelected)) {
                    nextDeck = j;
                }
                // Set the initial focus when opening the combo
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        ImGui::NewLine();
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Deck Actions").x) * 0.5f);
        ImGui::Text("Deck Actions");
        ImGui::Separator();
        ImGui::NewLine();
        ImGui::PushStyleColor(ImGuiCol_Button,  BLUE);
        if (ImGui::Button("Create New Deck", ImVec2(ImGui::GetWindowSize().x * 0.9f, 40.0f)))
            ImGui::OpenPopup("NewDeck");

        if (ImGui::BeginPopup("NewDeck"))
        {
            // Create new card
            static char deck_name[100];
            ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.9f);
            ImGui::Text("Deck Name: ");
            ImGui::InputText("##deck_name", deck_name, IM_ARRAYSIZE(deck_name));
            ImGui::NewLine();
            ImGui::PushStyleColor(ImGuiCol_Button, BLACK);
            if (ImGui::Button("New Deck", ImVec2(ImGui::GetWindowSize().x * 0.9f, 40.0f)))
            {
                Deck new_deck(deck_name);
                session.addDeck(new_deck);
                ImGui::CloseCurrentPopup();
                deck_name[0] = '\0';
            }
            ImGui::PopStyleColor(1);
            ImGui::PopItemWidth();
            ImGui::EndPopup();
        }
        ImGui::PopStyleColor(1);
        ImGui::NewLine();
        ImGui::PushStyleColor(ImGuiCol_Button,  GREEN);
        if (ImGui::Button("Open Deck", ImVec2(ImGui::GetWindowSize().x * 0.9f, 40.0f)))
        {
            is_open = true;
        }
        ImGui::PopStyleColor(1);
        ImGui::NewLine();
        ImGui::PushStyleColor(ImGuiCol_Button,  RED);
        if (ImGui::Button("Delete Deck", ImVec2(ImGui::GetWindowSize().x * 0.9f, 40.0f)))
        {
            if (session.getNbDecks() > 1)
            {
                session.delDeck(currDeck);
                if ((unsigned int) currDeck == session.getNbDecks() && currDeck != 0)
                    currDeck--;
                if ((unsigned int) nextDeck != session.getNbDecks() - 1)
                    nextDeck = currDeck + 1;

                cards = session.getDeckByNb(currDeck);
                deck = cards->getDeckName();
            }
        }
        ImGui::PopStyleColor(1);
        ImGui::End();

        // =================================================================================== //
        // =================================================================================== //

        // Render to window
        window.clear();
        ImGui::SFML::Render(window);
        window.display();

    }

    // Shutdown Dear ImGui
    ImGui::SFML::Shutdown();

    // Save decks of cards
    session.save();

    return 0;

}