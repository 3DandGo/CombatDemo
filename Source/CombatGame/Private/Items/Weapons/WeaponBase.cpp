// 2025 3DAndGo


#include "Items/Weapons/WeaponBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

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
	
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Interaction Interface Function
void AWeaponBase::Interact()
{

}

