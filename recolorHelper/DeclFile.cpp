#include "DeclFile.h"

namespace RecolorTool
{
    // DeclSingleLine functions
    bool DeclSingleLine::isRGB() const
    {
        if (lineVariable == "r" || lineVariable == "g" || lineVariable == "b" || lineVariable == "a")
            return 1;
        return 0;
    }
    void DeclSingleLine::readFromStream(std::ifstream& input, std::string line)
    {
        size_t splitPos = 0;
        auto it = std::find_if(line.begin(), line.end(), [](char c) {
            return std::isalpha(c, std::locale());
            });

        if (it != line.end())
        {
            splitPos = (line.length() - (line.end() - it));
            lineStart = line.substr(0, splitPos);
            line = line.substr(splitPos, line.length() - splitPos);

            splitPos = line.find(" = ");
            if (splitPos != -1)
            {
                lineVariable = line.substr(0, splitPos);
                lineAssignment = line.substr(splitPos, 3);
                lineValue = line.substr(splitPos + 3, line.length() - (splitPos + 3) - 1);
                lineTerminator = line.substr(line.length() - 1, 1);

                // support for unix line endings
                if (lineTerminator == "\r")
                {
                    lineValue = line.substr(splitPos + 3, line.length() - (splitPos + 3) - 2);
                    lineTerminator = line.substr(line.length() - 2, 1);
                }
            }
            else
            {
                // for "editorvars" format
                lineVariable = line;
            }
        }
        else
        {
            // catchall if line contains no alphanumeric values
            lineStart = line;
        }
    }

    // Multi-line search functions
    uint64 DeclFile::getLineAfterVariable(uint64 lineNumberStart, std::string variableName) const
    {
        uint64 maxLookAhead = config.maxColorLookAhead;
        if ((lineNumberStart + maxLookAhead) > lineCount)
            maxLookAhead = lineCount - lineNumberStart;

        for (uint64 i = 0; i < maxLookAhead; i++)
        {
            DeclSingleLine thisLine = _lineData[lineNumberStart + i];
            if (thisLine.lineVariable == variableName)
                return lineNumberStart + i;
        }
        return -1;
    }
    std::string DeclFile::findPreviousLineValue(uint64 lineNumberStart, std::string variableName) const
    {
        uint64 maxLookBehind = config.maxVarLookBehind;
        if (lineNumberStart < maxLookBehind)
            maxLookBehind = lineNumberStart;

        for (uint64 i = 1; i <= maxLookBehind; i++)
        {
            DeclSingleLine thisLine = _lineData[lineNumberStart - i];
            if (thisLine.lineVariable == variableName)
                return thisLine.lineValue;
        }
        return "";
    }
    std::string DeclFile::findNextLineValue(uint64 lineNumberStart, std::string variableName) const
    {
        uint64 maxLookAhead = config.maxVarLookAhead;
        if ((lineNumberStart + maxLookAhead) > lineCount)
            maxLookAhead = lineCount - lineNumberStart;

        for (uint64 i = 0; i < maxLookAhead; i++)
        {
            DeclSingleLine thisLine = _lineData[lineNumberStart + i];
            if (thisLine.lineVariable == variableName)
                return thisLine.lineValue;
        }
        return "";
    }
    std::vector<uint64> DeclFile::multiLookAhead_forVariable(uint64 lineNumberStart, std::string variableName, uint64 lookAheadDistance, uint64 maxNumToFind) const
    {
        int found = 0;
        std::vector<uint64> matchedLines;
        uint64 maxLookAhead = lookAheadDistance;

        if ((lineNumberStart + maxLookAhead) > lineCount)
            maxLookAhead = lineCount - lineNumberStart;

        for (uint64 i = 0; i < maxLookAhead; i++)
        {
            DeclSingleLine thisLine = _lineData[lineNumberStart + i];

            if (found >= maxNumToFind)
                return matchedLines;

            if (thisLine.lineVariable == variableName)
            {
                matchedLines.push_back(lineNumberStart + i);
                found++;
            }
        }
        return matchedLines;
    }

    // For identifying .decl files that need special handling
    bool DeclFile::inCharacterFXDirectory() const
    {
        #ifdef _WIN32
        if ((_declFileName.find("fx\\character") != -1) || (_declFileName.find("fx\\demon_player") != -1))
            return 1;
        return 0;
        #else
        if ((_declFileName.find("fx/character") != -1) || (_declFileName.find("fx/demon_player") != -1))
            return 1;
        return 0;
        #endif
    }
    bool DeclFile::inParticleDirectory() const
    {
        #ifdef _WIN32
        if (_declFileName.find("decls\\particle") != -1)
            return 1;
        return 0;
        #else
        if (_declFileName.find("decls/particle") != -1)
            return 1;
        return 0;
        #endif
    }
    bool DeclFile::inWeaponPlayerDirectory() const {
        #ifdef _WIN32
                if (_declFileName.find("weapon\\weapon\\player") != -1)
                    return 1;
                return 0;
        #else
                if (_declFileName.find("weapon/weapon/player") != -1)
                    return 1;
                return 0;
        #endif
    }

    // Determines which config typeList to use
    std::vector<std::string> DeclFile::getStandardTypeList() const
    {
        if (inParticleDirectory())
            return config.particleColorTypes;
        return config.colorTypes;
    }

    // Returns index of value found in config typeList
    size_t DeclFile::getListIndex_LineVariable(std::vector<std::string> searchList, uint64 lineNumber) const
    {
        auto it = std::find(searchList.begin(), searchList.end(), _lineData[lineNumber].lineVariable);
        if (it != searchList.end())
        {
            size_t listIndex = searchList.size() - (searchList.end() - it);
            return listIndex;
        }
        return -1;
    }
    size_t DeclFile::getListIndex_LineValue(std::vector<std::string> searchList, uint64 lineNumber) const
    {
        auto it = std::find(searchList.begin(), searchList.end(), _lineData[lineNumber].lineValue);
        if (it != searchList.end())
        {
            size_t listIndex = searchList.size() - (searchList.end() - it);
            return listIndex;
        }
        return -1;
    }

    // Returns the line number we need for DeclFile::setColorVars()
    uint64 DeclFile::getColorLineNumber(std::vector<std::string> searchList, size_t listIndex, uint64 startLine) const
    {
        if ((listIndex == 0) && (searchList[0] == "type"))
            return getColorLineNumber_byFXType(startLine);
        return startLine;
    }
    uint64 DeclFile::getColorLineNumber_byFXType(uint64 lineNumber) const
    {
        size_t fxTypeIndex = getListIndex_LineValue(config.fxTypes, lineNumber);
        if (fxTypeIndex == -1)
            return -1;

        switch (fxTypeIndex)
        {
        case 0:
        {
            // FX_LIGHT - change this *except* for ambient_light
            std::string fxItemName = findPreviousLineValue(lineNumber, "name");
            if (!case_insensitive_match(fxItemName, "\"ambient_light\""))
                return getLineAfterVariable(lineNumber, "color");
            break;
        }
        case 1:
        {
            // FX_RENDERPARM - *only* change "fire_primary" group
            std::string fxItemGroup = findPreviousLineValue(lineNumber, "group");
            if (case_insensitive_match(fxItemGroup, "\"fire_primary\""))
                return getLineAfterVariable(lineNumber, "color");
            break;
        }
        case 2:
        {
            // FX_SOUND - same as FX_RENDERPARM
            std::string fxItemGroup = findPreviousLineValue(lineNumber, "group");
            if (case_insensitive_match(fxItemGroup, "\"fire_primary\""))
                return getLineAfterVariable(lineNumber, "color");
            break;
        }
        default:
        {
            // FX_PARTICLE, FX_MODEL, FX_SCREEN_SHAKE, FX_RIBBON_2, FX_DECAL, FX_DESTRUCTIBLE
            return getLineAfterVariable(lineNumber, "color");
        }
        }
        return -1;
    }
    uint64 DeclFile::getColorLineNumber_bySpecialType(uint64 lineNumber) const
    {
        size_t listIndex = config.specialSearchTypeIndex;
        switch (listIndex)
        {
            case 0:
            {
                // DO ALL
                std::string fxItemGroup;

                // "bfg_stun" characterFX
                fxItemGroup = findPreviousLineValue(lineNumber, "group");
                if (case_insensitive_match(fxItemGroup, "\"bfg_stun\""))
                    return getLineAfterVariable(lineNumber, "color");

                // "ice_bomb_freeze" characterFX
                fxItemGroup = findPreviousLineValue(lineNumber, "group");
                if (case_insensitive_match(fxItemGroup, "\"ice_bomb_freeze\""))
                    return getLineAfterVariable(lineNumber, "color");

                // "on_fire" and "on_fire_upgraded" characterFX
                fxItemGroup = findPreviousLineValue(lineNumber, "group");
                if (case_insensitive_match(fxItemGroup, "\"on_fire\"") || case_insensitive_match(fxItemGroup, "\"on_fire_upgraded\""))
                    return getLineAfterVariable(lineNumber, "color");

                // "plasma_stun" characterFX
                fxItemGroup = findPreviousLineValue(lineNumber, "group");
                if (case_insensitive_match(fxItemGroup, "\"plasma_stun\""))
                    return getLineAfterVariable(lineNumber, "color");

                return -1;
            }
            case 1:
            {
                // "bfg_stun" characterFX
                std::string fxItemGroup = findPreviousLineValue(lineNumber, "group");
                if (case_insensitive_match(fxItemGroup, "\"bfg_stun\""))
                    return getLineAfterVariable(lineNumber, "color");
                return -1;
            }
            case 2:
            {
                // "ice_bomb_freeze" characterFX
                std::string fxItemGroup = findPreviousLineValue(lineNumber, "group");
                if (case_insensitive_match(fxItemGroup, "\"ice_bomb_freeze\""))
                    return getLineAfterVariable(lineNumber, "color");
                return -1;
            }
            case 3:
            {
                // "on_fire" and "on_fire_upgraded" characterFX
                std::string fxItemGroup = findPreviousLineValue(lineNumber, "group");
                if (case_insensitive_match(fxItemGroup, "\"on_fire\"") || case_insensitive_match(fxItemGroup, "\"on_fire_upgraded\""))
                    return getLineAfterVariable(lineNumber, "color");
                return -1;
            }
            case 4:
            {
                // "plasma_stun" characterFX
                std::string fxItemGroup = findPreviousLineValue(lineNumber, "group");
                if (case_insensitive_match(fxItemGroup, "\"plasma_stun\""))
                    return getLineAfterVariable(lineNumber, "color");
                return -1;
            }
            default:
                return -1;
        }
    }

    // Only function that actually sets color values
    void DeclFile::setColorVars(uint64 lineNumber)
    {
        // warning: 
        // cursed function, will fix later
        
        this->fileWasModified = 1;

        int numVars = 3;
        int numFound = 0;

        int foundVar1 = 0;
        int foundVar2 = 0;
        int foundVar3 = 0;

        std::string colorVar1 = "x";
        std::string colorVar2 = "y";
        std::string colorVar3 = "z";

        std::string colorValue1 = config.colorValue1;
        std::string colorValue2 = config.colorValue2;
        std::string colorValue3 = config.colorValue3;

        // makes sure we use correct color type (rgb or xyz)
        DeclSingleLine rgbCheckLine = _lineData[lineNumber + 1];
        if (rgbCheckLine.isRGB())
        {
            colorVar1 = "r";
            colorVar2 = "g";
            colorVar3 = "b";
        }

        // flags to exclude values set to -1 in user config
        int writeVar1 = 1;
        int writeVar2 = 1;
        int writeVar3 = 1;

        if (colorValue1 == "-1")
            writeVar1 = 0;

        if (colorValue2 == "-1")
            writeVar2 = 0;

        if (colorValue3 == "-1")
            writeVar3 = 0;

        // initial loop through to set colorVars
        // color not set if writeVar(n) = 0, but still counted in foundVar(n)
        for (uint64 i = lineNumber + 1; i < (lineNumber + 1 + numVars); i++)
        {
            DeclSingleLine valueLine = _lineData[i];
            if (valueLine.lineVariable == colorVar1 || valueLine.lineVariable == colorVar2 || valueLine.lineVariable == colorVar3)
            {
                if (valueLine.lineVariable == colorVar1) {
                    if (writeVar1)
                        _lineData[i].lineValue = colorValue1;
                    foundVar1 = 1;
                }

                if (valueLine.lineVariable == colorVar2) {                  
                    if (writeVar2)
                        _lineData[i].lineValue = colorValue2;
                    foundVar2 = 1;
                }

                if (valueLine.lineVariable == colorVar3) {
                    if (writeVar3)
                        _lineData[i].lineValue = colorValue3;
                    foundVar3 = 1;
                }
            }
            // this means we're in the wrong place
            else if (valueLine.lineVariable == "initialColor" || valueLine.lineVariable == "fadeColor" || valueLine.lineVariable == "gradient")
            {
                return;
            }
        }

        std::string newline;
        std::string indentation;
        numFound = foundVar1 + foundVar2 + foundVar3;
 
        if (numFound <= 0)
        {
            // This can happen if an alpha value is the only one present.
            // In this case we will just set all 3 values.
            // Append all values to inputDecl.Lines[lineNumber].lineTerminator
            
            newline = ""; 
            indentation = _lineData[lineNumber].lineStart + "\t";
            
            if (writeVar1)
                newline += "\n" + indentation + colorVar1 + " = " + colorValue1 + ";";
            if (writeVar2)
                newline += "\n" + indentation + colorVar2 + " = " + colorValue2 + ";";
            if (writeVar3)
                newline += "\n" + indentation + colorVar3 + " = " + colorValue3 + ";";
            
            _lineData[lineNumber].lineTerminator += newline;
            numFound = 3;
        }

        if (numFound < 3)
        {
            // If RGB/XYZ values are not defined in the file, we need to add them.
            // Any new "lines" we need are just appended to the DeclSingleLine.lineTerminator above it.
            // Hacky shortcut, leads to cursed functions like this. Will fix later.

            int addedR = 0;
            int addedG = 0;

            if (!foundVar1 && writeVar1)
            {              
                // R value missing, need to append to parent line 
                indentation = _lineData[lineNumber].lineStart + "\t";
                newline = "\n" + indentation + colorVar1 + " = " + colorValue1 + ";";
                _lineData[lineNumber].lineTerminator += newline;
            }

            if (!foundVar2 && writeVar2)
            {
                if (!foundVar1 && writeVar1)
                {
                    // R & G values missing, append (G) to parent line
                    indentation = _lineData[lineNumber].lineStart + "\t";
                    newline = "\n" + indentation + colorVar2 + " = " + colorValue2 + ";";
                    _lineData[lineNumber].lineTerminator += newline;
                }
                else
                {
                    // R value is present, append to same line as (R)
                    indentation = _lineData[lineNumber + 1].lineStart;
                    newline = "\n" + indentation + colorVar2 + " = " + colorValue2 + ";";
                    _lineData[lineNumber + 1].lineTerminator += newline;
                }
            }

            if (!foundVar3 && writeVar3)
            {
                if (foundVar1 && foundVar2)
                {
                    // both R and G are present
                    indentation = _lineData[lineNumber + 2].lineStart;
                    newline = "\n" + indentation + colorVar3 + " = " + colorValue3 + ";";
                    _lineData[lineNumber + 2].lineTerminator += newline;
                }
                else if (!foundVar1 || !foundVar2)
                {
                    // Either R or G are missing (doesn't matter which)
                    indentation = _lineData[lineNumber + 1].lineStart;
                    newline = "\n" + indentation + colorVar3 + " = " + colorValue3 + ";";
                    _lineData[lineNumber + 1].lineTerminator += newline;
                }
            }
        }
        return;
    }

    // Alternate path for special search types
    void DeclFile::loopSpecialSearchType() 
    {
        for (uint64 lineNumber = 0; lineNumber < lineCount; lineNumber++)
        {
            size_t listIndex;
            listIndex = getListIndex_LineValue(config.specialTypes, lineNumber);

            if (listIndex < 0)
                continue;

            if (listIndex > 1)
                continue;

            uint64 colorLineNumber = getColorLineNumber_bySpecialType(lineNumber);
            if (colorLineNumber == -1)
                continue;

            setColorVars(colorLineNumber);
        }
    }
    void DeclFile::loopPlasmaRifleSearchType()
    {
        for (uint64 lineNumber = 0; lineNumber < lineCount; lineNumber++) 
        {
            size_t listIndex;
            listIndex = getListIndex_LineValue(config.valueVec4Types, lineNumber);

            if (listIndex == -1)
                continue;

            std::vector<uint64> valueVec4Lines = multiLookAhead_forVariable(lineNumber, "valueVec4", 40, 4);

            if (valueVec4Lines.size() == 4)
                for (int i = 0; i < 4; i++)
                    setColorVars(valueVec4Lines[i]);
        }
        return;
    }

    // Main function
    void DeclFile::initRecolor()
    {          
        if ((config.useSpecial == "true") && (inCharacterFXDirectory()))
        {
            loopSpecialSearchType();
            return;
        }

        if (inCharacterFXDirectory())
        {
            this->fileWasModified = 1; // prevent deleting of skipped files
            return;
        }
   
        if (inWeaponPlayerDirectory())
        {
            loopPlasmaRifleSearchType();
            return;
        }
            

        std::vector<std::string> searchList = getStandardTypeList();
        for (uint64 lineNumber = 0; lineNumber < lineCount; lineNumber++)
        {
            size_t listIndex;
            listIndex = getListIndex_LineVariable(searchList, lineNumber);

            if (listIndex == -1)
                continue;

            uint64 colorLineNumber = getColorLineNumber(searchList, listIndex, lineNumber);      
            if (colorLineNumber == -1)
                continue;

            setColorVars(colorLineNumber); 
        }
        return;
    }
}