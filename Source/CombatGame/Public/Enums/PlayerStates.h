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
	EAS_UnequippedStance UMETA(DisplayName = "Unequipped Stance"), //When player doesn't have a weapon at all
	EAS_PassiveStance UMETA(DisplayName = "Passive Stance"), // When player has a weapon but it's unequipped
	EAS_AttackStance UMETA(DisplayName = "Attack Stance"), // When a weapon is equipped by the player
};