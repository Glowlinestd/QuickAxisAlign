// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Framework/SlateDelegates.h"
#include "Styling/SlateColor.h"

class SQAACellBorder : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SQAACellBorder)
		: _BorderColor(FSlateColor(EStyleColor::Recessed))
		, _BackgroundColor(FSlateColor(EStyleColor::Panel))
		, _Thickness(1.25f)
		, _Padding(FMargin(8.f, 3.f))
	{}
		SLATE_ATTRIBUTE(FSlateColor, BorderColor)
		SLATE_ATTRIBUTE(FSlateColor, BackgroundColor)
		SLATE_ATTRIBUTE(float, Thickness)
		SLATE_ATTRIBUTE(FMargin, Padding)
		SLATE_EVENT(FOnClicked, OnDoubleClick)
		SLATE_DEFAULT_SLOT(FArguments, Content)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	virtual int32 OnPaint(
		const FPaintArgs& Args,
		const FGeometry& AllottedGeometry,
		const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements,
		int32 LayerId,
		const FWidgetStyle& InWidgetStyle,
		bool bParentEnabled) const override;

	virtual FReply OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) override;

private:
	TAttribute<FSlateColor> BorderColor;
	TAttribute<FSlateColor> BackgroundColor;
	TAttribute<float> Thickness;
	FOnClicked OnDoubleClickHandler;
};
