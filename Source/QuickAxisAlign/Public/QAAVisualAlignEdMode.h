#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"
#include "InputCoreTypes.h"
#include "QAAVisualAlignEdMode.generated.h"

class AActor;
class FEditorViewportClient;
class FPrimitiveDrawInterface;
class FSceneView;
class FViewport;
class UPrimitiveComponent;
struct FViewportClick;

UENUM()
enum class EQAAVisualAlignStep : uint8
{
	Inactive,
	WaitingForSource,
	WaitingForTarget,
	ReadyToApply,
};

class FQAAVisualAlignEdMode : public FEdMode
{
public:
	const static FEditorModeID EM_QAAVisualAlignEdModeId;

	FQAAVisualAlignEdMode();

	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;
	virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	virtual bool InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event) override;
	virtual bool HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click) override;
	virtual bool MouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x, int32 y) override;
	virtual bool GetCursor(EMouseCursor::Type& OutCursor) const override;

	void StartSession();
	bool ApplyAlignment();
	void CancelSession();

	EQAAVisualAlignStep GetStep() const { return Step; }

private:
	bool TryGetActorAndPointUnderCursor(FEditorViewportClient* ViewportClient, int32 MouseX, int32 MouseY, AActor*& OutActor, FVector& OutWorldPoint) const;
	void SelectPickedActor(AActor* Actor) const;
	void DrawMarker(FPrimitiveDrawInterface* PDI, const FVector& WorldPos, const FLinearColor& Color) const;
	void DrawArrow(FPrimitiveDrawInterface* PDI, const FVector& From, const FVector& To, const FLinearColor& Color) const;

	EQAAVisualAlignStep Step;

	TWeakObjectPtr<AActor> Source;
	TWeakObjectPtr<AActor> Target;

	bool bHasSourcePoint;
	bool bHasTargetPoint;
	FVector SourcePoint;
	FVector TargetPoint;

	bool bHoverValid;

	int32 LastMouseX;
	int32 LastMouseY;
};
