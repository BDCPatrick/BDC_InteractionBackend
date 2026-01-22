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
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BDC_InteractionLibrary.generated.h"


UCLASS()
class BDC_INTERACTIONBACKEND_API UBDC_InteractionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	//TODO: The Library needs the functions equivalent to the publics in UBDC_InteractionSubsystem, and calling the UBDC_InteractionSubsystem functions, except AddReceiver.
};