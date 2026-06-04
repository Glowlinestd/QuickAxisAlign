#include "Widgets/SQAACellBorder.h"
#include "Rendering/DrawElements.h"
#include "Styling/AppStyle.h"

void SQAACellBorder::Construct(const FArguments& InArgs)
{
	BorderColor = InArgs._BorderColor;
	BackgroundColor = InArgs._BackgroundColor;
	Thickness = InArgs._Thickness;

	ChildSlot
	.Padding(InArgs._Padding)
	[
		InArgs._Content.Widget
	];
}

int32 SQAACellBorder::OnPaint(
	const FPaintArgs& Args,
	const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements,
	int32 LayerId,
	const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	const FLinearColor BgColor = BackgroundColor.Get(FSlateColor(EStyleColor::Panel)).GetColor(InWidgetStyle);
	if (BgColor.A > 0.0f)
	{
		FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(),
			FAppStyle::Get().GetBrush("WhiteBrush"),
			ESlateDrawEffect::None,
			BgColor);
	}

	const int32 ContentLayer = SCompoundWidget::OnPaint(
		Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	const FVector2f Size = AllottedGeometry.GetLocalSize();

	const float T = Thickness.Get();
	const float HalfPixel = (T <= 1.0f) ? 0.5f : 0.0f;

	const FVector2f TL(HalfPixel, HalfPixel);
	const FVector2f TR(Size.X - HalfPixel, HalfPixel);
	const FVector2f BR(Size.X - HalfPixel, Size.Y - HalfPixel);
	const FVector2f BL(HalfPixel, Size.Y - HalfPixel);

	const FLinearColor LineColor = BorderColor.Get(FSlateColor(EStyleColor::Recessed)).GetColor(InWidgetStyle);

	auto DrawLine = [&](const FVector2f& A, const FVector2f& B)
	{
		TArray<FVector2f> Points;
		Points.Add(A);
		Points.Add(B);

		FSlateDrawElement::MakeLines(
			OutDrawElements,
			ContentLayer + 1,
			AllottedGeometry.ToPaintGeometry(),
			Points,
			ESlateDrawEffect::None,
			LineColor,
			false,
			T);
	};

	DrawLine(TL, TR);
	DrawLine(TR, BR);
	DrawLine(BR, BL);
	DrawLine(BL, TL);

	return ContentLayer + 1;
}
