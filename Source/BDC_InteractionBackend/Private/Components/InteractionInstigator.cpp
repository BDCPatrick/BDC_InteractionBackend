/* Copyright © beginning at 2026 - BlackDevilCreations
 * Author: Patrick Wenzel
 * All rights reserved.
 * This file is part of a BlackDevilCreations project and may not be distributed, copied,
 * or modified without prior written permission from BlackDevilCreations.
 * Unreal Engine and its associated trademarks are property of Epic Games, Inc.
 * and are used with permission.
 */
#include "Components/InteractionInstigator.h"
#include "BDC_InteractionSubsystem.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

UInteractionInstigatorComponent::UInteractionInstigatorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = true;
	SetIsReplicatedByDefault(true);
}

FTransform UInteractionInstigatorComponent::GetInstigatorTransform()
{
	if (InstigatorComponent)
	{
		return InstigatorComponent->GetComponentTransform();
	}

	if (const AActor* Owner = GetOwner())
	{
		return Owner->GetActorTransform();
	}

	return FTransform::Identity;
}

void UInteractionInstigatorComponent::BeginPlay()
{
	Super::BeginPlay();

	if (const AActor* Owner = GetOwner())
	{
		TArray<USceneComponent*> SceneComponents;
		Owner->GetComponents<USceneComponent>(SceneComponents);

		for (USceneComponent* Component : SceneComponents)
		{
			if (Component && Component->GetFName() == NameOfInteractionComponent)
			{
				InstigatorComponent = Component;
				break;
			}
		}
	}
	
	if (const UWorld* World = GetWorld())
	{
		if (const UGameInstance* GI = World->GetGameInstance())
		{
			if (UBDC_InteractionSubsystem* Subsystem = GI->GetSubsystem<UBDC_InteractionSubsystem>())
			{
				Subsystem->AddInstigator(this);
			}
		}
	}
}

void UInteractionInstigatorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (const UWorld* World = GetWorld())
	{
		if (const UGameInstance* GI = World->GetGameInstance())
		{
			if (UBDC_InteractionSubsystem* Subsystem = GI->GetSubsystem<UBDC_InteractionSubsystem>())
			{
				Subsystem->RemoveInstigator(this);
			}
		}
	}

	Super::EndPlay(EndPlayReason);
}