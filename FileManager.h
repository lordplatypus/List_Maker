#ifndef FILE_MANAGER_H_
#define FILE_MANAGER_H_
#include <fstream>
#include <vector>
#include <dirent.h>

class FileManager
{
public:
    FileManager();
    ~FileManager();
    void Init();

    const bool SetDirectory(const std::string& string);
private:
    void ReadDirectory(DIR *dp);
public:
    const bool SetFile(const std::string& string);
private:
    const bool CheckFiles(const std::string& string);
public:
    const bool AddFile(const std::string& string);
    const bool SubFile(const std::string& string);
    void DisplayAllFiles() const;
    const std::string& GetFile() const;
    const std::string& GetDirectory() const;

    const bool AddElement(const std::string& string);
    const bool SubElement(const std::string& string);
    const std::string& GetRandElement() const;
    void DisplayAllElements() const;

private:
    std::vector<std::string> files_;
    std::string activeFile_;
    std::string activeDirectory_;
};

#endif