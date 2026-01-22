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
#include "Modules/ModuleInterface.h"

class FBDC_InteractionBackendModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};