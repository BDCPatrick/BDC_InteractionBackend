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
#include "InteractionReceiver.generated.h"

class UInteractionInstigatorComponent;
class USceneComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReceivedInteractionSignature, UInteractionInstigatorComponent*, ByInstigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReceivedFocusSignature, int32, AsIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLostFocusSignature);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BDC_INTERACTIONBACKEND_API UInteractionReceiverComponent : public UActorComponent
{
	GENERATED_BODY()
	
private:

public:
	UInteractionReceiverComponent();

	/**
	 * If true, debug lines will be drawn to visualize the direction of interactions for this receiver.
	 * Specifically, a line will indicate the direction from the interaction surface to the instigator's location.
	 * This is useful for debugging interaction mechanics within the game world.
	 *
	 * Category: "BDC|Interaction|Receiver|Debug"
	 * Default Value: false
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDC|Interaction|Receiver|Debug")
	bool bDrawDebugDirection = false;

	/**
	 * Represents a component responsible for handling tracking functionality, including related calculations and state management.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDC|Interaction|Receiver|Setup")
	USceneComponent* TrackingComponent = nullptr;

	/**
	 * Represents the half-dimensions of an axis-aligned bounding box (AABB) used to define the interaction bounds of this component.
	 *
	 * This determines the extents of the interaction area relative to the center point.
	 * The bounds are used for calculations such as finding the closest surface point toward a given world location.
	 *
	 * - X, Y, Z components define the size of the box along each axis, measured from the center to the edge.
	 * - When set to FVector::ZeroVector (default), the bounds are effectively disabled or degenerate.
	 *
	 * Configurable in Blueprint and editor with the following options:
	 * - EditAnywhere: The property can be edited in Blueprints and in the editor.
	 * - BlueprintReadWrite: The property can be read and written in Blueprints.
	 * - Category: Assigned to "BDC|Interaction|Receiver|Setup" for better organization in the editor.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDC|Interaction|Receiver|Setup")
	FVector BoundsExtent = FVector::ZeroVector;

	/**
	 * Enables or disables the drawing of debug bounds for the interaction receiver component.
	 *
	 * When set to true, the system will render a visual debug representation of the bounds
	 * associated with the interaction receiver, providing a visual aid for debugging purposes.
	 * This is particularly helpful during the development or testing phase to ensure that
	 * the bounds are properly defined and functioning as expected.
	 *
	 * By default, this property is set to false, meaning debug bounds will not be displayed.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDC|Interaction|Receiver|Debug")
	bool bDrawDebugBounds = false;

	/**
	 * Retrieves the world location where the interaction occurs.
	 *
	 * @return The world location associated with the interaction.
	 */
	UFUNCTION(BlueprintPure, Category = "BDC|Interaction|Receiver|Query")
	FVector GetInteractionWorldLocation() const;

	/**
	 * Calculates the closest point on the interaction receiver's bounding box surface towards a given world location.
	 * The bounding box is defined relative to the interaction receiver's world location and orientation.
	 * If the bounding box is not defined (BoundsExtent is nearly zero), the receiver's world location is returned.
	 *
	 * @param FromWorld The world-space location from which to calculate the closest point on the bounding box.
	 * @return The world-space location of the closest point on the surface of the bounding box towards the given location.
	 */
	UFUNCTION(BlueprintPure, Category = "BDC|Interaction|Receiver|Query")
	FVector GetSurfacePointToward(const FVector& FromWorld) const;

	/**
	 * An integer variable representing the index of this receiver instance.
	 * Used for identifying this specific receiver in the interaction subsystem.
	 * Initialized to 0 by default.
	 *
	 * Property Attributes:
	 * - EditAnywhere: Can be modified in editor or during runtime.
	 * - BlueprintReadOnly: Can be accessed in Blueprints but cannot be modified.
	 * - Category: Organized under "BDC|Interaction|Receiver" in the editor property hierarchy.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDC|Interaction|Receiver")
	int32 SelfIndex = 0;

	/**
	 * Delegate property that broadcasts when the interaction receiver component receives an interaction.
	 * This event is triggered when an appropriate interaction instigator interacts with the receiver.
	 *
	 * This delegate allows other systems or components to subscribe and react when an interaction occurs.
	 * It is particularly useful for handling various interaction events dynamically within gameplay.
	 *
	 * Category: BDC|Interaction|Receiver|Events
	 */
	UPROPERTY(BlueprintAssignable, Category = "BDC|Interaction|Receiver|Events")
	FOnReceivedInteractionSignature OnReceivedInteraction;

	/**
	 * Event triggered when this interaction receiver component gains focus.
	 *
	 * This delegate is broadcasted whenever the associated receiver becomes the focus of interaction.
	 * The specific conditions under which focus is gained are managed by the subsystem.
	 * Blueprint-assignable to allow for custom functionality to be implemented when focus is acquired.
	 *
	 * Category: BDC|Interaction|Receiver|Events
	 */
	UPROPERTY(BlueprintAssignable, Category = "BDC|Interaction|Receiver|Events")
	FOnReceivedFocusSignature OnReceivedFocus;

	/**
	 * Delegate triggered when an interaction receiver loses focus.
	 *
	 * This event is broadcasted when a receiver component no longer satisfies the
	 * interaction focus conditions, indicating a transition of focus away from the receiver.
	 *
	 * Category:
	 * - BDC|Interaction|Receiver|Events
	 *
	 * BlueprintAssignable:
	 * - Allows this event to be assigned in Blueprints for custom behavior.
	 */
	UPROPERTY(BlueprintAssignable, Category = "BDC|Interaction|Receiver|Events")
	FOnLostFocusSignature OnLostFocus;

	/**
	 * Represents the primary gameplay tag assigned to an interaction receiver.
	 * This tag is used to uniquely identify or classify the receiver in gameplay mechanics.
	 *
	 * EditAnywhere: Allows this property to be edited in the Unreal Editor.
	 * BlueprintReadOnly: Allows this property to be read in Blueprints but not modified.
	 * Category: Organized under "BDC|Interaction|Receiver|Value" in the property list.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BDC|Interaction|Receiver|Value")
	FGameplayTag ReceiverMainTag;

	/**
	 * Represents a collection of gameplay tags associated with the receiver.
	 * This container is used to store multiple gameplay tags that can define
	 * or categorize the receiver's behavior, attributes, or state within the system.
	 *
	 * The tags in this container can be used for querying, filtering, or processing
	 * interactions based on the receiver's associated gameplay tags.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BDC|Interaction|Receiver|Value")
	FGameplayTagContainer ReceiverTags;

	/**
	 * A unique identifier for the interaction receiver.
	 * This variable is used to distinguish different interaction receivers
	 * and can be utilized for categorization or identification purposes in gameplay logic.
	 *
	 * The value of this identifier can be edited in the editor and is read-only in Blueprint.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BDC|Interaction|Receiver|Value")
	FName ReceiverIdentity;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};