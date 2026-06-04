#pragma once

#include "Widgets/SCompoundWidget.h"
#include "Styling/SlateColor.h"

class SQAACellBorder : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SQAACellBorder)
		: _BorderColor(FSlateColor(EStyleColor::Recessed))
		, _BackgroundColor(FSlateColor(EStyleColor::Panel))
		, _Thickness(0.75f)
		, _Padding(FMargin(8.f, 3.f))
	{}
		SLATE_ATTRIBUTE(FSlateColor, BorderColor)
		SLATE_ATTRIBUTE(FSlateColor, BackgroundColor)
		SLATE_ATTRIBUTE(float, Thickness)
		SLATE_ATTRIBUTE(FMargin, Padding)
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

private:
	TAttribute<FSlateColor> BorderColor;
	TAttribute<FSlateColor> BackgroundColor;
	TAttribute<float> Thickness;
};
