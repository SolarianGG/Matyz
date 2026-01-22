#include "Widgets/Combination/Inv_JournalWidget.h"
#include "Research/Inv_ResearchComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "GameFramework/PlayerController.h"

void UInv_JournalWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	APlayerController* PlayerController = GetOwningPlayer();
	if (IsValid(PlayerController))
	{
		ResearchComponent = PlayerController->FindComponentByClass<UInv_ResearchComponent>();
		
		if (ResearchComponent.IsValid())
		{
			ResearchComponent->OnItemResearched.AddDynamic(this, &ThisClass::OnItemResearched);
		}
	}
	
	UpdateProgress();
}

void UInv_JournalWidget::UpdateProgress()
{
	if (!ResearchComponent.IsValid())
		return;
	
	int32 ResearchedCount = ResearchComponent->GetResearchedItems().Num();
	float ProgressPercent = 0.f;
	
	if (TotalDiscoverableItems > 0)
	{
		ProgressPercent = static_cast<float>(ResearchedCount) / static_cast<float>(TotalDiscoverableItems);
	}
	
	if (IsValid(ProgressBar))
	{
		ProgressBar->SetPercent(ProgressPercent);
	}
	
	if (IsValid(ProgressText))
	{
		FString ProgressString = FString::Printf(TEXT("%d / %d (%.1f%%)"), 
			ResearchedCount, TotalDiscoverableItems, ProgressPercent * 100.f);
		ProgressText->SetText(FText::FromString(ProgressString));
	}
}

void UInv_JournalWidget::AddJournalEntry(const FText& EntryText)
{
	if (!IsValid(EntriesScrollBox))
		return;
	
	UTextBlock* EntryTextBlock = NewObject<UTextBlock>(this);
	if (IsValid(EntryTextBlock))
	{
		EntryTextBlock->SetText(EntryText);
		EntriesScrollBox->AddChild(EntryTextBlock);
		
		EntriesScrollBox->ScrollToEnd();
	}
}

void UInv_JournalWidget::OnItemResearched(FGameplayTag ResearchID)
{
	UpdateProgress();
	
	FText EntryText = FText::FromString(FString::Printf(TEXT("Discovered: %s"), *ResearchID.ToString()));
	AddJournalEntry(EntryText);
}
