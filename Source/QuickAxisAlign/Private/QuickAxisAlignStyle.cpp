#include "QuickAxisAlignStyle.h"

#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"
#include "Brushes/SlateImageBrush.h"
#include "Misc/Paths.h"

TSharedPtr<FSlateStyleSet> FQuickAxisAlignStyle::StyleInstance = nullptr;

void FQuickAxisAlignStyle::Initialize()
{
	if (StyleInstance.IsValid())
		return;

	StyleInstance = MakeShareable(new FSlateStyleSet("QuickAxisAlignStyle"));

	const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("QuickAxisAlign"));
	if (!Plugin)
	{
		UE_LOG(LogTemp, Error, TEXT("QuickAxisAlignStyle: Plugin not found!"));
		return;
	}

	const FString ContentDir = Plugin->GetBaseDir() / TEXT("Resources");
	StyleInstance->SetContentRoot(ContentDir);

	const FString IconPath = StyleInstance->RootToContentDir(TEXT("Icon128"), TEXT(".png"));
	UE_LOG(LogTemp, Log, TEXT("QuickAxisAlignStyle: Icon path = %s, exists = %d"), *IconPath, FPaths::FileExists(IconPath) ? 1 : 0);

	FSlateImageBrush* IconBrush = new FSlateImageBrush(IconPath, FVector2D(128, 128));
	StyleInstance->Set("QuickAxisAlign.AlignActors", IconBrush);

	FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	UE_LOG(LogTemp, Log, TEXT("QuickAxisAlignStyle: Registered successfully"));
}

void FQuickAxisAlignStyle::Shutdown()
{
	if (StyleInstance.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
		StyleInstance.Reset();
	}
}

FName FQuickAxisAlignStyle::GetStyleSetName()
{
	static const FName StyleName("QuickAxisAlignStyle");
	return StyleName;
}

const FSlateBrush* FQuickAxisAlignStyle::GetBrush(const FName& PropertyName)
{
	if (StyleInstance.IsValid())
	{
		return StyleInstance->GetBrush(PropertyName);
	}
	return nullptr;
}
