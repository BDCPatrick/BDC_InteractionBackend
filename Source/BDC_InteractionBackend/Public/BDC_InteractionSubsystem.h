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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFocusesUpdated, bool, HasAnyFocus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTriggeredInteraction);

UCLASS()
class BDC_INTERACTIONBACKEND_API UBDC_InteractionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY()
	float InteractRange = 100.0f;
	
	UPROPERTY()
	float InteractFallOff = 15.0f;
	
	UPROPERTY()
	int32 HoldIndex = 0;
	
	UPROPERTY()
	UInteractionInstigatorComponent* InstigatorHold;
	
	UPROPERTY()
	UInteractionInstigatorComponent* RecentReceiver;

	UPROPERTY()
	TArray<UInteractionReceiverComponent*> ReceiversHold;

	TArray<UInteractionReceiverComponent*> ReceiversFittings;
	TArray<UInteractionReceiverComponent*> ReceiversDifference;
	
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
	
	UPROPERTY(BlueprintAssignable, Category = "BDC|Interaction|Subsystem|Events")
	FOnFocusesUpdated OnFocusesUpdated;
	
	UPROPERTY(BlueprintAssignable, Category = "BDC|Interaction|Subsystem|Events")
	FOnTriggeredInteraction OnTriggeredInteraction;

    /** Sets the instigator component */
    void SetInstigator(UInteractionInstigatorComponent* NewInstigator);

    /** Adds a receiver component to the list of receivers */
    void AddReceiver(UInteractionReceiverComponent* NewReceiver);

    /** Removes a receiver component from the list of receivers */
    void RemoveReceiver(UInteractionReceiverComponent* ReceiverToRemove);
	
	/** Updates the Range of the Subsystem to a new one. */
	UFUNCTION(BlueprintCallable, meta=(ClampMin="0"), Category="BDC|Interaction|Subsystem|Setup")
	void SetInteractionRange(float NewRange = 100.0f);
	
	/** Updates the Angle of the Direction Check to a new one. */
	UFUNCTION(BlueprintCallable, meta=(ClampMin="0", ClampMax="360"), Category="BDC|Interaction|Subsystem|Setup")
	void SetInteractionFallOff(float NewAngle = 15.0f);
	
	/** Returns the RecentReceiver */
	UFUNCTION(BlueprintPure, Category="BDC|Interaction|Subsystem")
	void GetRecentReceiver(UInteractionInstigatorComponent*& ReceiverComponent, AActor*& ReceiverActor, bool& bIsvalid);
	
	/** Returns the ReceiversFittings */
	UFUNCTION(BlueprintPure, Category="BDC|Interaction|Subsystem")
	void GetFittingReceivers(TArray<UInteractionInstigatorComponent*>& ReceiverComponent, TArray<AActor*>& ReceiverActor, bool& bIsvalid);
	
	/**
	* Gathers all Receivers in Range of the Instigator into ReceiversFittings and ReceiversHold, then checks which is closest and in most in front of the Instigator direction (+- Falloff).
	* Sets RecentReceiver to the resulting Receiver and Broadcasts OnReceivedInteraction on it.
	* Broadcasts OnTriggeredInteraction on Instigator.
	* Broadcasts OnTriggeredInteraction on this Subsystem.
	*/
	UFUNCTION(BlueprintCallable, Category="BDC|Interaction|Subsystem")
	void TriggerInteractionBestFitting();
	
	/**
	* Gathers all Receivers of ReceiversFittings, than takes the one of HoldIndex.
	* Sets RecentReceiver to the resulting Receiver and Broadcasts OnReceivedInteraction on it.
	* Broadcasts OnTriggeredInteraction on Instigator.
	* Broadcasts OnTriggeredInteraction on this Subsystem.
	*/
	UFUNCTION(BlueprintCallable, Category="BDC|Interaction|Subsystem")
	void TriggerInteractionOnHold();
	
	/**
	* Gathers all Receivers of ReceiversFittings, than takes the one of OnIndex.
	* Sets RecentReceiver to the resulting Receiver and Broadcasts OnReceivedInteraction on it.
	* Broadcasts OnTriggeredInteraction on Instigator.
	* Broadcasts OnTriggeredInteraction on this Subsystem.
	*/
	UFUNCTION(BlueprintCallable, Category="BDC|Interaction|Subsystem")
	void TriggerInteractionOnIndex(const int32& OnIndex);
	
	/** Sets HoldIndex to NewIndex */
	UFUNCTION(BlueprintCallable, meta=(ClampMin="0"), Category="BDC|Interaction|Subsystem")
	void SetHoldReceiverIndex(int32 NewIndex);
	
	/** Returns HoldIndex as CurrentIndex */
	UFUNCTION(BlueprintPure, Category="BDC|Interaction|Subsystem")
	void GetHoldReceiverIndex(int32& CurrentIndex);
	
	/**
	* Gathers all Receivers in Range of the Instigator into ReceiversHold. Check which are in Front of the Instigator (+- Falloff).
	* Fill ReceiversFittings with the Result.
	* Checks ReceiversFittings against ReceiversDifference.
	* Broadcasts OnReceivedFocus on new Added
	* Broadcasts OnLostFocus On Removed ones
	* then overrides ReceiversDifference with ReceiversFittings
	* Whenever a change happens (new added, removed), broadcast OnFocusesUpdated on this Subsystem.
	*/
	UFUNCTION(BlueprintCallable, Category="BDC|Interaction|Subsystem")
	void UpdateInteractionFocuses();
	
	/**
	* Gathers all Receivers in Range of the Instigator into ReceiversHold. Check which are in Front of the Instigator (+- Falloff).
	* Fill ReceiversFittings with the Result.
	*/
	UFUNCTION(BlueprintCallable, Category="BDC|Interaction|Subsystem")
	void UpdateInteractionFits();

};
