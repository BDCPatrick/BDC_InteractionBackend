/* Copyright © beginning at 2026 - BlackDevilCreations
 * Author: Patrick Wenzel
 * All rights reserved.
 * This file is part of a BlackDevilCreations project and may not be distributed, copied,
 * or modified without prior written permission from BlackDevilCreations.
 * Unreal Engine and its associated trademarks are property of Epic Games, Inc.
 * and are used with permission.
 */
#include "BDC_InteractionSettings.h"

UBDC_InteractionSettings::UBDC_InteractionSettings()
{
	CategoryName = TEXT("Plugins");
	SectionName = TEXT("BDC Interaction");
	
	InteractionRange = 200.0f;
	InteractionFoV = 60.0f;
}

#if WITH_EDITOR
void UBDC_InteractionSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property)
	{
		TryUpdateDefaultConfigFile();
	}
}
#endif