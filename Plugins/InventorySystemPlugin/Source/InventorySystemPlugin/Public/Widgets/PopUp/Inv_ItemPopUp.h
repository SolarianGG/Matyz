// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_ItemPopUp.generated.h"

class USizeBox;
class UTextBlock;
class USlider;
class UButton;
class UInv_InventoryItem;

DECLARE_DYNAMIC_DELEGATE_TwoParams(FPopUpMenuSplit, int32, SplitAmount, int32, Index);
DECLARE_DYNAMIC_DELEGATE_OneParam(FPopUpMenuDrop, int32, Index);
DECLARE_DYNAMIC_DELEGATE_OneParam(FPopUpMenuConsume, int32, Index);
DECLARE_DYNAMIC_DELEGATE_OneParam(FPopUpMenuResearch, int32, Index);
DECLARE_DYNAMIC_DELEGATE_OneParam(FPopUpMenuCombine, int32, Index);

UCLASS()
class INVENTORYSYSTEMPLUGIN_API UInv_ItemPopUp : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	FPopUpMenuSplit OnSplit;
	FPopUpMenuDrop OnDrop;
	FPopUpMenuConsume OnConsume;
	FPopUpMenuResearch OnResearch;
	FPopUpMenuCombine OnCombine;

	int32 GetSplitAmount() const;

	void CollapseSplitButton() const;
	void CollapseConsumeButton() const;
	void CollapseResearchButton() const;
	void CollapseCombineButton() const;
	
	void SetSliderParameters(const float Max, const float Value) const;

	FVector2D GetBoxSize() const;

	void SetGridIndex(int32 Index) { GridIndex = Index; }
	int32 GetGridIndex() const { return GridIndex; }

	// Set the item this popup is for (to check research/combine availability)
	void SetItem(UInv_InventoryItem* Item);
	
private:
	UFUNCTION()
	void SplitButtonClicked();

	UFUNCTION()
	void DropButtonClicked();

	UFUNCTION()
	void ConsumeButtonClicked();

	UFUNCTION()
	void ResearchButtonClicked();

	UFUNCTION()
	void CombineButtonClicked();

	UFUNCTION()
	void SliderValueChanged(float Value);

	void UpdateButtonVisibility();
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Split;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Drop;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Consume;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Research;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Combine;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> Slider_Split;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_SplitAmount;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;

	int32 GridIndex{INDEX_NONE};

	UPROPERTY()
	TObjectPtr<UInv_InventoryItem> CurrentItem;
};