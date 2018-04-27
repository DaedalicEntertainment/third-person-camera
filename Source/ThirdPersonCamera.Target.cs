using UnrealBuildTool;
using System.Collections.Generic;

public class ThirdPersonCameraTarget : TargetRules
{
	public ThirdPersonCameraTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.Add("ThirdPersonCamera");
	}
}
