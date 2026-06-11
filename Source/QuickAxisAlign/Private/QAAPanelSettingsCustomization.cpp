#include "QAAPanelSettingsCustomization.h"

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "QAAPanelSettings.h"
#include "QuickAxisAlign.h"
#include "QuickAxisAlignStyle.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SOverlay.h"
#include "Widgets/SBoxPanel.h"
#include "Styling/AppStyle.h"
#include "Widgets/SQAACellBorder.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "QAAPanelSettingsCustomization"

namespace
{
	TSharedRef<SWidget> MakeAxisText(const FText& Text, const FText& Tooltip, const IDetailLayoutBuilder& DetailBuilder)
	{
		return SNew(STextBlock)
			.Text(Text)
			.ToolTipText(Tooltip)
			.Justification(ETextJustify::Center)
			.Font(DetailBuilder.GetDetailFont());
	}

	TSharedRef<SWidget> MakeToggleNameContent(const FText& Text, const FText& Tooltip, TWeakObjectPtr<UQAAPanelSettings> Settings,
		bool UQAAPanelSettings::* MemX, bool UQAAPanelSettings::* MemY, bool UQAAPanelSettings::* MemZ,
		const IDetailLayoutBuilder& DetailBuilder)
	{
		return SNew(SQAACellBorder)
			.BorderColor(FSlateColor(FLinearColor::Transparent))
			.BackgroundColor(FSlateColor(FLinearColor::Transparent))
			.Thickness(0.f)
			.Padding(0.f)
			.ToolTipText(Tooltip)
			.OnDoubleClick_Lambda([Settings, MemX, MemY, MemZ]()
			{
				if (!Settings.IsValid()) return FReply::Unhandled();
				bool bAllOn = Settings.Get()->*MemX && Settings.Get()->*MemY && Settings.Get()->*MemZ;
				bool bNew = !bAllOn;
				Settings.Get()->*MemX = bNew;
				Settings.Get()->*MemY = bNew;
				Settings.Get()->*MemZ = bNew;
				Settings->SaveConfig();
				return FReply::Handled();
			})
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(Text)
					.Font(DetailBuilder.GetDetailFont())
				]
			];
	}

	TSharedRef<SWidget> MakeHeaderRowCell(TSharedRef<SWidget> Content)
	{
		return SNew(SOverlay)
			+ SOverlay::Slot()
			.Padding(FMargin(-12.f, 0.f, -2.f, 0.f))
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			[
				SNew(SColorBlock)
				.Color_Lambda([]() { return FSlateColor(EStyleColor::Header).GetSpecifiedColor(); })
			]
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Center)
			[
				Content
			];
	}

	TSharedRef<SWidget> MakeAxisCheckBox(TWeakObjectPtr<UQAAPanelSettings> Settings, bool UQAAPanelSettings::* Member, const FText& Tooltip)
	{
		return SNew(SBox)
			.HAlign(HAlign_Center)
			[
				SNew(SCheckBox)
				.ToolTipText(Tooltip)
				.IsChecked_Lambda([Settings, Member]()
				{
					return Settings.IsValid() && Settings.Get()->*Member ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
				})
				.OnCheckStateChanged_Lambda([Settings, Member](ECheckBoxState NewState)
				{
					if (Settings.IsValid())
					{
						Settings.Get()->*Member = (NewState == ECheckBoxState::Checked);
						Settings->SaveConfig();
					}
				})
			];
	}
}

TSharedRef<IDetailCustomization> FQAAPanelSettingsCustomization::MakeInstance()
{
	return MakeShared<FQAAPanelSettingsCustomization>();
}

void FQAAPanelSettingsCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> Objects;
	DetailBuilder.GetObjectsBeingCustomized(Objects);

	TWeakObjectPtr<UQAAPanelSettings> Settings;
	if (Objects.Num() > 0)
	{
		Settings = Cast<UQAAPanelSettings>(Objects[0].Get());
	}

	DetailBuilder.HideProperty(GET_MEMBER_NAME_CHECKED(UQAAPanelSettings, bLocationX));
	DetailBuilder.HideProperty(GET_MEMBER_NAME_CHECKED(UQAAPanelSettings, bLocationY));
	DetailBuilder.HideProperty(GET_MEMBER_NAME_CHECKED(UQAAPanelSettings, bLocationZ));
	DetailBuilder.HideProperty(GET_MEMBER_NAME_CHECKED(UQAAPanelSettings, bRotationX));
	DetailBuilder.HideProperty(GET_MEMBER_NAME_CHECKED(UQAAPanelSettings, bRotationY));
	DetailBuilder.HideProperty(GET_MEMBER_NAME_CHECKED(UQAAPanelSettings, bRotationZ));
	DetailBuilder.HideProperty(GET_MEMBER_NAME_CHECKED(UQAAPanelSettings, bScaleX));
	DetailBuilder.HideProperty(GET_MEMBER_NAME_CHECKED(UQAAPanelSettings, bScaleY));
	DetailBuilder.HideProperty(GET_MEMBER_NAME_CHECKED(UQAAPanelSettings, bScaleZ));

	IDetailCategoryBuilder& TransformCategory = DetailBuilder.EditCategory(TEXT("Transform"));

	TransformCategory.AddCustomRow(FText::FromString(TEXT("X Y Z")))
	.NameContent()
	[
		SNew(STextBlock)
		.Text(FText::GetEmpty())
		.Font(DetailBuilder.GetDetailFont())
	]
	.ValueContent()
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Fill)
	.MinDesiredWidth(160.f)
	.MaxDesiredWidth(TOptional<float>())
	[
		MakeHeaderRowCell(
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.FillWidth(1.f)
			.VAlign(VAlign_Center)
			[
				MakeAxisText(FText::FromString(TEXT("X")), LOCTEXT("XAxisTT", "Apply to X axis"), DetailBuilder)
			]
			+ SHorizontalBox::Slot()
			.FillWidth(1.f)
			.VAlign(VAlign_Center)
			[
				MakeAxisText(FText::FromString(TEXT("Y")), LOCTEXT("YAxisTT", "Apply to Y axis"), DetailBuilder)
			]
			+ SHorizontalBox::Slot()
			.FillWidth(1.f)
			.VAlign(VAlign_Center)
			[
				MakeAxisText(FText::FromString(TEXT("Z")), LOCTEXT("ZAxisTT", "Apply to Z axis"), DetailBuilder)
			])
	];

	TransformCategory.AddCustomRow(FText::FromString(TEXT("Location")))
	.NameContent()
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Fill)
	[
		MakeToggleNameContent(FText::FromString(TEXT("Location")), LOCTEXT("LocationToggleTT", "Double-click to toggle all Location axes"), Settings,
			&UQAAPanelSettings::bLocationX,
			&UQAAPanelSettings::bLocationY,
			&UQAAPanelSettings::bLocationZ,
			DetailBuilder)
	]
	.ValueContent()
	.HAlign(HAlign_Fill)
	.MinDesiredWidth(160.f)
	.MaxDesiredWidth(TOptional<float>())
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		[
			MakeAxisCheckBox(Settings, &UQAAPanelSettings::bLocationX, LOCTEXT("LocationXTT", "Apply Target Location X to Sources"))
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		[
			MakeAxisCheckBox(Settings, &UQAAPanelSettings::bLocationY, LOCTEXT("LocationYTT", "Apply Target Location Y to Sources"))
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		[
			MakeAxisCheckBox(Settings, &UQAAPanelSettings::bLocationZ, LOCTEXT("LocationZTT", "Apply Target Location Z to Sources"))
		]
	];

	TransformCategory.AddCustomRow(FText::FromString(TEXT("Rotation")))
	.NameContent()
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Fill)
	[
		MakeToggleNameContent(FText::FromString(TEXT("Rotation")), LOCTEXT("RotationToggleTT", "Double-click to toggle all Rotation axes"), Settings,
			&UQAAPanelSettings::bRotationX,
			&UQAAPanelSettings::bRotationY,
			&UQAAPanelSettings::bRotationZ,
			DetailBuilder)
	]
	.ValueContent()
	.HAlign(HAlign_Fill)
	.MinDesiredWidth(160.f)
	.MaxDesiredWidth(TOptional<float>())
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		[
			MakeAxisCheckBox(Settings, &UQAAPanelSettings::bRotationX, LOCTEXT("RotationXTT", "Apply Target Rotation X to Sources"))
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		[
			MakeAxisCheckBox(Settings, &UQAAPanelSettings::bRotationY, LOCTEXT("RotationYTT", "Apply Target Rotation Y to Sources"))
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		[
			MakeAxisCheckBox(Settings, &UQAAPanelSettings::bRotationZ, LOCTEXT("RotationZTT", "Apply Target Rotation Z to Sources"))
		]
	];

	TransformCategory.AddCustomRow(FText::FromString(TEXT("Scale")))
	.NameContent()
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Fill)
	[
		MakeToggleNameContent(FText::FromString(TEXT("Scale")), LOCTEXT("ScaleToggleTT", "Double-click to toggle all Scale axes"), Settings,
			&UQAAPanelSettings::bScaleX,
			&UQAAPanelSettings::bScaleY,
			&UQAAPanelSettings::bScaleZ,
			DetailBuilder)
	]
	.ValueContent()
	.HAlign(HAlign_Fill)
	.MinDesiredWidth(160.f)
	.MaxDesiredWidth(TOptional<float>())
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		[
			MakeAxisCheckBox(Settings, &UQAAPanelSettings::bScaleX, LOCTEXT("ScaleXTT", "Apply Target Scale X to Sources"))
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		[
			MakeAxisCheckBox(Settings, &UQAAPanelSettings::bScaleY, LOCTEXT("ScaleYTT", "Apply Target Scale Y to Sources"))
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		[
			MakeAxisCheckBox(Settings, &UQAAPanelSettings::bScaleZ, LOCTEXT("ScaleZTT", "Apply Target Scale Z to Sources"))
		]
	];

	IDetailCategoryBuilder& VisualAlignCategory = DetailBuilder.EditCategory(TEXT("Visual Align"));

	TSharedPtr<IPropertyHandle> VisualAlignModeHandle = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UQAAPanelSettings, VisualAlignMode));
	DetailBuilder.HideProperty(GET_MEMBER_NAME_CHECKED(UQAAPanelSettings, VisualAlignMode));

	VisualAlignModeHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda([Settings]()
	{
		if (Settings.IsValid())
		{
			Settings->SaveConfig();
		}
	}));

	VisualAlignCategory.AddCustomRow(FText::FromString(TEXT("Mode")))
	.NameContent()
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Fill)
	[
		SNew(SQAACellBorder)
		.BorderColor(FSlateColor(FLinearColor::Transparent))
		.BackgroundColor(FSlateColor(FLinearColor::Transparent))
		.Thickness(0.f)
		.Padding(0.f)
		.ToolTipText(LOCTEXT("ModeTT", "Controls whether sources are moved and rotated, or moved only"))
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("Mode")))
				.Font(DetailBuilder.GetDetailFont())
			]
		]
	]
	.ValueContent()
	.VAlign(VAlign_Center)
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SBox)
			.WidthOverride(150.f)
			[
				VisualAlignModeHandle->CreatePropertyValueWidget()
			]
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.f, 0.f, 0.f, 0.f)
		[
			SNew(SButton)
			.ButtonStyle(FAppStyle::Get(), "Button")
			.HAlign(HAlign_Left)
			.ContentPadding(FMargin(2.f, 3.5f))
			.ToolTipText(LOCTEXT("VisualAlignTT", "Start Visual Align mode to pick source and target points in the viewport"))
			.OnClicked_Lambda([]()
			{
				FQuickAxisAlignModule::Get().StartVisualAlignSession();
				return FReply::Handled();
			})
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				[
				SNew(SBox)
					.WidthOverride(12.f)
					.HeightOverride(12.f)
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
				[
					SNew(SImage)
						.Image(FAppStyle::GetBrush("ViewportToolbar.Snap"))
						.ColorAndOpacity(FSlateColor::UseForeground())
				]
			]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(6.f, 0.f, 0.f, 0.f)
				[
					SNew(STextBlock)
					.Text(FText::FromString(TEXT("Visual Align")))
				]
			]
		]
	];
}

#undef LOCTEXT_NAMESPACE
