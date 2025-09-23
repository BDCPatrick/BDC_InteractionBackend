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

#include "BDC_InteractionSubsystem.h"
#include "InteractionInstigator.h"
#include "InteractionReceiver.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

void UBDC_InteractionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UBDC_InteractionSubsystem::Deinitialize()
{
    Super::Deinitialize();
}

void UBDC_InteractionSubsystem::SetInstigator(UInteractionInstigatorComponent* NewInstigator)
{
    InstigatorHold = NewInstigator;
}

void UBDC_InteractionSubsystem::AddReceiver(UInteractionReceiverComponent* NewReceiver)
{
    if (NewReceiver)
    {
        ReceiversHold.RemoveAll([](const UInteractionReceiverComponent* Receiver)
        {
            return !IsValid(Receiver);
        });

        ReceiversHold.AddUnique(NewReceiver);
    }
}

void UBDC_InteractionSubsystem::RemoveReceiver(UInteractionReceiverComponent* ReceiverToRemove)
{
    if (ReceiverToRemove)
    {
        ReceiversHold.Remove(ReceiverToRemove);
		ReceiversFittings.Remove(ReceiverToRemove);
		ReceiversDifference.Remove(ReceiverToRemove);
		if(RecentReceiver == ReceiverToRemove)
		{
			RecentReceiver = nullptr;
		}
    }
}

void UBDC_InteractionSubsystem::SetInteractionRange(float NewRange)
{
	InteractRange = NewRange;
}

void UBDC_InteractionSubsystem::SetInteractionFallOff(float NewAngle)
{
	InteractFallOff = NewAngle;
}

void UBDC_InteractionSubsystem::GetRecentReceiver(UInteractionInstigatorComponent*& ReceiverComponent, AActor*& ReceiverActor, bool& bIsValid)
{
	if(RecentReceiver)
	{
		ReceiverComponent = RecentReceiver;
		ReceiverActor = RecentReceiver->GetOwner();
		bIsValid = true;
	}
	else
	{
		ReceiverComponent = nullptr;
		ReceiverActor = nullptr;
		bIsValid = false;
	}
}

void UBDC_InteractionSubsystem::GetFittingReceivers(TArray<UInteractionInstigatorComponent*>& ReceiverComponents, TArray<AActor*>& ReceiverActors, bool& bIsValid)
{
	if(ReceiversFittings.Num() > 0)
	{
		TArray<UInteractionInstigatorComponent*> TempCastedReceivers;
		for(UInteractionReceiverComponent* Receiver : ReceiversFittings)
		{
			if(UInteractionInstigatorComponent* CastedComponent = Cast<UInteractionInstigatorComponent>(Receiver))
			{
				TempCastedReceivers.Add(CastedComponent);
				ReceiverActors.Add(Receiver->GetOwner());
			}
		}
		ReceiverComponents = TempCastedReceivers;
		bIsValid = true;
	}
	else
	{
		ReceiverComponents.Empty();
		ReceiverActors.Empty();
		bIsValid = false;
	}
}

void UBDC_InteractionSubsystem::TriggerInteractionBestFitting()
{
	if (!InstigatorHold) return;

    UpdateInteractionFocuses();

    UInteractionReceiverComponent* BestReceiver = nullptr;
    float BestScore = -1.0f;

    for (UInteractionReceiverComponent* Receiver : ReceiversFittings)
    {
        if (!Receiver || !Receiver->GetOwner()) continue;

        const FVector InstigatorLocation = InstigatorHold->GetOwner()->GetActorLocation();
        const FVector ReceiverLocation = Receiver->GetOwner()->GetActorLocation();
        const float Distance = FVector::Dist(InstigatorLocation, ReceiverLocation);

        const FVector InstigatorForward = InstigatorHold->GetOwner()->GetActorForwardVector();
        const FVector DirectionToReceiver = (ReceiverLocation - InstigatorLocation).GetSafeNormal();
        float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(InstigatorForward, DirectionToReceiver)));

        if (Distance <= InteractRange && Angle <= InteractFallOff)
        {
            float Score = (1.0f - (Distance / InteractRange)) + (1.0f - (Angle / InteractFallOff));
            if (Score > BestScore)
            {
                BestScore = Score;
                BestReceiver = Receiver;
            }
        }
    }

    if (BestReceiver)
    {
        RecentReceiver = BestReceiver;
        BestReceiver->OnReceivedInteraction.Broadcast(InstigatorHold);
        InstigatorHold->OnTriggeredInteraction.Broadcast(BestReceiver);
        OnTriggeredInteraction.Broadcast();
    }
}

void UBDC_InteractionSubsystem::TriggerInteractionOnHold()
{
	if(ReceiversHold.IsValidIndex(HoldIndex))
	{
		TriggerInteractionOnIndex(HoldIndex);
	}
}

void UBDC_InteractionSubsystem::TriggerInteractionOnIndex(const int32& OnIndex)
{
    if (InstigatorHold && ReceiversHold.IsValidIndex(OnIndex))
    {
        if (UInteractionReceiverComponent* Receiver = ReceiversHold[OnIndex])
        {
            RecentReceiver = Receiver;
            Receiver->OnReceivedInteraction.Broadcast(InstigatorHold);
            InstigatorHold->OnTriggeredInteraction.Broadcast(Receiver);
            OnTriggeredInteraction.Broadcast();
        }
    }
}

void UBDC_InteractionSubsystem::SetHoldReceiverIndex(int32 NewIndex)
{
	if(NewIndex >= 0)
	{
		HoldIndex = NewIndex;
	}
}

void UBDC_InteractionSubsystem::GetHoldReceiverIndex(int32& CurrentIndex)
{
	CurrentIndex = HoldIndex;
}

void UBDC_InteractionSubsystem::UpdateInteractionFocuses()
{
    if (!InstigatorHold || !InstigatorHold->GetOwner()) return;
    
    UpdateInteractionFits();
    
    TArray<UInteractionReceiverComponent*> NewReceivers;
    for (UInteractionReceiverComponent* Receiver : ReceiversFittings)
    {
        if (!ReceiversDifference.Contains(Receiver))
        {
            NewReceivers.Add(Receiver);
        }
    }

    TArray<UInteractionReceiverComponent*> LostReceivers;
    for (UInteractionReceiverComponent* Receiver : ReceiversDifference)
    {
        if (!ReceiversFittings.Contains(Receiver))
        {
            LostReceivers.Add(Receiver);
        }
    }

    if (NewReceivers.Num() > 0 || LostReceivers.Num() > 0)
    {
        for (UInteractionReceiverComponent* Receiver : NewReceivers)
        {
            if(Receiver) Receiver->OnReceivedFocus.Broadcast(Receiver->SelfIndex);
        }

        for (UInteractionReceiverComponent* Receiver : LostReceivers)
        {
            if(Receiver) Receiver->OnLostFocus.Broadcast();
        }

        ReceiversDifference = ReceiversFittings;
        OnFocusesUpdated.Broadcast();
    }
}

void UBDC_InteractionSubsystem::UpdateInteractionFits()
{
	if (!InstigatorHold || !InstigatorHold->GetOwner()) return;

	ReceiversFittings.Empty();
	const FVector InstigatorLocation = InstigatorHold->GetOwner()->GetActorLocation();
	const FVector InstigatorForward = InstigatorHold->GetOwner()->GetActorForwardVector();

	for (UInteractionReceiverComponent* Receiver : ReceiversHold)
	{
		if (!Receiver || !Receiver->GetOwner()) continue;

		const FVector ReceiverLocation = Receiver->GetOwner()->GetActorLocation();
		const float Distance = FVector::Dist(InstigatorLocation, ReceiverLocation);

		if (Distance <= InteractRange)
		{
			const FVector DirectionToReceiver = (ReceiverLocation - InstigatorLocation).GetSafeNormal();
			const float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(InstigatorForward, DirectionToReceiver)));
			
			if (Angle <= InteractFallOff)
			{
				ReceiversFittings.Add(Receiver);
			}
		}
	}
}