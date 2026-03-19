// 2025 3DAndGo


#include "Items/Weapons/WeaponBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Characters/Enemies/BaseEnemy.h"
#include "Characters/PlayerCharacter.h"
#include "Debug/Debuggers.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	RootComponent = WeaponMesh;
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);

	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Overlapping Sphere"));
	OverlapSphere->SetupAttachment(GetRootComponent());

	WeaponHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	WeaponHitBox->SetupAttachment(GetRootComponent());
	WeaponHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponHitBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponHitBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponHitBox->SetGenerateOverlapEvents(true);

	StartPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Start Point"));
	StartPoint->SetupAttachment(GetRootComponent());

	EndPoint = CreateDefaultSubobject<USceneComponent>(TEXT("End Point"));
	EndPoint->SetupAttachment(GetRootComponent());
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	/* TO PICK UP OR DROP WEAPON OVERLAPS */
	OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnStartOverlap);
	OverlapSphere->OnComponentEndOverlap.AddDynamic(this, &AWeaponBase::OnEndOverlap);

	WeaponHitBox->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnBoxOverlap);

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

void AWeaponBase::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FVector Start = StartPoint->GetComponentLocation();
	FVector End = EndPoint->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FHitResult HitBox;

	for (AActor* Actor : IgnoreHitActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		FVector(5.f, 5.f, 5.f),
		StartPoint->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitBox,
		true);

	IgnoreHitActors.AddUnique(HitBox.GetActor());
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
		OverlapSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Player->SetActionState(EActionState::EAS_NoWeaponEquipped);
		Player->SetWeaponType(EWeaponType::EWT_NoWeapon);
	}

}

void AWeaponBase::AttackColOn()
{
	if (WeaponHitBox)
	{ 
	WeaponHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void AWeaponBase::Interact_Implementation()
{
	IInteractionInterface::Interact_Implementation();
	
	if (Player && Player->GetEquippedWeapon() == nullptr)
	{
		AttachToPlayer(Player->GetMesh(), FName("RightHandSocket"));
		
		OverlapSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Player->SetActionState(EActionState::EAS_EquippedStance);
		Player->SetEquippedWeapon(this);
	}
	else
	{
		DEBUG_MESSAGE(FColor::Red, TEXT("Weapon already equipped"));
	}

}


void AWeaponBase::AttackColOff()
{
	if (WeaponHitBox)
	{
		WeaponHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		IgnoreHitActors.Empty();
	}
}
