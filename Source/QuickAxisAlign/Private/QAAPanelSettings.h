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

UCLASS()
class UQAAPanelSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Transform")
	bool bLocationX = false;

	UPROPERTY(EditAnywhere, Category = "Transform")
	bool bLocationY = false;

	UPROPERTY(EditAnywhere, Category = "Transform")
	bool bLocationZ = false;

	UPROPERTY(EditAnywhere, Category = "Transform")
	bool bRotationX = false;

	UPROPERTY(EditAnywhere, Category = "Transform")
	bool bRotationY = false;

	UPROPERTY(EditAnywhere, Category = "Transform")
	bool bRotationZ = false;

	UPROPERTY(EditAnywhere, Category = "Transform")
	bool bScaleX = false;

	UPROPERTY(EditAnywhere, Category = "Transform")
	bool bScaleY = false;

	UPROPERTY(EditAnywhere, Category = "Transform")
	bool bScaleZ = false;

	UPROPERTY(EditAnywhere, Category = "Visual Align")
	EVisualAlignMode VisualAlignMode = EVisualAlignMode::MoveAndRotate;
};
