#include "QAAVisualAlignEdMode.h"

#include "EditorModeManager.h"
#include "EditorModes.h"
#include "EditorViewportClient.h"
#include "Engine/World.h"
#include "Engine/HitResult.h"
#include "Engine/Selection.h"
#include "EngineUtils.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "CollisionQueryParams.h"
#include "Editor.h"
#include "SceneManagement.h"
#include "HitProxies.h"

#define LOCTEXT_NAMESPACE "QAAVisualAlignEdMode"

const FEditorModeID FQAAVisualAlignEdMode::EM_QAAVisualAlignEdModeId(
	TEXT("EM_QAAVisualAlignEdMode"));

namespace
{
	constexpr float MarkerRadius = 12.0f;
	constexpr int32 MarkerSegments = 16;

	// Slab-method ray vs AABB intersection. Returns true and fills OutHit
	// with the entry point on the box if the ray starting at Origin and
	// going along Direction intersects the box.
	static bool RayIntersectsAABBCenterExtents(
		const FVector& Origin,
		const FVector& Direction,
		const FVector& BoxCenter,
		const FVector& BoxExtents,
		FVector& OutHit)
	{
		const FVector Min = BoxCenter - BoxExtents;
		const FVector Max = BoxCenter + BoxExtents;
		float TMin = -TNumericLimits<float>::Max();
		float TMax = TNumericLimits<float>::Max();
		for (int32 Axis = 0; Axis < 3; ++Axis)
		{
			const float O = Origin[Axis];
			const float D = Direction[Axis];
			const float MN = Min[Axis];
			const float MX = Max[Axis];
			if (FMath::IsNearlyZero(D))
			{
				if (O < MN || O > MX)
				{
					return false;
				}
			}
			else
			{
				float T1 = (MN - O) / D;
				float T2 = (MX - O) / D;
				if (T1 > T2) { Swap(T1, T2); }
				TMin = FMath::Max(TMin, T1);
				TMax = FMath::Min(TMax, T2);
				if (TMin > TMax)
				{
					return false;
				}
			}
		}
		const float T = (TMin < 0.f) ? 0.f : TMin;
		OutHit = Origin + Direction * T;
		return true;
	}
}

FQAAVisualAlignEdMode::FQAAVisualAlignEdMode()
	: Step(EQAAVisualAlignStep::Inactive)
	, bHasSourcePoint(false)
	, bHasTargetPoint(false)
	, bHoverValid(false)
	, LastMouseX(0)
	, LastMouseY(0)
{
}

void FQAAVisualAlignEdMode::Enter()
{
	FEdMode::Enter();
	Step = EQAAVisualAlignStep::Inactive;
	bHasSourcePoint = false;
	bHasTargetPoint = false;
	bHoverValid = false;
}

void FQAAVisualAlignEdMode::Exit()
{
	Step = EQAAVisualAlignStep::Inactive;
	bHasSourcePoint = false;
	bHasTargetPoint = false;
	Source.Reset();
	Target.Reset();
	FEdMode::Exit();
}

void FQAAVisualAlignEdMode::StartSession()
{
	Source.Reset();
	Target.Reset();
	bHasSourcePoint = false;
	bHasTargetPoint = false;
	Step = EQAAVisualAlignStep::WaitingForSource;
	bHoverValid = false;
}

void FQAAVisualAlignEdMode::CancelSession()
{
	Step = EQAAVisualAlignStep::Inactive;
	bHasSourcePoint = false;
	bHasTargetPoint = false;
	Source.Reset();
	Target.Reset();
	bHoverValid = false;
}

bool FQAAVisualAlignEdMode::ApplyAlignment()
{
	if (Step != EQAAVisualAlignStep::ReadyToApply || !bHasSourcePoint || !bHasTargetPoint)
	{
		return false;
	}

	AActor* SourceActor = Source.Get();
	AActor* TargetActor = Target.Get();
	if (!SourceActor || !TargetActor || SourceActor == TargetActor)
	{
		return false;
	}

	const FVector Delta = TargetPoint - SourcePoint;

	FScopedTransaction Transaction(LOCTEXT("QAAVisualAlignTxn", "Quick Axis Visual Align"));
	SourceActor->Modify();
	SourceActor->SetActorLocation(SourceActor->GetActorLocation() + Delta, false);

	UE_LOG(LogTemp, Log, TEXT("QAAVisualAlign: Moved %s by %s"),
		*SourceActor->GetActorLabel(), *Delta.ToString());

	Step = EQAAVisualAlignStep::Inactive;
	bHasSourcePoint = false;
	bHasTargetPoint = false;
	Source.Reset();
	Target.Reset();
	bHoverValid = false;
	return true;
}

void FQAAVisualAlignEdMode::Tick(FEditorViewportClient* ViewportClient, float DeltaTime)
{
	FEdMode::Tick(ViewportClient, DeltaTime);
}

bool FQAAVisualAlignEdMode::MouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x, int32 y)
{
	LastMouseX = x;
	LastMouseY = y;

	if (Step == EQAAVisualAlignStep::WaitingForSource || Step == EQAAVisualAlignStep::WaitingForTarget)
	{
		AActor* Hovered = nullptr;
		FVector WorldPoint;
		bHoverValid = TryGetActorAndPointUnderCursor(ViewportClient, x, y, Hovered, WorldPoint) && Hovered != nullptr;
		return true;
	}
	bHoverValid = false;
	return false;
}

bool FQAAVisualAlignEdMode::GetCursor(EMouseCursor::Type& OutCursor) const
{
	if (bHoverValid)
	{
		OutCursor = EMouseCursor::Crosshairs;
		return true;
	}
	return false;
}

bool FQAAVisualAlignEdMode::HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click)
{
	if (Step != EQAAVisualAlignStep::WaitingForSource && Step != EQAAVisualAlignStep::WaitingForTarget)
	{
		return false;
	}

	if (Click.GetKey() != EKeys::LeftMouseButton)
	{
		return false;
	}

	int32 MouseX = 0;
	int32 MouseY = 0;
	if (InViewportClient && InViewportClient->Viewport)
	{
		MouseX = InViewportClient->Viewport->GetMouseX();
		MouseY = InViewportClient->Viewport->GetMouseY();
	}

	AActor* PickedActor = nullptr;
	FVector PickedPoint;
	if (!TryGetActorAndPointUnderCursor(InViewportClient, MouseX, MouseY, PickedActor, PickedPoint) || !PickedActor)
	{
		return true;
	}

	SelectPickedActor(PickedActor);

	if (Step == EQAAVisualAlignStep::WaitingForSource)
	{
		Source = PickedActor;
		SourcePoint = PickedPoint;
		bHasSourcePoint = true;
		Step = EQAAVisualAlignStep::WaitingForTarget;
	}
	else
	{
		Target = PickedActor;
		TargetPoint = PickedPoint;
		bHasTargetPoint = true;
		Step = EQAAVisualAlignStep::ReadyToApply;
	}

	bHoverValid = false;
	return true;
}

bool FQAAVisualAlignEdMode::InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event)
{
	if (Key == EKeys::Escape && Event == IE_Pressed)
	{
		if (Step != EQAAVisualAlignStep::Inactive)
		{
			CancelSession();
		}
		return true;
	}
	if (Key == EKeys::Enter && Event == IE_Pressed)
	{
		if (Step == EQAAVisualAlignStep::ReadyToApply)
		{
			ApplyAlignment();
		}
		return true;
	}
	return false;
}

bool FQAAVisualAlignEdMode::TryGetActorAndPointUnderCursor(FEditorViewportClient* ViewportClient, int32 MouseX, int32 MouseY, AActor*& OutActor, FVector& OutWorldPoint) const
{
	OutActor = nullptr;

	if (!ViewportClient || !ViewportClient->Viewport)
	{
		return false;
	}

	UWorld* World = ViewportClient->GetWorld();
	if (!World)
	{
		return false;
	}

	FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(
		ViewportClient->Viewport,
		ViewportClient->GetScene(),
		ViewportClient->EngineShowFlags)
		.SetRealtimeUpdate(ViewportClient->IsRealtime()));

	FSceneView* View = ViewportClient->CalcSceneView(&ViewFamily);
	if (!View)
	{
		return false;
	}

	FViewportCursorLocation Cursor(View, ViewportClient, MouseX, MouseY);
	const FVector RayOrigin = Cursor.GetOrigin();
	const FVector RayDir = Cursor.GetDirection();
	const FVector RayEnd = RayOrigin + RayDir * 100000.0;

	// 1) Resolve the actor under the cursor via hit proxy (works even without collision).
	AActor* Hovered = nullptr;
	HHitProxy* HitProxy = ViewportClient->Viewport->GetHitProxy(MouseX, MouseY);
	if (HitProxy && HitProxy->IsA(HActor::StaticGetType()))
	{
		HActor* ActorHit = static_cast<HActor*>(HitProxy);
		AActor* Actor = ActorHit->Actor;
		if (Actor)
		{
			if (Actor->IsSelectionChild())
			{
				Actor = Actor->GetRootSelectionParent();
			}
			Hovered = Actor;
		}
	}

	if (!Hovered)
	{
		return false;
	}

	OutActor = Hovered;

	// 2) Prefer collision trace for an exact point on the surface.
	{
		FCollisionQueryParams Params(SCENE_QUERY_STAT(QAAVisualAlignTrace), true);
		Params.bTraceComplex = true;

		FHitResult Hit;
		const bool bHit = World->LineTraceSingleByObjectType(
			Hit,
			RayOrigin,
			RayEnd,
			FCollisionObjectQueryParams(ECC_WorldStatic),
			Params);

		if (bHit && Hit.GetActor() == Hovered)
		{
			OutWorldPoint = Hit.ImpactPoint;
			return true;
		}
	}

	// 3) Fallback: ray vs component bounds.
	float BestDist = TNumericLimits<float>::Max();
	FVector BestPoint = FVector::ZeroVector;
	bool bFound = false;

	TArray<UPrimitiveComponent*> Prims;
	Hovered->GetComponents<UPrimitiveComponent>(Prims);
	for (UPrimitiveComponent* Prim : Prims)
	{
		if (!Prim || !Prim->IsRegistered())
		{
			continue;
		}
		const FBoxSphereBounds B = Prim->Bounds;
		FVector HitOnBox;
		if (RayIntersectsAABBCenterExtents(RayOrigin, RayDir, B.Origin, B.BoxExtent, HitOnBox))
		{
			const float Dist = (HitOnBox - RayOrigin).Size();
			if (Dist < BestDist)
			{
				BestDist = Dist;
				BestPoint = HitOnBox;
				bFound = true;
			}
		}
	}

	// 4) Last resort: actor-level bounds.
	if (!bFound)
	{
		FVector Origin, Extent;
		Hovered->GetActorBounds(false, Origin, Extent, true);
		const FBoxSphereBounds B(Origin, Extent, Extent.Size());
		FVector HitOnBox;
		if (RayIntersectsAABBCenterExtents(RayOrigin, RayDir, B.Origin, B.BoxExtent, HitOnBox))
		{
			BestPoint = HitOnBox;
			bFound = true;
		}
	}

	if (bFound)
	{
		OutWorldPoint = BestPoint;
		return true;
	}

	return false;
}

void FQAAVisualAlignEdMode::SelectPickedActor(AActor* Actor) const
{
	if (!GEditor || !Actor)
	{
		return;
	}

	GEditor->SelectNone(/*bNoteSelectionChange*/ false, /*bDeselectBSPSurfs*/ true, /*WarnAboutManyActors*/ false);

	GEditor->SelectActor(Actor, /*bInSelected*/ true, /*bNotify*/ true, /*bSelectEvenIfHidden*/ true);
}

void FQAAVisualAlignEdMode::DrawMarker(FPrimitiveDrawInterface* PDI, const FVector& WorldPos, const FLinearColor& Color) const
{
	DrawWireSphere(PDI, WorldPos, FLinearColor::White, MarkerRadius, MarkerSegments, SDPG_Foreground);
	DrawWireSphere(PDI, WorldPos, Color, MarkerRadius * 0.7f, MarkerSegments, SDPG_Foreground);
	DrawCircle(PDI, WorldPos, FVector::ForwardVector, FVector::RightVector, Color, MarkerRadius * 0.5f, MarkerSegments, SDPG_Foreground);
	DrawCircle(PDI, WorldPos, FVector::RightVector, FVector::UpVector, Color, MarkerRadius * 0.5f, MarkerSegments, SDPG_Foreground);
	DrawCircle(PDI, WorldPos, FVector::UpVector, FVector::ForwardVector, Color, MarkerRadius * 0.5f, MarkerSegments, SDPG_Foreground);
}

void FQAAVisualAlignEdMode::DrawArrow(FPrimitiveDrawInterface* PDI, const FVector& From, const FVector& To, const FLinearColor& Color) const
{
	PDI->DrawLine(From, To, Color, SDPG_Foreground, 2.0f);

	const FVector Dir = (To - From).GetSafeNormal();
	if (Dir.IsNearlyZero())
	{
		return;
	}
	const FVector Right = FVector::CrossProduct(Dir, FVector::UpVector).GetSafeNormal();
	if (Right.IsNearlyZero())
	{
		return;
	}
	const FVector Up = FVector::CrossProduct(Right, Dir).GetSafeNormal();

	const float HeadLen = MarkerRadius * 1.5f;
	const float HeadRad = MarkerRadius * 0.7f;
	const FVector HeadBase = To - Dir * HeadLen;
	PDI->DrawLine(To, HeadBase + Right * HeadRad, Color, SDPG_Foreground, 2.0f);
	PDI->DrawLine(To, HeadBase - Right * HeadRad, Color, SDPG_Foreground, 2.0f);
	PDI->DrawLine(To, HeadBase + Up * HeadRad, Color, SDPG_Foreground, 2.0f);
	PDI->DrawLine(To, HeadBase - Up * HeadRad, Color, SDPG_Foreground, 2.0f);
}

void FQAAVisualAlignEdMode::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
{
	FEdMode::Render(View, Viewport, PDI);

	if (bHasSourcePoint)
	{
		DrawMarker(PDI, SourcePoint, FLinearColor(0.0f, 1.0f, 0.0f, 1.0f));
	}
	if (bHasTargetPoint)
	{
		DrawMarker(PDI, TargetPoint, FLinearColor(0.0f, 0.5f, 1.0f, 1.0f));
	}
	if (bHasSourcePoint && bHasTargetPoint)
	{
		DrawArrow(PDI, SourcePoint, TargetPoint, FLinearColor(1.0f, 0.7f, 0.0f, 1.0f));
	}
}

#undef LOCTEXT_NAMESPACE
