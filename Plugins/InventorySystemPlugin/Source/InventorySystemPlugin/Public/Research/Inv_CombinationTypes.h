#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Inv_CombinationTypes.generated.h"

USTRUCT(BlueprintType)
struct FCombinationRecipe
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combination")
	FGameplayTag InputItemA;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combination")
	FGameplayTag InputItemB;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combination")
	FGameplayTag OutputItem;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combination")
	bool bConsumeInputs{true};
	
	bool Matches(const FGameplayTag& ItemA, const FGameplayTag& ItemB) const
	{
		return (InputItemA.MatchesTagExact(ItemA) && InputItemB.MatchesTagExact(ItemB)) ||
			(InputItemA.MatchesTagExact(ItemB) && InputItemB.MatchesTagExact(ItemA));
	}
};