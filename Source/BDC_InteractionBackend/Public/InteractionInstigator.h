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

	/**
	 * A multicast delegate that is triggered when an interaction is initiated.
	 *
	 * This event is broadcasted by an interaction instigator to notify listeners
	 * that an interaction has occurred. It can be bound to in Blueprints or code
	 * to execute custom behavior when an interaction is triggered.
	 *
	 * Category:
	 * - BDC|Interaction|Receiver|Events
	 */
	UPROPERTY(BlueprintAssignable, Category = "BDC|Interaction|Receiver|Events")
	FOnHasTriggeredInteraction OnTriggeredInteraction;

	/**
	 * A BlueprintAssignable event that triggers whenever there is an update to the interaction focuses.
	 * This event is designed to notify subscribers when there are changes in the interaction state
	 * between the instigator and one or more interaction receivers. It broadcasts upon any new focus
	 * being received or lost by the relevant receivers.
	 *
	 * Category: BDC|Interaction|Receiver|Events
	 */
	UPROPERTY(BlueprintAssignable, Category = "BDC|Interaction|Receiver|Events")
	FOnFocusesUpdated OnFocusesUpdated;

	/**
	 * Represents the primary tag associated with an instigator in the interaction system.
	 * This tag is used to identify and categorize the instigator's main role or characteristic
	 * in gameplay mechanics, like interactions within the BDC framework.
	 *
	 * EditAnywhere: Allows configuration in the editor.
	 * BlueprintReadOnly: Accessible in Blueprints for reference but cannot be modified.
	 * Category: Organized under "BDC|Interaction|Instigator|Value" in the editor.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BDC|Interaction|Instigator|Value")
	FGameplayTag InstigatorMainTag;

	/**
	 * A container of gameplay tags associated with the instigator. These tags can be used to identify or classify the
	 * instigator's qualities, behaviors, or attributes within the context of an interaction system.
	 *
	 * This property is editable in the editor and readable in Blueprints, enabling the setup and usage of tags for
	 * interaction logic at design time or runtime. It belongs to the "BDC|Interaction|Instigator|Value" category for
	 * organizational purposes in the editor.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BDC|Interaction|Instigator|Value")
	FGameplayTagContainer InstigatorTags;

	/**
	 * Represents the identity of an instigator in the interaction system.
	 *
	 * This property is used to uniquely identify the instigator involved in
	 * interaction logic within the system. It is an editable property exposed
	 * to Blueprints for easier configuration and debugging purposes.
	 *
	 * Category:
	 * - BDC|Interaction|Instigator|Value
	 *
	 * Markup:
	 * - EditAnywhere: Allows editing in the editor.
	 * - BlueprintReadOnly: Can be read in Blueprints but not modified.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BDC|Interaction|Instigator|Value")
	FName InstigatorIdentity;

	/**
	 * Determines whether debug visuals should be drawn for the interaction instigator.
	 *
	 * If set to true, debug information such as visual cones or debug lines relevant to the
	 * interaction instigator component will be rendered during gameplay.
	 * Useful for debugging interaction systems and verifying instigator behavior.
	 *
	 * Blueprint and editor accessible:
	 * - Can be configured in the editor or via Blueprints.
	 * - Exposed on spawn, allowing initialization during the component's creation.
	 *
	 * Default value: false
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDC|Interaction|Instigator|Debug", meta=(ExposeOnSpawn="true"))
	bool bDrawDebug = false;

	/**
	 * Enables or disables debug drawing for the interaction instigator component.
	 *
	 * @param bEnable A boolean indicating whether to enable (true) or disable (false) debug drawing.
	 */
	UFUNCTION(BlueprintCallable, Category = "BDC|Interaction|Instigator|Debug")
	void SetDebugDrawing(bool bEnable);

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};