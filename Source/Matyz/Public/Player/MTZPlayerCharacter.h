// Project MATYZ. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MTZPlayerCharacter.generated.h"

struct FInputActionValue;
class UInputAction;

UCLASS()
class MATYZ_API AMTZPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMTZPlayerCharacter();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> MoveInputAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> LookInputAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> JumpInputAction;
public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void OnMove(const FInputActionValue& Value);
	void OnLook(const FInputActionValue& Value);
};
