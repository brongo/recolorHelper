# Configuration File

RecolorHelper v0.3 includes a configuration file "recolor.cfg" which can be edited using a standard text editor (notepad++ is recommended).

The default file looks like this:

```
#
#  RecolorHelper config
#  Version 0.3.0 
#  Released June 12 2021
#
#  CONFIG DOCUMENTATION:
#  https://github.com/brongo/recolorHelper/blob/main/CONFIG-README.md
#
#  MAKE CHANGES BELOW
#

color1 = 0.0001;
color2 = 0.00001;
color3 = 0.000001;

useSpecial = false;
specialSearchType = none;
deleteUnmodifiedFiles = false;
```

## Editing the Configuration File

- Lines starting with "#" are comments and will be skipped.
- All lines that are not comments **MUST** end with a semicolon (;) or the program will error.

The variables that can be changed are:

```
color1  ("r" or "x" color values)
color2  ("g" or "y" color values)
coloe3  ("b" or "z" color values)

useSpecial  (set to "true" if using specialSearchType)
specialSearchType  (see below for list of valid options)
deleteUnmodifiedFiles  (set to "true" to delete files that don't contain color values)
```

## Color Values

The colors can be set to specific values. For example:

```
color1 = 1;
color2 = 0;
color3 = 0;
```
This configuration would set all the red channels to 1, while setting green and blue channels to 0. It would make everything bright red.

**Please note the valid range for a color is from 0.0 - 1.0. Any values above 1 will result in an error message.** If you aren't sure what color to use, you can do a Google search for "RGB color picker." Most color pickers will have a value from 0-255 for each channel (r, g, b). To convert this to the format used by the game, simply divide by 255. For example, if the color picker gives you a value of 100. The value used in the game is: `(100/255) = 0.3921568`.

## Ignoring Color Channels

You can opt to ignore (leave unchanged) any color channel by setting the value to `-1`. For example:

```
color1 = 0;
color2 = -1;
color3 = 0;
```
This configuration would set all red and blue channels to zero. It would leave the green channel (color2) unchanged.

## Delete Unmodified Files / Empty Directories

This tool will not delete any files by default. However, you can enable this option in the config by setting:

```
deleteUnmodifiedFiles = true;
```

The name of this setting isn't quite accurate. What is *really* does is delete any files that don't contain color values recognized by the tool. It also will not delete **anything** in the fx/character or fx/demon_player folder, unless using `specialSearchType` settings discussed below. If those values aren't set, the tool skips this folder entirely and will not delete them.

Sometimes a file will not be modified, but also won't be deleted. For example, if you run the program twice with the same settings, *technically* the 2nd pass won't change anything, but it will not delete your files. This is done as a safety measure to prevent accidental deletions.

If using this setting, any empty directories will also be removed.

## Special Search Types

These are used only for .decl files in the **fx/character** and **fx/demon_player** folders. By default, the program will ignore these files. If you want to edit files in these folders, you must set `useSpecial = true;` and set `specialSearchType` to one of the following options.

**Options:**
```
all
bfg_stun
ice_bomb_freeze
on_fire
plasma_stun
```

**Only one option can be used at a time. A working example would be:**

```
useSpecial = true;
specialSearchType = plasma_stun;
```

This configuration tells recolorHelper to **only** modify the colors in fx/character and fx/demon_player folders that are associated with the "plasma_stun" group. The purpose of this option is to allow for recoloring plasma rifle effects independently from other weapon effects such as "on_fire", "bfg_stun", etc. Since those effects will generally be a different color.

**Note:** using `specialSearchType = all;` will change all 4 of the types listed (bfg_stun, ice_bomb_freeze, on_fire, and plasma_stun). It will still ignore other values. Most values outside of these effects should not be changed anyway. If you feel that some option is missing, please send feedback to @SamPT.
