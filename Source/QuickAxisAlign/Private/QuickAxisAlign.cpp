// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuickAxisAlign.h"
#include "QuickAxisAlignCommands.h"
#include "QuickAxisAlignStyle.h"
#include "QuickAxisAlignPanel.h"
#include "QAAPanelSettingsCustomization.h"
#include "QAAVisualAlignEdMode.h"
#include "QAAPanelSettings.h"

#include "Framework/Docking/TabManager.h"
#include "ToolMenus.h"
#include "Widgets/Docking/SDockTab.h"
#include "EditorModeManager.h"
#include "EditorModeRegistry.h"
#include "PropertyEditorModule.h"
#include "LevelEditor.h"

#define LOCTEXT_NAMESPACE "FQuickAxisAlignModule"

static const FName QuickAxisAlignTabName("QuickAxisAlign");

FQuickAxisAlignModule& FQuickAxisAlignModule::Get()
{
	return FModuleManager::LoadModuleChecked<FQuickAxisAlignModule>("QuickAxisAlign");
}

void FQuickAxisAlignModule::StartupModule()
{
	UE_LOG(LogTemp, Log, TEXT("QuickAxisAlign: StartupModule"));

	FQuickAxisAlignStyle::Initialize();
	FQuickAxisAlignCommands::Register();

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditorModule.RegisterCustomClassLayout(
		UQAAPanelSettings::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FQAAPanelSettingsCustomization::MakeInstance)
	);
	PropertyEditorModule.NotifyCustomizationModuleChanged();

	CommandList = MakeShareable(new FUICommandList);
	CommandList->MapAction(
		FQuickAxisAlignCommands::Get().StartVisualAlign,
		FExecuteAction::CreateRaw(this, &FQuickAxisAlignModule::StartVisualAlignSession),
		FCanExecuteAction()
	);

	{
		FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
		LevelEditorModule.GetGlobalLevelEditorActions()->Append(CommandList.ToSharedRef());
	}

	FEditorModeRegistry::Get().RegisterMode<FQAAVisualAlignEdMode>(
		FQAAVisualAlignEdMode::EM_QAAVisualAlignEdModeId,
		LOCTEXT("QAAVisualAlignEdMode", "Visual Align"),
		FSlateIcon(),
		true
	);

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FQuickAxisAlignModule::RegisterMenus));

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(QuickAxisAlignTabName, FOnSpawnTab::CreateRaw(this, &FQuickAxisAlignModule::SpawnTab))
		.SetDisplayName(LOCTEXT("QuickAxisAlignTabTitle", "Quick Axis Align"))
		.SetTooltipText(LOCTEXT("QuickAxisAlignTabTooltip", "Align actors by copying coordinates."))
		.SetMenuType(ETabSpawnerMenuType::Hidden)
		.SetIcon(FSlateIcon(FQuickAxisAlignStyle::GetStyleSetName(), "QuickAxisAlign.AlignActors"));

}

void FQuickAxisAlignModule::ShutdownModule()
{
	FEditorModeRegistry::Get().UnregisterMode(FQAAVisualAlignEdMode::EM_QAAVisualAlignEdModeId);

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(QuickAxisAlignTabName);

	UToolMenus::UnRegisterStartupCallback(this);

	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyEditorModule.UnregisterCustomClassLayout(UQAAPanelSettings::StaticClass()->GetFName());
		PropertyEditorModule.NotifyCustomizationModuleChanged();
	}

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
	UToolMenus* ToolMenus = UToolMenus::Get();
	if (!ToolMenus) return;

	UToolMenu* ToolsMenu = ToolMenus->ExtendMenu("LevelEditor.MainMenu.Tools");
	FToolMenuSection& Section = ToolsMenu->AddSection("QuickAxisAlign", LOCTEXT("QuickAxisAlignSection", "Quick Axis Align"));

	Section.AddMenuEntry(
		"QuickAxisAlign_OpenPanel",
		LOCTEXT("QuickAxisAlignTabTitle", "Quick Axis Align"),
		LOCTEXT("QuickAxisAlignTabTooltip", "Align actors by copying coordinates."),
		FSlateIcon(FQuickAxisAlignStyle::GetStyleSetName(), "QuickAxisAlign.AlignActors"),
		FToolUIActionChoice(FUIAction(FExecuteAction::CreateLambda([]()
			{
				FGlobalTabmanager::Get()->TryInvokeTab(QuickAxisAlignTabName);
			})))
	);

	FToolMenuEntry& MenuEntry = Section.AddMenuEntry(
		"QuickAxisAlign_VisualAlign",
		FQuickAxisAlignCommands::Get().StartVisualAlign,
		LOCTEXT("VisualAlignMenuLabel", "Visual Align"),
		LOCTEXT("VisualAlignMenuTooltip", "Start Visual Align mode to pick source and target points in the viewport"),
		FSlateIcon(FQuickAxisAlignStyle::GetStyleSetName(), "QuickAxisAlign.AlignActors")
	);
	MenuEntry.SetCommandList(CommandList);
}

void FQuickAxisAlignModule::AlignSelectedActors()
{
	FGlobalTabmanager::Get()->TryInvokeTab(QuickAxisAlignTabName);
}

namespace
{
	FEditorModeTools* GetModeManager()
	{
		if (!GLevelEditorModeToolsIsValid())
		{
			return nullptr;
		}
		return &GLevelEditorModeTools();
	}
}

void FQuickAxisAlignModule::StartVisualAlignSession()
{
	FEditorModeTools* ModeTools = GetModeManager();
	if (!ModeTools) return;

	ModeTools->ActivateMode(FQAAVisualAlignEdMode::EM_QAAVisualAlignEdModeId);
	FQAAVisualAlignEdMode* Mode = ModeTools->GetActiveModeTyped<FQAAVisualAlignEdMode>(FQAAVisualAlignEdMode::EM_QAAVisualAlignEdModeId);
	if (Mode)
	{
		Mode->StartSession();
	}
}

void FQuickAxisAlignModule::ApplyVisualAlign()
{
	FEditorModeTools* ModeTools = GetModeManager();
	if (!ModeTools) return;

	FQAAVisualAlignEdMode* Mode = ModeTools->GetActiveModeTyped<FQAAVisualAlignEdMode>(FQAAVisualAlignEdMode::EM_QAAVisualAlignEdModeId);
	if (Mode)
	{
		Mode->ApplyAlignment();
	}
	ModeTools->DeactivateMode(FQAAVisualAlignEdMode::EM_QAAVisualAlignEdModeId);
}

void FQuickAxisAlignModule::CancelVisualAlign()
{
	FEditorModeTools* ModeTools = GetModeManager();
	if (!ModeTools) return;

	FQAAVisualAlignEdMode* Mode = ModeTools->GetActiveModeTyped<FQAAVisualAlignEdMode>(FQAAVisualAlignEdMode::EM_QAAVisualAlignEdModeId);
	if (Mode)
	{
		Mode->CancelSession();
	}
	ModeTools->DeactivateMode(FQAAVisualAlignEdMode::EM_QAAVisualAlignEdModeId);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FQuickAxisAlignModule, QuickAxisAlign)
