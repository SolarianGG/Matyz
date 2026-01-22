#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_JournalWidget.generated.h"

class UInv_ResearchComponent;
class UProgressBar;
class UTextBlock;
class UScrollBox;

UCLASS()
class INVENTORYSYSTEMPLUGIN_API UInv_JournalWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable, Category = "Journal")
	void UpdateProgress();
	
	UFUNCTION(BlueprintCallable, Category = "Journal")
	void AddJournalEntry(const FText& EntryText);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ProgressText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> EntriesScrollBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Journal")
	int32 TotalDiscoverableItems = 100;
	
private:
	UFUNCTION()
	void OnItemResearched(FGameplayTag ResearchID);
	
	TWeakObjectPtr<UInv_ResearchComponent> ResearchComponent;
};