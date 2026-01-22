/* Copyright © beginning at 2026 - BlackDevilCreations
 * Author: Patrick Wenzel
 * All rights reserved.
 * This file is part of a BlackDevilCreations project and may not be distributed, copied,
 * or modified without prior written permission from BlackDevilCreations.
 * Unreal Engine and its associated trademarks are property of Epic Games, Inc.
 * and are used with permission.
 */
#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "BDC_InteractionSettings.generated.h"

UCLASS(Config=Game, DefaultConfig)
class BDC_INTERACTIONBACKEND_API UBDC_InteractionSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UBDC_InteractionSettings();

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Interaction")
	float InteractionRange;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Interaction", meta = (ClampMin = "1", ClampMax = "360"))
	float InteractionFoV;
	
public:
	#if WITH_EDITOR
		virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif
};