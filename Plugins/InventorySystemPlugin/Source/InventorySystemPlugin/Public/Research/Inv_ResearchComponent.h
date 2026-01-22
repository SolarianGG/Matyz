#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Inv_ResearchComponent.generated.h"

class UInv_InventoryItem;
class AInv_MicroscopeArea;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemResearched, FGameplayTag, ResearchID);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYSYSTEMPLUGIN_API UInv_ResearchComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UInv_ResearchComponent();
	
	// Check if player is near microscope
	bool CanResearch() const;
	
	UFUNCTION(BlueprintCallable, Category = "Research")
	void ResearchItem(UInv_InventoryItem* Item);
	
	// Check if Item itself can be researched
	bool CanResearchItem(UInv_InventoryItem* Item) const;
	
	const TSet<FGameplayTag>& GetResearchedItems() const { return ResearchedItems; }
	
	FOnItemResearched OnItemResearched;
	
protected:
	virtual void BeginPlay() override;
	
private:
	UFUNCTION(Server, Reliable)
	void Server_ResearchItem(UInv_InventoryItem* Item);
	
	void FindNearbyMicroscope();
	
	TWeakObjectPtr<APlayerController> OwningController;
	TWeakObjectPtr<AInv_MicroscopeArea> NearbyMicroscope;
	
	// Track Researched Items for journal
	UPROPERTY()
	TSet<FGameplayTag> ResearchedItems;
};