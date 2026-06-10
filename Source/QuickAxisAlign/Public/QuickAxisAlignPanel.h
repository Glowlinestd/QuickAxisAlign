#pragma once

#include "UObject/StrongObjectPtr.h"
#include "Widgets/SCompoundWidget.h"

class UQAAPanelSettings;

class SQuickAxisAlignPanel : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SQuickAxisAlignPanel) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual ~SQuickAxisAlignPanel();

private:
	void OnActorSelectionChanged(UObject* InObject);
	FReply OnApply();

	FReply OnStartVisualAlign();

	FText GetSourceLabel() const;
	FText GetSourceValueText() const;
	FText GetTargetValueText() const;
	FText GetSelectionTooltipText() const;

	FText GetInstructionText() const;
	bool HasValidSelection() const;

	bool IsVisualAlignReady() const;
	bool IsVisualAlignActive() const;
	bool IsVisualAlignInactive() const;

	FText GetInfoText() const;
	FSlateColor GetInfoColor() const;
	EVisibility GetInfoVisibility() const;

	FText FeedbackText;
	TStrongObjectPtr<UQAAPanelSettings> PanelSettings;
};
