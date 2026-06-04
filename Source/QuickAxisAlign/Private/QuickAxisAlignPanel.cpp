#include "QuickAxisAlignPanel.h"

#include "Widgets/SQAACellBorder.h"

#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SSplitter.h"
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
		bPosX = bPosY = bPosZ = true;
		bRotX = bRotY = bRotZ = false;
		bScaleX = bScaleY = bScaleZ = false;

		constexpr float LabelW = 76.f;
		constexpr float TableRowH = 28.f;

		FSlateFontInfo PanelFont = FAppStyle::Get().GetFontStyle("NormalText");
		PanelFont.Size = 9;

	const auto MakeAxisRow = [this, TableRowH](
		ECheckBoxState(SQuickAxisAlignPanel::* GetX)() const,
		ECheckBoxState(SQuickAxisAlignPanel::* GetY)() const,
		ECheckBoxState(SQuickAxisAlignPanel::* GetZ)() const,
		void(SQuickAxisAlignPanel::* OnX)(ECheckBoxState),
		void(SQuickAxisAlignPanel::* OnY)(ECheckBoxState),
		void(SQuickAxisAlignPanel::* OnZ)(ECheckBoxState)) -> TSharedRef<SWidget>
	{
		return SNew(SBox)
		.HeightOverride(TableRowH)
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.FillWidth(1.f)
			[
				SNew(SQAACellBorder)
				.Padding(FMargin(8.f, 5.f))
				[
					SNew(SBox)
					.HAlign(HAlign_Center)
					[
						SNew(SCheckBox)
						.ToolTipText(LOCTEXT("AxisX_TT", "Copy this axis"))
						.IsChecked(this, GetX)
						.OnCheckStateChanged(this, OnX)
					]
				]
			]

			+ SHorizontalBox::Slot()
			.FillWidth(1.f)
			[
				SNew(SQAACellBorder)
				.Padding(FMargin(8.f, 5.f))
				[
					SNew(SBox)
					.HAlign(HAlign_Center)
					[
						SNew(SCheckBox)
						.ToolTipText(LOCTEXT("AxisY_TT", "Copy this axis"))
						.IsChecked(this, GetY)
						.OnCheckStateChanged(this, OnY)
					]
				]
			]

			+ SHorizontalBox::Slot()
			.FillWidth(1.f)
			[
				SNew(SQAACellBorder)
				.Padding(FMargin(8.f, 5.f))
				[
					SNew(SBox)
					.HAlign(HAlign_Center)
					[
						SNew(SCheckBox)
						.ToolTipText(LOCTEXT("AxisZ_TT", "Copy this axis"))
						.IsChecked(this, GetZ)
						.OnCheckStateChanged(this, OnZ)
					]
				]
			]
		];
	};

	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::Get().GetBrush("WhiteBrush"))
		.BorderBackgroundColor(FSlateColor(EStyleColor::Recessed))
		.Padding(8.0f)
		[
			SNew(SVerticalBox)

			// ── Selection status ────────────────────────────
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 0, 0, 6)
			[
				SNew(SBorder)
				.BorderImage(FAppStyle::Get().GetBrush("WhiteBrush"))
				.BorderBackgroundColor(FSlateColor(EStyleColor::Background))
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
						.Font(PanelFont)
						.ColorAndOpacity(FSlateColor::UseSubduedForeground())
						]
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(12, 0, 0, 0)
						[
							SNew(STextBlock)
							.Text(this, &SQuickAxisAlignPanel::GetSourceValueText)
							.Font(PanelFont)
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
							.Font(PanelFont)
							.ColorAndOpacity(FSlateColor::UseSubduedForeground())
							]
							+ SHorizontalBox::Slot()
							.AutoWidth()
							.Padding(12, 0, 0, 0)
							[
								SNew(STextBlock)
								.Text(this, &SQuickAxisAlignPanel::GetTargetValueText)
								.Font(PanelFont)
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
				.Font(PanelFont)
				.ColorAndOpacity(FSlateColor::UseSubduedForeground())
				.Visibility(EVisibility::SelfHitTestInvisible)
			]

			// ── Table: Location / Rotation / Scale ──────────
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 0, 0, 4)
			[
				SNew(SSplitter)
				.PhysicalSplitterHandleSize(1.25f)

				+ SSplitter::Slot()
				.Value(0.46f)
				.MinSize(LabelW)
				[
					SNew(SVerticalBox)

					// ── Label column header ────────────────────
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SBox)
						.HeightOverride(TableRowH)
						[
							SNew(SQAACellBorder)
							.Padding(FMargin(8.f, 5.f))
							.BorderColor(FSlateColor(FLinearColor::Transparent))
							.BackgroundColor(FSlateColor(EStyleColor::Recessed))
							[
								SNew(SSpacer)
								.Size(FVector2D(1.f, 1.f))
							]
						]
					]

					// ── Location label ────────────────────────────
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SBox)
						.HeightOverride(TableRowH)
						[
							SNew(SQAACellBorder)
							.Padding(FMargin(24.f, 5.f, 8.f, 5.f))
							[
								SNew(SBox)
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Center)
								[
									SNew(STextBlock)
									.Text(LOCTEXT("LocationHeader", "Location"))
									.Font(PanelFont)
									.MinDesiredWidth(LabelW)
									.ToolTipText(LOCTEXT("Loc_TT", "Copy location from target"))
								]
							]
						]
					]

					// ── Rotation label ────────────────────────────
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SBox)
						.HeightOverride(TableRowH)
						[
							SNew(SQAACellBorder)
							.Padding(FMargin(24.f, 5.f, 8.f, 5.f))
							[
								SNew(SBox)
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Center)
								[
									SNew(STextBlock)
									.Text(LOCTEXT("RotationHeader", "Rotation"))
									.Font(PanelFont)
									.MinDesiredWidth(LabelW)
									.ToolTipText(LOCTEXT("Rot_TT", "Copy rotation from target"))
								]
							]
						]
					]

					// ── Scale label ────────────────────────────
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SBox)
						.HeightOverride(TableRowH)
						[
							SNew(SQAACellBorder)
							.Padding(FMargin(24.f, 5.f, 8.f, 5.f))
							[
								SNew(SBox)
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Center)
								[
									SNew(STextBlock)
									.Text(LOCTEXT("ScaleHeader", "Scale"))
									.Font(PanelFont)
									.MinDesiredWidth(LabelW)
									.ToolTipText(LOCTEXT("Scale_TT", "Copy scale from target"))
								]
							]
						]
					]
				]

				+ SSplitter::Slot()
				[
					SNew(SVerticalBox)

					// ── Header: X  Y  Z ──────────────────────────
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SBox)
						.HeightOverride(TableRowH)
						[
							SNew(SHorizontalBox)

							+ SHorizontalBox::Slot()
							.FillWidth(1.f)
							[
								SNew(SQAACellBorder)
								.Padding(FMargin(8.f, 5.f))
								.BackgroundColor(FSlateColor(EStyleColor::Header))
								[
									SNew(SBox)
									.HAlign(HAlign_Fill)
									.VAlign(VAlign_Center)
									[
										SNew(STextBlock)
										.Text(FText::FromString("X"))
										.Font(PanelFont)
										.ColorAndOpacity(FSlateColor::UseForeground())
										.Justification(ETextJustify::Center)
									]
								]
							]

							+ SHorizontalBox::Slot()
							.FillWidth(1.f)
							[
								SNew(SQAACellBorder)
								.Padding(FMargin(8.f, 5.f))
								.BackgroundColor(FSlateColor(EStyleColor::Header))
								[
									SNew(SBox)
									.HAlign(HAlign_Fill)
									.VAlign(VAlign_Center)
									[
										SNew(STextBlock)
										.Text(FText::FromString("Y"))
										.Font(PanelFont)
										.ColorAndOpacity(FSlateColor::UseForeground())
										.Justification(ETextJustify::Center)
									]
								]
							]

							+ SHorizontalBox::Slot()
							.FillWidth(1.f)
							[
								SNew(SQAACellBorder)
								.Padding(FMargin(8.f, 5.f))
								.BackgroundColor(FSlateColor(EStyleColor::Header))
								[
									SNew(SBox)
									.HAlign(HAlign_Fill)
									.VAlign(VAlign_Center)
									[
										SNew(STextBlock)
										.Text(FText::FromString("Z"))
										.Font(PanelFont)
										.ColorAndOpacity(FSlateColor::UseForeground())
										.Justification(ETextJustify::Center)
									]
								]
							]
						]
					]

					// ── Location checkboxes ────────────────────
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						MakeAxisRow(
							&SQuickAxisAlignPanel::GetPosXState, &SQuickAxisAlignPanel::GetPosYState, &SQuickAxisAlignPanel::GetPosZState,
							&SQuickAxisAlignPanel::OnPosXChanged, &SQuickAxisAlignPanel::OnPosYChanged, &SQuickAxisAlignPanel::OnPosZChanged
						)
					]

					// ── Rotation checkboxes ────────────────────
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						MakeAxisRow(
							&SQuickAxisAlignPanel::GetRotXState, &SQuickAxisAlignPanel::GetRotYState, &SQuickAxisAlignPanel::GetRotZState,
							&SQuickAxisAlignPanel::OnRotXChanged, &SQuickAxisAlignPanel::OnRotYChanged, &SQuickAxisAlignPanel::OnRotZChanged
						)
					]

					// ── Scale checkboxes ───────────────────────
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						MakeAxisRow(
							&SQuickAxisAlignPanel::GetScaleXState, &SQuickAxisAlignPanel::GetScaleYState, &SQuickAxisAlignPanel::GetScaleZState,
							&SQuickAxisAlignPanel::OnScaleXChanged, &SQuickAxisAlignPanel::OnScaleYChanged, &SQuickAxisAlignPanel::OnScaleZChanged
						)
					]
				]
			]

			// ── Spacer ───────────────────────────────────────────
			+ SVerticalBox::Slot()
			.FillHeight(1.f)

			// ── Apply ─────────────────────────────────────────
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Left)
			[
				SNew(SBox)
				.MinDesiredWidth(110.f)
				.MinDesiredHeight(28.f)
				[
					SNew(SButton)
					.Text(LOCTEXT("ApplyButton", "Apply"))
					.OnClicked(this, &SQuickAxisAlignPanel::OnApply)
					.IsEnabled(this, &SQuickAxisAlignPanel::HasValidSelection)
					.ButtonStyle(FAppStyle::Get(), "PrimaryButton")
					.ContentPadding(FMargin(20.f, 6.f))
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.ToolTipText(LOCTEXT("ApplyTT", "Apply the selected transform channels from Target to Source"))
				]
			]

			// ── Feedback ──────────────────────────────────────
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 4, 0, 0)
			.HAlign(HAlign_Center)
			[
				SNew(STextBlock)
				.Text(this, &SQuickAxisAlignPanel::GetFeedbackText)
				.Font(PanelFont)
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

void SQuickAxisAlignPanel::OnPosXChanged(ECheckBoxState NewState)
{
	bPosX = (NewState == ECheckBoxState::Checked);
}

void SQuickAxisAlignPanel::OnPosYChanged(ECheckBoxState NewState)
{
	bPosY = (NewState == ECheckBoxState::Checked);
}

void SQuickAxisAlignPanel::OnPosZChanged(ECheckBoxState NewState)
{
	bPosZ = (NewState == ECheckBoxState::Checked);
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

void SQuickAxisAlignPanel::OnRotXChanged(ECheckBoxState NewState)
{
	bRotX = (NewState == ECheckBoxState::Checked);
}

void SQuickAxisAlignPanel::OnRotYChanged(ECheckBoxState NewState)
{
	bRotY = (NewState == ECheckBoxState::Checked);
}

void SQuickAxisAlignPanel::OnRotZChanged(ECheckBoxState NewState)
{
	bRotZ = (NewState == ECheckBoxState::Checked);
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

void SQuickAxisAlignPanel::OnScaleXChanged(ECheckBoxState NewState)
{
	bScaleX = (NewState == ECheckBoxState::Checked);
}

void SQuickAxisAlignPanel::OnScaleYChanged(ECheckBoxState NewState)
{
	bScaleY = (NewState == ECheckBoxState::Checked);
}

void SQuickAxisAlignPanel::OnScaleZChanged(ECheckBoxState NewState)
{
	bScaleZ = (NewState == ECheckBoxState::Checked);
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

	if (bPosX) NewLoc.X = TargetLoc.X;
	if (bPosY) NewLoc.Y = TargetLoc.Y;
	if (bPosZ) NewLoc.Z = TargetLoc.Z;

	if (bRotX) NewRot.Roll = TargetRot.Roll;
	if (bRotY) NewRot.Pitch = TargetRot.Pitch;
	if (bRotZ) NewRot.Yaw = TargetRot.Yaw;

	if (bScaleX) NewScale.X = TargetScale.X;
	if (bScaleY) NewScale.Y = TargetScale.Y;
	if (bScaleZ) NewScale.Z = TargetScale.Z;

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
