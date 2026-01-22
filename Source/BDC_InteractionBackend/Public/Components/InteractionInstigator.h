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
#include "BDC_InteractionSubsystem.h"

#include "InteractionInstigator.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BDC_INTERACTIONBACKEND_API UInteractionInstigatorComponent : public UActorComponent
{
		GENERATED_BODY()
	
private:

public:
	UInteractionInstigatorComponent();

	UPROPERTY(BlueprintReadWrite, Editanywhere, Category = "BDC|Interaction|Instigator")
	FName NameOfInstigator = FName("Nancy");
	UPROPERTY(BlueprintReadWrite, Editanywhere, Category = "BDC|Interaction|Instigator")
	FGameplayTag TagOfInstigator = FGameplayTag();
	UPROPERTY(BlueprintReadWrite, Editanywhere, Category = "BDC|Interaction|Instigator")
	FGameplayTagContainer InstigatingTags = FGameplayTagContainer();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};