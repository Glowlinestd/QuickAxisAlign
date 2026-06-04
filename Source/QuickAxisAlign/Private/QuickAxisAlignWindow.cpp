#include "QuickAxisAlignWindow.h"

#include "Widgets/Layout/SBorder.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/SWindow.h"
#include "Styling/AppStyle.h"

#include "Engine/Selection.h"
#include "Editor.h"

#define LOCTEXT_NAMESPACE "SQuickAxisAlignWindow"

void SQuickAxisAlignWindow::Construct(const FArguments& InArgs)
{
	ParentWindow = InArgs._ParentWindow;

	bPosX = bPosY = bPosZ = bPosAll = true;
	bRotX = bRotY = bRotZ = bRotAll = true;
	bScaleX = bScaleY = bScaleZ = bScaleAll = true;

	const float PaddingX = 4.0f;

	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::Get().GetBrush("ToolPanel.GroupBorder"))
		.Padding(12.0f)
		[
			SNew(SVerticalBox)

			// Title
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 0, 0, 8)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("WindowTitle", "Quick Axis Align"))
				.Font(FAppStyle::Get().GetFontStyle("NormalFontBold"))
				.Justification(ETextJustify::Center)
			]

			// Actor info
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 0, 0, 6)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(STextBlock)
					.Text(this, &SQuickAxisAlignWindow::GetSourceText)
					.Font(FAppStyle::Get().GetFontStyle("NormalFont"))
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(STextBlock)
					.Text(this, &SQuickAxisAlignWindow::GetTargetText)
					.Font(FAppStyle::Get().GetFontStyle("NormalFont"))
				]
			]

			// Separator
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 2, 0, 6)
			[
				SNew(SBorder)
				.BorderImage(FAppStyle::Get().GetBrush("Separator"))
				.Padding(0)
			]

			// --- Position ---
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 2)
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
				.Padding(PaddingX, 0)
				[
					SNew(SCheckBox)
					.IsChecked(this, &SQuickAxisAlignWindow::GetPosXState)
					.OnCheckStateChanged(this, &SQuickAxisAlignWindow::OnPosXChanged)
					[
						SNew(STextBlock).Text(FText::FromString("X"))
					]
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(PaddingX, 0)
				[
					SNew(SCheckBox)
					.IsChecked(this, &SQuickAxisAlignWindow::GetPosYState)
					.OnCheckStateChanged(this, &SQuickAxisAlignWindow::OnPosYChanged)
					[
						SNew(STextBlock).Text(FText::FromString("Y"))
					]
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(PaddingX, 0)
				[
					SNew(SCheckBox)
					.IsChecked(this, &SQuickAxisAlignWindow::GetPosZState)
					.OnCheckStateChanged(this, &SQuickAxisAlignWindow::OnPosZChanged)
					[
						SNew(STextBlock).Text(FText::FromString("Z"))
					]
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(PaddingX, 0)
				[
					SNew(SCheckBox)
					.IsChecked(this, &SQuickAxisAlignWindow::GetPosAllState)
					.OnCheckStateChanged(this, &SQuickAxisAlignWindow::OnPosAllChanged)
					[
						SNew(STextBlock).Text(LOCTEXT("AllLabel", "All"))
					]
				]
			]

			// --- Rotation ---
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 2)
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
				.Padding(PaddingX, 0)
				[
					SNew(SCheckBox)
					.IsChecked(this, &SQuickAxisAlignWindow::GetRotXState)
					.OnCheckStateChanged(this, &SQuickAxisAlignWindow::OnRotXChanged)
					[
						SNew(STextBlock).Text(FText::FromString("X"))
					]
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(PaddingX, 0)
				[
					SNew(SCheckBox)
					.IsChecked(this, &SQuickAxisAlignWindow::GetRotYState)
					.OnCheckStateChanged(this, &SQuickAxisAlignWindow::OnRotYChanged)
					[
						SNew(STextBlock).Text(FText::FromString("Y"))
					]
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(PaddingX, 0)
				[
					SNew(SCheckBox)
					.IsChecked(this, &SQuickAxisAlignWindow::GetRotZState)
					.OnCheckStateChanged(this, &SQuickAxisAlignWindow::OnRotZChanged)
					[
						SNew(STextBlock).Text(FText::FromString("Z"))
					]
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(PaddingX, 0)
				[
					SNew(SCheckBox)
					.IsChecked(this, &SQuickAxisAlignWindow::GetRotAllState)
					.OnCheckStateChanged(this, &SQuickAxisAlignWindow::OnRotAllChanged)
					[
						SNew(STextBlock).Text(LOCTEXT("AllLabel", "All"))
					]
				]
			]

			// --- Scale ---
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 2)
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
				.Padding(PaddingX, 0)
				[
					SNew(SCheckBox)
					.IsChecked(this, &SQuickAxisAlignWindow::GetScaleXState)
					.OnCheckStateChanged(this, &SQuickAxisAlignWindow::OnScaleXChanged)
					[
						SNew(STextBlock).Text(FText::FromString("X"))
					]
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(PaddingX, 0)
				[
					SNew(SCheckBox)
					.IsChecked(this, &SQuickAxisAlignWindow::GetScaleYState)
					.OnCheckStateChanged(this, &SQuickAxisAlignWindow::OnScaleYChanged)
					[
						SNew(STextBlock).Text(FText::FromString("Y"))
					]
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(PaddingX, 0)
				[
					SNew(SCheckBox)
					.IsChecked(this, &SQuickAxisAlignWindow::GetScaleZState)
					.OnCheckStateChanged(this, &SQuickAxisAlignWindow::OnScaleZChanged)
					[
						SNew(STextBlock).Text(FText::FromString("Z"))
					]
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(PaddingX, 0)
				[
					SNew(SCheckBox)
					.IsChecked(this, &SQuickAxisAlignWindow::GetScaleAllState)
					.OnCheckStateChanged(this, &SQuickAxisAlignWindow::OnScaleAllChanged)
					[
						SNew(STextBlock).Text(LOCTEXT("AllLabel", "All"))
					]
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

			// Buttons
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Right)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(4, 0)
				[
					SNew(SButton)
					.Text(LOCTEXT("ApplyButton", "Apply"))
					.OnClicked(this, &SQuickAxisAlignWindow::OnApply)
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(4, 0)
				[
					SNew(SButton)
					.Text(LOCTEXT("CancelButton", "Cancel"))
					.OnClicked(this, &SQuickAxisAlignWindow::OnCancel)
				]
			]
		]
	];
}

void SQuickAxisAlignWindow::SetSourceActor(AActor* InActor)
{
	SourceActor = InActor;
}

void SQuickAxisAlignWindow::SetTargetActor(AActor* InActor)
{
	TargetActor = InActor;
}

FText SQuickAxisAlignWindow::GetSourceText() const
{
	if (SourceActor.IsValid())
	{
		return FText::Format(LOCTEXT("SourceFormat", "Source: {0}"), FText::FromString(SourceActor->GetActorLabel()));
	}
	return LOCTEXT("InvalidSource", "Source: <invalid>");
}

FText SQuickAxisAlignWindow::GetTargetText() const
{
	if (TargetActor.IsValid())
	{
		return FText::Format(LOCTEXT("TargetFormat", "Target: {0}"), FText::FromString(TargetActor->GetActorLabel()));
	}
	return LOCTEXT("InvalidTarget", "Target: <invalid>");
}

// --- Position ---

void SQuickAxisAlignWindow::OnPosAllChanged(ECheckBoxState NewState)
{
	bPosAll = (NewState == ECheckBoxState::Checked);
	bPosX = bPosY = bPosZ = bPosAll;
}

void SQuickAxisAlignWindow::OnPosXChanged(ECheckBoxState NewState)
{
	bPosX = (NewState == ECheckBoxState::Checked);
	bPosAll = bPosX && bPosY && bPosZ;
}

void SQuickAxisAlignWindow::OnPosYChanged(ECheckBoxState NewState)
{
	bPosY = (NewState == ECheckBoxState::Checked);
	bPosAll = bPosX && bPosY && bPosZ;
}

void SQuickAxisAlignWindow::OnPosZChanged(ECheckBoxState NewState)
{
	bPosZ = (NewState == ECheckBoxState::Checked);
	bPosAll = bPosX && bPosY && bPosZ;
}

ECheckBoxState SQuickAxisAlignWindow::GetPosAllState() const
{
	return bPosAll ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

ECheckBoxState SQuickAxisAlignWindow::GetPosXState() const
{
	return bPosX ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

ECheckBoxState SQuickAxisAlignWindow::GetPosYState() const
{
	return bPosY ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

ECheckBoxState SQuickAxisAlignWindow::GetPosZState() const
{
	return bPosZ ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

// --- Rotation ---

void SQuickAxisAlignWindow::OnRotAllChanged(ECheckBoxState NewState)
{
	bRotAll = (NewState == ECheckBoxState::Checked);
	bRotX = bRotY = bRotZ = bRotAll;
}

void SQuickAxisAlignWindow::OnRotXChanged(ECheckBoxState NewState)
{
	bRotX = (NewState == ECheckBoxState::Checked);
	bRotAll = bRotX && bRotY && bRotZ;
}

void SQuickAxisAlignWindow::OnRotYChanged(ECheckBoxState NewState)
{
	bRotY = (NewState == ECheckBoxState::Checked);
	bRotAll = bRotX && bRotY && bRotZ;
}

void SQuickAxisAlignWindow::OnRotZChanged(ECheckBoxState NewState)
{
	bRotZ = (NewState == ECheckBoxState::Checked);
	bRotAll = bRotX && bRotY && bRotZ;
}

ECheckBoxState SQuickAxisAlignWindow::GetRotAllState() const
{
	return bRotAll ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

ECheckBoxState SQuickAxisAlignWindow::GetRotXState() const
{
	return bRotX ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

ECheckBoxState SQuickAxisAlignWindow::GetRotYState() const
{
	return bRotY ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

ECheckBoxState SQuickAxisAlignWindow::GetRotZState() const
{
	return bRotZ ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

// --- Scale ---

void SQuickAxisAlignWindow::OnScaleAllChanged(ECheckBoxState NewState)
{
	bScaleAll = (NewState == ECheckBoxState::Checked);
	bScaleX = bScaleY = bScaleZ = bScaleAll;
}

void SQuickAxisAlignWindow::OnScaleXChanged(ECheckBoxState NewState)
{
	bScaleX = (NewState == ECheckBoxState::Checked);
	bScaleAll = bScaleX && bScaleY && bScaleZ;
}

void SQuickAxisAlignWindow::OnScaleYChanged(ECheckBoxState NewState)
{
	bScaleY = (NewState == ECheckBoxState::Checked);
	bScaleAll = bScaleX && bScaleY && bScaleZ;
}

void SQuickAxisAlignWindow::OnScaleZChanged(ECheckBoxState NewState)
{
	bScaleZ = (NewState == ECheckBoxState::Checked);
	bScaleAll = bScaleX && bScaleY && bScaleZ;
}

ECheckBoxState SQuickAxisAlignWindow::GetScaleAllState() const
{
	return bScaleAll ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

ECheckBoxState SQuickAxisAlignWindow::GetScaleXState() const
{
	return bScaleX ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

ECheckBoxState SQuickAxisAlignWindow::GetScaleYState() const
{
	return bScaleY ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

ECheckBoxState SQuickAxisAlignWindow::GetScaleZState() const
{
	return bScaleZ ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

// --- Apply / Cancel ---

FReply SQuickAxisAlignWindow::OnApply()
{
	if (!SourceActor.IsValid() || !TargetActor.IsValid())
	{
		if (ParentWindow.IsValid())
		{
			FSlateApplication::Get().RequestDestroyWindow(ParentWindow.ToSharedRef());
		}
		return FReply::Handled();
	}

	AActor* Source = SourceActor.Get();
	AActor* Target = TargetActor.Get();

	const FVector SourceLoc = Source->GetActorLocation();
	const FRotator SourceRot = Source->GetActorRotation();
	const FVector SourceScale = Source->GetActorScale3D();

	const FVector TargetLoc = Target->GetActorLocation();
	const FRotator TargetRot = Target->GetActorRotation();
	const FVector TargetScale = Target->GetActorScale3D();

	FVector NewLoc = SourceLoc;
	FRotator NewRot = SourceRot;
	FVector NewScale = SourceScale;

	// Apply position
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

	// Apply rotation (X=Roll, Y=Pitch, Z=Yaw)
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

	// Apply scale
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

	// Apply with undo support
	const FScopedTransaction Transaction(LOCTEXT("AlignTransaction", "Quick Axis Align"));
	Source->Modify();

	if (NewLoc != SourceLoc)
	{
		Source->SetActorLocation(NewLoc, false);
	}
	if (NewRot != SourceRot)
	{
		Source->SetActorRotation(NewRot);
	}
	if (NewScale != SourceScale)
	{
		Source->SetActorScale3D(NewScale);
	}

	// Close window
	if (ParentWindow.IsValid())
	{
		FSlateApplication::Get().RequestDestroyWindow(ParentWindow.ToSharedRef());
	}

	return FReply::Handled();
}

FReply SQuickAxisAlignWindow::OnCancel()
{
	if (ParentWindow.IsValid())
	{
		FSlateApplication::Get().RequestDestroyWindow(ParentWindow.ToSharedRef());
	}
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
