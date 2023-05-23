#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <cctype>
#include <filesystem>
#include <tuple>
#include <cxxopts.hpp>

#include "EasyFileHandle.hpp"
#include "PDFTopicReader.hpp"
#include "Deck.hpp"

std::tuple<std::string,std::string> verifyInput(cxxopts::ParseResult result)
{

    if (result.count("h") || result.count("help") || result.count("name") == 0)
    {
        throw std::exception();
    }
    
    std::string csvName = result["name"].as<std::string>();
    std::string pdfFolder = result["file"].as<std::string>();

    return std::tuple<std::string,std::string>(csvName, pdfFolder);
}

int main(int argc, char* argv[])
{
    std::string csvName, pdfFolder;

    cxxopts::Options options("Bullet Parser", "Read PDF with bullets and convert it to a CSV");
    options.add_options()
        ("h,help", "Print help")
        ("file", "PDF Folder", cxxopts::value<std::string>()->default_value("./"))
        ("name", "CSV Name", cxxopts::value<std::string>());

    try
    {
        cxxopts::ParseResult result = options.parse(argc, argv);
        std::tie(csvName, pdfFolder) = verifyInput(result);
    }
    catch(const std::exception& e)
    {
        std::cerr << options.help({"", "Group"}) << '\n';
        return -1;
    }
    
    auto pdfList = EasyFile::get_file_list(pdfFolder, "pdf");
    
    DeckExporter exporter;
    for(std::string pdfPath : pdfList)
    {
        PDFTopicReader reader({"•",""}, pdfPath);
        std::vector<std::string> lines = reader.get_txt();
        if (!lines.empty())
        {
            std::string txtFile = "./" + EasyFile::get_file_name(pdfPath) + ".txt";
            if (!std::filesystem::exists(txtFile))
                EasyFile(txtFile, lines);
            else
                std::cout << "Skipping file " << pdfPath << std::endl;
        }
        else
        {
            std::cout << "Pattern missing for " << pdfPath << std::endl;
        }
    }

    std::cout << "Review txt and "; system("pause");

    for(std::string pdfPath : pdfList)
    {
        std::string fileName = EasyFile::get_file_name(pdfPath);
        std::string txtFile = "./" + fileName + ".txt";
        std::vector<std::string> list = EasyFile(txtFile).get();
        
        Deck deck(csvName);
        for (std::string text : list)
        {
            deck.push_back(Card(text, fileName));
        }

        exporter.push_back(deck);
    }

    csvName = "./" + csvName + ".csv";
    bool exported = exporter.export_csv(csvName);
    
    std::cout << "exported? " << ((exported) ? "yes " + csvName : "no") << std::endl;

    system("pause");

    return 0;
}