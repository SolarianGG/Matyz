#include "Research/Inv_CombinationRecipeDataAsset.h"

const FCombinationRecipe* UInv_CombinationRecipeDataAsset::FindRecipe(const FGameplayTag& ItemA, const FGameplayTag& ItemB) const
{
	for (const FCombinationRecipe& Recipe : Recipes)
	{
		if (Recipe.Matches(ItemA, ItemB))
		{
			return &Recipe;
		}
	}
	
	return nullptr;
}
