#include "QuickAxisAlignCommands.h"

#define LOCTEXT_NAMESPACE "FQuickAxisAlignCommands"

void FQuickAxisAlignCommands::RegisterCommands()
{
	UI_COMMAND(AlignSelectedActors, "Align Actors", "Align the first selected actor to the position of the second selected actor", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
