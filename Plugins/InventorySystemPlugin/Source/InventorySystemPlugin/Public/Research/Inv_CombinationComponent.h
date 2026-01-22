#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Inv_CombinationComponent.generated.h"

class UInv_InventoryItem;
class UInv_CombinationRecipeDataAsset;
class UInv_InventoryComponent;
class UInv_ItemRegistry;
struct FCombinationRecipe;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombinationSuccess, FGameplayTag, ResultItemType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCombinationFailed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotChanged, int32, SlotIndex);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYSYSTEMPLUGIN_API UInv_CombinationComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UInv_CombinationComponent();
	
	// Check if player is near a combination area
	bool CanCombine() const;
	
	UFUNCTION(BlueprintCallable, Category = "Combination")
	bool AddItemToSlot(UInv_InventoryItem* Item, int32 SlotIndex);
	
	UFUNCTION(BlueprintCallable, Category = "Combination")
	void RemoveItemFromSlot(int32 SlotIndex);
	
	UFUNCTION(BlueprintCallable, Category = "Combination")
	void ClearSlots();
	
	UFUNCTION(BlueprintCallable, Category = "Combination")
	void AttemptCombination();
	
	UInv_InventoryItem* GetItemInSlot(int32 SlotIndex) const;
	
	bool AreSlotsReady() const;
	
	FOnCombinationSuccess OnCombinationSuccess;
	FOnCombinationFailed OnCombinationFailed;
	FOnSlotChanged OnSlotChanged;
	
protected:
	virtual void BeginPlay() override;
	
private:
	UFUNCTION(Server, Reliable)
	void Server_AttemptCombination(UInv_InventoryItem* ItemA, UInv_InventoryItem* ItemB);
	
	void SpawnCombinedItem(const FCombinationRecipe* Recipe);
	
	TWeakObjectPtr<APlayerController> OwningController;
	TWeakObjectPtr<UInv_InventoryComponent> InventoryComponent;
	
	// Combination slots
	UPROPERTY()
	TObjectPtr<UInv_InventoryItem> CombinationSlots[2];
	
	// Recipe database
	UPROPERTY(EditDefaultsOnly, Category = "Combination")
	TObjectPtr<UInv_CombinationRecipeDataAsset> RecipeDatabase;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combination")
	TObjectPtr<UInv_ItemRegistry> ItemRegistry;
};