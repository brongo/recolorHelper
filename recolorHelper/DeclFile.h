#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <locale> // std::isalpha
#include "Config.h"
#include "Utilities.h"

namespace RecolorTool
{
    typedef unsigned long long uint64;

    class DeclSingleLine
    {
        public:
            std::string lineStart = "";
            std::string lineVariable = "";
            std::string lineAssignment = "";
            std::string lineValue = "";
            std::string lineTerminator = "";
            int formatIsGood = 1;

        public:
            bool isRGB() const;
            bool isCommentedOut() const;
            bool endsWithSemicolon() const;
            void readFromStream(std::ifstream& input, std::string line);
    };

    class DeclFile
    {
        private:
            std::vector<DeclSingleLine> _lineData;
            std::string _declFileName;

        public:
            int lineCount = 0;
            int fileWasModified = 0;
            Config config;

            // Basic Getters/Setters
            std::string getFileName() const { return _declFileName; };
            DeclSingleLine getLineData(int lineNumber) const { return _lineData[lineNumber]; }
            void setFileName(std::string fileName) { _declFileName = fileName; }
            void setLineData(DeclSingleLine lineData) { _lineData.push_back(lineData); }
            void setDeclConfig(Config mainConfig) { config = mainConfig; }

            // Multi-line search functions
            uint64 getLineAfterVariable(uint64 lineNumberStart, std::string variableName) const;
            std::string findPreviousLineValue(uint64 lineNumberStart, std::string variableName) const;
            std::string findNextLineValue(uint64 lineNumberStart, std::string variableName) const;
            std::vector<uint64> multiLookAhead_forVariable(uint64 lineNumberStart, std::string variableName, uint64 lookAheadDistance, uint64 maxNumToFind) const;

            // For identifying .decl files that need special handling
            bool inCharacterFXDirectory() const;
            bool inParticleDirectory() const;
            bool inWeaponPlayerDirectory() const;

            // Determines which config typeList to use
            std::vector<std::string> getStandardTypeList() const;

            // Returns index of value found in config typeList
            size_t getListIndex_LineVariable(const std::vector<std::string> typeList, uint64 startLine) const;
            size_t getListIndex_LineValue(const std::vector<std::string> typeList, uint64 startLine) const;

            // Returns the line number we need for DeclFile::setColorVars()

            uint64 getColorLineNumber(const std::vector<std::string> typeList, size_t listIndex, uint64 startLine) const;
            uint64 getColorLineNumber_byFXType(uint64 lineNumber) const;
            uint64 getColorLineNumber_bySpecialType(uint64 lineNumber) const;

            // Only function that actually sets color values
            void setColorVars(uint64 lineNumber);

            // Alternate path for special search types
            void loopSpecialSearchType();
            void loopPlasmaRifleSearchType();

            // Main function
            void initRecolor();
    };
}

