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
#include "DrawDebugHelpers.h"

UInteractionInstigatorComponent::UInteractionInstigatorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UInteractionInstigatorComponent::BeginPlay()
{
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

void UInteractionInstigatorComponent::SetDebugDrawing(bool bEnable)
{
	bDrawDebug = bEnable;
	SetComponentTickEnabled(bDrawDebug);
}

void UInteractionInstigatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GameInstance = World->GetGameInstance())
		{
			if (UBDC_InteractionSubsystem* InteractionSubsystem = GameInstance->GetSubsystem<UBDC_InteractionSubsystem>())
			{
				if(InteractionSubsystem->HasInstigatorLocationOverride())
				{
					const FVector Location = InteractionSubsystem->GetInstigatorLocation();
					const float Rotation = InteractionSubsystem->GetInstigatorRotation();
					const float Range = InteractionSubsystem->GetInteractionRange();
					const float FallOffAngle = InteractionSubsystem->GetInteractionFallOff();

					const FVector Direction = FRotationMatrix(FRotator(0.f, Rotation, 0.f)).GetUnitAxis(EAxis::X);

					DrawDebugCone(World,Location,Direction,Range,FMath::DegreesToRadians(FallOffAngle),FMath::DegreesToRadians(FallOffAngle),12,FColor::Green,false,-1.f,0,1.f);
				}
			}
		}
	}
}