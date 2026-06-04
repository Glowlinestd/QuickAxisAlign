// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuickAxisAlign.h"
#include "QuickAxisAlignCommands.h"
#include "QuickAxisAlignStyle.h"
#include "QuickAxisAlignPanel.h"

#include "ToolMenus.h"
#include "Engine/Selection.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/Docking/TabManager.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/SWindow.h"
#include "LevelEditor.h"

#define LOCTEXT_NAMESPACE "FQuickAxisAlignModule"

static const FName QuickAxisAlignTabName("QuickAxisAlign");

void FQuickAxisAlignModule::StartupModule()
{
	UE_LOG(LogTemp, Log, TEXT("QuickAxisAlign: StartupModule"));

	FQuickAxisAlignStyle::Initialize();
	FQuickAxisAlignCommands::Register();

	CommandList = MakeShareable(new FUICommandList);
	CommandList->MapAction(
		FQuickAxisAlignCommands::Get().AlignSelectedActors,
		FExecuteAction::CreateRaw(this, &FQuickAxisAlignModule::AlignSelectedActors),
		FCanExecuteAction()
	);

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FQuickAxisAlignModule::RegisterMenus));

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(QuickAxisAlignTabName, FOnSpawnTab::CreateRaw(this, &FQuickAxisAlignModule::SpawnTab))
		.SetDisplayName(LOCTEXT("QuickAxisAlignTabTitle", "Quick Axis Align"))
		.SetTooltipText(LOCTEXT("QuickAxisAlignTabTooltip", "Align actors by copying coordinates."))
		.SetIcon(FSlateIcon(FQuickAxisAlignStyle::GetStyleSetName(), "QuickAxisAlign.AlignActors"));
}

void FQuickAxisAlignModule::ShutdownModule()
{
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(QuickAxisAlignTabName);

	UToolMenus::UnRegisterStartupCallback(this);

	FQuickAxisAlignCommands::Unregister();
	FQuickAxisAlignStyle::Shutdown();
}

TSharedRef<SDockTab> FQuickAxisAlignModule::SpawnTab(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SQuickAxisAlignPanel)
		];
}

void FQuickAxisAlignModule::RegisterMenus()
{
	UE_LOG(LogTemp, Log, TEXT("QuickAxisAlign: RegisterMenus"));

	UToolMenus* ToolMenus = UToolMenus::Get();
	if (!ToolMenus)
	{
		UE_LOG(LogTemp, Error, TEXT("QuickAxisAlign: UToolMenus not available"));
		return;
	}

	UToolMenu* ToolbarMenu = ToolMenus->ExtendMenu("LevelEditor.LevelEditorToolBar.User");
	if (!ToolbarMenu)
	{
		UE_LOG(LogTemp, Error, TEXT("QuickAxisAlign: Could not extend LevelEditor.LevelEditorToolBar"));
		return;
	}

	FToolMenuSection& Section = ToolbarMenu->AddSection("QuickAxisAlign", LOCTEXT("QuickAxisAlignSection", "Quick Axis Align"));

	FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(
		FQuickAxisAlignCommands::Get().AlignSelectedActors,
		LOCTEXT("AlignActorsLabel", "Align"),
		LOCTEXT("AlignActorsTooltip", "Quick Axis Align: alinear el primer actor seleccionado a la posicion del segundo"),
		FSlateIcon(FQuickAxisAlignStyle::GetStyleSetName(), "QuickAxisAlign.AlignActors")
	));
	Entry.SetCommandList(CommandList);

	UE_LOG(LogTemp, Log, TEXT("QuickAxisAlign: Button added to toolbar!"));
}

void FQuickAxisAlignModule::AlignSelectedActors()
{
	FGlobalTabmanager::Get()->TryInvokeTab(QuickAxisAlignTabName);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FQuickAxisAlignModule, QuickAxisAlign)