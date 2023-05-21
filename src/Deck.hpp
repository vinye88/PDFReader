#include <iostream>
#include <vector>
#include <sstream>
#include <string>

class Card
{
    public:
        Card(std::string question, std::string answer, std::string tag);
        Card(std::string answer, std::string tag);
        std::string to_csv_line();

    private:
        std::string question;
        std::string answer;
        std::string tag;

        std::string esc(std::string txt);
        std::string extract_uppercase_words(std::string txt);
};

class Deck : public std::vector<Card>
{
    public:
        std::string deckName;

        Deck(std::string deckName);
};

class DeckExporter : public std::vector<Deck>
{
    public:
        bool export_csv(std::string filePath);
};