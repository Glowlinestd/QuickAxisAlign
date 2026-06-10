#include "QuickAxisAlignPanel.h"
#include "QuickAxisAlign.h"
#include "QuickAxisAlignStyle.h"
#include "QAAPanelSettings.h"
#include "QAAVisualAlignEdMode.h"

#include "Widgets/SQAACellBorder.h"

#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Layout/SSplitter.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Styling/AppStyle.h"
#include "Engine/Selection.h"
#include "Editor.h"
#include "EditorModeManager.h"
#include "IDetailsView.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"

#define LOCTEXT_NAMESPACE "SQuickAxisAlignPanel"

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

	void SQuickAxisAlignPanel::Construct(const FArguments& InArgs)
	{
		PanelSettings.Reset(GetMutableDefault<UQAAPanelSettings>());

		FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

		FDetailsViewArgs DetailsArgs;
		DetailsArgs.bAllowSearch = false;
		DetailsArgs.bHideSelectionTip = true;
		DetailsArgs.bShowOptions = true;
		DetailsArgs.bShowPropertyMatrixButton = false;
		DetailsArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
		DetailsArgs.ColumnWidth = 0.7f;

		TSharedRef<IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(DetailsArgs);
		DetailsView->SetObject(PanelSettings.Get());

		FSlateFontInfo PanelFont = FAppStyle::Get().GetFontStyle("NormalText");
		PanelFont.Size = 9;

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
				.BorderBackgroundColor(FSlateColor(EStyleColor::Input))
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

			// ── Details: Transform / Visual Align ───────────────
			+ SVerticalBox::Slot()
			.FillHeight(1.f)
			.Padding(0, 8, 0, 8)
			[
				DetailsView
			]

			// ── Bottom bar ─────────────────────────────────────────
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(8, 0, 8, 4)
				[
					SNew(STextBlock)
					.Text(this, &SQuickAxisAlignPanel::GetInfoText)
					.Font(PanelFont)
					.ColorAndOpacity(this, &SQuickAxisAlignPanel::GetInfoColor)
					.Visibility(this, &SQuickAxisAlignPanel::GetInfoVisibility)
				]

				+ SHorizontalBox::Slot()
				.FillWidth(1.f)

				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(0, 0, 8, 4)
				[
					SNew(SButton)
					.Text(LOCTEXT("ApplyButton", "Apply"))
					.OnClicked(this, &SQuickAxisAlignPanel::OnApply)
					.IsEnabled(TAttribute<bool>::CreateLambda([this]() { return HasValidSelection() || IsVisualAlignReady(); }))
					.ButtonStyle(FAppStyle::Get(), "PrimaryButton")
					.ContentPadding(FMargin(20.f, 6.f))
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.ToolTipText(LOCTEXT("ApplyTT", "Apply the selected transform channels from Target to all Sources"))
				]
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
	FQAAVisualAlignEdMode* Mode = GetVisualAlignMode();
	if (Mode && Mode->GetStep() != EQAAVisualAlignStep::Inactive)
	{
		return LOCTEXT("Source", "Source");
	}

	if (!GEditor) return LOCTEXT("Source", "Source");

	USelection* Sel = GEditor->GetSelectedActors();
	if (!Sel) return LOCTEXT("Source", "Source");

	return (Sel->Num() > 2) ? LOCTEXT("Sources", "Sources") : LOCTEXT("Source", "Source");
}

FText SQuickAxisAlignPanel::GetSourceValueText() const
{
	FQAAVisualAlignEdMode* Mode = GetVisualAlignMode();
	if (Mode && Mode->GetStep() != EQAAVisualAlignStep::Inactive)
	{
		AActor* SourceActor = Mode->GetSourceActor();
		return SourceActor ? FText::FromString(SourceActor->GetActorLabel()) : LOCTEXT("NoneSelected", "—");
	}

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
	FQAAVisualAlignEdMode* Mode = GetVisualAlignMode();
	if (Mode && Mode->GetStep() != EQAAVisualAlignStep::Inactive)
	{
		AActor* TargetActor = Mode->GetTargetActor();
		return TargetActor ? FText::FromString(TargetActor->GetActorLabel()) : LOCTEXT("NoneSelected", "—");
	}

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
	FQAAVisualAlignEdMode* Mode = GetVisualAlignMode();
	if (Mode && Mode->GetStep() != EQAAVisualAlignStep::Inactive)
	{
		AActor* SourceActor = Mode->GetSourceActor();
		AActor* TargetActor = Mode->GetTargetActor();
		return FText::Format(
			LOCTEXT("TT_VisualAlignActors", "Source: {0}\nTarget: {1}"),
			SourceActor ? FText::FromString(SourceActor->GetActorLabel()) : LOCTEXT("NoneSelected", "—"),
			TargetActor ? FText::FromString(TargetActor->GetActorLabel()) : LOCTEXT("NoneSelected", "—")
		);
	}

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

// ── Info ────────────────────────────────────────────────────

FText SQuickAxisAlignPanel::GetInfoText() const
{
	if (!FeedbackText.IsEmpty()) return FeedbackText;

	if (IsVisualAlignActive())
	{
		return LOCTEXT("VA_CancelWithEsc", "Visual Align active — Press ESC to cancel");
	}

	return GetInstructionText();
}

FSlateColor SQuickAxisAlignPanel::GetInfoColor() const
{
	if (!FeedbackText.IsEmpty()) return FLinearColor(0.2f, 0.6f, 0.2f);
	return FSlateColor::UseSubduedForeground();
}

EVisibility SQuickAxisAlignPanel::GetInfoVisibility() const
{
	const FText T = GetInfoText();
	const FString S = T.ToString();
	if (S.IsEmpty() || S == TEXT(" ")) return EVisibility::Collapsed;
	return EVisibility::HitTestInvisible;
}

// ── Apply ──────────────────────────────────────────────────

FReply SQuickAxisAlignPanel::OnApply()
{
	// If Visual Align is ready, apply it instead
	if (IsVisualAlignReady())
	{
		FQuickAxisAlignModule::Get().ApplyVisualAlign();
		return FReply::Handled();
	}

	if (!GEditor)
		return FReply::Handled();

	USelection* Selection = GEditor->GetSelectedActors();
	if (!Selection || Selection->Num() < 2)
		return FReply::Handled();

	TArray<AActor*> SelectedActors;
	Selection->GetSelectedObjects(SelectedActors);

	AActor* Target = SelectedActors.Last();
	const int32 SourceCount = SelectedActors.Num() - 1;
	UQAAPanelSettings* Settings = PanelSettings.Get();

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

		if (Settings && Settings->bLocationX) NewLoc.X = TargetLoc.X;
		if (Settings && Settings->bLocationY) NewLoc.Y = TargetLoc.Y;
		if (Settings && Settings->bLocationZ) NewLoc.Z = TargetLoc.Z;

		if (Settings && Settings->bRotationX) NewRot.Roll = TargetRot.Roll;
		if (Settings && Settings->bRotationY) NewRot.Pitch = TargetRot.Pitch;
		if (Settings && Settings->bRotationZ) NewRot.Yaw = TargetRot.Yaw;

		if (Settings && Settings->bScaleX) NewScale.X = TargetScale.X;
		if (Settings && Settings->bScaleY) NewScale.Y = TargetScale.Y;
		if (Settings && Settings->bScaleZ) NewScale.Z = TargetScale.Z;

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

FReply SQuickAxisAlignPanel::OnStartVisualAlign()
{
	FQuickAxisAlignModule::Get().StartVisualAlignSession();
	return FReply::Handled();
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
