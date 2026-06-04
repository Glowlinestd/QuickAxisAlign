#pragma once

#include "Widgets/SCompoundWidget.h"

class AActor;
class SWindow;

class SQuickAxisAlignWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SQuickAxisAlignWindow) {}
		SLATE_ARGUMENT(TSharedPtr<SWindow>, ParentWindow)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void SetSourceActor(AActor* InActor);
	void SetTargetActor(AActor* InActor);

private:
	FReply OnApply();
	FReply OnCancel();

	FText GetSourceText() const;
	FText GetTargetText() const;

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

	TWeakObjectPtr<AActor> SourceActor;
	TWeakObjectPtr<AActor> TargetActor;

	bool bPosX, bPosY, bPosZ, bPosAll;
	bool bRotX, bRotY, bRotZ, bRotAll;
	bool bScaleX, bScaleY, bScaleZ, bScaleAll;

	TSharedPtr<SWindow> ParentWindow;
};
