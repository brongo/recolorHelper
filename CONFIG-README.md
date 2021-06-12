# Configuration File

RecolorHelper v0.3 includes a new configuration file "recolor.cfg" which can be edited using a standard text editor (notepad++ is recommended).

The default file looks like this:

```
#
#  RecolorHelper config
#  Version 0.3.0 
#  Released June 12 2021
#
#  DOCUMENTATION:
#  https://github.com/brongo/recolorHelper/blob/main/README.md
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

- Lines starting with "#" are considered commented and will be skipped.
- All lines that are not commented **MUST** end with a semicolon (;) or the program will error.

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
This configuration would set all the red channels to 100% (1), while setting green and blue channels zero. It would make everything bright red.

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
