#include "Utilities.h"

namespace RecolorTool
{
    std::string stripWhiteSpace(std::string str)
    {
        std::string::iterator end_pos = std::remove(str.begin(), str.end(), ' ');
        str.erase(end_pos, str.end());
        return str;
    }  
    std::string stripQuotes(std::string str)
    {
        std::string::iterator end_pos = std::remove(str.begin(), str.end(), '"');
        str.erase(end_pos, str.end());
        std::string::iterator end_pos2 = std::remove(str.begin(), str.end(), '\'');
        str.erase(end_pos2, str.end());
        return str;
    }
    std::string normalizePath(std::string s1)
    {
        fs::path p = s1;
        return p.string();
    }
    std::string stringToLowercase(std::string str)
    {
        transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    } 
    int case_insensitive_match(std::string s1, std::string s2)
    {
        s1 = stringToLowercase(s1);
        s2 = stringToLowercase(s2);
        if (s1.compare(s2) == 0)
            return 1; // match
        return 0;
    }
    void closeProgramAfterSleep(int sleepTime)
    {
        printf("Program will exit now...\n\n\n");
        auto sleepTime_ms = std::chrono::milliseconds(sleepTime);
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(sleepTime_ms);
    }

    #ifdef _WIN32
    void closeProgramWIN32()
    {
        std::cin.sync();
        printf("Press any key to continue...\n");
        (void)_getch();
    }
    #endif
}