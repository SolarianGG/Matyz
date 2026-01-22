#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Research/Inv_CombinationTypes.h"
#include "Inv_CombinationRecipeDataAsset.generated.h"

UCLASS(BlueprintType)
class INVENTORYSYSTEMPLUGIN_API UInv_CombinationRecipeDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	const FCombinationRecipe* FindRecipe(const FGameplayTag& ItemA, const FGameplayTag& ItemB) const;
	
	const TArray<FCombinationRecipe>& GetAllRecipes() const { return Recipes; }
	
private:
	UPROPERTY(EditAnywhere, Category = "Combination")
	TArray<FCombinationRecipe> Recipes;
};