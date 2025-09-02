// 2025 3DAndGo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "WeaponBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class COMBATGAME_API AWeaponBase : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();
	virtual void Tick(float DeltaTime) override;

	virtual void Interact() override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* OverlapSphere;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* WeaponHitBox;

private:	
	

};
