#include "Research/Inv_ResearchComponent.h"
#include "Research/Inv_MicroscopeArea.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

UInv_ResearchComponent::UInv_ResearchComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UInv_ResearchComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwningController = Cast<APlayerController>(GetOwner());
}

bool UInv_ResearchComponent::CanResearch() const
{
	if (!OwningController.IsValid())
		return false;
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInv_MicroscopeArea::StaticClass(), FoundActors);
	
	for (AActor* Actor : FoundActors)
	{
		if (AInv_MicroscopeArea* Microscope = Cast<AInv_MicroscopeArea>(Actor))
		{
			if (Microscope->IsPlayerInArea(OwningController.Get()))
			{
				return true;
			}
		}
	}
	
	return false;
}

bool UInv_ResearchComponent::CanResearchItem(UInv_InventoryItem* Item) const
{
	if (!IsValid(Item) || !CanResearch())
		return false;
	
	FInv_ResearchFragment* ResearchFragment = Item->GetItemManifestMutable().GetFragmentOfTypeMutable<FInv_ResearchFragment>();
	
	if (!ResearchFragment)
		return false;
	
	return !ResearchFragment->IsResearched();
}

void UInv_ResearchComponent::ResearchItem(UInv_InventoryItem* Item)
{
	if (!CanResearchItem(Item))
		return;
	
	if (OwningController->HasAuthority())
	{
		Server_ResearchItem(Item);
	}
	else
	{
		Server_ResearchItem(Item);
	}
}

void UInv_ResearchComponent::Server_ResearchItem_Implementation(UInv_InventoryItem* Item)
{
	if (!CanResearchItem(Item))
		return;
	
	FInv_ResearchFragment* ResearchFragment = Item->GetItemManifestMutable().GetFragmentOfTypeMutable<FInv_ResearchFragment>();
	
	if (!ResearchFragment)
		return;
	
	ResearchFragment->SetResearched(true);
	
	FGameplayTag ResearchID = ResearchFragment->GetResearchID();
	if (ResearchID.IsValid())
	{
		ResearchedItems.Add(ResearchID);
		OnItemResearched.Broadcast(ResearchID);
	}
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, 
			FString::Printf(TEXT("Item Researched! %s"), *ResearchFragment->GetCurrentDescription().ToString()));
	}
}
