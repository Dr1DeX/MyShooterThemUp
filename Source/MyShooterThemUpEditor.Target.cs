// Shoot Them Up Game, All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MyShooterThemUpEditorTarget : TargetRules
{
	public MyShooterThemUpEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "MyShooterThemUp" } );
	}
}
