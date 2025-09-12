// 2025 3DAndGo


#include "Items/Weapons/WeaponBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Characters/PlayerCharacter.h"
#include "Debug/Debuggers.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	RootComponent = WeaponMesh;
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Overlapping Sphere"));
	OverlapSphere->SetupAttachment(GetRootComponent());

	WeaponHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	WeaponHitBox->SetupAttachment(GetRootComponent());
	WeaponHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnStartOverlap);
	OverlapSphere->OnComponentEndOverlap.AddDynamic(this, &AWeaponBase::OnEndOverlap);

	InitialRotation = GetActorRotation();

	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}



// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Player && Player->GetEquippedWeapon() != this)
	{
		WeaponSpin(DeltaTime);
		Hovering(DeltaTime);
	}
}

void AWeaponBase::Hovering(float DeltaTime)
{
	RunningTime += DeltaTime;
	FVector NewLocation = GetActorLocation();
	NewLocation.Z += FMath::Sin(RunningTime * FloatSpeed) * FloatAmplitude * DeltaTime;
	SetActorLocation(NewLocation);
}

void AWeaponBase::WeaponSpin(float DeltaTime)
{
	AddActorLocalRotation(FRotator(SpinSpeed * DeltaTime, 0.f, 0.f));
}



// Interaction Interface Function
void AWeaponBase::Interact()
{
	if (Player && Player->GetEquippedWeapon() == nullptr)
	{
		AttachToPlayer(Player->GetMesh(), FName("RightHandSocket"));

		DEBUG_MESSAGE(FColor::Blue, TEXT("Weapon is equipped"));
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OverlapSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Player->SetActionState(EActionState::EAS_EquippedStance);
		Player->SetEquippedWeapon(this);
	}
	else
	{
		DEBUG_MESSAGE(FColor::Red, TEXT("Weapon already equipped"));
	}

}

void AWeaponBase::OnStartOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	if (OtherActor == Player)
	{
		Player->SetOverlappingWeapon(this);
	}
}

void AWeaponBase::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex)
{
	if (OtherActor == Player)
	{
		Player->SetOverlappingWeapon(nullptr);
	}
}

void AWeaponBase::AttachToPlayer(USceneComponent* InParent, FName InSocketName)
{
	FAttachmentTransformRules TransformRules(
		EAttachmentRule::SnapToTarget, // Location
		EAttachmentRule::SnapToTarget, // Rotation
		EAttachmentRule::KeepWorld, // Scale
		true
	);

	AttachToComponent(InParent, TransformRules, InSocketName);
	SetActorScale3D(WeaponScale);

	
	switch (WeaponType)
	{
		
		case EWeaponType::EWT_OneHanded:
			Player->SetWeaponType(EWeaponType::EWT_OneHanded);
			break;

		case EWeaponType::EWT_TwoHanded:
			Player->SetWeaponType(EWeaponType::EWT_TwoHanded);
			break;

		default:
			Player->SetWeaponType(EWeaponType::EWT_NoWeapon);
			break;
	}
	
}

void AWeaponBase::DetachFromPlayer()
{
	if (Player->GetEquippedWeapon() == this)
	{
		FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, false);
		DetachFromActor(DetachmentRules);
		FVector NewLocation = Player->WeaponDropArea->GetComponentLocation();
		SetActorLocation(NewLocation);
		SetActorRotation(InitialRotation);
		Player->SetEquippedWeapon(nullptr);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		OverlapSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Player->SetActionState(EActionState::EAS_NoWeaponEquipped);
		Player->SetWeaponType(EWeaponType::EWT_NoWeapon);
	}
}