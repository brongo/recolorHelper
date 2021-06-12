#include "DeclReadWrite.h"

namespace RecolorTool
{
    // DeclReader Functions
    bool DeclReader::openFileStream(const char* inputFile)
    {
        setInputFile(inputFile);
        _inputStream = std::ifstream(_inputFile);
        if (_inputStream.is_open()) {
            return 1;
        }
        return 0;
    }
    void DeclReader::closeFileStream()
    {
        if (_inputStream.is_open())
            _inputStream.close();
        return;
    }
    DeclFile DeclReader::readDeclFile()
    {
        DeclFile declFile;
        declFile.setFileName(_inputFile);
        std::string line;

        while (std::getline(_inputStream, line))
        {
            DeclSingleLine declSingleLine;
            declSingleLine.readFromStream(_inputStream, line);
            declFile.setLineData(declSingleLine);
            declFile.lineCount++;
        }
        return declFile;
    }

    // DeclWriter Functions
    bool DeclWriter::openFileStream(const char* outputFile)
    {
        _outputFile = outputFile;
        _outputStream = std::ofstream(_outputFile);
        if (_outputStream.is_open()) {
            return 1;
        }
        return 0;
    }
    void DeclWriter::closeFileStream()
    {
        if (_outputStream.is_open())
            _outputStream.close();
        return;
    }
    void DeclWriter::writeDeclFile(DeclFile declFile)
    {
        printf("Writing to file: %s \n", _outputFile.c_str());
        if (_outputStream.is_open())
        {
            int i = 0;
            for (i = 0; i < declFile.lineCount; i++)
            {
                DeclSingleLine out = declFile.getLineData(i);
                std::string output = out.lineStart + out.lineVariable + out.lineAssignment + out.lineValue + out.lineTerminator + "\n";
                _outputStream << output;
            }
        }
        _outputStream.close();
        return;
    }
}