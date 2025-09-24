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
#include "InteractionReceiver.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReceivedInteractionSignature, UInteractionInstigatorComponent*, ByInstigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReceivedFocusSignature, int32, AsIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLostFocusSignature);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BDC_INTERACTIONBACKEND_API UInteractionReceiverComponent : public UActorComponent
{
	GENERATED_BODY()
	
private:

public:
	UInteractionReceiverComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDC|Interaction|Receiver")
	int32 SelfIndex = 0;

	UPROPERTY(BlueprintAssignable, Category = "BDC|Interaction|Receiver|Events")
	FOnReceivedInteractionSignature OnReceivedInteraction;

	UPROPERTY(BlueprintAssignable, Category = "BDC|Interaction|Receiver|Events")
	FOnReceivedFocusSignature OnReceivedFocus;

	UPROPERTY(BlueprintAssignable, Category = "BDC|Interaction|Receiver|Events")
	FOnLostFocusSignature OnLostFocus;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BDC|Interaction|Receiver|Value")
	FGameplayTag ReceiverMainTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BDC|Interaction|Receiver|Value")
	FGameplayTagContainer ReceiverTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BDC|Interaction|Receiver|Value")
	FName ReceiverIdentity;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};