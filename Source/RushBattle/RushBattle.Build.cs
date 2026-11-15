// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class RushBattle : ModuleRules
{
	public RushBattle(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore","GameplayAbilities", "GameplayTags", "GameplayTasks", "UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] { "NavigationSystem" });


	}
}
