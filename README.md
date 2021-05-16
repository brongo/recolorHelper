# recolorHelper v0.1 by SamPT

recolorHelper is a tool for creating recolor bases for DOOM Eternal. It will automatically reset certain .decl color values to 0.0001, eliminating the tedious work of going through each .decl file and changing these line-by-line. 

**Some manual work is still required.** See the section titled "*What this tool DOES NOT do.*"

This initial release is a proof of concept. It has been tested with weapons and pickups only. If there is enough interest, future releases may expand support to other types of items or allow for custom configuration. 

__*Important:*__ Make sure you have a backup of your .decl files. This tool does not take backups. It will directly overwrite the files it is used on. I do not take responsibility for any file corruption or loss of data that may occur.

## What This Tool Does

The tool will read & edit files with the .decl file extension. It does not read any other type of file.

Within a .decl file, the tool will search for specific color values and change them as follows:

```
r = 0.0001;     // or "x"
g = 0.00001;    // or "y"
b = 0.000001;   // or "z"
```

Alpha values ("a" or "w") will not be changed.

### Supported color types:

To avoid changing colors that *should not* be changed, the tool operates only on specific color types:

```
1. colorOnSpawn + colorOnDeath
2. decalDiffuseTint + decalEmmissiveTint
3. particleColorTint
4. surfaceemissivecolor + surfaceemissivecolor2
5. surfacesheencolor
6. FX_PARTICLE
7. FX_MODEL
```

Color types with special handling:

```
1. FX_LIGHT         // changes all color values *except* for "ambient_light"
2. FX_RENDERPARM    // ignores these values *except* for "fire_primary" - buffs/etc will not be changed.
```

## What This Tool DOES NOT Do

**This tool does not create a recolor base for you automatically.** It speeds up the process by changing 90%+ of the color values needed. However, you still need to identify the correct .decl files to change, and manually review & test afterwards.

Some color values will **not** be changed automatically due to ambiguous or inconsistent usage in the .decl files. If you don't see the expected results, you should review the .decl files to see if any color values were skipped. 

## Usage 

This tool can operate on a either a *single* .decl file, or a folder/directory containing *multiple* .decl files.

If used on a folder/directory, it will search for .decl files **recursively**. This means it will operate on .decl files in that folder, and **any other folder within it**. This makes it easy to keep .decl files in the same folder hierarchy used by the game. 

### Option 1: Drag & Drop

1. Drag & drop the .decl file -or- folder you want to modify onto "recolorHelper.exe"
2. If you drag & drop a single file, it will be edited automatically. No output will be shown.
3. If you drag * drop a folder, manual confirmation will be required. Follow the instructions in the command prompt.

### Option 2: Command Line
 
```
recolorHelper.exe "D:\DoomEternal\DeclFolder\chaingun.decl"   // Modifies a single .decl file.
recolorHelper.exe "D:\DoomEternal\DeclFolder"                 // Modifies *ALL* .decl files in this directory.
```

## Troubleshooting

These files may trigger anti-virus warnings. However, they are safe to download and run.

**If you experience problems when using recolorHelper:** Please first read the text in the command prompt/console window. Make a note of any errors or instructions. Most error messages will include a specific reason for the problem and will tell you how to fix it.

If you are unable to resolve the issue, please make a note of the exact error message you received and send details to @SamPT#9513 on Discord. 

## Feedback:

If you have suggestions for improving the tool, please join the DOOM 2016+ Modding server on Discord, and send a message to @SamPT#9513. The Discord link is here: https://discord.gg/ymRvQaU.

## Credits:

- Thank you to PowerBall253 (https://github.com/PowerBall253) for helping test the Linux version.
- Thank you to Bez and Macy for testing the Windows version, and advice/suggestions to improve the tool.
- DOOM 2016+ Modding community on Discord: https://discord.gg/ymRvQaU.
