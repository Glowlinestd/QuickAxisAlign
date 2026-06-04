#include "QuickAxisAlignPanel.h"

#include "Widgets/SQAACellBorder.h"

#include "Widgets/Layout/SBorder.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Styling/AppStyle.h"
#include "Engine/Selection.h"
#include "Editor.h"

#define LOCTEXT_NAMESPACE "SQuickAxisAlignPanel"

void SQuickAxisAlignPanel::Construct(const FArguments& InArgs)
{
	bPosX = bPosY = bPosZ = bPosAll = true;
	bRotX = bRotY = bRotZ = bRotAll = false;
	bScaleX = bScaleY = bScaleZ = bScaleAll = false;

	constexpr float PadX = 4.0f;

	const auto MakeTableRow = [this, PadX](
		const FText& InLabel,
		const FText& InTooltip,
		ECheckBoxState(SQuickAxisAlignPanel::* GetX)() const,
		ECheckBoxState(SQuickAxisAlignPanel::* GetY)() const,
		ECheckBoxState(SQuickAxisAlignPanel::* GetZ)() const,
		ECheckBoxState(SQuickAxisAlignPanel::* GetAll)() const,
		void(SQuickAxisAlignPanel::* OnX)(ECheckBoxState),
		void(SQuickAxisAlignPanel::* OnY)(ECheckBoxState),
		void(SQuickAxisAlignPanel::* OnZ)(ECheckBoxState),
		void(SQuickAxisAlignPanel::* OnAll)(ECheckBoxState)) -> TSharedRef<SWidget>
	{
		return SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SQAACellBorder)
				.Padding(FMargin(8.f, 5.f))
				[
					SNew(STextBlock)
					.Text(InLabel)
					.Font(FAppStyle::Get().GetFontStyle("NormalFontBold"))
					.MinDesiredWidth(52)
					.ToolTipText(InTooltip)
				]
			]

			+ SHorizontalBox::Slot()
			.FillWidth(1.f)
			[
				SNew(SQAACellBorder)
				.Padding(FMargin(8.f, 5.f))
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot().AutoWidth().Padding(PadX, 0)
					[ SNew(SCheckBox).ToolTipText(LOCTEXT("AxisX_TT", "Copy this axis")).IsChecked(this, GetX).OnCheckStateChanged(this, OnX)[ SNew(STextBlock).Text(FText::FromString("X")) ] ]
					+ SHorizontalBox::Slot().AutoWidth().Padding(PadX, 0)
					[ SNew(SCheckBox).ToolTipText(LOCTEXT("AxisY_TT", "Copy this axis")).IsChecked(this, GetY).OnCheckStateChanged(this, OnY)[ SNew(STextBlock).Text(FText::FromString("Y")) ] ]
					+ SHorizontalBox::Slot().AutoWidth().Padding(PadX, 0)
					[ SNew(SCheckBox).ToolTipText(LOCTEXT("AxisZ_TT", "Copy this axis")).IsChecked(this, GetZ).OnCheckStateChanged(this, OnZ)[ SNew(STextBlock).Text(FText::FromString("Z")) ] ]
					+ SHorizontalBox::Slot().AutoWidth().Padding(PadX, 0)
					[ SNew(SCheckBox).ToolTipText(LOCTEXT("AxisAll_TT", "Copy all axes")).IsChecked(this, GetAll).OnCheckStateChanged(this, OnAll)[ SNew(STextBlock).Text(LOCTEXT("AllLabel", "All")) ] ]
				]
			];
	};

	const TSharedRef<SWidget> Row_Loc = MakeTableRow(
		LOCTEXT("LocationHeader", "Location"),
		LOCTEXT("Loc_TT", "Copy location from target"),
		&SQuickAxisAlignPanel::GetPosXState, &SQuickAxisAlignPanel::GetPosYState, &SQuickAxisAlignPanel::GetPosZState, &SQuickAxisAlignPanel::GetPosAllState,
		&SQuickAxisAlignPanel::OnPosXChanged, &SQuickAxisAlignPanel::OnPosYChanged, &SQuickAxisAlignPanel::OnPosZChanged, &SQuickAxisAlignPanel::OnPosAllChanged
	);

	const TSharedRef<SWidget> Row_Rot = MakeTableRow(
		LOCTEXT("RotationHeader", "Rotation"),
		LOCTEXT("Rot_TT", "Copy rotation from target"),
		&SQuickAxisAlignPanel::GetRotXState, &SQuickAxisAlignPanel::GetRotYState, &SQuickAxisAlignPanel::GetRotZState, &SQuickAxisAlignPanel::GetRotAllState,
		&SQuickAxisAlignPanel::OnRotXChanged, &SQuickAxisAlignPanel::OnRotYChanged, &SQuickAxisAlignPanel::OnRotZChanged, &SQuickAxisAlignPanel::OnRotAllChanged
	);

	const TSharedRef<SWidget> Row_Scale = MakeTableRow(
		LOCTEXT("ScaleHeader", "Scale"),
		LOCTEXT("Scale_TT", "Copy scale from target"),
		&SQuickAxisAlignPanel::GetScaleXState, &SQuickAxisAlignPanel::GetScaleYState, &SQuickAxisAlignPanel::GetScaleZState, &SQuickAxisAlignPanel::GetScaleAllState,
		&SQuickAxisAlignPanel::OnScaleXChanged, &SQuickAxisAlignPanel::OnScaleYChanged, &SQuickAxisAlignPanel::OnScaleZChanged, &SQuickAxisAlignPanel::OnScaleAllChanged
	);

	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::Get().GetBrush("ToolPanel.GroupBorder"))
		.Padding(8.0f)
		[
			SNew(SVerticalBox)

			// ── Selection status ────────────────────────────
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 0, 0, 6)
			[
				SNew(SBorder)
				.BorderImage(FAppStyle::Get().GetBrush("DetailsView.CategoryTop"))
				.Padding(FMargin(8, 6))
				.ToolTipText(this, &SQuickAxisAlignPanel::GetInstructionText)
				[
					SNew(SVerticalBox)

					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0, 0, 0, 2)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(STextBlock)
							.Text(LOCTEXT("SourceLabel", "Source"))
							.Font(FAppStyle::Get().GetFontStyle("NormalFontBold"))
							.ColorAndOpacity(FSlateColor::UseSubduedForeground())
						]
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(12, 0, 0, 0)
						[
							SNew(STextBlock)
							.Text(this, &SQuickAxisAlignPanel::GetSourceValueText)
							.Font(FAppStyle::Get().GetFontStyle("NormalFont"))
						]
					]

					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(STextBlock)
							.Text(LOCTEXT("TargetLabel", "Target"))
							.Font(FAppStyle::Get().GetFontStyle("NormalFontBold"))
							.ColorAndOpacity(FSlateColor::UseSubduedForeground())
						]
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(12, 0, 0, 0)
						[
							SNew(STextBlock)
							.Text(this, &SQuickAxisAlignPanel::GetTargetValueText)
							.Font(FAppStyle::Get().GetFontStyle("NormalFont"))
						]
					]
				]
			]

			// ── Instruction (always visible, keeps height) ───
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 0, 0, 6)
			[
				SNew(STextBlock)
				.Text(this, &SQuickAxisAlignPanel::GetInstructionText)
				.Font(FAppStyle::Get().GetFontStyle("SmallText"))
				.ColorAndOpacity(FSlateColor::UseSubduedForeground())
				.Visibility(EVisibility::SelfHitTestInvisible)
			]

			// ── Table: Location / Rotation / Scale ──────────
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 0, 0, 4)
			[
				SNew(SVerticalBox)

				+ SVerticalBox::Slot()
				.AutoHeight()
				[ Row_Loc ]

				+ SVerticalBox::Slot()
				.AutoHeight()
				[ Row_Rot ]

				+ SVerticalBox::Slot()
				.AutoHeight()
				[ Row_Scale ]
			]

			// ── Apply ─────────────────────────────────────────
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			[
				SNew(SButton)
				.Text(LOCTEXT("ApplyButton", "Apply Alignment"))
				.OnClicked(this, &SQuickAxisAlignPanel::OnApply)
				.IsEnabled(this, &SQuickAxisAlignPanel::HasValidSelection)
				.ButtonStyle(FAppStyle::Get(), "PrimaryButton")
				.ToolTipText(LOCTEXT("ApplyTT", "Apply the selected transform channels from Target to Source"))
			]

			// ── Feedback ──────────────────────────────────────
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 4, 0, 0)
			.HAlign(HAlign_Center)
			[
				SNew(STextBlock)
				.Text(this, &SQuickAxisAlignPanel::GetFeedbackText)
				.Font(FAppStyle::Get().GetFontStyle("SmallText"))
				.ColorAndOpacity(FLinearColor(0.2f, 0.6f, 0.2f))
				.Visibility(this, &SQuickAxisAlignPanel::GetFeedbackVisibility)
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

// ── Selection change ──────────────────────────────────────

void SQuickAxisAlignPanel::OnActorSelectionChanged(UObject* InObject)
{
	FeedbackText = FText::GetEmpty();
	Invalidate(EInvalidateWidgetReason::Paint);
}

// ── Source / Target labels ─────────────────────────────────

FText SQuickAxisAlignPanel::GetSourceValueText() const
{
	if (!GEditor) return FText::GetEmpty();

	USelection* Sel = GEditor->GetSelectedActors();
	if (!Sel) return FText::GetEmpty();

	TArray<AActor*> Actors;
	Sel->GetSelectedObjects(Actors);

	if (Actors.Num() >= 1)
	{
		FString Label = Actors[0]->GetActorLabel();
		return FText::FromString(Label);
	}
	return LOCTEXT("NoneSelected", "—");
}

FText SQuickAxisAlignPanel::GetTargetValueText() const
{
	if (!GEditor) return FText::GetEmpty();

	USelection* Sel = GEditor->GetSelectedActors();
	if (!Sel) return FText::GetEmpty();

	TArray<AActor*> Actors;
	Sel->GetSelectedObjects(Actors);

	if (Actors.Num() >= 2)
	{
		FString Label = Actors[1]->GetActorLabel();
		return FText::FromString(Label);
	}
	return LOCTEXT("NoneSelected", "—");
}

FText SQuickAxisAlignPanel::GetInstructionText() const
{
	if (!GEditor) return FText::FromString(" ");

	USelection* Sel = GEditor->GetSelectedActors();
	if (!Sel) return FText::FromString(" ");

	const int32 Num = Sel->Num();
	if (Num == 0 || Num == 1)
	{
		return LOCTEXT("Instr_SelectTwo", "Select exactly 2 actors (Source first, then Target with Shift)");
	}
	if (Num > 2)
	{
		return LOCTEXT("Instr_TooMany", "Too many selected — deselect and pick only 2 actors");
	}
	return FText::FromString(" ");
}

bool SQuickAxisAlignPanel::HasValidSelection() const
{
	if (!GEditor) return false;

	USelection* Sel = GEditor->GetSelectedActors();
	if (!Sel) return false;

	return Sel->Num() == 2;
}

// ── Feedback ───────────────────────────────────────────────

FText SQuickAxisAlignPanel::GetFeedbackText() const
{
	return FeedbackText;
}

EVisibility SQuickAxisAlignPanel::GetFeedbackVisibility() const
{
	return FeedbackText.IsEmpty() ? EVisibility::Collapsed : EVisibility::HitTestInvisible;
}

// ── Location ───────────────────────────────────────────────

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

// ── Rotation ───────────────────────────────────────────────

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

// ── Scale ──────────────────────────────────────────────────

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

// ── Apply ──────────────────────────────────────────────────

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

	FeedbackText = FText::Format(LOCTEXT("AppliedFeedback", "Applied to {0}"), FText::FromString(Source->GetActorLabel()));

	GEditor->SelectActor(Target, false, true);

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
