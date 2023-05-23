#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

/**
 * @brief Support class to easy handle txt file
 * 
 */
class EasyFile
{
    public:
        /**
         * @brief Construct a new Easy File object, which is going to read all lines from the file
         * 
         * @param file_path path to the txt file to be read
         */
        EasyFile(std::string file_path)
        {
            std::ifstream file(file_path, std::ios::in);
            
            if (file.is_open())
            {
                std::string line;
                while ( std::getline(file, line) )
                    lines.push_back(line);
            }

            file.close();
        }

        /**
         * @brief Construct a new Easy File object which is going to write all lines to the file
         * 
         * @param file_path path to the file which is going to be written
         * @param lines string vector containing all lines that should be writen
         */
        EasyFile(std::string file_path, std::vector<std::string> lines)
        {
            std::ofstream file(file_path, std::ios::out);
            this->lines = lines;
    
            if (file.is_open())
                for (std::string line : lines)
                    file << line << std::endl;
            
            file.close();
        }

        /**
         * @brief Get the file list given an extension and file path
         * 
         * @param file_path path to the folder to search files
         * @param ext file extension to search for
         * @return std::vector<std::string> containing a list of files that matches the extension
         */
        static std::vector<std::string> get_file_list(std::string file_path, std::string ext)
        {
            std::vector<std::string> list;

            for (const auto& entry : std::filesystem::directory_iterator(file_path))
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

        /**
         * @brief Get the file name (no extension) from a full path
         * 
         * @param file_name full file name path
         * @return std::string file name with no extension
         */
        static std::string get_file_name(std::string file_name)
        {
            size_t pos {0};
            std::string delimiter = "/";
            std::string txt = file_name;
            
            while ((pos = txt.find(delimiter)) != std::string::npos) 
                txt.erase(0, pos + delimiter.length());

            return txt.substr(0,txt.find_last_of('.'));

        }

        /**
         * @brief get all lines from the file just read
         * 
         * @return std::vector<std::string> 
         */
        std::vector<std::string> get()
        {
            std::vector<std::string> lines_copy;
            std::copy(lines.begin(), lines.end(), std::back_inserter(lines_copy));
            return lines_copy;
        }

        /**
         * @brief print to the std output the content of the file which was previosly parsed
         * 
         */
        void print()
        {
            for (auto line : lines)
            {
                std::cout << line << std::endl;
            }
        }

    private:
        /**
         * @brief string vector containing the lines which will be written to or was read from a file
         * 
         */
        std::vector<std::string> lines;
};