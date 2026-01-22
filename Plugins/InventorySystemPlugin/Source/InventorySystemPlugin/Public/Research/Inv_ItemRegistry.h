#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Items/Manifest/Inv_ItemManifest.h"
#include "Inv_ItemRegistry.generated.h"

USTRUCT(BlueprintType)
struct FItemDefinition
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FGameplayTag ItemTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FInv_ItemManifest ItemManifest;
};

UCLASS(BlueprintType)
class INVENTORYSYSTEMPLUGIN_API UInv_ItemRegistry : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	const FInv_ItemManifest* FindItemManifest(const FGameplayTag& ItemTag) const;
	
	const TArray<FItemDefinition>& GetAllItems() const { return Items; }
	
private:
	UPROPERTY(EditAnywhere, Category = "Item")
	TArray<FItemDefinition> Items;
};