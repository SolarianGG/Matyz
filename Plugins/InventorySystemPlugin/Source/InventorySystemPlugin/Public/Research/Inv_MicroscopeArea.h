#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inv_MicroscopeArea.generated.h"

class UBoxComponent;
class APlayerController;

UCLASS()
class INVENTORYSYSTEMPLUGIN_API AInv_MicroscopeArea : public AActor
{
	GENERATED_BODY()
	
public:
	AInv_MicroscopeArea();
	
	bool IsPlayerInArea(APlayerController* PlayerController) const;
	
protected:
	virtual void BeginPlay() override;
	
private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(VisibleAnywhere, Category = "Microscope")
	TObjectPtr<UBoxComponent> TriggerVolume;
	
	UPROPERTY(VisibleAnywhere, Category = "Microscope")
	TObjectPtr<UStaticMeshComponent> MicroscopeMesh;
	
	TSet<TWeakObjectPtr<APlayerController>> PlayersInArea;
};