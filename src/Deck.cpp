#include <algorithm>
#include <vector>
#include <fstream>

#include "Deck.hpp"

Card::Card(std::string question, std::string answer, std::string tag)
{
    this->question = question;
    this->answer = answer;
    this->tag = tag;
}

Card::Card(std::string answer, std::string tag)
{
    this->question = extract_uppercase_words(answer);
    this->answer = answer;
    this->tag = tag;
}

std::string Card::extract_uppercase_words(std::string txt)
{
    std::vector<std::string> words;
    std::vector<std::string> upperCaseWords;
    std::stringstream in(txt);
    std::stringstream out;

    std::string token;
    while (std::getline(in, token, ' '))
    {
        words.push_back(token);
    }
    
    std::copy_if(words.begin(), words.end(), std::back_inserter(upperCaseWords), 
    [](std::string str)
    {
        auto upperCnt = std::count_if(str.begin(), str.end(), [](char c) { return (std::isupper(c)); });
        auto lowerCnt = std::count_if(str.begin(), str.end(), [](char c) { return (std::islower(c)); });

        return (str.length() > 2) && (upperCnt > lowerCnt);
    });

    std::sort(upperCaseWords.begin(), upperCaseWords.end());
    upperCaseWords.erase(std::unique(upperCaseWords.begin(), upperCaseWords.end()), upperCaseWords.end());

    for (auto it = upperCaseWords.begin(); it != upperCaseWords.end(); it++)
    {
        out << ((it == upperCaseWords.begin()) ? "" : " ") << *it;
    }

    return out.str();
}

std::string Card::esc(std::string txt)
{
    char c = '"';
    std::string str = txt;

    for(size_t found = str.find(c); found != std::string::npos; found = str.find(c, found + 2))
    {
        str.replace(found, 1, std::string(2,c));
    }

    return str;
}

std::string Card::to_csv_line()
{
    std::stringstream ss;

    ss << "\"" << esc(question) << "\"";
    ss << ",\"" << esc(answer) << "\"";
    ss << ",\"" << esc(tag) << "\"";
    
    return ss.str(); 
}

Deck::Deck(std::string deckName)
{
    this->deckName = deckName;
}

bool DeckExporter::export_csv(std::string filePath)
{
    std::vector<std::string> lines;
    std::for_each(begin(), end(), [&lines](Deck& deck)
    {
        std::for_each(deck.begin(), deck.end(), [&lines](Card& card)
        {
            lines.push_back(card.to_csv_line());
        });
    });

    std::ofstream file(filePath, std::ios::out);

    if (file.is_open())
    {
        std::for_each(lines.begin(), lines.end(), [&file](std::string line)
        {
            file << line << std::endl;
        });
    }

    file.close();
    return true;
}