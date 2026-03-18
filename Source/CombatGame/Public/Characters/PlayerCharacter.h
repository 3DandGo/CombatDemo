// 2025 3DAndGo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enums/PlayerStates.h"
#include "interfaces/InteractionInterface.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AMainPlayerController;
class UCharacterMovementComponent;
class AWeaponBase;
class USceneComponent;
class UAnimMontage;

UCLASS()
class COMBATGAME_API APlayerCharacter : public ACharacter, public IInteractionInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void Tick(float DeltaTime) override;
	void SetPlayerMovementState();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//// INTERFACE FUNCTIONS ////////
	virtual void Interact_Implementation() override;
	
	///// REFERENCES /////
	UPROPERTY()
	AMainPlayerController* MainController;
	UPROPERTY(VisibleInstanceOnly)
	AWeaponBase* OverlappingWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AWeaponBase* EquippedWeapon;


	//// ENUM STATES //////
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enum")
	ECharacterState CharacterStates;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enum")
	EActionState ActionStates;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enum")
	ECombatState CombatStates;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enum")
	EWeaponType WeaponType;

	//// MOVEMENT VARIABLES ////
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float WalkSpeed = 600.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed = 900.f;

	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> WeaponDropArea;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* LightAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HeavyAttackMontage;

	void StartSprinting();
	void StopSprinting();
	void PlayLightAttackMontage();
	void PlayHeavyAttackMontage();

	UFUNCTION(BlueprintCallable)
	void AttackEnded();

protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	float TraceDistance = 5.0f;
	UPROPERTY(EditDefaultsOnly)
	float CapsuleRadius = 5.0f;
	UPROPERTY(EditDefaultsOnly)
	float CapsuleHalfHeight = 5.0f;
	
	UPROPERTY()
	TArray<AActor*> OverlappedActors;

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

public:
	FORCEINLINE void SetOverlappingWeapon(AWeaponBase* Weapon) { OverlappingWeapon = Weapon; } // Set the current overlapping weapon
	FORCEINLINE AWeaponBase* GetEquippedWeapon() const { return EquippedWeapon; } // Get the current equipped weapon (if any)
	FORCEINLINE void SetEquippedWeapon(AWeaponBase* Weapon) { EquippedWeapon = Weapon; } // Set the current weapon to equipped weapon
	FORCEINLINE EActionState GetActionState() const { return ActionStates; } // Get the current action state
	FORCEINLINE void SetActionState(EActionState NewActionState) { ActionStates = NewActionState; } // Set to new action state
	FORCEINLINE ECombatState GetCombatState() const { return CombatStates; } // Get the current combat state
	FORCEINLINE void SetCombatState(ECombatState NewCombatState) { CombatStates = NewCombatState; } // Set to new combat state
	FORCEINLINE void SetWeaponType(EWeaponType NewWeaponType) { WeaponType = NewWeaponType; }
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
};
