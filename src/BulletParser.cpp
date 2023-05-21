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

#include "PDFTopicReader.hpp"
#include "Deck.hpp"

std::vector<std::string> getLines(std::string filePath)
{
    std::vector<std::string> lines;
    std::ifstream file(filePath, std::ios::in);
    
    if (file.is_open())
    {
        std::string line;
        while ( std::getline(file, line) )
            lines.push_back(line);
    }

    return lines;
}

void printFile(std::string filePath)
{
    for (auto line : getLines(filePath))
    {
        std::cout << line << std::endl;
    }
}

void writeFile(std::string filePath, std::vector<std::string> lines)
{
    std::ofstream file(filePath, std::ios::out);
    
    if (file.is_open())
        for (std::string line : lines)
            file << line << std::endl;
    
    file.close();
}

std::vector<std::string> getFileList(std::string path, std::string ext)
{
    std::vector<std::string> list;

    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (std::filesystem::is_regular_file(entry.path()))
        {
            std::string filePath = entry.path().string();
            if (filePath.substr(filePath.find_last_of('.') + 1 ) == ext)
                list.push_back(filePath);
        }
    }

    return list;
}

std::string getCsvFileName(std::string file)
{
    return file.substr(0,file.find_last_of('.')) + ".csv";
}

std::string getFileName(std::string fileName)
{
    size_t pos {0};
    std::string delimiter = "/";
    std::string txt = fileName;//fileName.replace(0, fileName.length()-1, '\\', delimiter[0]);
    
    while ((pos = txt.find(delimiter)) != std::string::npos) 
        txt.erase(0, pos + delimiter.length());

    return txt.substr(0,txt.find_last_of('.'));
}

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
    
    auto pdfList = getFileList(pdfFolder, "pdf");
    
    DeckExporter exporter;
    for(std::string pdfPath : pdfList)
    {
        PDFTopicReader reader({"•",""}, pdfPath);
        std::vector<std::string> lines = reader.get_txt();
        if (!lines.empty())
        {
            std::string txtFile = "./" + getFileName(pdfPath) + ".txt";
            if (!std::filesystem::exists(txtFile))
                writeFile(txtFile, lines);
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
        std::string txtFile = "./" + getFileName(pdfPath) + ".txt";
        std::vector<std::string> list = getLines(txtFile);
        
        Deck deck(csvName);
        for (std::string text : list)
        {
            deck.push_back(Card(text,getFileName(pdfPath)));
        }

        exporter.push_back(deck);
    }

    csvName = "./" + csvName + ".csv";
    bool exported = exporter.export_csv(csvName);
    
    std::cout << "exported? " << ((exported) ? "yes " + csvName : "no") << std::endl;

    system("pause");

    return 0;
}