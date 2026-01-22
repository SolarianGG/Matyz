// Project MATYZ. All right reserved.


#include "Player/MTZPlayerCharacter.h"

#include "InputAction.h"
#include "EnhancedInputComponent.h"

AMTZPlayerCharacter::AMTZPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMTZPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMTZPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
	if (auto* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ThisClass::OnMove);
		EnhancedInput->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ThisClass::OnLook);
		EnhancedInput->BindAction(JumpInputAction, ETriggerEvent::Started, this, &ThisClass::Jump);
		EnhancedInput->BindAction(JumpInputAction, ETriggerEvent::Completed, this, &ThisClass::StopJumping);
	}
}

void AMTZPlayerCharacter::OnMove(const FInputActionValue& Value)
{
	const FVector2D Force = Value.Get<FVector2D>();
	AddMovementInput(GetActorRightVector(), Force.X);
	AddMovementInput(GetActorForwardVector(), Force.Y);
}

void AMTZPlayerCharacter::OnLook(const FInputActionValue& Value)
{
	const FVector2D Force = Value.Get<FVector2D>();
	AddControllerYawInput(Force.X);
	AddControllerPitchInput(-Force.Y);
}