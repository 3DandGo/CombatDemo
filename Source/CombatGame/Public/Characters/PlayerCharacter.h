// 2025 3DAndGo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enums/PlayerStates.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AMainPlayerController;
class UCharacterMovementComponent;

UCLASS()
class COMBATGAME_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void Tick(float DeltaTime) override;
	void SetPlayerMovementState();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
	AMainPlayerController* MainController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums", meta = (AllowPrivateAccess = "true"))
	ECharacterState PlayerStates = ECharacterState::ECS_Still;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	ECharacterState MovementState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float WalkSpeed = 600.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed = 900.f;

	void StartSprinting();
	void StopSprinting();

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USpringArmComponent> PlayerSpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UCameraComponent> PlayerCamera;

private:	
	// Gets MaxWalkSpeed
	FORCEINLINE float GetMaxWalkSpeed() const { return GetCharacterMovement() ? GetCharacterMovement()->MaxWalkSpeed : 0.f; }
	
	//Sets MaxWalkSpeed to new value  { GetCharacterMovement() ? GetCharacterMovement()->MaxWalkSpeed = NewSpeed; }
	FORCEINLINE void SetMaxWalkSpeed(float NewSpeed)
	{
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
		}
	}
};
