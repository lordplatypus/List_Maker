#include "FileManager.h"
#include <iostream>

FileManager::FileManager()
{}

FileManager::~FileManager()
{}

void FileManager::Init()
{
    std::fstream inputStream("config"); //open confige
    std::string dummy; //string to store lines
    int i = 0; //makeshift order of operations
    while (std::getline(inputStream, dummy))
    {//while there are still lines to read
        if (dummy[0] == '#' || dummy == "") continue; //don't read lines that start with '#', nor blank lines
        if (i == 0) 
        {//fist step
            if (!SetDirectory(dummy)) SetDirectory("./"); //if the directory in the config fails, use "./"
        }
        else if (i == 1)
        {//second step
            activeFile_ = dummy; //set the active file - if the file doesn't exist, it will be made 
        }
        i++; //next operation
    }
    inputStream.close(); //close config
}

const bool FileManager::SetDirectory(const std::string& string)
{
    DIR *dp; //directory

    if ((dp = opendir(string.c_str())) == NULL) return false; //if the directory doesn't exist, return false
    activeDirectory_ = string; //otherwise set the active directory
    ReadDirectory(dp); //then read the directory for text file
    return true; //return success
}

void FileManager::ReadDirectory(DIR *dp)
{
    files_.clear(); //remove stored file names

    struct dirent *dirp; 
    while ((dirp = readdir(dp)) != NULL) 
    {//while there are files to read
        std::string fname = dirp->d_name; //store name of the files
        if(fname.find(".txt") != std::string::npos) files_.push_back(fname); //if the files has '.txt' in the name, store the file name
    }
}

const bool FileManager::SetFile(const std::string& string)
{
    if (CheckFiles(string)) 
    {//if the file is listed 
        activeFile_ = string; //set active file to the new file
        return true; //return success
    }
    return false; //if not found, return false
}

const bool FileManager::CheckFiles(const std::string& string)
{
    for (auto file : files_) if (file == string) return true; //if found return success
    return false; //else return false
}

const bool FileManager::AddFile(const std::string& string)
{
    std::ofstream outputStream_(activeDirectory_ + string); //make file
    outputStream_.close(); //close file
    if (outputStream_.fail()) return false; //fail if something went wrong
    if (!SetDirectory(activeDirectory_)) return false; //resets the vector of file names, to add the new file
    if (!SetFile(string)) return false; //set the new file as the active file
    return true; //return success
}

const bool FileManager::SubFile(const std::string& string)
{
    if (remove((activeDirectory_ + string).c_str())) return false; //if the file doesn't exist return false
    SetDirectory(activeDirectory_); //resets the vector of file names, to remove the now deleted file
    if (!SetFile(files_[0])) activeFile_ = "Default.txt"; //set the active file to the first file in the vector, or use 'Default.txt' if no files are found
    return true; //return success
}

void FileManager::DisplayAllFiles() const
{
    for (auto file : files_) std::cout << "\033[36m" << file << "\033[0m" << std::endl; //print all text file names in the active directory
}

const std::string& FileManager::GetFile() const
{
    return activeFile_;
}

const std::string& FileManager::GetDirectory() const
{
    return activeDirectory_;
}




const bool FileManager::AddElement(const std::string& string)
{
    std::fstream inputStream(activeDirectory_ + activeFile_); //read the active file
    std::ofstream outputStream_("temp.txt"); //make a temp file to copy the data over to
    std::string dummy; //string to copy lines from the active file
    while (std::getline(inputStream, dummy))
    {//while there are lines to read copy them to the temp file
        outputStream_ << dummy << std::endl;
    }
    outputStream_ << string; //add the new element
    inputStream.close(); //close the active file
    outputStream_.close(); //close the temp file
    remove((activeDirectory_ + activeFile_).c_str()); //delete the now old active file and
    rename("temp.txt", (activeDirectory_ + activeFile_).c_str()); //replace it with the temp file
    return true; 
}

const bool FileManager::SubElement(const std::string& string)
{
    std::fstream inputStream(activeDirectory_ + activeFile_); //open active file
    std::string dummy; //string to store lines from the active file
    std::vector<std::string> elements_; //vector of strings to hold the contents of the file
    bool fail = true; //fail flag
    while (std::getline(inputStream, dummy))
    {//while reading lines
        if (dummy != string) elements_.push_back(dummy); //copy the files data, minus the element marked for deletion
        else fail = false; //if the marked element is found, then success
    }
    inputStream.close(); //close the active file
    if (fail) return false; //return false, if the marked element isn't found

    std::ofstream outputStream_("temp.txt"); //create a temp file to place the active files data, minus the marked element
    for (auto element : elements_) outputStream_ << element << std::endl; //actualy input data
    outputStream_.close(); //close the temp file
    remove((activeDirectory_ + activeFile_).c_str()); //delete the now old active file and
    rename("temp.txt", (activeDirectory_ + activeFile_).c_str()); //replace it with the temp file
    return true; //return success
}

const std::string& FileManager::GetRandElement() const
{
    std::fstream inputStream(activeDirectory_ + activeFile_); //read the active file
    std::string dummy ;//string to store lines from the active file
    std::vector<std::string> elements_; //vector of strings to hold the contents of the file
    while (std::getline(inputStream, dummy))
    {//while there are lines, store them in the string vector
        elements_.push_back(dummy);
    }
    inputStream.close(); //close the active file
    int random = rand() % elements_.size(); //get a random number depending how many elements 

    std::cout << "\033[36m" << elements_[random] << "\033[0m" << std::endl; //print element at rand position

    return elements_[random]; //returns the rand element
    //*Note: printing this directly in the switch in 'main' caused errors
    //that is why it is now being printed in this function
}

void FileManager::DisplayAllElements() const
{
    std::fstream inputStream(activeDirectory_ + activeFile_); //read the active file
    std::string dummy; //string to read lines from the active file
    while (std::getline(inputStream, dummy))
    {//while there are lines, display them on screen
        std::cout << "\033[36m" << dummy << "\033[0m" << std::endl;
    }
    inputStream.close(); //close the active file
}