
#include "PDFTopicReader.hpp"

#include <poppler-document.h>
#include <poppler-page.h>


PDFTopicReader::PDFTopicReader(std::vector<std::string> topicDelimiters, std::string pdfPath)
{
    this->topicDelimiters = topicDelimiters;
    this->pdfPath = pdfPath;
    extractedText = std::string();
    parse();
}

void PDFTopicReader::parse()
{
    poppler::document* doc = poppler::document::load_from_file(pdfPath);
    if (!doc) {
        throw std::invalid_argument("PDF loading error - " + pdfPath);
    }
    
    for (int i = 0; i < doc->pages(); ++i)
    {
        poppler::page* page = doc->create_page(i);
        if (!page)
        {
            delete doc;
            throw std::runtime_error("Error on creating PDF page");
        }

        std::vector<char> utf = page->text().to_utf8();
        extractedText += std::string(utf.begin(), utf.end());

        delete page;
    }
}

std::vector<std::string> PDFTopicReader::get_txt()
{
    for (std::string delimiter : topicDelimiters)
    {
        std::string str = extractedText;
        std::vector<std::string> tokens;
        size_t pos {0};
        
        while ((pos = str.find(delimiter)) != std::string::npos) 
        {
            std::string word = str.substr(0, pos);
            str.erase(0, pos + delimiter.length());
            if (!word.empty())
                tokens.push_back(word);
        }
        tokens.push_back(str);

        std::for_each(tokens.begin(), tokens.end(),
        [&](std::string & str)
        {
            str = clean_txt(str);
        });

        if (tokens.size() > 1)
            return tokens;
    }
    
    return std::vector<std::string>();
}

std::string PDFTopicReader::clean_txt(const std::string text)
{
    std::string newText = text;

    std::replace(newText.begin(), newText.end(), '\n', ' ');
    
    newText.erase(std::remove_if(newText.begin(), newText.end(),
        [](unsigned char c) { return std::iscntrl(c); }),
        newText.end());

    newText.erase(std::unique(newText.begin(), newText.end(),
        [](char a, char b) { return std::isspace(a) && std::isspace(b); }),
        newText.end());

    size_t begin = newText.find_first_not_of(" ");
    size_t end = newText.find_last_not_of(" ");
    if (begin == std::string::npos || end == std::string::npos)
        return "";
    
    return newText.substr(begin, end - begin + 1);
}