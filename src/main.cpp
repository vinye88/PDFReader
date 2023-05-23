#include <iostream>
#include <vector>
#include <poppler-document.h>
#include <poppler-page.h>

#include "EasyFileHandle.hpp"

std::vector<std::string> split(std::string txt, std::string delimiter)
{
    std::string str = txt;
    std::vector<std::string> list;
    size_t pos {0};
    while ((pos = str.find(delimiter)) != std::string::npos) 
    {
        std::string word = str.substr(0, pos);
        str.erase(0, pos + delimiter.length());
        if (!word.empty())
            list.push_back(word);
    }
    
    list.push_back(str);

    return list;
}

int main (int argc, char* argv[])
{
    std::string pdfPath = "./LEI_SECA.pdf";
    std::vector<std::string> lines;
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
        std::string pageText = std::string(utf.begin(), utf.end());
        std::vector<std::string> list = split(pageText, "\n");
        lines.insert(lines.end(), list.begin(), list.end());

        delete page;
    }

    std::string fileName = EasyFile::get_file_name(pdfPath);
    
    for (unsigned int i = 0; i < lines.size(); i++)
        std::cout << i << ": Size=" << lines[i].size() << "\tPos=" << lines[i].find("  ") << std::endl;
    
    //EasyFile("./" + fileName + ".txt", lines).print();
    
    system("pause");
    return 0;
}