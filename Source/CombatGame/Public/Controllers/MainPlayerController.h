// 2025 3DAndGo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class APlayerCharacter;
class USceneComponent;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class COMBATGAME_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AMainPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool bSprintToggled;

	APlayerCharacter* MainPlayerRef;

protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> PlayerContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> PlayerMovementAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> PlayerSprintAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> PlayerInteractAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> PlayerDropWeaponAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> PlayerBasicAttackAction;

	void Move(const FInputActionValue& InputActionValue);
	void ToggleSprint();
	void PlayerInteraction();
	void DropWeapon();
	void BasicAttack();
};
