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
#include "Widgets/Layout/SBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"

namespace
{
	TSharedRef<SWidget> MakeAxisText(const FText& Text, const IDetailLayoutBuilder& DetailBuilder)
	{
		return SNew(STextBlock)
			.Text(Text)
			.Justification(ETextJustify::Center)
			.Font(DetailBuilder.GetDetailFont());
	}

	TSharedRef<SWidget> MakeLocationCheckBox(TWeakObjectPtr<UQAAPanelSettings> Settings, bool UQAAPanelSettings::* Member)
	{
		return SNew(SBox)
			.HAlign(HAlign_Center)
			[
				SNew(SCheckBox)
				.IsChecked_Lambda([Settings, Member]()
				{
					return Settings.IsValid() && Settings.Get()->*Member ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
				})
				.OnCheckStateChanged_Lambda([Settings, Member](ECheckBoxState NewState)
				{
					if (Settings.IsValid())
					{
						Settings.Get()->*Member = (NewState == ECheckBoxState::Checked);
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
	.MinDesiredWidth(160.f)
	.MaxDesiredWidth(TOptional<float>())
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		[
			MakeAxisText(FText::FromString(TEXT("X")), DetailBuilder)
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		[
			MakeAxisText(FText::FromString(TEXT("Y")), DetailBuilder)
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		[
			MakeAxisText(FText::FromString(TEXT("Z")), DetailBuilder)
		]
	];

	TransformCategory.AddCustomRow(FText::FromString(TEXT("Location")))
	.NameContent()
	[
		SNew(STextBlock)
		.Text(FText::FromString(TEXT("Location")))
		.Font(DetailBuilder.GetDetailFont())
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
			MakeLocationCheckBox(Settings, &UQAAPanelSettings::bLocationX)
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		[
			MakeLocationCheckBox(Settings, &UQAAPanelSettings::bLocationY)
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		[
			MakeLocationCheckBox(Settings, &UQAAPanelSettings::bLocationZ)
		]
	];

	TransformCategory.AddCustomRow(FText::FromString(TEXT("Rotation")))
	.NameContent()
	[
		SNew(STextBlock)
		.Text(FText::FromString(TEXT("Rotation")))
		.Font(DetailBuilder.GetDetailFont())
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
			MakeLocationCheckBox(Settings, &UQAAPanelSettings::bRotationX)
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		[
			MakeLocationCheckBox(Settings, &UQAAPanelSettings::bRotationY)
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		[
			MakeLocationCheckBox(Settings, &UQAAPanelSettings::bRotationZ)
		]
	];

	TransformCategory.AddCustomRow(FText::FromString(TEXT("Scale")))
	.NameContent()
	[
		SNew(STextBlock)
		.Text(FText::FromString(TEXT("Scale")))
		.Font(DetailBuilder.GetDetailFont())
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
			MakeLocationCheckBox(Settings, &UQAAPanelSettings::bScaleX)
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		[
			MakeLocationCheckBox(Settings, &UQAAPanelSettings::bScaleY)
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		[
			MakeLocationCheckBox(Settings, &UQAAPanelSettings::bScaleZ)
		]
	];

	IDetailCategoryBuilder& VisualAlignCategory = DetailBuilder.EditCategory(TEXT("Visual Align"));

	TSharedPtr<IPropertyHandle> VisualAlignModeHandle = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UQAAPanelSettings, VisualAlignMode));
	DetailBuilder.HideProperty(GET_MEMBER_NAME_CHECKED(UQAAPanelSettings, VisualAlignMode));

	VisualAlignCategory.AddCustomRow(FText::FromString(TEXT("Mode")))
	.NameContent()
	[
		SNew(STextBlock)
		.Text(FText::FromString(TEXT("Mode")))
		.Font(DetailBuilder.GetDetailFont())
	]
	.ValueContent()
	[
		VisualAlignModeHandle->CreatePropertyValueWidget()
	];

	VisualAlignCategory.AddCustomRow(FText::FromString(TEXT("Visual Align Button")))
	.NameContent()
	[
		SNew(STextBlock)
		.Text(FText::GetEmpty())
		.Font(DetailBuilder.GetDetailFont())
	]
	.ValueContent()
	[
		SNew(SButton)
		.ContentPadding(FMargin(6.f, 2.f))
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
				.WidthOverride(16.f)
				.HeightOverride(16.f)
				[
					SNew(SImage)
					.Image(FQuickAxisAlignStyle::GetBrush("QuickAxisAlign.AlignActors"))
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(4.f, 0.f, 0.f, 0.f)
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("Use Visual Align")))
			]
		]
	];
}
