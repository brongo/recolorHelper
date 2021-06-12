#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include "DeclReadWrite.h"

namespace RecolorTool 
{
    namespace fs = std::filesystem;

    typedef unsigned char byte;
    typedef unsigned char uint8;
    typedef signed short int16;
    typedef unsigned short uint16;
    typedef signed int int32;
    typedef unsigned int uint;
    typedef unsigned int uint32;
    typedef long long int64;
    typedef unsigned long long uint64;

    class FileList
    {
        public:
            int fileCount = 0;
            int searchIterations = 0;
            std::vector<std::string> fileNames;
    };

    class RecolorHelper
    {
        private:
            std::string _inputPath;
            int _invalidConfig = 0;

        public:
            
            Config config;
            FileList fileList;
            void setInputPath(std::string inputPath) { _inputPath = inputPath; };

            // read user-defined configuration
            DeclFile openConfigFile();
            DeclFile configNormalizeFormatting(DeclFile configFile);
            bool configColorsAreValid(std::vector<std::string> colorValues) const;
            bool configSearchTypeIsValid(std::string searchType) const;
            bool configIsValid(DeclFile configFile);
            void setConfigValues(DeclFile configFile);

            // deletes unmodified files if set in config
            bool deleteUnmodifiedFile(DeclFile declFile);
            bool deleteEmptyDirectories(const fs::path& basedir);
            bool deleteFromList(std::vector<std::string> emptyDirectories);
            std::vector<std::string> getEmptyDirectories(const fs::path& basedir) const;

            // recursive directory search, adds files to RecolorHelper->FileList
            void getFileList(const fs::path& basedir);
            bool tryGetFileList();

            // user confirmation functions
            void displayConfirmationPrompt() const;
            bool userConfirmationReceived() const;

            // input validation subroutines
            bool inputExceedsMaxFileCount() const;
            bool inputExceedsMaxSearchSize() const;
            bool inputHasDeclExtension() const;
            bool inputNoDeclsFound() const;
            bool inputPathTooShort() const;
            bool inputPathTooLong() const;
            void inputPathRemoveTrailingSlash();

            // main input validation function
            bool inputIsValid();

            // main file i/o and recolor function 
            void processFileList();
    };
}
    