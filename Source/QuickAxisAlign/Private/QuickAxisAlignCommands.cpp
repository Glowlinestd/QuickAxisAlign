// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuickAxisAlignCommands.h"

#define LOCTEXT_NAMESPACE "FQuickAxisAlignCommands"

void FQuickAxisAlignCommands::RegisterCommands()
{
	UI_COMMAND(StartVisualAlign, "Visual Align", "Start Visual Align mode to pick source and target points in the viewport", EUserInterfaceActionType::Button, FInputChord(EKeys::X, EModifierKey::Alt));
}

#undef LOCTEXT_NAMESPACE
