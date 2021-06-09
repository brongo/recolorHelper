#include "RecolorHelper.h"
using namespace RecolorTool;

namespace RecolorTool
{
    // read user-defined configuration
    DeclFile RecolorHelper::openConfigFile()
    {
        DeclFile configFile;
        const char* configFileName = "recolor.cfg";
        fs::path configPath = fs::current_path() / "recolor.cfg";
        try 
        {
            DeclReader declReader;
            declReader.openFileStream(configFileName);
            configFile = declReader.readDeclFile();
            declReader.closeFileStream();       
        }
        catch (...) 
        {
            printf("\n\n");
            printf("ERROR: Unable to read configuration file.\n");
            _invalidConfig = 1;
        }
        return configFile;
    }
    bool RecolorHelper::configColorsAreValid(std::vector<std::string> colorValues) const
    {
        std::string value;
        for (int i = 0; i < 3; i++)
        {
            if (colorValues[i].empty())
            {
                printf("\n\n");
                printf("ERROR: Config file is missing one or more color settings.\n");
                return 0;
            }

            if (colorValues[i] == "-1")
                continue;

            if ((stof(colorValues[i]) > 1) || (stof(colorValues[i]) < 0))
            {
                printf("\n\n");
                printf("ERROR: Config has invalid color value: %s \n", colorValues[i].c_str());
                printf("Valid range is between 0.0 to 1.0, or -1 to ignore.\n");
                return 0;
            }
                
        }
        return 1;
    }
    bool RecolorHelper::configSearchTypeIsValid(std::string specialSearchType) const
    {
        std::vector<std::string> searchList = config.predefinedSearchList;
        auto it = std::find(searchList.begin(), searchList.end(), config.specialSearchType);
        
        if (it != searchList.end())
            return 1; 

        printf("\n\n");
        printf("ERROR: Config file set \"useSpecial = true\", but \"specialSearchType\" is missing or invalid.\n");
        printf("Please set \"useSpecial = false\", or see documentation for supported \"specialSearchType\" options.\n");
        return 0;
    }
    bool RecolorHelper::configIsValid(DeclFile configFile)
    {
        if (_invalidConfig == 1)
            return 0;

        std::vector<std::string> colorValues = 
        {
            stripQuotes(stripWhiteSpace(configFile.findNextLineValue(0, "color1"))),
            stripQuotes(stripWhiteSpace(configFile.findNextLineValue(0, "color2"))),
            stripQuotes(stripWhiteSpace(configFile.findNextLineValue(0, "color3")))
        };

        if (!configColorsAreValid(colorValues))
            return 0;

        std::string useSpecial = stripQuotes(stripWhiteSpace(configFile.findNextLineValue(0, "useSpecial")));
        std::string specialSearchType = stripQuotes(stripWhiteSpace(configFile.findNextLineValue(0, "specialSearchType")));
        
        if ((config.useSpecial == "true") && (!configSearchTypeIsValid(specialSearchType)))
            return 0;
        return 1;
    }
    void RecolorHelper::setConfigValues(DeclFile configFile)
    {
        config.colorValue1 = stripQuotes(stripWhiteSpace(configFile.findNextLineValue(0, "color1")));
        config.colorValue2 = stripQuotes(stripWhiteSpace(configFile.findNextLineValue(0, "color2")));
        config.colorValue3 = stripQuotes(stripWhiteSpace(configFile.findNextLineValue(0, "color3")));

        config.useSpecial = stripQuotes(stripWhiteSpace(configFile.findNextLineValue(0, "useSpecial")));
        config.specialSearchType = stripQuotes(stripWhiteSpace(configFile.findNextLineValue(0, "specialSearchType")));
        config.deleteUnmodifiedFiles = stripQuotes(stripWhiteSpace(configFile.findNextLineValue(0, "deleteUnmodifiedFiles")));

        if (config.useSpecial == "true")
        {
            std::vector<std::string> searchList = config.predefinedSearchList;
            auto it = std::find(searchList.begin(), searchList.end(), config.specialSearchType);

            if (it != searchList.end())
                config.specialSearchTypeIndex = searchList.size() - (searchList.end() - it);
        }

    }

    // deletes unneeded files if set in config
    bool RecolorHelper::deleteUnmodifiedFile(DeclFile declFile)
    {
        if (declFile.fileWasModified != 0)
            return 0;

        try 
        {
            // deletes the file
            fs::path p = declFile.getFileName();
            remove(p);

            fs::path parent = p.parent_path();
        
            // deletes the directory if empty
            if (fs::is_empty(parent))
                remove(parent);
        }
        catch (...)
        {
            printf("\n\n");
            printf("WARNING: Unable to remove file or directory: %s \n", declFile.getFileName().c_str());
            return 0;
        }

        return 1;
    };

    // recursive directory search, adds files to RecolorHelper->FileList
    bool RecolorHelper::getFileList(const fs::path& basedir)
    {
        uint64 iterationCount = 0;
        for (const auto& p : fs::recursive_directory_iterator(basedir))
        {
            if (p.path().extension() == ".decl")
            {
                fileList.fileNames.push_back(p.path().string());
                fileList.fileCount++;
            }

            // abort early if too many .decl files found
            if (fileList.fileCount > config.maxFileCount)
                return 0;

            // abort early if too many files/folders found (any type).
            if (iterationCount > config.maxSearchIterationCount)
                return 0;

            iterationCount++;
        }
        return 1;
    }
    bool RecolorHelper::tryGetFileList()
    {
        try {
            const char* filePathArg = _inputPath.c_str();
            if (getFileList(filePathArg))
                return 1;
            return 0;
        }
        catch (...) {
            printf("\n\n");
            printf("ERROR: invalid filepath. Please check the filepath and try again.\n");
            return 0;
        }
        return 0;
    }

    // user confirmation functions
    void RecolorHelper::displayConfirmationPrompt() const
    {
        printf("\n\n");
        printf("PLEASE CONFIRM:\n");
        printf("This operation will modify %d file(s) in the directory:\n", fileList.fileCount);
        printf("\"%s\"\n\n", _inputPath.c_str());
        printf("This action is IRREVERSIBLE. No backups are taken.\n\n");
        printf("Type \"Y\" and press ENTER to confirm, or type \"N\" to cancel.\n\n\n");
        return;
    }
    bool RecolorHelper::userConfirmationReceived() const
    {
        std::string userInput;
        int userConfirmed = 0;
        int tries = 0;
        for (tries = 0; tries < 3; tries++)
        {
            getline(std::cin, userInput);
            if (case_insensitive_match(userInput, "N"))
            {
                printf("\n");
                printf("Cancelled by user.\n");
                return 0;
            }
            if (case_insensitive_match(userInput, "Y"))
            {
                printf("\n");
                userConfirmed = 1;
                tries = 4;
                continue;
            }
            if (tries < 2)
            {
                printf("Command not recognized.\n");
            }
        }
        if ((tries == 3) && (userConfirmed == 0))
        {
            printf("No confirmation received.\n");
            return 0;
        }
        return 1;
    }

    // input validation subroutines
    bool RecolorHelper::inputExceedsMaxFileCount() const
    {
        if (fileList.fileCount > config.maxFileCount)
        {
            printf("\n\n");
            printf("ERROR: Too many .decl files found in this directory (over %d). Disabled for safety reasons.\n", config.maxFileCount);
            return 1;
        }
        return 0;
    }
    bool RecolorHelper::inputExceedsMaxSearchSize() const
    {
        if (fileList.searchIterations > config.maxSearchIterationCount)
        {
            printf("\n\n");
            printf("ERROR: encountered too many non-decl files. Filepath may be incorrect.\n");
            return 1;
        }
        return 0;
    }
    bool RecolorHelper::inputHasDeclExtension() const
    {
        if (_inputPath.substr(_inputPath.length() - 5, 5) == ".decl") {
            return 1;
        }
        return 0;
    }
    bool RecolorHelper::inputNoDeclsFound() const
    {
        if (fileList.fileCount <= 0) {
            printf("\n\n");
            printf("ERROR: no .decl files found. Please check the filepath and try again.\n");
            return 1;
        }
        return 0;
    }
    bool RecolorHelper::inputPathTooShort() const
    {
        // makes sure this isn't a root directory
        if (_inputPath.length() < config.minInputPathLength)
        {
            printf("\n\n");
            printf("ERROR: the filepath you entered is too short. Disabled for safety reasons.\n");
            return 1;
        }
        return 0;
    }
    bool RecolorHelper::inputPathTooLong() const
    {
        if (_inputPath.length() > config.maxInputPathLength)
        {
            printf("\n\n");
            printf("ERROR: the filepath you entered is too long.\n");
            return 1;
        }
        return 0;
    }
    void RecolorHelper::inputPathRemoveTrailingSlash()
    {
        size_t trailingSlashCheck = _inputPath.rfind("\\");
        if (trailingSlashCheck == _inputPath.length() - 1)
            _inputPath = _inputPath.substr(0, _inputPath.length() - 1);
        return;
    }

    // main input validation function
    bool RecolorHelper::inputIsValid()
    {
        if (inputPathTooShort())
            return 0;

        if (inputPathTooLong())
            return 0;

        if (inputHasDeclExtension())
        {
            // just 1 file - add to filelist and return, no need for further checks    
            fileList.fileCount = 1;
            fileList.fileNames.push_back(_inputPath);
            displayConfirmationPrompt();

            if (userConfirmationReceived())
                return 1;
            return 0;
        }

        #ifdef _WIN32
        inputPathRemoveTrailingSlash();
        #endif  
        
        if (!tryGetFileList())
            return 0;

        if (inputExceedsMaxFileCount())
            return 0;

        if (inputExceedsMaxSearchSize())
            return 0;

        if (inputNoDeclsFound())
            return 0;

        // passed safety checks, ask user to verify before proceeding.
        displayConfirmationPrompt();

        if (userConfirmationReceived())
            return 1;
        return 0;
    }

    // loops file i/o and recolor functions
    void RecolorHelper::processFileList()
    {
        for (int i = 0; i < fileList.fileCount; i++)
        {
            const char* filename = fileList.fileNames[i].c_str();

            // Reads .decl file into memory
            DeclReader declReader;
            declReader.openFileStream(filename);
            DeclFile declFile = declReader.readDeclFile();
            declReader.closeFileStream();

            // Applies recolor to .decl file in memory
            declFile.setDeclConfig(config);
            declFile.initRecolor();

            // Deletes unneeded files and empty directories if set in config
            // Disabled until testing is finished + user confirmation is added.
            /*
            if (config.deleteUnmodifiedFiles == "true")
                if (deleteUnmodifiedFile(declFile))
                    continue;
            */

            // Overwrites original .decl file with recolored version.
            DeclWriter declWriter;
            declWriter.openFileStream(filename);
            declWriter.writeDeclFile(declFile);
            declWriter.closeFileStream();
        }
        return;
    }
}

int main(int argc, char* argv[])
{
    #ifdef _WIN32
        std::string examplePath1 = "D:\\DoomEternal\\DeclFolder\\chaingun.decl";
        std::string examplePath2 = "D:\\DoomEternal\\DeclFolder";
        std::string exampleCommand = "recolorHelper.exe";
    #else
        std::string examplePath1 = "/full/path/to/DoomEternal/DeclFolder/chaingun.decl";
        std::string examplePath2 = "/full/path/to/DoomEternal/DeclFolder";
        std::string exampleCommand = "./recolorHelper";
    #endif

    printf("\n");
    printf("RecolorHelper-v0.3.0 by SamPT\n\n");
    printf("Example usage:\n\n");
    printf("  %s \"%s\" - Modify a single.decl file.\n", exampleCommand.c_str(), examplePath1.c_str());
    printf("  %s \"%s\" - Modify *ALL* .decl files in this directory.\n\n", exampleCommand.c_str(), examplePath2.c_str());

    if (argc < 2)
        return 1;

    printf("//////////////////////////////////////////////////////\n");
    printf("//                                                  //\n");
    printf("//                     WARNING!                     //\n");
    printf("//                                                  //\n");
    printf("//      THIS WILL OVERWRITE YOUR .DECL FILES        //\n");
    printf("//    BACK UP YOUR FILES BEFORE USING THIS TOOL     //\n");
    printf("//                                                  //\n");
    printf("//////////////////////////////////////////////////////\n");

    if (argc > 2)
    {
        printf("\n\n");
        printf("ERROR: too many parameters passed in.\n");
        printf("To use this program on multiple files, place all files in a single folder and try again.\n");
        closeProgramAfterSleep(2000);
        return 1;
    }

    RecolorHelper recolorHelper;
    DeclFile configFile = recolorHelper.openConfigFile();

    if (!recolorHelper.configIsValid(configFile))
    {
        closeProgramAfterSleep(2000);
        return 0;
    }

    recolorHelper.setConfigValues(configFile);
    recolorHelper.setInputPath(argv[1]);
    
    if (recolorHelper.inputIsValid())
    {
        recolorHelper.processFileList();
        printf("\n\n");
        printf("Operation completed successfully.\n");
        closeProgramAfterSleep(1600);
        return 0;
    }

    closeProgramAfterSleep(2000);
    return 1;
}