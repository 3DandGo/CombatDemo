#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Still UMETA(DisplayName = "Idle"),
	ECS_Moving UMETA(DisplayName = "Moving"),
	ECS_Sprinting UMETA(DisplayName = "Sprinting")
};