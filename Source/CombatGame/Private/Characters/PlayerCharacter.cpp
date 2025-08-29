// 2025 3DAndGo


#include "Characters/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Gameframework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Controllers/MainPlayerController.h"


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	/* Setting up player camera settings and viewport */
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	/* Creating player components */
	PlayerSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	PlayerSpringArm->SetupAttachment(RootComponent);
	
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	PlayerCamera->SetupAttachment(PlayerSpringArm);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	MainController = Cast<AMainPlayerController>(GetController());
	
}


void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetPlayerMovementState();
}

void APlayerCharacter::SetPlayerMovementState()
{
	float CurrentSpeed = GetVelocity().Size2D();
	
	// If current velocity is less than 300 set the player state to Idle
	if (CurrentSpeed < 300.f)
	{
		MovementState = ECharacterState::ECS_Still;

	}
	// Of if the current state is not set to sprinting, set it to moving and change the walk speed back to normal
	else
	{
		if (MovementState != ECharacterState::ECS_Sprinting)
		{
			MovementState = ECharacterState::ECS_Moving;
			SetMaxWalkSpeed(600.f);
		}
	}
}


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerCharacter::StartSprinting()
{
	SetMaxWalkSpeed(900.f);
	MovementState = ECharacterState::ECS_Sprinting;
}

void APlayerCharacter::StopSprinting()
{
	SetMaxWalkSpeed(600.f);
	MovementState = ECharacterState::ECS_Moving;
}
