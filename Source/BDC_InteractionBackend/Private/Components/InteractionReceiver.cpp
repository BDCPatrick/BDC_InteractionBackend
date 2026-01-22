/* Copyright © beginning at 2026 - BlackDevilCreations
 * Author: Patrick Wenzel
 * All rights reserved.
 * This file is part of a BlackDevilCreations project and may not be distributed, copied,
 * or modified without prior written permission from BlackDevilCreations.
 * Unreal Engine and its associated trademarks are property of Epic Games, Inc.
 * and are used with permission.
 */
#include "Components/InteractionReceiver.h"

/* Start of TODO: This Component has to be Auto Activated and Replicated!*/

/* Start of TODO: On BeginPlay, the Receiver Component needs to search the owning Actor's Component Hierarchy for the first one with the name NameOfInteractionComponent.
* If found, it has to hold the reference to this Component in ReceiverComponent.
* If not found, set ReceiverComponent nullptr.
*
* The Receiver should also register itself in UBDC_InteractionSubsystem.ReceiversOfLevel, by calling UBDC_InteractionSubsystem::AddReceiver.
*/