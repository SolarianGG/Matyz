#include "Research/Inv_CombinationComponent.h"
#include "Research/Inv_CombinationArea.h"
#include "Research/Inv_CombinationRecipeDataAsset.h"
#include "Research/Inv_ItemRegistry.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Manifest/Inv_ItemManifest.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

UInv_CombinationComponent::UInv_CombinationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	
	CombinationSlots[0] = nullptr;
	CombinationSlots[1] = nullptr;
}

void UInv_CombinationComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwningController = Cast<APlayerController>(GetOwner());
	
	if (OwningController.IsValid())
	{
		InventoryComponent = OwningController->FindComponentByClass<UInv_InventoryComponent>();
	}
}

bool UInv_CombinationComponent::CanCombine() const
{
	if (!OwningController.IsValid())
		return false;
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInv_CombinationArea::StaticClass(), FoundActors);
	
	for (AActor* Actor : FoundActors)
	{
		if (AInv_CombinationArea* CombinationArea = Cast<AInv_CombinationArea>(Actor))
		{
			if (CombinationArea->IsPlayerInArea(OwningController.Get()))
			{
				return true;
			}
		}
	}
	
	return false;
}

bool UInv_CombinationComponent::AddItemToSlot(UInv_InventoryItem* Item, int32 SlotIndex)
{
	if (!IsValid(Item) || SlotIndex < 0 || SlotIndex > 1)
		return false;
	
	// Check for the duplicates
	if (CombinationSlots[0] == Item || CombinationSlots[1] == Item)
		return false;
	
	CombinationSlots[SlotIndex] = Item;
	OnSlotChanged.Broadcast(SlotIndex);
	
	return true;
}

void UInv_CombinationComponent::RemoveItemFromSlot(int32 SlotIndex)
{
	if (SlotIndex < 0 || SlotIndex > 1)
		return;
	
	CombinationSlots[SlotIndex] = nullptr;
	OnSlotChanged.Broadcast(SlotIndex);
}

void UInv_CombinationComponent::ClearSlots()
{
	CombinationSlots[0] = nullptr;
	CombinationSlots[1] = nullptr;
	
	OnSlotChanged.Broadcast(0);
	OnSlotChanged.Broadcast(1);
}

UInv_InventoryItem* UInv_CombinationComponent::GetItemInSlot(int32 SlotIndex) const
{
	if (SlotIndex < 0 || SlotIndex > 1)
		return nullptr;
	
	return CombinationSlots[SlotIndex];
}

bool UInv_CombinationComponent::AreSlotsReady() const
{
	return IsValid(CombinationSlots[0]) && IsValid(CombinationSlots[1]);
}

void UInv_CombinationComponent::AttemptCombination()
{
	if (!CanCombine() || !AreSlotsReady())
		return;
	
	if (OwningController->HasAuthority())
	{
		Server_AttemptCombination(CombinationSlots[0], CombinationSlots[1]);
	}
	else
	{
		Server_AttemptCombination(CombinationSlots[0], CombinationSlots[1]);
	}
}

void UInv_CombinationComponent::Server_AttemptCombination_Implementation(UInv_InventoryItem* ItemA, UInv_InventoryItem* ItemB)
{
	if (!IsValid(ItemA) || !IsValid(ItemB) || !IsValid(RecipeDatabase) || !InventoryComponent.IsValid())
	{
		OnCombinationFailed.Broadcast();
		ClearSlots();
		return;
	}
	
	FGameplayTag TypeA = ItemA->GetItemManifest().GetItemType();
	FGameplayTag TypeB = ItemB->GetItemManifest().GetItemType();
	
	const FCombinationRecipe* Recipe = RecipeDatabase->FindRecipe(TypeA, TypeB);
	
	if (Recipe)
	{
		SpawnCombinedItem(Recipe);
		
		// For now just Drop Item. Maybe add normal delete function later to the Inventory Component
		InventoryComponent->Server_DropItem(ItemA, ItemA->GetTotalStackCount());
		InventoryComponent->Server_DropItem(ItemB, ItemB->GetTotalStackCount());
		
		OnCombinationSuccess.Broadcast(Recipe->OutputItem);
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Combination Successful!"));
		}
	}
	else
	{
		InventoryComponent->Server_DropItem(ItemA, ItemA->GetTotalStackCount());
		InventoryComponent->Server_DropItem(ItemB, ItemB->GetTotalStackCount());
		
		OnCombinationFailed.Broadcast();
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Combination Failed! Items Lost!"));
		}
	}
	
	ClearSlots();
}

void UInv_CombinationComponent::SpawnCombinedItem(const FCombinationRecipe* Recipe)
{
	if (!Recipe || !OwningController.IsValid() || !IsValid(ItemRegistry) || !InventoryComponent.IsValid())
		return;
	
	const FInv_ItemManifest* ItemManifest = ItemRegistry->FindItemManifest(Recipe->OutputItem);
	
	if (!ItemManifest)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, 
				FString::Printf(TEXT("Item not found in registry: %s"), *Recipe->OutputItem.ToString()));
		}
		return;
	}
	
	APawn* OwningPawn = OwningController->GetPawn();
	if (!IsValid(OwningPawn))
		return;
	
	FVector SpawnLocation = OwningPawn->GetActorLocation() + OwningPawn->GetActorForwardVector() * 100.f;
	FRotator SpawnRotation = FRotator::ZeroRotator;
	
	FInv_ItemManifest ManifestCopy = *ItemManifest;
	ManifestCopy.SpawnPickUpActor(OwningController.Get(), SpawnLocation, SpawnRotation);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, 
			FString::Printf(TEXT("Spawned combined item: %s"), *Recipe->OutputItem.ToString()));
	}
}