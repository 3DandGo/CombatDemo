// 2025 3DAndGo


#include "Characters/PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"
#include "Gameframework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Controllers/MainPlayerController.h"
#include "Components/SceneComponent.h"
#include "Animation/AnimMontage.h"
#include "DrawDebugHelpers.h"
#include "Debug/Debuggers.h"
#include "Items/Weapons/WeaponBase.h"


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

	WeaponDropArea = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Drop Location"));
	WeaponDropArea->SetupAttachment(RootComponent);

	/* Setting default values for variables */
	CharacterStates = ECharacterState::ECS_Still;
	ActionStates = EActionState::EAS_NoWeaponEquipped;
	CombatStates = ECombatState::CAS_NotAttacking;
	WeaponType = EWeaponType::EWT_NoWeapon;
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
		CharacterStates = ECharacterState::ECS_Still;

	}
	// Of if the current state is not set to sprinting, set it to moving and change the walk speed back to normal
	else
	{
		if (CharacterStates != ECharacterState::ECS_Sprinting)
		{
			CharacterStates = ECharacterState::ECS_Moving;
			SetMaxWalkSpeed(600.f);
		}
	}
}


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerCharacter::Interact_Implementation()
{
	IInteractionInterface::Interact_Implementation();
	
	UE_LOG(LogTemp, Warning, TEXT("[PLAYER] Player interacted Called"));
	
	// FORWARD TRACE WITH SPHERE TO CHECK 
	FVector StartLocation = GetActorLocation();
	FVector ForwardLocation = GetActorForwardVector();
	FVector EndLocation = StartLocation + (ForwardLocation * TraceDistance);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bIgnoreTouches = true;
	
	TArray<FHitResult> Hits;
	
	bool bIsHit = GetWorld()->SweepMultiByChannel(
		Hits,
		StartLocation,
		EndLocation,
		FQuat::Identity,
		ECC_WorldDynamic,
		FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight)
		);
	
	FColor HitColor = bIsHit ? FColor::Magenta : FColor::White;
	
	
	
	if (bIsHit)
	{
		for (const FHitResult& HitActor : Hits)
		{
			AActor* Actor = HitActor.GetActor();
			if (AWeaponBase* Weapon = Cast<AWeaponBase>(Actor))
			{
				if (OverlappedActors.Contains(Weapon))
				{
					UE_LOG(LogTemp, Warning, TEXT("Weapon already overlapped - ignore"));
				}
				else
				{
					OverlappedActors.AddUnique(Weapon);
					UE_LOG(LogTemp, Warning, TEXT("Hit Weapon: %s"), *Weapon->GetName());
				}
			}
		}
		OverlappedActors.Empty();
	}
	
	FVector Center = (StartLocation + EndLocation) * 0.5f;
	float HalfHeight = FVector::Distance(StartLocation, EndLocation) * 0.5f;
	float Radius = CapsuleRadius;
	FVector Direction = (EndLocation - StartLocation).GetSafeNormal();
	FQuat CapsuleRotation = FRotationMatrix::MakeFromZ(Direction).ToQuat();
	
	DrawDebugCapsule(GetWorld(), Center, HalfHeight, Radius, CapsuleRotation, HitColor, true, 3.f);
	
}

void APlayerCharacter::StartSprinting()
{
	SetMaxWalkSpeed(900.f);
	CharacterStates = ECharacterState::ECS_Sprinting;
}

void APlayerCharacter::StopSprinting()
{
	SetMaxWalkSpeed(600.f);
	CharacterStates = ECharacterState::ECS_Moving;
}

void APlayerCharacter::PlayLightAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && LightAttackMontage)
	{
		AnimInstance->Montage_Play(LightAttackMontage);
		CombatStates = ECombatState::CAS_Attacking;
	}
}

void APlayerCharacter::PlayHeavyAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HeavyAttackMontage)
	{
		AnimInstance->Montage_Play(HeavyAttackMontage);
		CombatStates = ECombatState::CAS_Attacking;
	}
}

void APlayerCharacter::AttackEnded()
{
	CombatStates = ECombatState::CAS_NotAttacking;
}