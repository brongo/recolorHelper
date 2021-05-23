# Changelog
All notable changes to this project will be documented in this file.

## [0.2.0] - 2021-05-23

- Fixed a bug that could cause both xyz and rgb values to be printed in some cases.
- Fixed a bug that could cause some FX values to be skipped.
- Added support for **FX types:** 
  ```
  1. FX_DECAL
  2. FX_DESTRUCTIBLE
  3. FX_RIBBON_2
  4. FX_SCREEN_SHAKE
  ```
- Added support for **standard types:** emissiveTint, explosionGradient, explosionGradient2, explosionGradient3, emissiveMaskColor, emissiveMaskColor2, lightColor, surfaceColor2.
- Added support for **particle types:** color, colorNEW, fadeColor, initialColor, explosionGradient, explosionGradient2, explosionGradient3, emissiveMaskColor, emissiveMaskColor2.


## [0.1.1] - 2021-05-17

- Fixed issue with parsing "editorVars" (unusual .decl formatting).
- Removed .bat file, added short delay before closing console (Windows version).

## [0.1.0] - 2021-05-16

- Initial Release
