/* Copyright © beginning at 2026 - BlackDevilCreations
 * Author: Patrick Wenzel
 * All rights reserved.
 * This file is part of a BlackDevilCreations project and may not be distributed, copied,
 * or modified without prior written permission from BlackDevilCreations.
 * Unreal Engine and its associated trademarks are property of Epic Games, Inc.
 * and are used with permission.
 */
using UnrealBuildTool;

public class BDC_InteractionBackend : ModuleRules
{
	public BDC_InteractionBackend(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PrivatePCHHeaderFile = "Public/BDC_InteractionBackend.h";
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Engine",
				"GameplayTags"
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine"
			}
		);
	}
}