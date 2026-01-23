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
#include "GameplayTagContainer.h"
#include "Components/InteractionReceiver.h"
#include "GameFramework/Actor.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BDC_InteractionSubsystem.generated.h"

class UInteractionInstigatorComponent;

USTRUCT(BlueprintType)
struct FInteractionReceivers
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "BDC|Interaction|Struct")
	AActor* InteractionActor = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "BDC|Interaction|Struct")
	UActorComponent* InteractionComponent = nullptr;

	bool operator==(const FInteractionReceivers& Other) const
	{
		return InteractionActor == Other.InteractionActor && InteractionComponent == Other.InteractionComponent;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFoundReceivers, const TArray<UInteractionReceiverComponent*>&, NewReceivers);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLostReceivers, const TArray<UInteractionReceiverComponent*>&, ReceiversGone);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionFired, UInteractionReceiverComponent*, OnReceivers);

UCLASS()
class BDC_INTERACTIONBACKEND_API UBDC_InteractionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	FTransform InstigatorTransform = FTransform();
	
	UPROPERTY()
	UInteractionInstigatorComponent* Instigator;

	UPROPERTY()
	TArray<UInteractionInstigatorComponent*> InstigatorsOfLevel;

	UPROPERTY()
	FInteractionReceivers LastInteractedWith;
	
	UPROPERTY()
	FInteractionReceivers CurrentBestFittingReceiver;
	
	UPROPERTY()
	TArray<UInteractionReceiverComponent*> ReceiversInField;
	
	UPROPERTY()
	TArray<FInteractionReceivers> ReceiversOfLevel;

public: 
	UPROPERTY(BlueprintAssignable, Category = "BDC|Interaction|Dispatchers|Subsystem")
	FOnFoundReceivers OnFoundReceivers;
	UPROPERTY(BlueprintAssignable, Category = "BDC|Interaction|Dispatchers|Subsystem")
	FOnLostReceivers OnLostReceivers;
	UPROPERTY(BlueprintAssignable, Category = "BDC|Interaction|Dispatchers|Subsystem")
	FOnInteractionFired OnInteractionFired;

	void SetInstigator(UInteractionInstigatorComponent* NewInstigator);
	void GetLastInteraction(FInteractionReceivers& LastReceiver);
	void InjectInteraction();
	void UpdateInteractions(FVector InstigatorLocation, FRotator InstigatorRotation);
	void GetAllReceiversField(TArray<UInteractionReceiverComponent*>& Receivers);
	void GetAllReceiversOfLevel(TArray<FInteractionReceivers>& Receivers);
	void GetReceiverByTag(FGameplayTag OfReceiverTag, FInteractionReceivers& ReceiverData);
	void GetReceiverByName(FName OfReceiverName, FInteractionReceivers& ReceiverData);
	void GetInstigatorByTag(FGameplayTag OfInstigatorTag, FInteractionReceivers& InstigatorData);
	void GetInstigatorByName(FName OfInstigatorName, FInteractionReceivers& InstigatorData);
	void AddReceiver(FInteractionReceivers NewReceiver);
	void RemoveReceiver(UInteractionReceiverComponent* ReceiverComponent);
	void AddInstigator(UInteractionInstigatorComponent* NewInstigator);
	void RemoveInstigator(UInteractionInstigatorComponent* InstigatorComponent);
};