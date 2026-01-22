#include "Research/Inv_CombinationArea.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"

AInv_CombinationArea::AInv_CombinationArea()
{
	PrimaryActorTick.bCanEverTick = false;
	
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	TriggerVolume->SetupAttachment(RootComponent);
	TriggerVolume->SetBoxExtent(FVector(200.f, 200.f, 100.f));
	TriggerVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	CombinationTableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CombinationTableMesh"));
	CombinationTableMesh->SetupAttachment(RootComponent);
	CombinationTableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AInv_CombinationArea::BeginPlay()
{
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnOverlapEnd);
}

void AInv_CombinationArea::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACharacter* Character = Cast<ACharacter>(OtherActor))
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			PlayersInArea.Add(PlayerController);
		}
	}
}

void AInv_CombinationArea::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ACharacter* Character = Cast<ACharacter>(OtherActor))
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			PlayersInArea.Remove(PlayerController);
		}
	}
}

bool AInv_CombinationArea::IsPlayerInArea(APlayerController* PlayerController) const
{
	if (!IsValid(PlayerController))
		return false;
	
	return PlayersInArea.Contains(PlayerController);
}
