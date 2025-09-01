#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Still UMETA(DisplayName = "Idle"),
	ECS_Moving UMETA(DisplayName = "Moving"),
	ECS_Sprinting UMETA(DisplayName = "Sprinting")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_UnequippedStance UMETA(DisplayName = "Unequipped Stance"),
	EAS_PassiveStance UMETA(DisplayName = "Passive Stance"),
	EAS_AttackStance UMETA(DisplayName = "Attack Stance"),
};