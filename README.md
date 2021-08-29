# recolorHelper v0.3.1 by SamPT

recolorHelper is a tool for mass-editing color values in DOOM Eternal. When used on a folder containing .decl files, it will change all color values to the colors you specify. It can also remove any unneeded files automatically. 

__*Important:*__ Make sure you have a backup of your .decl files. This tool does not take backups. It will directly overwrite the files it is used on. I do not take responsibility for any file corruption or loss of data that may occur.

### \*\* New in Version 0.3 \*\*

Now includes a configuration file (recolor.cfg) to allow for easy customization of the recolor program. The configuration options allow you to:

- Set colors to any value (instead of the default 0.0001 for recolor bases).
- Ignore color channels (e.g. set only "red" channels, ignore green and blue).
- Automatically remove unneeded files and delete empty folders.
- Target specific fx groups in fx/character and fx/demon_player (e.g. change "on_fire" effects and skip others).

&nbsp;
## What This Tool Does

The tool will read & edit files with the .decl file extension. It does not read any other type of file. Within a .decl file, the tool will search for specific color values and change them as follows:

```
r = 0.0001;     // or "x"
g = 0.00001;    // or "y"
b = 0.000001;   // or "z"
```

Alpha values ("a" or "w") will not be changed.

**New with version 0.3:** You can override these default colors by editing the "recolor.cfg" file. For instructions on using the custom configuration options, see here:  https://github.com/brongo/recolorHelper/blob/main/CONFIG-README.md

&nbsp;
## Usage 

This tool can operate on a either a *single* .decl file, or a folder/directory containing *multiple* .decl files. **It can not be used on a .zip folder.** You must extract the files first.

If used on a folder/directory, it will search for .decl files **recursively**. This means it will operate on .decl files in that folder, and **any other folder within it**. This makes it easy to keep .decl files in the same folder hierarchy used by the game. 

### Option 1: Drag & Drop

1. Drag & drop the .decl file -or- folder you want to modify onto "recolorHelper.exe"
2. Follow the instructions in the command prompt.

### Option 2: Command Line
 
```
recolorHelper.exe "D:\DoomEternal\DeclFolder\chaingun.decl"   // Modifies a single .decl file.
recolorHelper.exe "D:\DoomEternal\DeclFolder"                 // Modifies *ALL* .decl files in this directory.
```

&nbsp;
## Customizing the Config File ##

The configuration file is explained in a separate documentation file. See here: https://github.com/brongo/recolorHelper/blob/main/CONFIG-README.md

&nbsp;
## Supported color types:

To avoid changing colors that *should not* be changed, the tool operates only on specific color types:

```
colorOnDeath   
colorOnSpawn               
decalDiffuseTint     
decalEmmissiveTint   
emissiveTint     
emissiveMaskColor
emissiveMaskColor2
explosionGradient
explosionGradient2
explosionGradient3
FX_DECAL
FX_DESTRUCTIBLE
FX_MODEL
FX_RIBBON_2
FX_SCREEN_SHAKE
lightColor
particleColorTint    
surfacecolor2
surfaceemissivecolor 
surfaceemissivecolor2
surfacesheencolor
```

Color types with special handling:

```
FX_LIGHT         // changes all color values *except* for "ambient_light"
FX_RENDERPARM    // ignores these values *except* for "fire_primary" - buffs/etc will not be changed.
FX_SOUND         // ignores these values *except* for "fire_primary"
FX_PARTICLE      // see below
```

Color types used with FX_PARTICLE only. **Files must be in /decls/particle or /decls/particlestage directories**

```
color
fadeColor
initialColor
gradient
```

Specific color groups in fx/character and fx/demon_player, if set via recolor.cfg. **Files must be in fx/character or fx/demon_player directories**

```
bfg_stun
ice_bomb_freeze
on_fire
plasma_stun
```

&nbsp;
## Troubleshooting

The .exe file may trigger an anti-virus warning, but it is safe to download and run.

**If you experience problems when using recolorHelper:** Please first read the text in the command prompt/console window. Make a note of any errors or instructions. Most error messages will include a specific reason for the problem and will tell you how to fix it.

If you are unable to resolve the issue, please make a note of the exact error message you received and send details to @SamPT#9513 on Discord. 

&nbsp;
## Feedback:

If you have suggestions for improving the tool, please join the DOOM 2016+ Modding server on Discord, and send a message to @SamPT#9513. The Discord link is here: https://discord.gg/ymRvQaU.

&nbsp;
## Credits:

- Thank you to PowerBall253 (https://github.com/PowerBall253) for helping test the Linux version.
- Thank you to Bezellik and Todema for testing the Windows versions, and advice/suggestions to improve the tool.
- DOOM 2016+ Modding community on Discord: https://discord.gg/ymRvQaU.
