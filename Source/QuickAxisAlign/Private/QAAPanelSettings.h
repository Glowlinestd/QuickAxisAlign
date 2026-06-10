#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "QAAPanelSettings.generated.h"

UENUM()
enum class EVisualAlignMode : uint8
{
	MoveAndRotate UMETA(DisplayName = "Move and rotate objects"),
	MoveOnly UMETA(DisplayName = "Move objects only"),
};

UCLASS(Config = QuickAxisAlign, DefaultConfig)
class UQAAPanelSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Config, Category = "Transform")
	bool bLocationX = true;

	UPROPERTY(EditAnywhere, Config, Category = "Transform")
	bool bLocationY = true;

	UPROPERTY(EditAnywhere, Config, Category = "Transform")
	bool bLocationZ = true;

	UPROPERTY(EditAnywhere, Config, Category = "Transform")
	bool bRotationX = false;

	UPROPERTY(EditAnywhere, Config, Category = "Transform")
	bool bRotationY = false;

	UPROPERTY(EditAnywhere, Config, Category = "Transform")
	bool bRotationZ = false;

	UPROPERTY(EditAnywhere, Config, Category = "Transform")
	bool bScaleX = false;

	UPROPERTY(EditAnywhere, Config, Category = "Transform")
	bool bScaleY = false;

	UPROPERTY(EditAnywhere, Config, Category = "Transform")
	bool bScaleZ = false;

	UPROPERTY(EditAnywhere, Config, Category = "Visual Align")
	EVisualAlignMode VisualAlignMode = EVisualAlignMode::MoveAndRotate;
};
