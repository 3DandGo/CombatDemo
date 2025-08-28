// 2025 3DAndGo

using UnrealBuildTool;
using System.Collections.Generic;

public class CombatGameEditorTarget : TargetRules
{
	public CombatGameEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "CombatGame" } );
	}
}
