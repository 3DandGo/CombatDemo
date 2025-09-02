// 2025 3DAndGo


#include "Items/Weapons/WeaponBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Debug/Debuggers.h"

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

}

// Interaction Interface Function
void AWeaponBase::Interact()
{

}

void AWeaponBase::OnStartOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	DEBUG_MESSAGE(FColor::Green, TEXT("Overlapped with something"));
	
}

void AWeaponBase::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex)
{
	DEBUG_MESSAGE(FColor::Red, TEXT("Finished Overlapping with something"));
}

