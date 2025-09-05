// 2025 3DAndGo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "WeaponBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UBoxComponent;
class APlayerCharacter;

UCLASS()
class COMBATGAME_API AWeaponBase : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	AWeaponBase();
	virtual void Tick(float DeltaTime) override;

	void WeaponSpin(float DeltaTime);

	virtual void Interact() override;

	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* WeaponMesh;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* OverlapSphere;


	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* WeaponHitBox;

	APlayerCharacter* Player;

private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float SpinSpeed = 45.f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float FloatAmplitude = 20.f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float FloatSpeed = 2.0f;

	float RunningTime = 0.0f;
};
