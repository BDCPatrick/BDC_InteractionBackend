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
	UPROPERTY()
	USceneComponent* InstigatorComponent;

public:
	UInteractionInstigatorComponent();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BDC|Interaction|Instigator")
	FName NameOfInteractionComponent = FName("CapsuleComponent");
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BDC|Interaction|Instigator")
	FName NameOfInstigator = FName("Nancy");
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BDC|Interaction|Instigator")
	FGameplayTag TagOfInstigator = FGameplayTag();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BDC|Interaction|Instigator")
	FGameplayTagContainer InstigatingTags = FGameplayTagContainer();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BDC|Interaction|Instigator")
	float InstigatorOffsetViewRotation = 0.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BDC|Interaction|Debug")
	bool bShowDebugging = false;
	
	UFUNCTION(BlueprintCallable, Category="BDC|Interaction|Event")
	FTransform GetInstigatorTransform() const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};