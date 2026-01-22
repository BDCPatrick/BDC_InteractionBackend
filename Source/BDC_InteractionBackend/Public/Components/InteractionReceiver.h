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
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Components/SceneComponent.h"
#include "InteractionReceiver.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnReceivedInteraction, AActor*, OfInstigator, FName, OfInstigatorName, FGameplayTagContainer, OfInstigatedTags);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEntersInteractionField, AActor*, OfInstigator, FName, OfInstigatorName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLeavesInteractionField, AActor*, OfInstigator, FName, OfInstigatorName);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BDC_INTERACTIONBACKEND_API UInteractionReceiverComponent : public UActorComponent
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	USceneComponent* ReceiverComponent;

public:
	UInteractionReceiverComponent();
	
	UPROPERTY(BlueprintAssignable, Category = "BDC|Interaction|Dispatchers")
	FOnReceivedInteraction OnReceivedInteraction;
	UPROPERTY(BlueprintAssignable, Category = "BDC|Interaction|Dispatchers")
	FOnEntersInteractionField OnEntersInteractionField;
	UPROPERTY(BlueprintAssignable, Category = "BDC|Interaction|Dispatchers")
	FOnLeavesInteractionField OnLeavesInteractionField;
	
	UPROPERTY(BlueprintReadWrite, Editanywhere, Category = "BDC|Interaction|Receiver")
	FName NameOfInteractionComponent = FName("CapsuleComponent");
	UPROPERTY(BlueprintReadWrite, Editanywhere, Category = "BDC|Interaction|Receiver")
	FName NameOfReiceiver = FName("Steven");
	UPROPERTY(BlueprintReadWrite, Editanywhere, Category = "BDC|Interaction|Receiver")
	FGameplayTag TagOfReceiver = FGameplayTag();
	UPROPERTY(BlueprintReadWrite, Editanywhere, Category = "BDC|Interaction|Receiver")
	FGameplayTagContainer OnlyInteractOnTag = FGameplayTagContainer();
	UPROPERTY(BlueprintReadWrite, Editanywhere, Category = "BDC|Interaction|Receiver")
	bool bAlltagsHaveToBepresent = false;
	
	UFUNCTION(BlueprintCallable, Category="BDC|Interaction|Event")
	FTransform GetReceiverTransform();
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};