#pragma once
#include <string>
#include <fstream>
#include "DeclFile.h"

namespace RecolorTool
{
    class DeclReader
    {
        private:
            std::string _inputFile;
            std::ifstream _inputStream;

        public:
            void setInputFile(const char* inputFile) { _inputFile = inputFile; }
            bool openFileStream(const char* inputFile);
            void closeFileStream();
            DeclFile readDeclFile(); // creates DeclFile object in memory
    };

    class DeclWriter
    {
        private:
            std::string _outputFile;
            std::ofstream _outputStream;

        public:
            void setOutputFile(const char* outputFile) { _outputFile = outputFile; }
            bool openFileStream(const char* outputFile);
            void closeFileStream();
            void writeDeclFile(DeclFile declFile);
    };
}