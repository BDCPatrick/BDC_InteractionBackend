/* Copyright © beginning at 2025 - BlackDevilCreations
 * Author: Patrick Wenzel
 * All rights reserved.
 *
 * This file is part of a BlackDevilCreations project and may not be distributed, copied,
 * or modified without prior written permission from BlackDevilCreations.
 *
 * Unreal Engine and its associated trademarks are property of Epic Games, Inc.
 * and are used with permission.
 */
#include "InteractionInstigator.h"
#include "BDC_InteractionSubsystem.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"


UInteractionInstigatorComponent::UInteractionInstigatorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractionInstigatorComponent::BeginPlay()
{
    bAutoActivate = true;
	Super::BeginPlay();

    if (UWorld* World = GetWorld())
    {
        if (UGameInstance* GameInstance = World->GetGameInstance())
        {
            if (UBDC_InteractionSubsystem* InteractionSubsystem = GameInstance->GetSubsystem<UBDC_InteractionSubsystem>())
            {
                InteractionSubsystem->SetInstigator(this);
            }
        }
    }
}