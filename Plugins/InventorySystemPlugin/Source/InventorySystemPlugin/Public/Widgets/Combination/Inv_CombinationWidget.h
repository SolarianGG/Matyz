#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_CombinationWidget.generated.h"

class UInv_CombinationComponent;
class UButton;
class UImage;
class UTextBlock;

UCLASS()
class INVENTORYSYSTEMPLUGIN_API UInv_CombinationWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable, Category = "Combination")
	void UpdateSlot(int32 SlotIndex);
	
	UFUNCTION(BlueprintCallable, Category = "Combination")
	void UpdateCombineButton();
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SlotImage_0;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SlotImage_1;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CombineButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StatusText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ClearButton;
	
private:
	UFUNCTION()
	void OnCombineButtonClicked();
	
	UFUNCTION()
	void OnClearButtonClicked();
	
	UFUNCTION()
	void OnSlotChanged(int32 SlotIndex);
	
	UFUNCTION()
	void OnCombinationSuccess(FGameplayTag ResultItemType);
	
	UFUNCTION()
	void OnCombinationFailed();
	
	TWeakObjectPtr<UInv_CombinationComponent> CombinationComponent;
};