/* Copyright © beginning at 2026 - BlackDevilCreations
 * Author: Patrick Wenzel
 * All rights reserved.
 * This file is part of a BlackDevilCreations project and may not be distributed, copied,
 * or modified without prior written permission from BlackDevilCreations.
 * Unreal Engine and its associated trademarks are property of Epic Games, Inc.
 * and are used with permission.
 */
#include "BDC_InteractionSubsystem.h"

#include "BDC_InteractionSettings.h"
#include "Components/InteractionInstigator.h"
#include "Components/InteractionReceiver.h"
#include "Engine/World.h"

void UBDC_InteractionSubsystem::GetLastInteraction(FInteractionReceivers& LastReceiver)
{
	LastReceiver = LastInteractedWith;
}

void UBDC_InteractionSubsystem::SetInstigator(UInteractionInstigatorComponent* NewInstigator)
{
	Instigator = NewInstigator;
}

void UBDC_InteractionSubsystem::InjectInteraction()
{
	const UBDC_InteractionSettings* Settings = GetDefault<UBDC_InteractionSettings>();
	if (!Settings || !Instigator) return;

	UInteractionReceiverComponent* BestReceiver = nullptr;
	float BestDistance = Settings->InteractionRange;

	const FVector InstigatorLocation = InstigatorTransform.GetLocation();
	const FVector InstigatorForward = InstigatorTransform.GetRotation().GetForwardVector();

	for (UInteractionReceiverComponent* Receiver : ReceiversInField)
	{
		if (!Receiver) continue;

		FVector ReceiverLocation = Receiver->GetReceiverTransform().GetLocation();

		if (const float Distance = FVector::Dist(InstigatorLocation, ReceiverLocation); Distance <= Settings->InteractionRange)
		{
			FVector DirectionToReceiver = (ReceiverLocation - InstigatorLocation).GetSafeNormal();
			const float DotProduct = FVector::DotProduct(InstigatorForward, DirectionToReceiver);

			if (const float Angle = FMath::RadiansToDegrees(FMath::Acos(DotProduct)); Angle <= Settings->InteractionFoV * 0.5f)
			{
				if (Distance < BestDistance)
				{
					BestDistance = Distance;
					BestReceiver = Receiver;
				}
			}
		}
	}

	if (BestReceiver)
	{
		LastInteractedWith.InteractionComponent = BestReceiver;
		LastInteractedWith.InteractionActor = BestReceiver->GetOwner();

		BestReceiver->OnReceivedInteraction.Broadcast(Instigator->GetOwner(), Instigator->NameOfInstigator, Instigator->InstigatingTags);
		OnInteractionFired.Broadcast(BestReceiver);
	}
}

void UBDC_InteractionSubsystem::UpdateInteractions(FVector InstigatorLocation, FRotator InstigatorRotation)
{
	InstigatorTransform.SetLocation(InstigatorLocation);
	InstigatorTransform.SetRotation(InstigatorRotation.Quaternion());

	const UBDC_InteractionSettings* Settings = GetDefault<UBDC_InteractionSettings>();
	if (!Settings) return;

	TArray<UInteractionReceiverComponent*> NewReceiversInField;
	TArray<UInteractionReceiverComponent*> AddedReceivers;
	TArray<UInteractionReceiverComponent*> RemovedReceivers;

	AActor* InstigatorActor = Instigator ? Instigator->GetOwner() : nullptr;
	const FName InstigatorName = Instigator ? Instigator->NameOfInstigator : NAME_None;

	for (const FInteractionReceivers& ReceiverData : ReceiversOfLevel)
	{
		UInteractionReceiverComponent* ReceiverComp = Cast<UInteractionReceiverComponent>(ReceiverData.InteractionComponent);
		if (!ReceiverComp) continue;

		FVector ReceiverLocation = ReceiverComp->GetReceiverTransform().GetLocation();

		if (const float Distance = FVector::Dist(InstigatorLocation, ReceiverLocation); Distance <= Settings->InteractionRange)
		{
			NewReceiversInField.Add(ReceiverComp);
			if (!ReceiversInField.Contains(ReceiverComp))
			{
				AddedReceivers.Add(ReceiverComp);
				ReceiverComp->OnEntersInteractionField.Broadcast(InstigatorActor, InstigatorName);
			}
		}
	}

	for (UInteractionReceiverComponent* Receiver : ReceiversInField)
	{
		if (!NewReceiversInField.Contains(Receiver))
		{
			RemovedReceivers.Add(Receiver);
			if (Receiver)
			{
				Receiver->OnLeavesInteractionField.Broadcast(InstigatorActor, InstigatorName);
			}
		}
	}

	ReceiversInField = NewReceiversInField;

	if (AddedReceivers.Num() > 0)
	{
		OnFoundReceivers.Broadcast(AddedReceivers);
	}

	if (RemovedReceivers.Num() > 0)
	{
		OnLostReceivers.Broadcast(RemovedReceivers);
	}
}

void UBDC_InteractionSubsystem::GetAllReceiversField(TArray<UInteractionReceiverComponent*>& Receivers)
{
	Receivers = ReceiversInField;
}

void UBDC_InteractionSubsystem::GetAllReceiversOfLevel(TArray<FInteractionReceivers>& Receivers)
{
	Receivers = ReceiversOfLevel;
}

void UBDC_InteractionSubsystem::GetReceiverByTag(FGameplayTag OfReceiverTag, FInteractionReceivers& ReceiverData)
{
	for (const FInteractionReceivers& R : ReceiversOfLevel)
	{
		if (UInteractionReceiverComponent* Comp = Cast<UInteractionReceiverComponent>(R.InteractionComponent))
		{
			if (Comp->TagOfReceiver == OfReceiverTag)
			{
				ReceiverData = R;
				return;
			}
		}
	}
	ReceiverData = FInteractionReceivers();
}

void UBDC_InteractionSubsystem::GetReceiverByName(FName OfReceiverName, FInteractionReceivers& ReceiverData)
{
	for (const FInteractionReceivers& R : ReceiversOfLevel)
	{
		if (UInteractionReceiverComponent* Comp = Cast<UInteractionReceiverComponent>(R.InteractionComponent))
		{
			if (Comp->NameOfReiceiver == OfReceiverName)
			{
				ReceiverData = R;
				return;
			}
		}
	}
	ReceiverData = FInteractionReceivers();
}

void UBDC_InteractionSubsystem::GetInstigatorByTag(FGameplayTag OfInstigatorTag, FInteractionReceivers& InstigatorData)
{
	for (const UInteractionInstigatorComponent* I : InstigatorsOfLevel)
	{
		if (I && I->TagOfInstigator == OfInstigatorTag)
		{
			InstigatorData.InteractionActor = I->GetOwner();
			InstigatorData.InteractionComponent = const_cast<UInteractionInstigatorComponent*>(I);
			return;
		}
	}
	InstigatorData = FInteractionReceivers();
}

void UBDC_InteractionSubsystem::GetInstigatorByName(FName OfInstigatorName, FInteractionReceivers& InstigatorData)
{
	for (UInteractionInstigatorComponent* I : InstigatorsOfLevel)
	{
		if (I && I->NameOfInstigator == OfInstigatorName)
		{
			InstigatorData.InteractionActor = I->GetOwner();
			InstigatorData.InteractionComponent = I;
			return;
		}
	}
	InstigatorData = FInteractionReceivers();
}

void UBDC_InteractionSubsystem::AddReceiver(FInteractionReceivers NewReceiver)
{
	ReceiversOfLevel.AddUnique(NewReceiver);
}

void UBDC_InteractionSubsystem::RemoveReceiver(UInteractionReceiverComponent* ReceiverComponent)
{
	ReceiversOfLevel.RemoveAll([ReceiverComponent](const FInteractionReceivers& R) {
		return R.InteractionComponent == ReceiverComponent;
	});
	ReceiversInField.Remove(ReceiverComponent);
}

void UBDC_InteractionSubsystem::AddInstigator(UInteractionInstigatorComponent* NewInstigator)
{
	InstigatorsOfLevel.AddUnique(NewInstigator);
}

void UBDC_InteractionSubsystem::RemoveInstigator(UInteractionInstigatorComponent* InstigatorComponent)
{
	InstigatorsOfLevel.Remove(InstigatorComponent);
	if (Instigator == InstigatorComponent)
	{
		Instigator = nullptr;
	}
}

