// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"
#include "QuickAxisAlignStyle.h"

class FQuickAxisAlignCommands : public TCommands<FQuickAxisAlignCommands>
{
public:
	FQuickAxisAlignCommands()
		: TCommands<FQuickAxisAlignCommands>(
			TEXT("QuickAxisAlign"),
			NSLOCTEXT("Contexts", "QuickAxisAlign", "Quick Axis Align"),
			NAME_None,
			FQuickAxisAlignStyle::GetStyleSetName())
	{
	}

	virtual void RegisterCommands() override;

	TSharedPtr<FUICommandInfo> StartVisualAlign;
};
