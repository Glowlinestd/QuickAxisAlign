// Copyright Epic Games, Inc. All Rights Reserved.

#include "QAAVisualAlignEdMode.h"

#include "QAAPanelSettings.h"

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
#include "Styling/StyleColors.h"
#include "HitProxies.h"

#define LOCTEXT_NAMESPACE "QAAVisualAlignEdMode"

const FEditorModeID FQAAVisualAlignEdMode::EM_QAAVisualAlignEdModeId(
	TEXT("EM_QAAVisualAlignEdMode"));

namespace
{
	constexpr float MarkerRadius = 8.0f;
	constexpr float ArrowLength = 35.0f;
	constexpr float ArrowThickness = 1.5f;
	constexpr float CurveThickness = 1.2f;
	constexpr int32 CurveSegments = 32;

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
	, SourceNormal(FVector::ZeroVector)
	, TargetNormal(FVector::ZeroVector)
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
	SourceNormal = FVector::ZeroVector;
	TargetNormal = FVector::ZeroVector;
	bHoverValid = false;
}

void FQAAVisualAlignEdMode::Exit()
{
	Step = EQAAVisualAlignStep::Inactive;
	bHasSourcePoint = false;
	bHasTargetPoint = false;
	Source.Reset();
	Target.Reset();
	SourceNormal = FVector::ZeroVector;
	TargetNormal = FVector::ZeroVector;
	FEdMode::Exit();
}

void FQAAVisualAlignEdMode::StartSession()
{
	Source.Reset();
	Target.Reset();
	bHasSourcePoint = false;
	bHasTargetPoint = false;
	SourceNormal = FVector::ZeroVector;
	TargetNormal = FVector::ZeroVector;
	Step = EQAAVisualAlignStep::WaitingForSource;
	bHoverValid = false;

	GEditor->SelectNone(false, true, false);
}

void FQAAVisualAlignEdMode::CancelSession()
{
	Step = EQAAVisualAlignStep::Inactive;
	bHasSourcePoint = false;
	bHasTargetPoint = false;
	Source.Reset();
	Target.Reset();
	SourceNormal = FVector::ZeroVector;
	TargetNormal = FVector::ZeroVector;
	bHoverValid = false;

	GEditor->SelectNone(false, true, false);
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

	const FVector SourceNorm = SourceNormal.GetSafeNormal();
	const FVector TargetNorm = TargetNormal.GetSafeNormal();
	const UQAAPanelSettings* Settings = GetDefault<UQAAPanelSettings>();
	const bool bMoveOnly = Settings && Settings->VisualAlignMode == EVisualAlignMode::MoveOnly;

	FScopedTransaction Transaction(LOCTEXT("QAAVisualAlignTxn", "Quick Axis Visual Align"));
	SourceActor->Modify();

	const FVector ActorLocation = SourceActor->GetActorLocation();
	FVector NewLocation = ActorLocation + (TargetPoint - SourcePoint);

	if (bMoveOnly)
	{
		SourceActor->SetActorLocation(NewLocation, false);
	}
	else
	{
		const FQuat DeltaRotation = FQuat::FindBetweenNormals(SourceNorm, -TargetNorm);
		const FQuat NewRotation = DeltaRotation * SourceActor->GetActorQuat();
		const FVector SourceOffset = SourcePoint - ActorLocation;
		const FVector RotatedSourcePoint = ActorLocation + DeltaRotation.RotateVector(SourceOffset);
		NewLocation = ActorLocation + (TargetPoint - RotatedSourcePoint);
		SourceActor->SetActorLocationAndRotation(NewLocation, NewRotation, false);
	}

	UE_LOG(LogTemp, Log, TEXT("QAAVisualAlign: %s %s by %s"),
		bMoveOnly ? TEXT("Moved") : TEXT("Moved+Rotated"),
		*SourceActor->GetActorLabel(), *(NewLocation - ActorLocation).ToString());

	Step = EQAAVisualAlignStep::Inactive;
	bHasSourcePoint = false;
	bHasTargetPoint = false;
	Source.Reset();
	Target.Reset();
	SourceNormal = FVector::ZeroVector;
	TargetNormal = FVector::ZeroVector;
	bHoverValid = false;

	GEditor->SelectNone(false, true, false);
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
		FVector WorldNormal;
		bHoverValid = TryGetActorAndPointUnderCursor(ViewportClient, x, y, Hovered, WorldPoint, WorldNormal) && Hovered != nullptr;
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
	if (Step != EQAAVisualAlignStep::WaitingForSource && Step != EQAAVisualAlignStep::WaitingForTarget && Step != EQAAVisualAlignStep::ReadyToApply)
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
	FVector PickedNormal;
	if (!TryGetActorAndPointUnderCursor(InViewportClient, MouseX, MouseY, PickedActor, PickedPoint, PickedNormal) || !PickedActor)
	{
		return true;
	}

	if (Step == EQAAVisualAlignStep::WaitingForTarget && bHasSourcePoint && PickedActor == Source.Get())
	{
		SourcePoint = PickedPoint;
		SourceNormal = PickedNormal;
		bHoverValid = false;
		return true;
	}

	if (Step == EQAAVisualAlignStep::ReadyToApply && bHasSourcePoint && PickedActor == Source.Get())
	{
		SourcePoint = PickedPoint;
		SourceNormal = PickedNormal;
		bHoverValid = false;
		return true;
	}

	if (Step == EQAAVisualAlignStep::WaitingForSource)
	{
		Source = PickedActor;
		SourcePoint = PickedPoint;
		SourceNormal = PickedNormal;
		bHasSourcePoint = true;
		Step = EQAAVisualAlignStep::WaitingForTarget;
	}
	else if (Step == EQAAVisualAlignStep::WaitingForTarget)
	{
		Target = PickedActor;
		TargetPoint = PickedPoint;
		TargetNormal = PickedNormal;
		bHasTargetPoint = true;
		Step = EQAAVisualAlignStep::ReadyToApply;
	}
	else
	{
		Target = PickedActor;
		TargetPoint = PickedPoint;
		TargetNormal = PickedNormal;
		bHasTargetPoint = true;
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

bool FQAAVisualAlignEdMode::TryGetActorAndPointUnderCursor(FEditorViewportClient* ViewportClient, int32 MouseX, int32 MouseY, AActor*& OutActor, FVector& OutWorldPoint, FVector& OutNormal) const
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
			OutNormal = Hit.ImpactNormal;
			return true;
		}
	}

	// 3) Fallback: ray vs component bounds.
	float BestDist = TNumericLimits<float>::Max();
	FVector BestPoint = FVector::ZeroVector;
	FVector BestNormal = FVector::ZeroVector;
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
				BestNormal = (HitOnBox - B.Origin).GetSafeNormal();
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
			BestNormal = (HitOnBox - B.Origin).GetSafeNormal();
			bFound = true;
		}
	}

	if (bFound)
	{
		OutWorldPoint = BestPoint;
		OutNormal = BestNormal;
		return true;
	}

	return false;
}



void FQAAVisualAlignEdMode::DrawSmallArrow(FPrimitiveDrawInterface* PDI, const FVector& Base, const FVector& Direction, const FLinearColor& Color) const
{
	const FVector Dir = Direction.GetSafeNormal();
	if (Dir.IsNearlyZero())
	{
		return;
	}

	const FVector Tip = Base + Dir * ArrowLength;
	PDI->DrawLine(Base, Tip, Color, SDPG_Foreground, ArrowThickness);
	DrawArrowHead(PDI, Tip, Dir, Color);
}

void FQAAVisualAlignEdMode::DrawConnectCurve(FPrimitiveDrawInterface* PDI, const FVector& From, const FVector& To, const FVector& FromNormal, const FVector& ToNormal, const FLinearColor& Color) const
{
	const FVector Delta = To - From;
	const float Distance = Delta.Size();
	if (Distance < 1.0f)
	{
		return;
	}

	const FVector MidDir = Delta / Distance;

	const FVector AvgNormal = (FromNormal + ToNormal).GetSafeNormal();
	FVector ArcNormal = FVector::CrossProduct(MidDir, AvgNormal).GetSafeNormal();
	if (ArcNormal.IsNearlyZero())
	{
		ArcNormal = FVector::CrossProduct(MidDir, FVector::UpVector).GetSafeNormal();
	}
	if (ArcNormal.IsNearlyZero())
	{
		return;
	}

	const float ArcOffset = FMath::Clamp(Distance * 0.08f, 10.0f, 60.0f);
	const float TangentLen = FMath::Min(Distance * 0.4f, ArrowLength * 3.0f);

	const FVector ControlA = From + FromNormal * TangentLen + ArcNormal * ArcOffset;
	const FVector ControlB = To + ToNormal * TangentLen;

	FVector Prev = From;
	for (int32 i = 1; i <= CurveSegments; ++i)
	{
		const float T = static_cast<float>(i) / static_cast<float>(CurveSegments);
		const float U = 1.0f - T;
		const FVector Point = U * U * U * From
			+ 3.0f * U * U * T * ControlA
			+ 3.0f * U * T * T * ControlB
			+ T * T * T * To;
		PDI->DrawLine(Prev, Point, Color, SDPG_Foreground, CurveThickness);
		Prev = Point;
	}
}

void FQAAVisualAlignEdMode::DrawArrowHead(FPrimitiveDrawInterface* PDI, const FVector& Tip, const FVector& Direction, const FLinearColor& Color) const
{
	if (Direction.IsNearlyZero())
	{
		return;
	}

	const FVector Dir = Direction.GetSafeNormal();

	FVector Right = FVector::CrossProduct(Dir, FVector::UpVector).GetSafeNormal();
	if (Right.IsNearlyZero())
	{
		Right = FVector::CrossProduct(Dir, FVector::ForwardVector).GetSafeNormal();
	}
	const FVector Up = FVector::CrossProduct(Right, Dir).GetSafeNormal();

	const float HeadLen = MarkerRadius * 2.0f;
	const float HeadRad = MarkerRadius * 0.45f;
	const FVector HeadBase = Tip - Dir * HeadLen;

	constexpr int32 NumSegments = 16;
	FVector PrevPoint = HeadBase + Right * HeadRad;

	for (int32 i = 1; i <= NumSegments; ++i)
	{
		const float Angle = (2.0f * PI * i) / NumSegments;
		const FVector Point = HeadBase + (Right * FMath::Cos(Angle) + Up * FMath::Sin(Angle)) * HeadRad;

		PDI->DrawLine(Tip, Point, Color, SDPG_Foreground, 2.0f);
		PDI->DrawLine(PrevPoint, Point, Color, SDPG_Foreground, 2.0f);

		PrevPoint = Point;
	}
}

void FQAAVisualAlignEdMode::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
{
	FEdMode::Render(View, Viewport, PDI);

	const FLinearColor& ArrowColor = USlateThemeManager::Get().GetColor(EStyleColor::Primary);

	if (bHasSourcePoint)
	{
		DrawSmallArrow(PDI, SourcePoint, SourceNormal, ArrowColor);
	}

	if (bHasTargetPoint)
	{
		const FVector TargetBase = TargetPoint + TargetNormal * ArrowLength;
		DrawSmallArrow(PDI, TargetBase, -TargetNormal, ArrowColor);
	}

	if (bHasSourcePoint && bHasTargetPoint)
	{
		const FVector SourceTip = SourcePoint + SourceNormal * ArrowLength;
		const FVector TargetBase = TargetPoint + TargetNormal * ArrowLength;
		DrawConnectCurve(PDI, SourceTip, TargetBase, SourceNormal, TargetNormal, ArrowColor);
	}
}

#undef LOCTEXT_NAMESPACE
