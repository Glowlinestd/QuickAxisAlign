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

	FReply OnToggleLocationAxes();
	FReply OnToggleRotationAxes();
	FReply OnToggleScaleAxes();

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

	// Position
	void OnPosXChanged(ECheckBoxState NewState);
	void OnPosYChanged(ECheckBoxState NewState);
	void OnPosZChanged(ECheckBoxState NewState);
	ECheckBoxState GetPosXState() const;
	ECheckBoxState GetPosYState() const;
	ECheckBoxState GetPosZState() const;

	// Rotation
	void OnRotXChanged(ECheckBoxState NewState);
	void OnRotYChanged(ECheckBoxState NewState);
	void OnRotZChanged(ECheckBoxState NewState);
	ECheckBoxState GetRotXState() const;
	ECheckBoxState GetRotYState() const;
	ECheckBoxState GetRotZState() const;

	// Scale
	void OnScaleXChanged(ECheckBoxState NewState);
	void OnScaleYChanged(ECheckBoxState NewState);
	void OnScaleZChanged(ECheckBoxState NewState);
	ECheckBoxState GetScaleXState() const;
	ECheckBoxState GetScaleYState() const;
	ECheckBoxState GetScaleZState() const;

	bool bPosX, bPosY, bPosZ;
	bool bRotX, bRotY, bRotZ;
	bool bScaleX, bScaleY, bScaleZ;
};
