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
#include "Engine/Engine.h"
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
		if (RecentReceiver == ReceiverToRemove)
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

void UBDC_InteractionSubsystem::SetInstigatorRotation(float NewRotation)
{
	InteractRotation = NewRotation;
}

void UBDC_InteractionSubsystem::SetInstigatorLocation(const FVector& NewWorldLocation)
{
	InstigatorLocationOverride = NewWorldLocation;
	bHasInstigatorLocationOverride = true;
}

void UBDC_InteractionSubsystem::ClearInstigatorLocationOverride()
{
	bHasInstigatorLocationOverride = false;
	InstigatorLocationOverride = FVector::ZeroVector;
}

float UBDC_InteractionSubsystem::Move2D_ToDir(const FVector2D& MovementInput, int32 NumberOfDirections) const
{
	if (!MovementInput.IsNearlyZero())
	{
		const double AngleRad = FMath::Atan2((double)MovementInput.Y, (double)MovementInput.X);
		double AngleDeg = FMath::RadiansToDegrees(AngleRad);
		if (AngleDeg < 0.0)
		{
			AngleDeg += 360.0;
		}

		if (NumberOfDirections >= 2)
		{
			const double SectorSize = 360.0 / static_cast<double>(NumberOfDirections);
			const double Quantized = FMath::RoundToDouble(AngleDeg / SectorSize) * SectorSize;
			const double Wrapped = FMath::Fmod(Quantized, 360.0);
			return static_cast<float>(Wrapped < 0.0 ? Wrapped + 360.0 : Wrapped);
		}

		return static_cast<float>(AngleDeg);
	}

	return 0.0f;
}

void UBDC_InteractionSubsystem::GetRecentReceiver(UInteractionReceiverComponent*& ReceiverComponent, AActor*& ReceiverActor, bool& bIsValid)
{
	if (RecentReceiver)
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

void UBDC_InteractionSubsystem::GetFittingReceivers(TArray<UInteractionReceiverComponent*>& ReceiverComponents, TArray<AActor*>& ReceiverActors, bool& bIsValid)
{
	if (ReceiversFittings.Num() > 0)
	{
		ReceiverComponents = ReceiversFittings;
		ReceiverActors.Reset();
		for (UInteractionReceiverComponent* Receiver : ReceiversFittings)
		{
			if (Receiver && Receiver->GetOwner())
			{
				ReceiverActors.Add(Receiver->GetOwner());
			}
		}
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
	UpdateInteractionFits();

	if (!InstigatorHold) return;
	if (!bHasInstigatorLocationOverride) return;
	if (InteractRotation < 0.f) return;
	if (ReceiversFittings.Num() == 0) return;

	const FVector InstLoc = InstigatorLocationOverride;
	const FVector FacingForward = FRotationMatrix(FRotator(0.f, InteractRotation, 0.f)).GetUnitAxis(EAxis::X);

	int32 BestIdx = INDEX_NONE;
	float BestScore = -FLT_MAX;

	for (int32 i = 0; i < ReceiversFittings.Num(); ++i)
	{
		UInteractionReceiverComponent* Receiver = ReceiversFittings[i];
		if (!Receiver || !Receiver->GetOwner()) continue;

		FVector Delta = Receiver->GetOwner()->GetActorLocation() - InstLoc; Delta.Z = 0.f;
		const float Dist = Delta.Size();
		float Dot = 0.f;
		if (Dist > KINDA_SMALL_NUMBER)
		{
			Dot = FMath::Clamp(FVector::DotProduct(FacingForward, Delta / Dist), -1.f, 1.f);
		}
		const float Score = (Dot * 10000.f) - Dist;
		if (FMath::IsFinite(Score) && Score > BestScore)
		{
			BestScore = Score;
			BestIdx = i;
		}
	}

	if (ReceiversFittings.IsValidIndex(BestIdx))
	{
		if (UInteractionReceiverComponent* Receiver = ReceiversFittings[BestIdx])
		{
			RecentReceiver = Receiver;
			Receiver->OnReceivedInteraction.Broadcast(InstigatorHold);
			InstigatorHold->OnTriggeredInteraction.Broadcast(Receiver);
			OnTriggeredInteraction.Broadcast();
		}
	}
}

void UBDC_InteractionSubsystem::TriggerInteractionOnHold()
{
	UpdateInteractionFits();
	if (ReceiversFittings.IsValidIndex(HoldIndex))
	{
		TriggerInteractionOnIndex(HoldIndex);
	}
}

void UBDC_InteractionSubsystem::TriggerInteractionOnIndex(const int32& OnIndex)
{
	if (!InstigatorHold) return;
	UpdateInteractionFits();
	if (ReceiversFittings.IsValidIndex(OnIndex))
	{
		if (UInteractionReceiverComponent* Receiver = ReceiversFittings[OnIndex])
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
	if (NewIndex >= 0)
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
	UpdateInteractionFits();
	if (!InstigatorHold || !InstigatorHold->GetOwner()) return;

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
			if (Receiver) Receiver->OnReceivedFocus.Broadcast(Receiver->SelfIndex);
		}

		for (UInteractionReceiverComponent* Receiver : LostReceivers)
		{
			if (Receiver) Receiver->OnLostFocus.Broadcast();
		}

		ReceiversDifference = ReceiversFittings;
		OnFocusesUpdated.Broadcast(ReceiversFittings.Num() > 0);
		if (InstigatorHold)
		{
			InstigatorHold->OnFocusesUpdated.Broadcast(ReceiversFittings.Num() > 0);
		}
	}
}

void UBDC_InteractionSubsystem::UpdateInteractionFits()
{
	ReceiversFittings.Empty();

	if (!InstigatorHold) return;
	if (!bHasInstigatorLocationOverride) return; 
	if (InteractRotation < 0.f) return;

	const FVector InstLoc = InstigatorLocationOverride;
	const FVector FacingForward = FRotationMatrix(FRotator(0.f, InteractRotation, 0.f)).GetUnitAxis(EAxis::X);
	const float CosThreshold = FMath::Cos(FMath::DegreesToRadians(FMath::Clamp(InteractFallOff, 0.f, 180.f)));
	const float Range = FMath::Max(0.f, InteractRange);

	for (UInteractionReceiverComponent* Receiver : ReceiversHold)
	{
		if (!Receiver || !Receiver->GetOwner()) continue;
		FVector Delta = Receiver->GetOwner()->GetActorLocation() - InstLoc; Delta.Z = 0.f;
		const float Dist = Delta.Size();
		if (Dist > Range) continue;

		float Dot = 1.f;
		if (Dist > KINDA_SMALL_NUMBER)
		{
			Dot = FMath::Clamp(FVector::DotProduct(FacingForward, Delta / Dist), -1.f, 1.f);
		}
		if (Dot >= CosThreshold)
		{
			ReceiversFittings.Add(Receiver);
		}
	}
}
