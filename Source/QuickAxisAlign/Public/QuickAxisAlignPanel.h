#pragma once

#include "Widgets/SCompoundWidget.h"
#include "Engine/Selection.h"

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

	FText GetLine1Text() const;
	FText GetLine2Text() const;
	bool IsApplyEnabled() const;

	// Position
	void OnPosAllChanged(ECheckBoxState NewState);
	void OnPosXChanged(ECheckBoxState NewState);
	void OnPosYChanged(ECheckBoxState NewState);
	void OnPosZChanged(ECheckBoxState NewState);
	ECheckBoxState GetPosAllState() const;
	ECheckBoxState GetPosXState() const;
	ECheckBoxState GetPosYState() const;
	ECheckBoxState GetPosZState() const;

	// Rotation
	void OnRotAllChanged(ECheckBoxState NewState);
	void OnRotXChanged(ECheckBoxState NewState);
	void OnRotYChanged(ECheckBoxState NewState);
	void OnRotZChanged(ECheckBoxState NewState);
	ECheckBoxState GetRotAllState() const;
	ECheckBoxState GetRotXState() const;
	ECheckBoxState GetRotYState() const;
	ECheckBoxState GetRotZState() const;

	// Scale
	void OnScaleAllChanged(ECheckBoxState NewState);
	void OnScaleXChanged(ECheckBoxState NewState);
	void OnScaleYChanged(ECheckBoxState NewState);
	void OnScaleZChanged(ECheckBoxState NewState);
	ECheckBoxState GetScaleAllState() const;
	ECheckBoxState GetScaleXState() const;
	ECheckBoxState GetScaleYState() const;
	ECheckBoxState GetScaleZState() const;

	bool bPosX, bPosY, bPosZ, bPosAll;
	bool bRotX, bRotY, bRotZ, bRotAll;
	bool bScaleX, bScaleY, bScaleZ, bScaleAll;
};
