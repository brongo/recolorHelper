#pragma once
#include <vector>
#include <string>

namespace RecolorTool
{
    class Config
    {
        public:
            int cfgHeaderSize = 5;
            int maxFileCount = 1000;
            int maxSearchIterationCount = maxFileCount * 2;
            int maxColorLookAhead = 12;
            int maxVarLookAhead = 100; // only used in config files
            int maxVarLookBehind = 5;
            int maxInputPathLength = 512;
            int minInputPathLength = 8;

            std::string colorValue1 = "0.0001";
            std::string colorValue2 = "0.00001";
            std::string colorValue3 = "0.000001";
        
            std::string useSpecial = "false";
            std::string specialSearchType = "none";
            size_t specialSearchTypeIndex = -1;

            std::string deleteUnmodifiedFiles = "false";

            // List of supported color types
            std::vector<std::string> colorTypes
            {
                "type",                 // fxTypes
                "colorOnSpawn",
                "colorOnDeath",
                "decalEmmissiveTint",
                "decalDiffuseTint",
                "emissiveTint",
                "explosionGradient",
                "explosionGradient2",
                "explosionGradient3",
                "emissiveMaskColor",
                "emissiveMaskColor2",
                "lightColor",
                "particleColorTint",
                "surfacecolor2",
                "surfaceemissivecolor",
                "surfaceemissivecolor2",
                "surfacesheencolor",
                "default_noMatch"
            };
            std::vector<std::string> particleColorTypes
            {
                "initialColor",
                "fadeColor",
                "color",
                "explosionGradient",
                "explosionGradient2",
                "explosionGradient3",
                "emissiveMaskColor",
                "emissiveMaskColor2",
                "default_noMatch"
            };
            std::vector<std::string> fxTypes
            {
                "\"FX_LIGHT\"",         // change all except "ambient_light"
                "\"FX_RENDERPARM\"",    // change none except "fire_primary"
                "\"FX_SOUND\"",         // change none except "fire_primary"
                "\"FX_PARTICLE\"",
                "\"FX_MODEL\"",
                "\"FX_SCREEN_SHAKE\"",
                "\"FX_RIBBON_2\"",
                "\"FX_DECAL\"",
                "\"FX_DESTRUCTIBLE\"",
                "default_noMatch"
            };
            std::vector<std::string> specialTypes
            {
                "\"FX_DECAL\"",
                "\"FX_PARTICLE\"",
                "default_noMatch"
            };

            // List of predefined search types
            std::vector<std::string> predefinedSearchList
            {
                "bfg_stun",
                "ice_bomb_freeze",
                "on_fire",
                "plasma_stun",
                "default_noMatch"
            };
    };
}
