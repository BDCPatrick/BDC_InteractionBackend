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
#include "InteractionReceiver.h"
#include "BDC_InteractionSubsystem.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"

UInteractionReceiverComponent::UInteractionReceiverComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractionReceiverComponent::BeginPlay()
{
	Super::BeginPlay();

    if (UWorld* World = GetWorld())
    {
        if (UGameInstance* GameInstance = World->GetGameInstance())
        {
            if (UBDC_InteractionSubsystem* InteractionSubsystem = GameInstance->GetSubsystem<UBDC_InteractionSubsystem>())
            {
                InteractionSubsystem->AddReceiver(this);
            }
        }
    }
}

void UInteractionReceiverComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (UWorld* World = GetWorld())
    {
        if (UGameInstance* GameInstance = World->GetGameInstance())
        {
            if (UBDC_InteractionSubsystem* InteractionSubsystem = GameInstance->GetSubsystem<UBDC_InteractionSubsystem>())
            {
                InteractionSubsystem->RemoveReceiver(this);
            }
        }
    }

	Super::EndPlay(EndPlayReason);
}