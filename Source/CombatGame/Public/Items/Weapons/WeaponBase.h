// 2025 3DAndGo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Enums/PlayerStates.h"
#include "WeaponBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UBoxComponent;
class APlayerCharacter;
class USceneComponent;

UCLASS()
class COMBATGAME_API AWeaponBase : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	AWeaponBase();
	virtual void Tick(float DeltaTime) override;

	void Hovering(float DeltaTime);

	void WeaponSpin(float DeltaTime);
	void AttachToPlayer(USceneComponent* InParent, FName InSocketName);
	void DetachFromPlayer();

	UFUNCTION(BlueprintCallable)
	void AttackColOff();

	UFUNCTION(BlueprintCallable)
	void AttackColOn();

	virtual void Interact() override;

	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enums")
	EWeaponType WeaponType = EWeaponType::EWT_NoWeapon;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* StartPoint;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* EndPoint;

	TArray<AActor*> IgnoreHitActors;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* OverlapSphere;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* WeaponHitBox;

	FRotator InitialRotation;

	APlayerCharacter* Player;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Attributes")
	FVector WeaponScale = FVector(1.0f);

private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float SpinSpeed = 45.f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float FloatAmplitude = 20.f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float FloatSpeed = 2.0f;

	float RunningTime = 0.0f;

public:
	FORCEINLINE void SetWeaponType(EWeaponType NewWeaponType) { WeaponType = NewWeaponType; }
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
};
