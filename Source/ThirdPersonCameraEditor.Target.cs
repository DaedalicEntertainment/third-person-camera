using UnrealBuildTool;
using System.Collections.Generic;

public class ThirdPersonCameraEditorTarget : TargetRules
{
	public ThirdPersonCameraEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.Add("ThirdPersonCamera");
	}
}
