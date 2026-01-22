/* Copyright © beginning at 2026 - BlackDevilCreations
 * Author: Patrick Wenzel
 * All rights reserved.
 * This file is part of a BlackDevilCreations project and may not be distributed, copied,
 * or modified without prior written permission from BlackDevilCreations.
 * Unreal Engine and its associated trademarks are property of Epic Games, Inc.
 * and are used with permission.
 */
#include "Components/InteractionReceiver.h"
#include "BDC_InteractionSubsystem.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

UInteractionReceiverComponent::UInteractionReceiverComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = true;
	SetIsReplicatedByDefault(true);
}

FTransform UInteractionReceiverComponent::GetReceiverTransform()
{
	if (ReceiverComponent)
	{
		return ReceiverComponent->GetComponentTransform();
	}

	if (const AActor* Owner = GetOwner())
	{
		return Owner->GetActorTransform();
	}

	return FTransform::Identity;
}

void UInteractionReceiverComponent::BeginPlay()
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
				ReceiverComponent = Component;
				break;
			}
		}
	}

	if (UBDC_InteractionSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UBDC_InteractionSubsystem>())
	{
		FInteractionReceivers NewReceiver;
		NewReceiver.InteractionActor = GetOwner();
		NewReceiver.InteractionComponent = this;
		Subsystem->AddReceiver(NewReceiver);
	}
}

void UInteractionReceiverComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UBDC_InteractionSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UBDC_InteractionSubsystem>())
	{
		Subsystem->RemoveReceiver(this);
	}

	Super::EndPlay(EndPlayReason);
}