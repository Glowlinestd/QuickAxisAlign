// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FUICommandList;
class SDockTab;
class FSpawnTabArgs;

class FQuickAxisAlignModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static FQuickAxisAlignModule& Get();
	void StartVisualAlignSession();
	void ApplyVisualAlign();
	void CancelVisualAlign();

private:
	void RegisterMenus();
	TSharedRef<SDockTab> SpawnTab(const FSpawnTabArgs& Args);

	TSharedPtr<FUICommandList> CommandList;
};
