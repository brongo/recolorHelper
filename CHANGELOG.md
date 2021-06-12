# Changelog
All notable changes to this project will be documented in this file.

## [0.3.0] - 2021-06-12

- Added config file (recolor.cfg), for users to set custom config options.
- Added config option for custom color values, set via config file.
- Added config option to ignore color channels (example: set only "red" channels, ignore green and blue).
- Added config option to remove unmodified files.
- Added config options to target specific fx/character and fx/demon_player groups.
- Added support for FX_SOUND color used by plasma rifle.
- Added support for "valueVec4" colors used by plasma rifle heat blast mod.
- Additional error handling and safety checks throughout program.
- Significant code refactor for maintainability.

## [0.2.0] - 2021-05-23

- Added support for **FX types:** 
  ```
  1. FX_DECAL
  2. FX_DESTRUCTIBLE
  3. FX_RIBBON_2
  4. FX_SCREEN_SHAKE
  ```
- Added support for **Standard types:** 
  ```
  1. emissiveTint
  2. explosionGradient 
  3. explosionGradient2
  4. explosionGradient3
  5. emissiveMaskColor
  6. emissiveMaskColor2
  7. lightColor
  8. surfaceColor2
  ```
- Added support for **Particle types:** 
  ```
  1. color
  2. fadeColor
  3. initialColor
  4. gradient
  5. explosionGradient
  6. explosionGradient2
  7. explosionGradient3
  8. emissiveMaskColor
  9. emissiveMaskColor2
  ```
- Added multiple error checks and error messages.
- Added small delay before closing program, so user has time to read console output. (Windows only)
- Fixed a bug that could cause both xyz and rgb values to be printed in some cases.
- Fixed a bug that could cause some FX values to be skipped.

## [0.1.1] - 2021-05-17

- Fixed issue with parsing "editorVars" (unusual .decl formatting).
- Removed .bat file, added short delay before closing console (Windows version).

## [0.1.0] - 2021-05-16

- Initial Release
