// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Styling/SlateStyle.h"

class FQuickAxisAlignStyle
{
public:
	static void Initialize();
	static void Shutdown();
	static FName GetStyleSetName();
	static const FSlateBrush* GetBrush(const FName& PropertyName);

private:
	static TSharedPtr<FSlateStyleSet> StyleInstance;
};
