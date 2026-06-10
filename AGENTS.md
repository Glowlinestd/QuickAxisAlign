# QuickAxisAlign — Agent Guide

UE 5.7 editor-only plugin: one `QuickAxisAlign` module, `Editor` type, `Default` loading phase.

## Build & Run

- Build with `build.bat`; it hardcodes `C:\Program Files\Epic Games\UE_5.7`, `C:\Users\aleja\Documents\Unreal Projects\QAA\QAA.uproject`, target `QAAEditor`, Win64 Development.
- Run with `run.bat`; it launches `UnrealEditor.exe` for the same hardcoded project.
- Do not use `RunUAT.bat BuildPlugin` for verification; this repo is built through the host project's `QAAEditor` target.
- There are no repo-local tests, CI, lint, typecheck, or formatter configs; `build.bat` is the practical verification step.

## Architecture

- Module entrypoint: `Source/QuickAxisAlign/Private/QuickAxisAlign.cpp` (`FQuickAxisAlignModule`, `IMPLEMENT_MODULE`).
- Startup registers the style, command list, `UQAAPanelSettings` detail customization, `FQAAVisualAlignEdMode`, hidden nomad tab `QuickAxisAlign`, and Tools menu entries under `LevelEditor.MainMenu.Tools`.
- Main tab UI: `SQuickAxisAlignPanel` in `QuickAxisAlignPanel.cpp`; it hosts an `IDetailsView` over `GetMutableDefault<UQAAPanelSettings>()` and applies selected transform channels from last-selected Target to earlier Sources.
- Settings UI lives in `QAAPanelSettingsCustomization.cpp`; raw `UQAAPanelSettings` bools are hidden and rebuilt as X/Y/Z rows plus a `Use Visual Align` button.
- Visual point-picking mode is `FQAAVisualAlignEdMode`; it uses `GLevelEditorModeTools()` directly from Level Editor UI paths and supports Esc cancel / Enter apply.
- `References/AssetAuditorPro` is a reference plugin, not QuickAxisAlign product code; ignore it unless explicitly comparing patterns.

## UI & Slate Notes

- Use `FAppStyle::Get()` and `EStyleColor` for colors/brushes; existing palette centers on `Recessed`, `Input`, `Header`, and `Panel`.
- For fixed Slate widths, use `SBox` + `WidthOverride()`; do not put `MinDesiredWidth` on `SCheckBox`.
- `SQAACellBorder` is a custom `SCompoundWidget` that paints a solid fill plus outline; defaults are border `EStyleColor::Recessed`, background `EStyleColor::Panel`, thickness `1.25f`, padding `FMargin(8.f, 3.f)`.
- Detail customization helper lambdas return `TSharedRef<SWidget>` inline; avoid storing generated row widgets unless a later callback needs them.

## Generated & Ignored

- `.gitignore` ignores `Binaries`, `Intermediate`, and `Config`; do not treat generated build output or saved plugin config as source changes.
