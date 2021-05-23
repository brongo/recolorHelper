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
- Added support for **standard types:** 
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
- Added support for **particle types:** 
  ```
  1. color
  2. colorNEW
  3. fadeColor
  4. initialColor
  5. explosionGradient
  6. explosionGradient2
  7. explosionGradient3
  8. emissiveMaskColor
  9. emissiveMaskColor2
  ```

## [0.1.1] - 2021-05-17

- Fixed issue with parsing "editorVars" (unusual .decl formatting).
- Removed .bat file, added short delay before closing console (Windows version).

## [0.1.0] - 2021-05-16

- Initial Release
