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
#include "Subsystems/GameInstanceSubsystem.h"
#include "BDC_InteractionSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FInteractionReceivers
{
	GENERATED_BODY()
	
	public:
	UPROPERTY(BlueprintAssignable, Category = "BDC|Interaction|Struct")
	AActor* ReceiverActor;
	UPROPERTY(BlueprintAssignable, Category = "BDC|Interaction|Struct")
	UInteractionReceiverComponent* ReceiverComponent;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFoundReceivers, TArray<ReceiversInField*>, NewReceivers);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLostReceivers, TArray<ReceiversInField*>, ReceiversGone);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionFired, ReceiversInField*, OnReceivers);

UCLASS()
class BDC_INTERACTIONBACKEND_API UBDC_InteractionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	FTranform InstigatorTransform = FTransform();

	UPROPERTY()
	FInteractionReceivers LastInteractedWith;
	
	UPROPERTY()
	TArray<FInteractionReceivers*> ReceiversInField;
	
	UPROPERTY()
	TArray<FInteractionReceivers*> ReceiversOfLevel;
	
	
public: 
	UPROPERTY(BlueprintAssignable, Category = "BDC|Interaction|Dispatchers|Subsystem")
	FOnFoundReceivers OnFoundReceivers;
	UPROPERTY(BlueprintAssignable, Category = "BDC|Interaction|Dispatchers|Subsystem")
	FOnLostReceivers OnLostReceivers;
	UPROPERTY(BlueprintAssignable, Category = "BDC|Interaction|Dispatchers|Subsystem")
	FOnInteractionFired OnInteractionFired;

	//Even public, all function below are should only callable via BDC_InteractionLibrary.
	
	// TODO: Returns LastInteractedWith
	UFUNCTION(Category="BDC Interaction|Subsystem")
	void GetLastInteraction(FInteractionReceivers*& LastReceiver);
	
	/* Start of TODO: Takes the UBDC_InteractionSettings.InteractionRange and Gets all overlapping Receivers in that range around the InstigatorTransform.
	* After receving all possible Receivers in Range, check which of them fits best in distance to the InstigatorTransform, and is inside the  UBDC_InteractionSettings.InteractionFoV in front of the InstigatorTransform.
	* The best fitting UInteractionReceiverComponent needs the FOnReceivedInteraction being Broadcasted and LastInteractedWith being set to that Receiver.
	* Broadcast FOnInteractionFired of the Subsystem.
	*/
	UFUNCTION(Category="BDC Interaction|Subsystem")
	void InjectInteraction();
	
	/* Start of TODO: This function is called from a Tick Event of the Game, linked by the User.
	* It updates the InstigatorTransform based on the Input Parameters.
	* After updating the Transform, it checks for All overlapping Receivers within the Range of UBDC_InteractionSettings.InteractionRange of InstigatorTransform. The Receivers Location can be get by ReceiverComponent.GetReceiverTransform.
	* It then checks, if any of the ReceiversInField is not in the range anymore - then broadcast FOnLeavesInteractionField on it and remove it from the .ReceiversInField. Collect all of the removed ones to Broadcast FOnLostReceivers with the list.
	* It then checks, if any of the Overlapping Actors is not already in the ReceiversInField - then broadcast FOnEntersInteractionField on it and add it to the .ReceiversInField. Collect all of the added ones to Broadcast FOnFoundReceivers with the list.
	*/
	UFUNCTION(Category="BDC Interaction|Subsystem")
	void UpdateInteractions(FVector InstigatorLocation, FRotator InstigatorRotation);
	
	// TODO: Returns ReceiversInField.
	UFUNCTION(Category="BDC Interaction|Subsystem")
	void GetAllReceiversField(TArray<FInteractionReceivers*>& Receivers);
	
	// TODO: Returns ReceiversOfLevel.
	UFUNCTION(Category="BDC Interaction|Subsystem")
	void GetAllReceiversOfLevel(TArray<FInteractionReceivers*>& Receivers);
	
	// TODO: GetAllReceiversOfLevel and returns the first one with the specific TagOfReceiver.
	UFUNCTION(Category="BDC Interaction|Subsystem")
	void GetReceiverByTag(FGameplayTag OfReceiverTag, FInteractionReceivers& Receiver);
	
	// TODO: Like GetReceiverByTag, but by NameOfReiceiver instead of TagOfReceiver
	UFUNCTION(Category="BDC Interaction|Subsystem")
	void GetReceiverByName(FName OfReceiverName, FInteractionReceivers& Receiver);
	
	// TODO: Collects all UInteractionInstigatorComponent of the level and returns the first one with the specific TagOfInstigator.
	UFUNCTION(Category="BDC Interaction|Subsystem")
	void GetInstigatorByTag(FGameplayTag OfInstigatorTag, FInteractionReceivers& Instigator);
	
	// TODO: Like GetInstigatorByTag, but by NameOfInstigator instead of TagOfInstigator
	UFUNCTION(Category="BDC Interaction|Subsystem")
	void GetInstigatorByName(FName OfInstigatorName, FInteractionReceivers& Instigator);
	
	// TODO: Not callable by Blueprint and not being added to the Blueprint Library. This function only gets called by the UInteractionReceiverComponent on beginPlay.
	UFUNCTION(Category="BDC Interaction|Subsystem")
	void AddReceiver(FInteractionReceivers NewReceiver);
};