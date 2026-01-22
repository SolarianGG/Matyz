// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/PopUp/Inv_ItemPopUp.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "Research/Inv_ResearchComponent.h"
#include "Research//Inv_CombinationComponent.h"
#include "GameFramework/PlayerController.h"

void UInv_ItemPopUp::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_Split->OnClicked.AddDynamic(this, &ThisClass::SplitButtonClicked);
	Button_Drop->OnClicked.AddDynamic(this, &ThisClass::DropButtonClicked);
	Button_Consume->OnClicked.AddDynamic(this, &ThisClass::ConsumeButtonClicked);
	Button_Research->OnClicked.AddDynamic(this, &ThisClass::ResearchButtonClicked);
	Button_Combine->OnClicked.AddDynamic(this, &ThisClass::CombineButtonClicked);
	Slider_Split->OnValueChanged.AddDynamic(this, &ThisClass::SliderValueChanged);
}

void UInv_ItemPopUp::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	RemoveFromParent();
}

int32 UInv_ItemPopUp::GetSplitAmount() const
{
	return FMath::Floor(Slider_Split->GetValue());
}

void UInv_ItemPopUp::SetItem(UInv_InventoryItem* Item)
{
	CurrentItem = Item;
	UpdateButtonVisibility();
}

void UInv_ItemPopUp::UpdateButtonVisibility()
{
	APlayerController* PC = GetOwningPlayer();
	if (!IsValid(PC) || !IsValid(CurrentItem))
		return;

	// Check research availability
	UInv_ResearchComponent* ResearchComp = PC->FindComponentByClass<UInv_ResearchComponent>();
	if (IsValid(ResearchComp) && ResearchComp->CanResearchItem(CurrentItem))
	{
		Button_Research->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Button_Research->SetVisibility(ESlateVisibility::Collapsed);
	}

	// Check combination availability
	UInv_CombinationComponent* CombineComp = PC->FindComponentByClass<UInv_CombinationComponent>();
	if (IsValid(CombineComp) && CombineComp->CanCombine())
	{
		Button_Combine->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Button_Combine->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInv_ItemPopUp::SplitButtonClicked()
{
	if (OnSplit.ExecuteIfBound(GetSplitAmount(), GridIndex))
	{
		RemoveFromParent();
	}
}

void UInv_ItemPopUp::DropButtonClicked()
{
	if (OnDrop.ExecuteIfBound(GridIndex))
	{
		RemoveFromParent();
	}
}

void UInv_ItemPopUp::ConsumeButtonClicked()
{
	if (OnConsume.ExecuteIfBound(GridIndex))
	{
		RemoveFromParent();
	}
}

void UInv_ItemPopUp::ResearchButtonClicked()
{
	if (OnResearch.ExecuteIfBound(GridIndex))
	{
		RemoveFromParent();
	}
}

void UInv_ItemPopUp::CombineButtonClicked()
{
	if (OnCombine.ExecuteIfBound(GridIndex))
	{
		RemoveFromParent();
	}
}

void UInv_ItemPopUp::SliderValueChanged(float Value)
{
	Text_SplitAmount->SetText(FText::AsNumber(FMath::Floor(Value)));
}

void UInv_ItemPopUp::CollapseSplitButton() const
{
	Button_Split->SetVisibility(ESlateVisibility::Collapsed);
	Slider_Split->SetVisibility(ESlateVisibility::Collapsed);
	Text_SplitAmount->SetVisibility(ESlateVisibility::Collapsed);
}

void UInv_ItemPopUp::CollapseConsumeButton() const
{
	Button_Consume->SetVisibility(ESlateVisibility::Collapsed);
}

void UInv_ItemPopUp::CollapseResearchButton() const
{
	Button_Research->SetVisibility(ESlateVisibility::Collapsed);
}

void UInv_ItemPopUp::CollapseCombineButton() const
{
	Button_Combine->SetVisibility(ESlateVisibility::Collapsed);
}

void UInv_ItemPopUp::SetSliderParameters(const float Max, const float Value) const
{
	Slider_Split->SetMaxValue(Max);
	Slider_Split->SetMinValue(1);
	Slider_Split->SetValue(Value);

	Text_SplitAmount->SetText(FText::AsNumber(FMath::Floor(Value)));
}

FVector2D UInv_ItemPopUp::GetBoxSize() const
{
	return FVector2D(SizeBox_Root->GetWidthOverride(), SizeBox_Root->GetHeightOverride());
}
