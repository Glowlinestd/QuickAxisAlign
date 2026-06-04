#include "QuickAxisAlignPanel.h"

#include "Widgets/Layout/SBorder.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Styling/AppStyle.h"
#include "Editor.h"

#define LOCTEXT_NAMESPACE "SQuickAxisAlignPanel"

void SQuickAxisAlignPanel::Construct(const FArguments& InArgs)
{
	bPosX = bPosY = bPosZ = bPosAll = true;
	bRotX = bRotY = bRotZ = bRotAll = false;
	bScaleX = bScaleY = bScaleZ = bScaleAll = false;

	const float PadX = 4.0f;

	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::Get().GetBrush("ToolPanel.GroupBorder"))
		.Padding(10.0f)
		[
			SNew(SVerticalBox)

			// Status line 1
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 0, 0, 2)
			[
				SNew(STextBlock)
				.Text(this, &SQuickAxisAlignPanel::GetLine1Text)
				.Font(FAppStyle::Get().GetFontStyle("NormalFont"))
			]

			// Status line 2
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 0, 0, 8)
			[
				SNew(STextBlock)
				.Text(this, &SQuickAxisAlignPanel::GetLine2Text)
				.Font(FAppStyle::Get().GetFontStyle("NormalFont"))
			]

			// Separator
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 0, 0, 6)
			[
				SNew(SBorder)
				.BorderImage(FAppStyle::Get().GetBrush("Separator"))
				.Padding(0)
			]

			// --- Position ---
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 3)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(0, 0, 12, 0)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("PositionLabel", "Position"))
					.Font(FAppStyle::Get().GetFontStyle("NormalFontBold"))
					.MinDesiredWidth(55)
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(PadX, 0)
				[
					SNew(SCheckBox)
					.IsChecked(this, &SQuickAxisAlignPanel::GetPosXState)
					.OnCheckStateChanged(this, &SQuickAxisAlignPanel::OnPosXChanged)
					[ SNew(STextBlock).Text(FText::FromString("X")) ]
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(PadX, 0)
				[
					SNew(SCheckBox)
					.IsChecked(this, &SQuickAxisAlignPanel::GetPosYState)
					.OnCheckStateChanged(this, &SQuickAxisAlignPanel::OnPosYChanged)
					[ SNew(STextBlock).Text(FText::FromString("Y")) ]
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(PadX, 0)
				[
					SNew(SCheckBox)
					.IsChecked(this, &SQuickAxisAlignPanel::GetPosZState)
					.OnCheckStateChanged(this, &SQuickAxisAlignPanel::OnPosZChanged)
					[ SNew(STextBlock).Text(FText::FromString("Z")) ]
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(PadX, 0)
				[
					SNew(SCheckBox)
					.IsChecked(this, &SQuickAxisAlignPanel::GetPosAllState)
					.OnCheckStateChanged(this, &SQuickAxisAlignPanel::OnPosAllChanged)
					[ SNew(STextBlock).Text(LOCTEXT("AllLabel", "All")) ]
				]
			]

			// --- Rotation ---
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 3)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(0, 0, 12, 0)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("RotationLabel", "Rotation"))
					.Font(FAppStyle::Get().GetFontStyle("NormalFontBold"))
					.MinDesiredWidth(55)
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(PadX, 0)
				[
					SNew(SCheckBox)
					.IsChecked(this, &SQuickAxisAlignPanel::GetRotXState)
					.OnCheckStateChanged(this, &SQuickAxisAlignPanel::OnRotXChanged)
					[ SNew(STextBlock).Text(FText::FromString("X")) ]
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(PadX, 0)
				[
					SNew(SCheckBox)
					.IsChecked(this, &SQuickAxisAlignPanel::GetRotYState)
					.OnCheckStateChanged(this, &SQuickAxisAlignPanel::OnRotYChanged)
					[ SNew(STextBlock).Text(FText::FromString("Y")) ]
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(PadX, 0)
				[
					SNew(SCheckBox)
					.IsChecked(this, &SQuickAxisAlignPanel::GetRotZState)
					.OnCheckStateChanged(this, &SQuickAxisAlignPanel::OnRotZChanged)
					[ SNew(STextBlock).Text(FText::FromString("Z")) ]
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(PadX, 0)
				[
					SNew(SCheckBox)
					.IsChecked(this, &SQuickAxisAlignPanel::GetRotAllState)
					.OnCheckStateChanged(this, &SQuickAxisAlignPanel::OnRotAllChanged)
					[ SNew(STextBlock).Text(LOCTEXT("AllLabel", "All")) ]
				]
			]

			// --- Scale ---
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 3)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(0, 0, 12, 0)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("ScaleLabel", "Scale"))
					.Font(FAppStyle::Get().GetFontStyle("NormalFontBold"))
					.MinDesiredWidth(55)
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(PadX, 0)
				[
					SNew(SCheckBox)
					.IsChecked(this, &SQuickAxisAlignPanel::GetScaleXState)
					.OnCheckStateChanged(this, &SQuickAxisAlignPanel::OnScaleXChanged)
					[ SNew(STextBlock).Text(FText::FromString("X")) ]
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(PadX, 0)
				[
					SNew(SCheckBox)
					.IsChecked(this, &SQuickAxisAlignPanel::GetScaleYState)
					.OnCheckStateChanged(this, &SQuickAxisAlignPanel::OnScaleYChanged)
					[ SNew(STextBlock).Text(FText::FromString("Y")) ]
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(PadX, 0)
				[
					SNew(SCheckBox)
					.IsChecked(this, &SQuickAxisAlignPanel::GetScaleZState)
					.OnCheckStateChanged(this, &SQuickAxisAlignPanel::OnScaleZChanged)
					[ SNew(STextBlock).Text(FText::FromString("Z")) ]
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(PadX, 0)
				[
					SNew(SCheckBox)
					.IsChecked(this, &SQuickAxisAlignPanel::GetScaleAllState)
					.OnCheckStateChanged(this, &SQuickAxisAlignPanel::OnScaleAllChanged)
					[ SNew(STextBlock).Text(LOCTEXT("AllLabel", "All")) ]
				]
			]

			// Separator
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 6, 0, 6)
			[
				SNew(SBorder)
				.BorderImage(FAppStyle::Get().GetBrush("Separator"))
				.Padding(0)
			]

			// Apply
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			[
				SNew(SButton)
				.Text(LOCTEXT("ApplyButton", "Apply"))
				.OnClicked(this, &SQuickAxisAlignPanel::OnApply)
				.IsEnabled(this, &SQuickAxisAlignPanel::IsApplyEnabled)
				.ButtonStyle(FAppStyle::Get(), "PrimaryButton")
			]
		]
	];

	if (GEditor)
	{
		USelection* Selection = GEditor->GetSelectedActors();
		if (Selection)
		{
			Selection->SelectionChangedEvent.AddRaw(this, &SQuickAxisAlignPanel::OnActorSelectionChanged);
		}
	}
}

SQuickAxisAlignPanel::~SQuickAxisAlignPanel()
{
	if (GEditor)
	{
		USelection* Selection = GEditor->GetSelectedActors();
		if (Selection)
		{
			Selection->SelectionChangedEvent.RemoveAll(this);
		}
	}
}

void SQuickAxisAlignPanel::OnActorSelectionChanged(UObject* InObject)
{
	Invalidate(EInvalidateWidgetReason::Paint);
}

// --- Status text ---

FText SQuickAxisAlignPanel::GetLine1Text() const
{
	if (!GEditor)
		return LOCTEXT("NoEditor", "Editor not available.");

	USelection* Selection = GEditor->GetSelectedActors();
	if (!Selection)
		return LOCTEXT("NoSelection", "No selection available.");

	const int32 Num = Selection->Num();
	if (Num == 0)
		return LOCTEXT("SelectFirst", "1. Select the actor to align");

	TArray<AActor*> Actors;
	Selection->GetSelectedObjects(Actors);

	if (Num == 1)
	{
		return FText::Format(LOCTEXT("SourceOnly", "Source: {0}"), FText::FromString(Actors[0]->GetActorLabel()));
	}
	if (Num == 2)
	{
		return FText::Format(LOCTEXT("SourceName", "Source: {0}"), FText::FromString(Actors[0]->GetActorLabel()));
	}

	return LOCTEXT("TooMany", "Too many actors selected.");
}

FText SQuickAxisAlignPanel::GetLine2Text() const
{
	if (!GEditor)
		return FText::GetEmpty();

	USelection* Selection = GEditor->GetSelectedActors();
	if (!Selection)
		return FText::GetEmpty();

	const int32 Num = Selection->Num();
	if (Num == 0)
		return LOCTEXT("SelectSecond", "2. Select the target actor");

	if (Num == 1)
		return LOCTEXT("NeedTarget", "2. Select the target actor");

	TArray<AActor*> Actors;
	Selection->GetSelectedObjects(Actors);

	if (Num >= 2)
	{
		return FText::Format(LOCTEXT("TargetName", "Target: {0}"), FText::FromString(Actors[1]->GetActorLabel()));
	}

	return FText::GetEmpty();
}

bool SQuickAxisAlignPanel::IsApplyEnabled() const
{
	if (!GEditor)
		return false;

	USelection* Selection = GEditor->GetSelectedActors();
	if (!Selection || Selection->Num() != 2)
		return false;

	return true;
}

// --- Position ---

void SQuickAxisAlignPanel::OnPosAllChanged(ECheckBoxState NewState)
{
	bPosAll = (NewState == ECheckBoxState::Checked);
	bPosX = bPosY = bPosZ = bPosAll;
}

void SQuickAxisAlignPanel::OnPosXChanged(ECheckBoxState NewState)
{
	bPosX = (NewState == ECheckBoxState::Checked);
	bPosAll = bPosX && bPosY && bPosZ;
}

void SQuickAxisAlignPanel::OnPosYChanged(ECheckBoxState NewState)
{
	bPosY = (NewState == ECheckBoxState::Checked);
	bPosAll = bPosX && bPosY && bPosZ;
}

void SQuickAxisAlignPanel::OnPosZChanged(ECheckBoxState NewState)
{
	bPosZ = (NewState == ECheckBoxState::Checked);
	bPosAll = bPosX && bPosY && bPosZ;
}

ECheckBoxState SQuickAxisAlignPanel::GetPosAllState() const
{
	return bPosAll ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

ECheckBoxState SQuickAxisAlignPanel::GetPosXState() const
{
	return bPosX ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

ECheckBoxState SQuickAxisAlignPanel::GetPosYState() const
{
	return bPosY ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

ECheckBoxState SQuickAxisAlignPanel::GetPosZState() const
{
	return bPosZ ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

// --- Rotation ---

void SQuickAxisAlignPanel::OnRotAllChanged(ECheckBoxState NewState)
{
	bRotAll = (NewState == ECheckBoxState::Checked);
	bRotX = bRotY = bRotZ = bRotAll;
}

void SQuickAxisAlignPanel::OnRotXChanged(ECheckBoxState NewState)
{
	bRotX = (NewState == ECheckBoxState::Checked);
	bRotAll = bRotX && bRotY && bRotZ;
}

void SQuickAxisAlignPanel::OnRotYChanged(ECheckBoxState NewState)
{
	bRotY = (NewState == ECheckBoxState::Checked);
	bRotAll = bRotX && bRotY && bRotZ;
}

void SQuickAxisAlignPanel::OnRotZChanged(ECheckBoxState NewState)
{
	bRotZ = (NewState == ECheckBoxState::Checked);
	bRotAll = bRotX && bRotY && bRotZ;
}

ECheckBoxState SQuickAxisAlignPanel::GetRotAllState() const
{
	return bRotAll ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

ECheckBoxState SQuickAxisAlignPanel::GetRotXState() const
{
	return bRotX ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

ECheckBoxState SQuickAxisAlignPanel::GetRotYState() const
{
	return bRotY ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

ECheckBoxState SQuickAxisAlignPanel::GetRotZState() const
{
	return bRotZ ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

// --- Scale ---

void SQuickAxisAlignPanel::OnScaleAllChanged(ECheckBoxState NewState)
{
	bScaleAll = (NewState == ECheckBoxState::Checked);
	bScaleX = bScaleY = bScaleZ = bScaleAll;
}

void SQuickAxisAlignPanel::OnScaleXChanged(ECheckBoxState NewState)
{
	bScaleX = (NewState == ECheckBoxState::Checked);
	bScaleAll = bScaleX && bScaleY && bScaleZ;
}

void SQuickAxisAlignPanel::OnScaleYChanged(ECheckBoxState NewState)
{
	bScaleY = (NewState == ECheckBoxState::Checked);
	bScaleAll = bScaleX && bScaleY && bScaleZ;
}

void SQuickAxisAlignPanel::OnScaleZChanged(ECheckBoxState NewState)
{
	bScaleZ = (NewState == ECheckBoxState::Checked);
	bScaleAll = bScaleX && bScaleY && bScaleZ;
}

ECheckBoxState SQuickAxisAlignPanel::GetScaleAllState() const
{
	return bScaleAll ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

ECheckBoxState SQuickAxisAlignPanel::GetScaleXState() const
{
	return bScaleX ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

ECheckBoxState SQuickAxisAlignPanel::GetScaleYState() const
{
	return bScaleY ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

ECheckBoxState SQuickAxisAlignPanel::GetScaleZState() const
{
	return bScaleZ ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

// --- Apply ---

FReply SQuickAxisAlignPanel::OnApply()
{
	if (!GEditor)
		return FReply::Handled();

	USelection* Selection = GEditor->GetSelectedActors();
	if (!Selection || Selection->Num() != 2)
		return FReply::Handled();

	TArray<AActor*> SelectedActors;
	Selection->GetSelectedObjects(SelectedActors);

	AActor* Source = SelectedActors[0];
	AActor* Target = SelectedActors[1];

	const FVector SourceLoc = Source->GetActorLocation();
	const FRotator SourceRot = Source->GetActorRotation();
	const FVector SourceScale = Source->GetActorScale3D();

	const FVector TargetLoc = Target->GetActorLocation();
	const FRotator TargetRot = Target->GetActorRotation();
	const FVector TargetScale = Target->GetActorScale3D();

	FVector NewLoc = SourceLoc;
	FRotator NewRot = SourceRot;
	FVector NewScale = SourceScale;

	if (bPosAll)
	{
		NewLoc = TargetLoc;
	}
	else
	{
		if (bPosX) NewLoc.X = TargetLoc.X;
		if (bPosY) NewLoc.Y = TargetLoc.Y;
		if (bPosZ) NewLoc.Z = TargetLoc.Z;
	}

	if (bRotAll)
	{
		NewRot = TargetRot;
	}
	else
	{
		if (bRotX) NewRot.Roll = TargetRot.Roll;
		if (bRotY) NewRot.Pitch = TargetRot.Pitch;
		if (bRotZ) NewRot.Yaw = TargetRot.Yaw;
	}

	if (bScaleAll)
	{
		NewScale = TargetScale;
	}
	else
	{
		if (bScaleX) NewScale.X = TargetScale.X;
		if (bScaleY) NewScale.Y = TargetScale.Y;
		if (bScaleZ) NewScale.Z = TargetScale.Z;
	}

	const FScopedTransaction Transaction(LOCTEXT("AlignTransaction", "Quick Axis Align"));
	Source->Modify();

	if (NewLoc != SourceLoc)
		Source->SetActorLocation(NewLoc, false);
	if (NewRot != SourceRot)
		Source->SetActorRotation(NewRot);
	if (NewScale != SourceScale)
		Source->SetActorScale3D(NewScale);

	GEditor->SelectActor(Target, false, true);

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
