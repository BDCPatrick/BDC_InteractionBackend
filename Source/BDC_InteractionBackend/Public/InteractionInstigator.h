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

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "BDC_InteractionSubsystem.h"

#include "InteractionInstigator.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHasTriggeredInteraction, UInteractionReceiverComponent*, AtReceiver);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BDC_INTERACTIONBACKEND_API UInteractionInstigatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractionInstigatorComponent();
	
	UPROPERTY(BlueprintAssignable, Category = "BDC|Interaction|Receiver|Events")
	FOnHasTriggeredInteraction OnTriggeredInteraction;

	UPROPERTY(BlueprintAssignable, Category = "BDC|Interaction|Receiver|Events")
	FOnFocusesUpdated OnFocusesUpdated;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BDC|Interaction|Instigator|Value")
	FGameplayTag InstigatorMainTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BDC|Interaction|Instigator|Value")
	FGameplayTagContainer InstigatorTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BDC|Interaction|Instigator|Value")
	FName InstigatorIdentity;

protected:
	virtual void BeginPlay() override;

};
