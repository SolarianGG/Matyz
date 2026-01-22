#include "Research/Inv_ItemRegistry.h"

const FInv_ItemManifest* UInv_ItemRegistry::FindItemManifest(const FGameplayTag& ItemTag) const
{
	for (const FItemDefinition& ItemDefinition : Items)
	{
		if (ItemDefinition.ItemTag.MatchesTagExact(ItemTag))
		{
			return &ItemDefinition.ItemManifest;
		}
	}
	
	return nullptr;
}