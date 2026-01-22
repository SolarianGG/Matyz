#include "Widgets/Combination/Inv_CombinationWidget.h"
#include "Research/Inv_CombinationComponent.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerController.h"

void UInv_CombinationWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	APlayerController* PlayerController = GetOwningPlayer();
	if (IsValid(PlayerController))
	{
		CombinationComponent = PlayerController->FindComponentByClass<UInv_CombinationComponent>();
		
		if (CombinationComponent.IsValid())
		{
			CombinationComponent->OnSlotChanged.AddDynamic(this, &ThisClass::OnSlotChanged);
			CombinationComponent->OnCombinationSuccess.AddDynamic(this, &ThisClass::OnCombinationSuccess);
			CombinationComponent->OnCombinationFailed.AddDynamic(this, &ThisClass::OnCombinationFailed);
		}
	}
	
	if (IsValid(CombineButton))
	{
		CombineButton->OnClicked.AddDynamic(this, &ThisClass::OnCombineButtonClicked);
	}
	
	if (IsValid(ClearButton))
	{
		ClearButton->OnClicked.AddDynamic(this, &ThisClass::OnClearButtonClicked);
	}
	
	UpdateCombineButton();
}

void UInv_CombinationWidget::UpdateSlot(int32 SlotIndex)
{
	if (!CombinationComponent.IsValid())
		return;
	
	UImage* SlotImage = SlotIndex == 0 ? SlotImage_0 : SlotImage_1;
	if (!IsValid(SlotImage))
	{
		return;
	}
	
	UInv_InventoryItem* Item = CombinationComponent->GetItemInSlot(SlotIndex);
	
	if (IsValid(Item))
	{
		const FInv_ImageFragment* ImageFragment = Item->GetItemManifest().GetFragmentOfType<FInv_ImageFragment>();
		if (ImageFragment && ImageFragment->GetIcon())
		{
			SlotImage->SetBrushFromTexture(ImageFragment->GetIcon());
			SlotImage->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		SlotImage->SetVisibility(ESlateVisibility::Hidden);
	}
	
	UpdateCombineButton();
}

void UInv_CombinationWidget::UpdateCombineButton()
{
	if (!IsValid(CombineButton) || !CombinationComponent.IsValid())
		return;
	
	bool bCanCombine = CombinationComponent->AreSlotsReady() && CombinationComponent->CanCombine();
	CombineButton->SetIsEnabled(bCanCombine);
}

void UInv_CombinationWidget::OnCombineButtonClicked()
{
	if (CombinationComponent.IsValid())
	{
		CombinationComponent->AttemptCombination();
	}
}

void UInv_CombinationWidget::OnClearButtonClicked()
{
	if (CombinationComponent.IsValid())
	{
		CombinationComponent->ClearSlots();
	}
}

void UInv_CombinationWidget::OnSlotChanged(int32 SlotIndex)
{
	UpdateSlot(SlotIndex);
}

void UInv_CombinationWidget::OnCombinationSuccess(FGameplayTag ResultItemType)
{
	if (IsValid(StatusText))
	{
		StatusText->SetText(FText::FromString("Success!"));
		StatusText->SetColorAndOpacity(FLinearColor::Green);
	}
	
	// TODO: If there is time left add timer to clear the message or restore it to default
}

void UInv_CombinationWidget::OnCombinationFailed()
{
	if (IsValid(StatusText))
	{
		StatusText->SetText(FText::FromString("Failed!"));
		StatusText->SetColorAndOpacity(FLinearColor::Red);
	}
	
	// TODO: If there is time left add timer to clear the message or restore it to default
}
