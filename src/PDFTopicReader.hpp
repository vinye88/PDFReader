
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

class PDFTopicReader
{
    public:
        std::string pdfPath;
        PDFTopicReader(std::vector<std::string> topicDelimiters, std::string pdfPath); 

        std::vector<std::string> get_txt();
    private:
        std::string extractedText;
        std::vector<std::string> topicDelimiters;

        void parse();
        std::string clean_txt(const std::string text);
};