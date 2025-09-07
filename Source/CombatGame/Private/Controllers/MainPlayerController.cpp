// 2025 3DAndGo


#include "Controllers/MainPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "Characters/PlayerCharacter.h"
#include "Debug/Debuggers.h"
#include "Enums/PlayerStates.h"
#include "Items/Weapons/WeaponBase.h"
#include "Interfaces/InteractionInterface.h"
#include "Components/SceneComponent.h"


AMainPlayerController::AMainPlayerController()
{
	// If there is multiplayer/other clients, replicates changes across all users
	bReplicates = true;

	bSprintToggled = false;
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Get player ref from the controller
	MainPlayerRef = Cast<APlayerCharacter>(GetCharacter());

	// Checks if the Player's initial context is set and not a nullptr
	check(PlayerContext);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);

	Subsystem->AddMappingContext(PlayerContext, 0);
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	/* Input Action Bindings */
	EnhancedInputComponent->BindAction(PlayerMovementAction, ETriggerEvent::Triggered, this, &AMainPlayerController::Move);
	EnhancedInputComponent->BindAction(PlayerSprintAction, ETriggerEvent::Started, this, &AMainPlayerController::ToggleSprint);
	EnhancedInputComponent->BindAction(PlayerInteractAction, ETriggerEvent::Started, this, &AMainPlayerController::Interaction);
	EnhancedInputComponent->BindAction(PlayerDropWeaponAction, ETriggerEvent::Started, this, &AMainPlayerController::DropWeapon);
}

void AMainPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.X);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.Y);
	}
}

void AMainPlayerController::ToggleSprint()
{
	if (!MainPlayerRef) return;

	float CurrentSpeed = MainPlayerRef->GetVelocity().Size2D();
	
	if (CurrentSpeed >= 300.f)
	{
		if (MainPlayerRef->CharacterStates == ECharacterState::ECS_Moving)
		{
			MainPlayerRef->StartSprinting();
		}
		else if (MainPlayerRef->CharacterStates == ECharacterState::ECS_Sprinting)
		{
			MainPlayerRef->StopSprinting();
		}
	}
	
}

void AMainPlayerController::Interaction()
{
	DEBUG_MESSAGE(FColor::Green, TEXT("Interaction Button Pressed"));

	if (MainPlayerRef)
	{
		if (MainPlayerRef->OverlappingWeapon)
		{
			AWeaponBase* Weapon = MainPlayerRef->OverlappingWeapon;
			Weapon->Interact();
		}
	}
}

void AMainPlayerController::DropWeapon()
{
	if (MainPlayerRef && MainPlayerRef->GetEquippedWeapon())
	{
		DEBUG_MESSAGE(FColor::Black, TEXT("Drop current equipped weapon"));
		MainPlayerRef->GetEquippedWeapon()->DetachFromPlayer();
	}
	else
	{
		DEBUG_MESSAGE(FColor::Red, TEXT("No weapon equipped"));
	}
}