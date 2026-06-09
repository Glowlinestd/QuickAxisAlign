#include "QuickAxisAlignPanel.h"
#include "QuickAxisAlign.h"
#include "QAAVisualAlignEdMode.h"

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
#include "EditorModeManager.h"

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
				.ToolTipText(this, &SQuickAxisAlignPanel::GetSelectionTooltipText)
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
						.Text(this, &SQuickAxisAlignPanel::GetSourceLabel)
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
							.OnDoubleClick_Raw(this, &SQuickAxisAlignPanel::OnToggleLocationAxes)
							.ToolTipText(LOCTEXT("Loc_TT", "Copy location from target\n\nDouble-click: toggle all 3 axes"))
							[
								SNew(SBox)
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Center)
								[
									SNew(STextBlock)
									.Text(LOCTEXT("LocationHeader", "Location"))
									.Font(PanelFont)
									.MinDesiredWidth(LabelW)
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
							.OnDoubleClick_Raw(this, &SQuickAxisAlignPanel::OnToggleRotationAxes)
							.ToolTipText(LOCTEXT("Rot_TT", "Copy rotation from target\n\nDouble-click: toggle all 3 axes"))
							[
								SNew(SBox)
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Center)
								[
									SNew(STextBlock)
									.Text(LOCTEXT("RotationHeader", "Rotation"))
									.Font(PanelFont)
									.MinDesiredWidth(LabelW)
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
							.OnDoubleClick_Raw(this, &SQuickAxisAlignPanel::OnToggleScaleAxes)
							.ToolTipText(LOCTEXT("Scale_TT", "Copy scale from target\n\nDouble-click: toggle all 3 axes"))
							[
								SNew(SBox)
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Center)
								[
									SNew(STextBlock)
									.Text(LOCTEXT("ScaleHeader", "Scale"))
									.Font(PanelFont)
									.MinDesiredWidth(LabelW)
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

			// ── Visual Align ─────────────────────────────────
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 6, 0, 0)
			[
				SNew(SBorder)
				.BorderImage(FAppStyle::Get().GetBrush("WhiteBrush"))
				.BorderBackgroundColor(FSlateColor(EStyleColor::Background))
				.Padding(FMargin(8, 6))
				[
					SNew(SVerticalBox)

					// Header
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0, 0, 0, 4)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("VisualAlignHeader", "Visual Align"))
						.Font(FAppStyle::Get().GetFontStyle("NormalFontBold"))
					]

					// Status line
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0, 0, 0, 4)
					[
						SNew(STextBlock)
						.Text(this, &SQuickAxisAlignPanel::GetVisualAlignStatus)
						.Font(PanelFont)
						.ColorAndOpacity(FSlateColor::UseSubduedForeground())
					]

					// Buttons row
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)

						+ SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(0, 0, 6, 0)
						[
							SNew(SBox)
							.MinDesiredWidth(100.f)
							.MinDesiredHeight(26.f)
							[
								SNew(SButton)
								.Text(LOCTEXT("StartVisualAlign", "Use Visual Align"))
								.HAlign(HAlign_Center)
								.VAlign(VAlign_Center)
								.ContentPadding(FMargin(12.f, 4.f))
								.OnClicked(this, &SQuickAxisAlignPanel::OnStartVisualAlign)
								.IsEnabled(this, &SQuickAxisAlignPanel::IsVisualAlignInactive)
								.ToolTipText(LOCTEXT("StartVisualAlignTT", "Enter Visual Align mode: click on a point of the Source, then a point of the Target"))
							]
						]

						+ SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(0, 0, 6, 0)
						[
							SNew(SBox)
							.MinDesiredWidth(100.f)
							.MinDesiredHeight(26.f)
							[
								SNew(SButton)
								.Text(LOCTEXT("ApplyVisualAlign", "Apply"))
								.HAlign(HAlign_Center)
								.VAlign(VAlign_Center)
								.ContentPadding(FMargin(12.f, 4.f))
								.ButtonStyle(FAppStyle::Get(), "PrimaryButton")
								.OnClicked(this, &SQuickAxisAlignPanel::OnApplyVisualAlign)
								.IsEnabled(this, &SQuickAxisAlignPanel::IsVisualAlignReady)
								.ToolTipText(LOCTEXT("ApplyVisualAlignTT", "Move sources so source point matches target point"))
							]
						]

						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SBox)
							.MinDesiredWidth(100.f)
							.MinDesiredHeight(26.f)
							[
								SNew(SButton)
								.Text(LOCTEXT("CancelVisualAlign", "Cancel"))
								.HAlign(HAlign_Center)
								.VAlign(VAlign_Center)
								.ContentPadding(FMargin(12.f, 4.f))
								.OnClicked(this, &SQuickAxisAlignPanel::OnCancelVisualAlign)
								.IsEnabled(this, &SQuickAxisAlignPanel::IsVisualAlignActive)
								.ToolTipText(LOCTEXT("CancelVisualAlignTT", "Exit Visual Align mode"))
							]
						]
					]
				]
			]

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
					.ToolTipText(LOCTEXT("ApplyTT", "Apply the selected transform channels from Target to all Sources"))
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

FText SQuickAxisAlignPanel::GetSourceLabel() const
{
	if (!GEditor) return LOCTEXT("Source", "Source");

	USelection* Sel = GEditor->GetSelectedActors();
	if (!Sel) return LOCTEXT("Source", "Source");

	return (Sel->Num() > 2) ? LOCTEXT("Sources", "Sources") : LOCTEXT("Source", "Source");
}

FText SQuickAxisAlignPanel::GetSourceValueText() const
{
	if (!GEditor) return FText::GetEmpty();

	USelection* Sel = GEditor->GetSelectedActors();
	if (!Sel) return FText::GetEmpty();

	TArray<AActor*> Actors;
	Sel->GetSelectedObjects(Actors);

	if (Actors.Num() == 0)
	{
		return LOCTEXT("NoneSelected", "—");
	}

	if (Actors.Num() <= 2)
	{
		return FText::FromString(Actors[0]->GetActorLabel());
	}

	return FText::Format(
		LOCTEXT("SourcesCount", "{0} actors"),
		FText::AsNumber(Actors.Num() - 1)
	);
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
		return FText::FromString(Actors.Last()->GetActorLabel());
	}
	return LOCTEXT("NoneSelected", "—");
}

FText SQuickAxisAlignPanel::GetSelectionTooltipText() const
{
	if (!GEditor) return FText::GetEmpty();

	USelection* Sel = GEditor->GetSelectedActors();
	if (!Sel) return FText::GetEmpty();

	TArray<AActor*> Actors;
	Sel->GetSelectedObjects(Actors);

	if (Actors.Num() == 0)
	{
		return LOCTEXT("TT_NoSelection", "No actors selected");
	}

	if (Actors.Num() == 1)
	{
		return FText::Format(
			LOCTEXT("TT_OneActor", "Source: {0}"),
			FText::FromString(Actors[0]->GetActorLabel())
		);
	}

	if (Actors.Num() == 2)
	{
		return FText::Format(
			LOCTEXT("TT_TwoActors", "Source: {0}, Target: {1}"),
			FText::FromString(Actors[0]->GetActorLabel()),
			FText::FromString(Actors[1]->GetActorLabel())
		);
	}

	TArray<FString> AllLabels;
	AllLabels.Reserve(Actors.Num());
	for (AActor* A : Actors)
	{
		AllLabels.Add(A->GetActorLabel());
	}

	FString SourceList;
	for (int32 i = 0; i < AllLabels.Num() - 1; ++i)
	{
		if (i > 0) SourceList += TEXT(", ");
		SourceList += AllLabels[i];
	}
	const FString TargetLabel = AllLabels.Last();

	return FText::Format(
		LOCTEXT("TT_MultiActors", "Sources: {0}\nTarget: {1}"),
		FText::FromString(SourceList),
		FText::FromString(TargetLabel)
	);
}

FText SQuickAxisAlignPanel::GetInstructionText() const
{
	if (!GEditor) return FText::FromString(" ");

	USelection* Sel = GEditor->GetSelectedActors();
	if (!Sel) return FText::FromString(" ");

	if (Sel->Num() < 2)
	{
		return LOCTEXT("Instr_SelectTwoPlus", "Select 2+ actors in the viewport (last selected = Target)");
	}
	return FText::FromString(" ");
}

bool SQuickAxisAlignPanel::HasValidSelection() const
{
	if (!GEditor) return false;

	USelection* Sel = GEditor->GetSelectedActors();
	if (!Sel) return false;

	return Sel->Num() >= 2;
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

// ── Row toggle (double-click on row label) ─────────────────

FReply SQuickAxisAlignPanel::OnToggleLocationAxes()
{
	const bool bAllChecked = bPosX && bPosY && bPosZ;
	const bool bNewState = !bAllChecked;
	bPosX = bPosY = bPosZ = bNewState;
	return FReply::Handled();
}

FReply SQuickAxisAlignPanel::OnToggleRotationAxes()
{
	const bool bAllChecked = bRotX && bRotY && bRotZ;
	const bool bNewState = !bAllChecked;
	bRotX = bRotY = bRotZ = bNewState;
	return FReply::Handled();
}

FReply SQuickAxisAlignPanel::OnToggleScaleAxes()
{
	const bool bAllChecked = bScaleX && bScaleY && bScaleZ;
	const bool bNewState = !bAllChecked;
	bScaleX = bScaleY = bScaleZ = bNewState;
	return FReply::Handled();
}

// ── Apply ──────────────────────────────────────────────────

FReply SQuickAxisAlignPanel::OnApply()
{
	if (!GEditor)
		return FReply::Handled();

	USelection* Selection = GEditor->GetSelectedActors();
	if (!Selection || Selection->Num() < 2)
		return FReply::Handled();

	TArray<AActor*> SelectedActors;
	Selection->GetSelectedObjects(SelectedActors);

	AActor* Target = SelectedActors.Last();
	const int32 SourceCount = SelectedActors.Num() - 1;

	const FVector TargetLoc = Target->GetActorLocation();
	const FRotator TargetRot = Target->GetActorRotation();
	const FVector TargetScale = Target->GetActorScale3D();

	const FScopedTransaction Transaction(
		SourceCount > 1
			? LOCTEXT("AlignMultiTransaction", "Quick Axis Align (Multi)")
			: LOCTEXT("AlignTransaction", "Quick Axis Align")
	);

	for (int32 i = 0; i < SourceCount; ++i)
	{
		AActor* Source = SelectedActors[i];

		const FVector SourceLoc = Source->GetActorLocation();
		const FRotator SourceRot = Source->GetActorRotation();
		const FVector SourceScale = Source->GetActorScale3D();

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

		Source->Modify();

		if (NewLoc != SourceLoc)
			Source->SetActorLocation(NewLoc, false);
		if (NewRot != SourceRot)
			Source->SetActorRotation(NewRot);
		if (NewScale != SourceScale)
			Source->SetActorScale3D(NewScale);
	}

	if (SourceCount == 1)
	{
		FeedbackText = FText::Format(
			LOCTEXT("AppliedFeedbackSingle", "Applied to {0}"),
			FText::FromString(SelectedActors[0]->GetActorLabel())
		);
	}
	else
	{
		FeedbackText = FText::Format(
			LOCTEXT("AppliedFeedbackMulti", "Applied to {0} actors"),
			FText::AsNumber(SourceCount)
		);
	}

	GEditor->SelectActor(Target, false, true);

	return FReply::Handled();
}

// ── Visual Align ──────────────────────────────────────────

namespace
{
	FQAAVisualAlignEdMode* GetVisualAlignMode()
	{
		if (!GLevelEditorModeToolsIsValid())
		{
			return nullptr;
		}
		return GLevelEditorModeTools().GetActiveModeTyped<FQAAVisualAlignEdMode>(FQAAVisualAlignEdMode::EM_QAAVisualAlignEdModeId);
	}
}

FReply SQuickAxisAlignPanel::OnStartVisualAlign()
{
	FQuickAxisAlignModule::Get().StartVisualAlignSession();
	return FReply::Handled();
}

FReply SQuickAxisAlignPanel::OnApplyVisualAlign()
{
	FQuickAxisAlignModule::Get().ApplyVisualAlign();
	FeedbackText = LOCTEXT("VisualAlignDone", "Visual align applied");
	return FReply::Handled();
}

FReply SQuickAxisAlignPanel::OnCancelVisualAlign()
{
	FQuickAxisAlignModule::Get().CancelVisualAlign();
	return FReply::Handled();
}

FText SQuickAxisAlignPanel::GetVisualAlignStatus() const
{
	FQAAVisualAlignEdMode* Mode = GetVisualAlignMode();
	if (!Mode)
	{
		return LOCTEXT("VA_Idle", "Click 'Use Visual Align' to start.");
	}
	switch (Mode->GetStep())
	{
		case EQAAVisualAlignStep::Inactive:
			return LOCTEXT("VA_Inactive", "Click 'Use Visual Align' to start.");
		case EQAAVisualAlignStep::WaitingForSource:
			return LOCTEXT("VA_PickSource", "Click on a point of the Source actor");
		case EQAAVisualAlignStep::WaitingForTarget:
			return LOCTEXT("VA_PickTarget", "Click on a point of the Target actor");
	case EQAAVisualAlignStep::ReadyToApply:
		return LOCTEXT("VA_Ready", "Ready. Click another target or press Apply.");
	}
	return FText::GetEmpty();
}

EVisibility SQuickAxisAlignPanel::GetVisualAlignReadyVisibility() const
{
	FQAAVisualAlignEdMode* Mode = GetVisualAlignMode();
	if (Mode && Mode->GetStep() == EQAAVisualAlignStep::ReadyToApply)
	{
		return EVisibility::Visible;
	}
	return EVisibility::Collapsed;
}

bool SQuickAxisAlignPanel::IsVisualAlignReady() const
{
	FQAAVisualAlignEdMode* Mode = GetVisualAlignMode();
	return Mode && Mode->GetStep() == EQAAVisualAlignStep::ReadyToApply;
}

bool SQuickAxisAlignPanel::IsVisualAlignActive() const
{
	FQAAVisualAlignEdMode* Mode = GetVisualAlignMode();
	return Mode && Mode->GetStep() != EQAAVisualAlignStep::Inactive;
}

bool SQuickAxisAlignPanel::IsVisualAlignInactive() const
{
	FQAAVisualAlignEdMode* Mode = GetVisualAlignMode();
	return !Mode || Mode->GetStep() == EQAAVisualAlignStep::Inactive;
}

#undef LOCTEXT_NAMESPACE
