// 2025 3DAndGo


#include "Characters/Enemies/BaseEnemy.h"


ABaseEnemy::ABaseEnemy()
{
 	
	PrimaryActorTick.bCanEverTick = true;

}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

