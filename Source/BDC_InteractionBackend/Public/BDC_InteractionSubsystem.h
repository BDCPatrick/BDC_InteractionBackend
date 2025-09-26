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
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/EngineTypes.h"
#include "BDC_InteractionSubsystem.generated.h"

class UInteractionInstigatorComponent;
class UInteractionReceiverComponent;
class AActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFocusesUpdated, bool, HasAnyFocus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTriggeredInteraction);

UCLASS()
class BDC_INTERACTIONBACKEND_API UBDC_InteractionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY()
	float InteractRotation = -1.0f;
	
	UPROPERTY()
	float InteractRange = 100.0f;
	
	UPROPERTY()
	float InteractFallOff = 60.0f;
	
	UPROPERTY(EditAnywhere, Category="BDC|Interaction|Subsystem|Setup")
	float RotationYawOffsetDegrees = 0.0f;
	
	UPROPERTY()
	FVector InstigatorLocationOverride = FVector::ZeroVector;
	UPROPERTY()
	bool bHasInstigatorLocationOverride = false;
	
	UPROPERTY()
	int32 HoldIndex = 0;
	UPROPERTY()
	UInteractionInstigatorComponent* InstigatorHold;
	UPROPERTY()
	UInteractionReceiverComponent* RecentReceiver;
	UPROPERTY()
	TArray<UInteractionReceiverComponent*> ReceiversHold;
	UPROPERTY()
	TArray<UInteractionReceiverComponent*> ReceiversFittings;
	UPROPERTY()
	TArray<UInteractionReceiverComponent*> ReceiversDifference;

	UPROPERTY(Transient)
	bool bDeferredFocusUpdateRequested = false;
	
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
	FDelegateHandle WorldTickHandle;
	void DebugTick(UWorld* World);

	/**
	 * Triggered when the focus states of elements are updated.
	 * This event is typically invoked to signal that the focus status has changed,
	 * allowing dependent systems or components to react to the new focus configuration.
	 *
	 * It can be used to handle updates like UI focus shifts, input redirection, or visual indicators.
	 */
	UPROPERTY(BlueprintAssignable, Category = "BDC|Interaction|Subsystem|Events")
	FOnFocusesUpdated OnFocusesUpdated;

	/**
	 * Triggers and manages interactions when a specified condition or event occurs.
	 * This is used to handle and encapsulate logic related to activated interactions.
	 */
	UPROPERTY(BlueprintAssignable, Category = "BDC|Interaction|Subsystem|Events")
	FOnTriggeredInteraction OnTriggeredInteraction;


	/**
	 * Moves an object in a 2D space towards a specified direction.
	 * This method is used to update the position based on the given direction vector.
	 *
	 * @param Direction A vector representing the direction of movement in the 2D space.
	 * @return The updated position after the movement.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BDC|Interaction|Subsystem|Math", DisplayName="2DMoveToDir")
	float Move2D_ToDir(const FVector2D& MovementInput, int32 NumberOfDirections = 0) const;

	/**
	 * Sets the actor instigator responsible for initiating an action or event.
	 * This can be used to track which actor caused a specific interaction or behavior.
	 *
	 * @param InstigatorActor The actor that is initiating the action or event.
	 */
	void SetInstigator(UInteractionInstigatorComponent* NewInstigator);

	/**
	 * Sets the instigator's rotation to a specified orientation.
	 * This can be used to update the instigator's facing direction within the game world.
	 *
	 * @param NewRotation The desired rotation to be applied to the instigator
	 */
	UFUNCTION(BlueprintCallable, Category="BDC|Interaction|Subsystem")
	void SetInstigatorRotation(float NewRotation);

	/**
	 * Sets the location of the instigator, typically representing the source or origin of an action or event.
	 * This method defines the specific position to associate with the instigator.
	 *
	 * @param NewLocation The new location to assign to the instigator.
	 */
	UFUNCTION(BlueprintCallable, Category="BDC|Interaction|Subsystem")
	void SetInstigatorLocation(const FVector& NewWorldLocation);

	/**
	 * Clears any previously set instigator location override, resetting it to its default behavior.
	 * This ensures that the instigator's location is no longer overridden during relevant calculations.
	 */
	UFUNCTION(BlueprintCallable, Category="BDC|Interaction|Subsystem")
	void ClearInstigatorLocationOverride();

	/**
	 * Requests an update for focus management to occur after the current tick.
	 * This ensures that any focus-related changes are processed at the appropriate time.
	 */
	UFUNCTION(BlueprintCallable, Category="BDC|Interaction|Subsystem|Focus")
	void RequestFocusUpdatePostTick();

	/**
	 * Adds a receiver to handle specific events or messages.
	 * This allows the system to send notifications or data to the designated receiver.
	 *
	 * @param Receiver The object or component intended to receive the events or messages.
	 */
	void AddReceiver(UInteractionReceiverComponent* NewReceiver);

	/**
	 * Removes the specified receiver from the current configuration.
	 * This is used to detach or unregister a receiver from the system.
	 *
	 * @param Receiver The receiver object to be removed.
	 */
	void RemoveReceiver(UInteractionReceiverComponent* ReceiverToRemove);

	/**
	 * Sets the maximum range within which interactions can be initiated.
	 * This determines how far an object or entity can be to allow interaction.
	 *
	 * @param Range The interaction range to set, specified in units.
	 */
	UFUNCTION(BlueprintCallable, meta=(ClampMin="0"), Category="BDC|Interaction|Subsystem|Setup")
	void SetInteractionRange(float NewRange = 100.0f);

	/**
	 * Sets the falloff distance for interaction effects, controlling the range at which interaction influence diminishes.
	 *
	 * @param FallOffDistance The distance beyond which interaction effects decrease or cease.
	 */
	UFUNCTION(BlueprintCallable, meta=(ClampMin="0", ClampMax="360"), Category="BDC|Interaction|Subsystem|Setup")
	void SetInteractionFallOff(float NewAngle = 15.0f);

	/**
	 * Sets the yaw offset for rotation to align the interaction reference orientation.
	 * This can be used to adjust the reference direction (e.g., aligning 0° to a specific axis).
	 *
	 * @param NewOffsetDegrees The new yaw offset in degrees to apply to the interaction system's rotation reference.
	 */
	UFUNCTION(BlueprintCallable, Category="BDC|Interaction|Subsystem|Setup")
	void SetRotationYawOffset(float NewOffsetDegrees) { RotationYawOffsetDegrees = NewOffsetDegrees; }

	/**
	 * Retrieves the current yaw offset for rotation that aligns the interaction reference orientation.
	 *
	 * @return The current yaw offset in degrees.
	 */
	UFUNCTION(BlueprintPure, Category="BDC|Interaction|Subsystem|Setup")
	float GetRotationYawOffset() const { return RotationYawOffsetDegrees; }

	/**
	 * Retrieves the range within which interactions are possible.
	 * This range typically defines the maximum distance an entity can interact with an object.
	 *
	 * @return The interaction range as a floating-point value.
	 */
	UFUNCTION(BlueprintPure, Category="BDC|Interaction|Subsystem|Setup")
	float GetInteractionRange() const { return InteractRange; }

	/**
	 * Retrieves the falloff distance for the interaction, which determines how interaction strength decreases with distance.
	 *
	 * @return The falloff distance value for the interaction.
	 */
	UFUNCTION(BlueprintPure, Category="BDC|Interaction|Subsystem|Setup")
	float GetInteractionFallOff() const { return InteractFallOff; }

	/**
	 * Checks whether there is a location override for the instigator.
	 *
	 * @return True if a location override exists for the instigator, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category="BDC|Interaction|Subsystem|State")
	bool HasInstigatorLocationOverride() const { return bHasInstigatorLocationOverride; }

	/**
	 * Retrieves the location of the actor responsible for initiating the action or event.
	 *
	 * @return The location of the instigator as a vector. If no instigator is present, the result may be undefined.
	 */
	UFUNCTION(BlueprintPure, Category="BDC|Interaction|Subsystem|State")
	FVector GetInstigatorLocation() const { return InstigatorLocationOverride; }

	/**
	 * Retrieves the current rotation of the interaction instigator.
	 *
	 * @return The rotation of the interaction instigator as a float value.
	 */
	UFUNCTION(BlueprintPure, Category="BDC|Interaction|Subsystem|State")
	float GetInstigatorRotation() const { return InteractRotation; }

	/**
	 * Retrieves the most recent receiver details or object.
	 *
	 * @param receiverCount The maximum number of recent receivers to consider.
	 */
	UFUNCTION(BlueprintPure, Category="BDC|Interaction|Subsystem")
	void GetRecentReceiver(UInteractionReceiverComponent*& ReceiverComponent, AActor*& ReceiverActor, bool& bIsvalid);

	/**
	 * Retrieves the list of receivers that are suitable or compatible based on specific criteria.
	 *
	 * @param Criteria The conditions or parameters used to evaluate and filter the compatible receivers.
	 */
	UFUNCTION(BlueprintPure, Category="BDC|Interaction|Subsystem")
	void GetFittingReceivers(TArray<UInteractionReceiverComponent*>& ReceiverComponent, TArray<AActor*>& ReceiverActor, bool& bIsvalid);

	/**
	 * Determines the best-fitting interaction for a given trigger event based on the provided conditions.
	 * This method evaluates all possible interactions and selects the one that best matches the specified criteria.
	 *
	 * @param TriggerEvent The event that initiated the trigger, containing contextual data.
	 * @param Conditions A set of conditions used to assess the suitability of potential interactions.
	 * @return The best-fitting interaction that meets the conditions, or null if no suitable interaction is found.
	 */
	UFUNCTION(BlueprintCallable, Category="BDC|Interaction|Subsystem")
	void TriggerInteractionBestFitting();

	/**
	 * Triggers an interaction event while holding down a specified input.
	 * This method provides functionality to activate and maintain an interaction sequence until the input is released.
	 *
	 * @param HoldDuration The duration for which the interaction should be maintained while the input is held.
	 */
	UFUNCTION(BlueprintCallable, Category="BDC|Interaction|Subsystem")
	void TriggerInteractionOnHold();

	/**
	 * Triggers an interaction event based on a specified index.
	 * This is used to activate or perform an interaction tied to the given index.
	 *
	 * @param Index The index of the interaction to trigger.
	 */
	UFUNCTION(BlueprintCallable, Category="BDC|Interaction|Subsystem")
	void TriggerInteractionOnIndex(const int32& OnIndex);

	/**
	 * Sets the receiver index that will be held for operations or interactions.
	 * This specifies which receiver should be targeted or activated.
	 *
	 * @param ReceiverIndex The index of the receiver to hold.
	 */
	UFUNCTION(BlueprintCallable, meta=(ClampMin="0"), Category="BDC|Interaction|Subsystem")
	void SetHoldReceiverIndex(int32 NewIndex);

	/**
	 * Retrieves the index of the hold receiver associated with this object.
	 * This index can be used to look up or manage hold-related operations.
	 *
	 * @param None
	 */
	UFUNCTION(BlueprintPure, Category="BDC|Interaction|Subsystem")
	void GetHoldReceiverIndex(int32& CurrentIndex);

	/**
	 * Updates the current interaction focuses based on the latest state or context.
	 * This function ensures that interaction focuses are correctly recalculated
	 * or adjusted as needed by the system.
	 */
	UFUNCTION(BlueprintCallable, Category="BDC|Interaction|Subsystem")
	void UpdateInteractionFocuses();

	/**
	 * Updates the interaction fit parameters based on the current state.
	 * This ensures that the interaction configuration is recalculated
	 * to match the desired alignment or operational constraints.
	 */
	UFUNCTION(BlueprintCallable, Category="BDC|Interaction|Subsystem")
	void UpdateInteractionFits();
};
