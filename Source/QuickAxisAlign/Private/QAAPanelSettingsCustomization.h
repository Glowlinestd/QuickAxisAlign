#pragma once

#include "IDetailCustomization.h"
#include "Templates/SharedPointer.h"

class FQAAPanelSettingsCustomization : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
};
