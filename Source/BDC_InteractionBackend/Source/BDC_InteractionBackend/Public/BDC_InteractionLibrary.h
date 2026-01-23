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
#include "Components/InteractionInstigator.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BDC_InteractionLibrary.generated.h"


UCLASS()
class BDC_INTERACTIONBACKEND_API UBDC_InteractionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "BDC|Interaction|Library", meta = (WorldContext = "WorldContextObject"))
	static void SetInstigator(const UObject* WorldContextObject, UInteractionInstigatorComponent* NewInstigator);

	UFUNCTION(BlueprintCallable, Category = "BDC|Interaction|Library", meta = (WorldContext = "WorldContextObject"))
	static void GetLastInteraction(const UObject* WorldContextObject, FInteractionReceivers& LastReceiver);

	UFUNCTION(BlueprintCallable, Category = "BDC|Interaction|Library", meta = (WorldContext = "WorldContextObject"))
	static void InjectInteraction(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "BDC|Interaction|Library", meta = (WorldContext = "WorldContextObject"))
	static void UpdateInteractions(const UObject* WorldContextObject, FVector InstigatorLocation, FRotator InstigatorRotation);

	UFUNCTION(BlueprintCallable, Category = "BDC|Interaction|Library", meta = (WorldContext = "WorldContextObject"))
	static void GetAllReceiversField(const UObject* WorldContextObject, TArray<UInteractionReceiverComponent*>& Receivers);

	UFUNCTION(BlueprintCallable, Category = "BDC|Interaction|Library", meta = (WorldContext = "WorldContextObject"))
	static void GetAllReceiversOfLevel(const UObject* WorldContextObject, TArray<FInteractionReceivers>& Receivers);

	UFUNCTION(BlueprintCallable, Category = "BDC|Interaction|Library", meta = (WorldContext = "WorldContextObject"))
	static void GetReceiverByTag(const UObject* WorldContextObject, FGameplayTag OfReceiverTag, FInteractionReceivers& Receiver);

	UFUNCTION(BlueprintCallable, Category = "BDC|Interaction|Library", meta = (WorldContext = "WorldContextObject"))
	static void GetReceiverByName(const UObject* WorldContextObject, FName OfReceiverName, FInteractionReceivers& Receiver);

	UFUNCTION(BlueprintCallable, Category = "BDC|Interaction|Library", meta = (WorldContext = "WorldContextObject"))
	static void GetInstigatorByTag(const UObject* WorldContextObject, FGameplayTag OfInstigatorTag, FInteractionReceivers& Instigator);

	UFUNCTION(BlueprintCallable, Category = "BDC|Interaction|Library", meta = (WorldContext = "WorldContextObject"))
	static void GetInstigatorByName(const UObject* WorldContextObject, FName OfInstigatorName, FInteractionReceivers& Instigator);
};