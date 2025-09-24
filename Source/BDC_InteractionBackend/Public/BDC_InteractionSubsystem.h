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
	float InteractFallOff = 30.0f;
	
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
	
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
	
	UPROPERTY(BlueprintAssignable, Category = "BDC|Interaction|Subsystem|Events")
	FOnFocusesUpdated OnFocusesUpdated;
	
	UPROPERTY(BlueprintAssignable, Category = "BDC|Interaction|Subsystem|Events")
	FOnTriggeredInteraction OnTriggeredInteraction;


	/**
	 * Converts a 2D movement input vector into a quantized directional angle in degrees based on the specified number of directions.
	 *
	 * @param MovementInput A 2D vector representing the input movement direction.
	 * @param NumberOfDirections The number of discrete directions to quantize the angle into.
	 *        If set to 0 or less, the method will return the exact direction angle in degrees.
	 *
	 * @return The calculated direction in degrees, either quantized to the nearest sector defined by
	 *         the number of directions or the exact angle if no quantization is specified.
	 *         Returns 0.0f if the input vector is approximately zero (no movement).
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BDC|Interaction|Subsystem|Math", DisplayName="2DMoveToDir")
	float Move2D_ToDir(const FVector2D& MovementInput, int32 NumberOfDirections = 0) const;

	/**
	 * Sets the current instigator for the interaction subsystem.
	 *
	 * @param NewInstigator The interaction instigator component to set as the current instigator.
	 */
	void SetInstigator(UInteractionInstigatorComponent* NewInstigator);

	/**
	 * Sets the rotation of the instigator in the interaction subsystem.
	 *
	 * @param NewRotation The new desired rotation value to be assigned for the instigator.
	 */
	UFUNCTION(BlueprintCallable, Category="BDC|Interaction|Subsystem")
	void SetInstigatorRotation(float NewRotation);

	/**
	 * Updates the instigator's world location and marks the instigator location override as valid.
	 *
	 * @param NewWorldLocation The new world position to set for the instigator.
	 */
	UFUNCTION(BlueprintCallable, Category="BDC|Interaction|Subsystem")
	void SetInstigatorLocation(const FVector& NewWorldLocation);

	/**
	 * Resets the instigator location override to its default state.
	 *
	 * This function clears any previously set location override for the instigator
	 * within the interaction subsystem. Specifically, it sets the `bHasInstigatorLocationOverride`
	 * flag to `false` and resets the `InstigatorLocationOverride` vector to `FVector::ZeroVector`.
	 *
	 * This is used to remove any custom-defined override and restore the default behavior
	 * of determining the instigator location dynamically.
	 */
	UFUNCTION(BlueprintCallable, Category="BDC|Interaction|Subsystem")
	void ClearInstigatorLocationOverride();

	/**
	 * Adds a new receiver component to the interaction subsystem.
	 *
	 * This function ensures the provided receiver is valid and adds it to the `ReceiversHold` list
	 * while removing any invalid entries from the list. If the receiver is already present, it will
	 * not be added again.
	 *
	 * @param NewReceiver Pointer to the UInteractionReceiverComponent instance to be added.
	 */
	void AddReceiver(UInteractionReceiverComponent* NewReceiver);

	/**
	 * Removes the specified interaction receiver from the subsystem's management.
	 *
	 * This function removes the given receiver component from multiple internal arrays
	 * that track active receivers, receiver fittings, and receiver differences. If the
	 * specified receiver is the most recently interacted receiver, the subsystem sets
	 * the recently interacted receiver to null.
	 *
	 * @param ReceiverToRemove The interaction receiver component to remove. If null, this function does nothing.
	 */
	void RemoveReceiver(UInteractionReceiverComponent* ReceiverToRemove);

	/**
	 * Sets the interaction range for the interaction subsystem.
	 *
	 * @param NewRange The new range value to be set for interaction. Must be a non-negative value.
	 */
	UFUNCTION(BlueprintCallable, meta=(ClampMin="0"), Category="BDC|Interaction|Subsystem|Setup")
	void SetInteractionRange(float NewRange = 100.0f);

	/**
	 * Updates the interaction fall-off angle for the interaction subsystem.
	 *
	 * This function adjusts the value of the fall-off angle, which determines the angle
	 * within which interactions are considered valid. It is used to modify the interaction
	 * system's parameters based on gameplay needs.
	 *
	 * @param NewAngle The new fall-off angle value to set. It must be within the range defined in the associated metadata.
	 */
	UFUNCTION(BlueprintCallable, meta=(ClampMin="0", ClampMax="360"), Category="BDC|Interaction|Subsystem|Setup")
	void SetInteractionFallOff(float NewAngle = 15.0f);

	/**
	 * Retrieves the most recent receiver involved in the interaction subsystem.
	 */
	UFUNCTION(BlueprintPure, Category="BDC|Interaction|Subsystem")
	void GetRecentReceiver(UInteractionReceiverComponent*& ReceiverComponent, AActor*& ReceiverActor, bool& bIsvalid);

	/**
	 * Retrieves a collection of receivers that are deemed suitable based on predefined criteria.
	 */
	UFUNCTION(BlueprintPure, Category="BDC|Interaction|Subsystem")
	void GetFittingReceivers(TArray<UInteractionReceiverComponent*>& ReceiverComponent, TArray<AActor*>& ReceiverActor, bool& bIsvalid);

	/**
	 * Triggers the interaction with the best-fitting receiver based on the current context
	 * and instigator's parameters. This method identifies the closest and most relevant
	 * interaction receiver component by scoring potential interactions with respect to
	 * spatial and directional alignment.
	 *
	 * This method performs the following steps:
	 * - Updates the list of interaction fits.
	 * - Validates the preconditions such as the presence of an instigator, valid location,
	 *   and suitable interaction rotation.
	 * - Calculates a score for each interaction receiver based on proximity and directional
	 *   alignment in comparison to the instigator.
	 * - Chooses the receiver with the best score and broadcasts the corresponding interaction events.
	 * - Updates the recent receiver to the best-fitting receiver.
	 *
	 * Preconditions:
	 * - InstigatorHold must be set.
	 * - The instigator's location override must be valid and bHasInstigatorLocationOverride
	 *   must be true.
	 * - InteractRotation must be a valid positive value.
	 * - ReceiversFittings array must contain at least one valid receiver component.
	 *
	 * Postconditions:
	 * - On success, triggers interaction events for the best-fitting receiver and the instigator.
	 * - Updates the RecentReceiver property if a valid receiver is found.
	 * - If no suitable receiver is found, no events are triggered.
	 *
	 * Note:
	 * - The scoring function prioritizes receivers based on directional alignment to the
	 *   instigator's facing direction, penalizing excessive distance.
	 */
	UFUNCTION(BlueprintCallable, Category="BDC|Interaction|Subsystem")
	void TriggerInteractionBestFitting();

	/**
	 * Triggers an interaction for the currently held interaction index, if valid.
	 *
	 * This function first updates the list of fitting interaction receivers by
	 * calling the UpdateInteractionFits() method. If the current HoldIndex
	 * corresponds to a valid index within the list of fitting receivers, it
	 * triggers the interaction associated with that index by calling
	 * TriggerInteractionOnIndex().
	 */
	UFUNCTION(BlueprintCallable, Category="BDC|Interaction|Subsystem")
	void TriggerInteractionOnHold();

	/**
	 * Triggers an interaction event based on the specified index.
	 *
	 * This function verifies the validity of an instigator and updates the available interaction fittings.
	 * If the provided index is valid within the `ReceiversFittings` array, the function proceeds to
	 * broadcast interaction events between the instigator and the receiver at the specified index.
	 *
	 * @param OnIndex The index of the interaction receiver within the `ReceiversFittings` array.
	 *                This parameter is passed by reference to ensure it is immutable within the function scope.
	 *
	 * Precondition:
	 * - The instigator (`InstigatorHold`) must be valid.
	 * - `UpdateInteractionFits` is invoked to refresh the interaction fittings list.
	 * - The provided index (`OnIndex`) must correspond to a valid entry in the `ReceiversFittings` array.
	 *
	 * Behavior:
	 * - Validates if the index (`OnIndex`) exists within the `ReceiversFittings` array.
	 * - Establishes the interaction between `InstigatorHold` and the receiver component at the specified index.
	 * - Broadcasts the `OnReceivedInteraction` event on the receiver and the `OnTriggeredInteraction` event
	 *   on the instigator.
	 * - Triggers the subsystem-wide `OnTriggeredInteraction` event.
	 *
	 * Postcondition:
	 * - `RecentReceiver` is updated to point to the receiver component at the provided index.
	 * - Interaction events are broadcasted, enabling other components subscribing to these events
	 *   to react accordingly.
	 *
	 * Notes:
	 * - If the index is invalid or the instigator is null, the function will exit without executing.
	 */
	UFUNCTION(BlueprintCallable, Category="BDC|Interaction|Subsystem")
	void TriggerInteractionOnIndex(const int32& OnIndex);

	/**
	 * Updates the hold receiver index if the provided index is non-negative.
	 *
	 * This function sets the `HoldIndex` of the interaction subsystem to the value
	 * of `NewIndex` if `NewIndex` is greater than or equal to zero. The `HoldIndex`
	 * determines which receiver within the subsystem's internal list is marked
	 * as being actively held or interacted with.
	 *
	 * @param NewIndex The new index to set for the hold receiver. Must be greater
	 *                 than or equal to zero to be accepted.
	 */
	UFUNCTION(BlueprintCallable, meta=(ClampMin="0"), Category="BDC|Interaction|Subsystem")
	void SetHoldReceiverIndex(int32 NewIndex);

	/**
	 * Retrieves the current index of the receiver being held.
	 *
	 * @param CurrentIndex Reference to an integer where the current hold index will be stored.
	 *                     This value corresponds to the `HoldIndex` maintained by the Interaction Subsystem.
	 */
	UFUNCTION(BlueprintPure, Category="BDC|Interaction|Subsystem")
	void GetHoldReceiverIndex(int32& CurrentIndex);

	/**
	 * Updates the current interaction focuses by determining which interaction receivers
	 * have been gained or lost focus and broadcasting the respective events.
	 *
	 * This function performs the following operations:
	 * - Calls UpdateInteractionFits() to refresh the list of fitting receivers.
	 * - Compares the updated receiver list with the previous list to calculate:
	 *   * New receivers that have gained focus.
	 *   * Lost receivers that have lost focus.
	 * - For new receivers, broadcasts OnReceivedFocus events.
	 * - For lost receivers, broadcasts OnLostFocus events.
	 * - Updates ReceiversDifference to reflect the current state of fitting receivers.
	 * - Broadcasts OnFocusesUpdated events to inform about the focus changes.
	 * - Additionally, if InstigatorHold is valid, it will invoke OnFocusesUpdated on it.
	 *
	 * Preconditions:
	 * - InstigatorHold must be set and have a valid owner for the method to execute focus updates.
	 * - ReceiversFittings and ReceiversDifference must be valid collections for comparison.
	 *
	 * Postconditions:
	 * - Updates the internal state of ReceiversDifference.
	 * - Fires appropriate focus-related events based on receiver changes.
	 *
	 * Intended to ensure that interaction focus management is kept up-to-date based
	 * on changes in the surrounding environment and the instigator's state.
	 */
	UFUNCTION(BlueprintCallable, Category="BDC|Interaction|Subsystem")
	void UpdateInteractionFocuses();

	/**
	 * Updates the list of interaction candidates (ReceiversFittings) based on certain conditions such as
	 * the instigator's location, range, and rotation.
	 * Filters eligible receivers from the ReceiversHold array to compute the current valid interaction targets.
	 *
	 * Conditions for a receiver to be added to ReceiversFittings:
	 * - The receiver is valid and has an owner.
	 * - The distance between the receiver and the instigator's location is within the specified interaction range.
	 * - The angle between the forward direction of the instigator and the direction to the receiver is within the specified threshold.
	 *
	 * This method is used by several other functions in the system to determine valid interaction targets dynamically.
	 *
	 * Private member variables involved:
	 * - InstigatorLocationOverride: The location used as the basis for interaction checks.
	 * - InteractRotation: The rotation used to determine the forward direction.
	 * - InteractRange: The maximum range within which receivers are considered valid.
	 * - InteractFallOff: The angle threshold for valid interaction.
	 * - ReceiversHold: The array of potential receivers before filtering.
	 * - ReceiversFittings: The filtered list of valid receivers.
	 *
	 * This method does the following:
	 * - Clears the existing ReceiversFittings array.
	 * - Verifies preconditions such as valid InstigatorHold and InstigatorLocationOverride.
	 * - Iterates through ReceiversHold and applies distance and angle checks to filter valid receivers.
	 */
	UFUNCTION(BlueprintCallable, Category="BDC|Interaction|Subsystem")
	void UpdateInteractionFits();
};
