#pragma once

#include "Styling/SlateStyle.h"

class FQuickAxisAlignStyle
{
public:
	static void Initialize();
	static void Shutdown();
	static FName GetStyleSetName();

private:
	static TSharedPtr<FSlateStyleSet> StyleInstance;
};
