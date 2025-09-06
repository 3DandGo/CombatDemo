// 2025 3DAndGo


#include "Items/Weapons/WeaponBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Characters/PlayerCharacter.h"
#include "Debug/Debuggers.h"
#include "Components/SceneComponent.h"

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
}



// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	WeaponSpin(DeltaTime);

	Hovering(DeltaTime);
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
	if (Player)
	{
		AttachToPlayer(Player->GetMesh(), FName("RightHandSocket"));

		DEBUG_MESSAGE(FColor::Blue, TEXT("Weapon is equipped to the RightHandSocket"));
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OverlapSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

}

void AWeaponBase::OnStartOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	Player = Cast<APlayerCharacter>(OtherActor);
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
	Player = Cast<APlayerCharacter>(OtherActor);
	if (OtherActor == Player)
	{
		Player->SetOverlappingWeapon(nullptr);
	}
}

void AWeaponBase::AttachToPlayer(USceneComponent* InParent, FName InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(InParent, TransformRules, InSocketName);
}