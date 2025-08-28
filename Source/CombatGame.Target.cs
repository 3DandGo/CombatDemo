// 2025 3DAndGo

using UnrealBuildTool;
using System.Collections.Generic;

public class CombatGameTarget : TargetRules
{
	public CombatGameTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "CombatGame" } );
	}
}
