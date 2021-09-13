#include <iostream>
#include <string>
#include <vector>
#include "FileManager.h"

enum Command
{//List of commands
    COMMAND_HELP,
    GET,
    ADD,
    SUB,
    CHANGE,
    LIST,
    RAND,
    COMMAND_ERROR
};

enum Option
{//List of Options
    OPTION_HELP,
    ELEMENT,
    TEXT_FILE,
    DIRECTORY,
    OPTION_ERROR
};

std::vector<std::string> commands_
{//These are the actual typed commands
    "-h",
    "-g",
    "-a",
    "-s",
    "-c",
    "-l",
    "-r"
};

std::vector<std::string> options_
{//These are the actual typed options
    "-h",
    "-e",
    "-f",
    "-d",
};

std::vector<std::string> SeperateParts(const std::string& string)
{//Seperate the user input so they can be analyzed
    std::vector<std::string> parts;
    parts.push_back(""); //populate the vector with the first empty string
    int j = 0; //represents the # of strings in the vector
    for (auto i = string.begin(); i != string.end(); )
    {
        if (*i == ' ') //if 'space'
        {
            parts.push_back(""); //create a new string
            j++; //'activate' the new string
        }
        else
        {
            parts[j].push_back(*i); //push back chars into the 'active' string
        }
        i++; //next char
    }

    return parts; //return the vector of strings
}

int GetCommand(const std::string& string)
{//Analyze the command
    for (int i = 0; i < commands_.size(); i++)
    {//look through the list of commands and look for a match
        if (commands_[i] == string) return i; //return match
    }
    return COMMAND_ERROR; //if no match return error
} 

int GetOption(const std::string& string)
{//Analyze the option
    for (int i = 0; i < options_.size(); i++)
    {//look through the list of options and look for a match
        if (options_[i] == string) return i; //return match
    }
    return OPTION_ERROR; //if no match return error
}

//Not sure why this dosen't work as a function but works fine in "main"
// const std::string& GetElement(std::vector<std::string> parts)
// {
//     std::string element;
//     for (int i = 2; i < parts.size(); i++)
//     {
//         element += parts[i];
//         if (i != parts.size() - 1) element += " ";
//     }
//     return element;
// }

int main()
{
    srand (time(NULL)); //give rand a random seed

    FileManager fm_; //Set up File Manager
    fm_.Init(); //init File Manager - read config file, etc.

    //starting text
    std::cout << "\033[36m" << "Makes lists" << std::endl 
              << "{-h} for help" << std::endl
              << "{exit} to exit the program" << "\033[0m" << std::endl;


    while (true)
    {//keep running until user exits the program
        std::string input_{""}; //string to store user input
        std::getline(std::cin, input_); //actually recieve input
        if (input_ == "exit") return 0; //exit the program

        std::vector<std::string> parts = SeperateParts(input_); //break up user input into a command, option, and element
        int command_ = GetCommand(parts[0]); //determine command
        int option_ = GetOption(parts[1]); //determine option
        std::string element_; //determine element
        //element_ = GetElement(parts);
        for (int i = 2; i < parts.size(); i++)
        {//as the 'SeperateParts' function breaks up the input at spaces this puts anything that wasn't a command or option into 1 string
            element_ += parts[i];
            if (i != parts.size() - 1) element_ += " ";
        }

        //The logic that reads commands and options goes here
        switch (command_)
        {
            case COMMAND_HELP: //Basic help
            std::cout << "\033[36m" << "-h                        this" << std::endl
                      << "-g {option}               get file/directory" << std::endl
                      << "-a {option} {element}     add element/file" << std::endl
                      << "-s {option} {element}     remove element/file" << std::endl
                      << "-c {option} {element}     change file/directory" << std::endl
                      << "-l {option}               list all elements/files" << std::endl
                      << "-r                        output a random element from the current file" << std::endl
                      << "use {-h} as an option to see the other options" << "\033[0m" << std::endl;
            break;

            case GET: //Get information
            switch (option_)
            {
                case OPTION_HELP: //Get help
                std::cout << "\033[36m" << "-g -h               this" << std::endl
                          << "-g -f               get the current text file name" << std::endl
                          << "-g -d               get the current directory" << "\033[0m" << std::endl;
                break;

                case TEXT_FILE: //Get active file name
                std::cout << "\033[36m" << fm_.GetFile() << "\033[0m" << std::endl;
                break;

                case DIRECTORY: //Get active directory
                std::cout << "\033[36m" << fm_.GetDirectory() << "\033[0m" << std::endl;
                break;

                default: //if the input is wrong, throw an error
                std::cout << "\033[31m" << "Whoops, something went wrong" << "\033[0m" << std::endl;
                break;
            }
            break;

            case ADD: //Add an element to the active file or add a file to the active directory
            switch (option_)
            {
                case OPTION_HELP: //Add help
                std::cout << "\033[36m" << "-a -h               this" << std::endl
                          << "-a -e {element}     add element to the current file" << std::endl
                          << "-a -f {element}     add file to the current directory" << "\033[0m" << std::endl;
                break;

                case ELEMENT: //add an element to the active file
                if (fm_.AddElement(element_)) std::cout << "\033[36m" << "Done" << "\033[0m" << std::endl;
                //The above funtion doesn't have a fail state, but it shouldn't fail...
                //as even if the file name is wrong it will just make a new file and put the element there
                break;

                case TEXT_FILE: //add a file to the active directory
                if (fm_.AddFile(element_)) std::cout << "\033[36m" << "Done" << "\033[0m" << std::endl;
                //Shouldn't fail as the program shouldn't allow the user to put in a wrong directory
                break;

                default: //if the input is wrong, throw an error
                std::cout << "\033[31m" << "Whoops, something went wrong" << "\033[0m" << std::endl;
                break;
            }
            break;

            case SUB: //Remove (sub) an element from the active file or remove a file from the active directory
            switch (option_)
            {
                case OPTION_HELP: //Sub help
                std::cout << "\033[36m" << "-s -h               this" << std::endl
                          << "-s -e {element}     remove {element} from the current file" << std::endl
                          << "-s -f {element}     remove the file:{element} from the current directory" << "\033[0m" << std::endl;
                break;

                case ELEMENT: //remove an element from the active file
                if (fm_.SubElement(element_)) std::cout << "\033[36m" << "Done" << "\033[0m" << std::endl;
                else std::cout << "\033[31m" << "Whoops, element not found" << "\033[0m" << std::endl;
                break;

                case TEXT_FILE: //remove a file from the active directory
                if (fm_.SubFile(element_)) std::cout << "\033[36m" << "Done" << "\033[0m" << std::endl;
                else std::cout << "\033[31m" << "Whoops, file not found" << "\033[0m" << std::endl;
                break;

                default: //if the input is wrong, throw an error
                std::cout << "\033[31m" << "Whoops, something went wrong" << "\033[0m" << std::endl;
                break;
            }
            break;

            case CHANGE: //Change the active file or directory
            switch (option_)
            {
                case OPTION_HELP: //Change help
                std::cout << "\033[36m" << "-c -h               this" << std::endl
                          << "-c -f {element}     change the current file to {element}" << std::endl
                          << "-c -d {element}     change the current directory to {element}" << "\033[0m" << std::endl;
                break;

                case TEXT_FILE: //change the active file
                if (fm_.SetFile(element_)) std::cout << "\033[36m" << "Done" << "\033[0m" << std::endl;
                else std::cout << "\033[31m" << "Whoops, file not found" << "\033[0m" << std::endl;
                break;

                case DIRECTORY: //change the active directory
                if (fm_.SetDirectory(element_)) std::cout << "\033[36m" << "Done" << "\033[0m" << std::endl;
                else std::cout << "\033[31m" << "Whoops, directory not found" << "\033[0m" << std::endl;
                break;

                default: //if the input is wrong, throw an error
                std::cout << "\033[31m" << "Whoops, something went wrong" << "\033[0m" << std::endl;
                break;
            }
            break;

            case LIST: //List all elements in the active file or list all files in the active directory
            switch (option_)
            {
                case OPTION_HELP:
                std::cout << "\033[36m" << "-l -h               this" << std::endl
                          << "-l -e               list all elements from the current file" << std::endl
                          << "-l -f               list all text files from the current directory" << "\033[0m" << std::endl;
                break;

                case ELEMENT: //list all elements in the active file
                fm_.DisplayAllElements();
                break;

                case TEXT_FILE: //list all files in the active directory
                fm_.DisplayAllFiles();
                break;

                default: //if the input is wrong, throw an error
                std::cout << "\033[31m" << "Whoops, something went wrong" << "\033[0m" << std::endl;
                break;
            }
            break;

            case RAND: //Grab a random element from the active file
            fm_.GetRandElement();
            break;
        
            default: //if the input is wrong, throw an error
            std::cout << "\033[31m" << "Whoops, something went wrong" << "\033[0m" << std::endl;
            break;
        }
    }
}