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
	EAS_NoWeaponEquipped UMETA(DisplayName = "UnArmed"), //When player doesn't have a weapon at all
	EAS_UnEquippedStance UMETA(DisplayName = "UnEquipped"), // When player has a weapon but it's unequipped
	EAS_EquippedStance UMETA(DisplayName = "Equipped"), // When a weapon is equipped by the player
};