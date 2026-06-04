# QuickAxisAlign — Agent Guide

UE 5.7 Editor plugin (single `QuickAxisAlign` module, `Editor` type, `Default` loading phase).

## Build & Run

- **Build**: `build.bat` — calls `UE_5.7\Engine\Build\BatchFiles\Build.bat` for `QAAEditor` target, Dev Win64. Hardcodes `UE_5.7` and `QAA.uproject` paths.
- **Run**: `run.bat` — launches `UnrealEditor.exe` with the project.
- Do NOT use `RunUAT.bat BuildPlugin` — use the project's `build.bat` instead.

## Architecture

- **Module entrypoint**: `Source/QuickAxisAlign/Private/QuickAxisAlign.cpp` — `FQuickAxisAlignModule` registers a nomad tab spawner (`QuickAxisAlign` tab name) and adds a toolbar button under `LevelEditor.LevelEditorToolBar.User`.
- **Main UI**: `SQuickAxisAlignPanel` (Slate `SCompoundWidget`) — selection info, axis checkboxes (X/Y/Z per Location/Rotation/Scale), Apply button.
- **Custom widget**: `SQAACellBorder` — `SCompoundWidget` that paints a solid `BackgroundColor` fill + `BorderColor` outline with configurable `Thickness`. Defaults: border=`EStyleColor::Recessed`, bg=`EStyleColor::Panel`.
- **No tests, no CI, no lint/typecheck** — standard UE Editor plugin.
- **No source file header convention** enforced.

## UI conventions

- All colors/brushes from `FAppStyle::Get()` or `EStyleColor` enums.
- Common palette: `EStyleColor::Recessed` (window bg), `EStyleColor::Background` (section bg), `EStyleColor::Header` (column header bg), `EStyleColor::Panel` (cell bg).
- Column widths should use `SBox` + `WidthOverride()` (NOT `MinDesiredWidth` on `SCheckBox` — that API does not exist).
- Custom `SQAACellBorder` uses `.Padding(FMargin(8.f, 5.f))` as standard cell padding for table rows.
- `TSharedRef<SWidget>` returned from lambda builder, not stored.
