/* Copyright © beginning at 2026 - BlackDevilCreations
 * Author: Patrick Wenzel
 * All rights reserved.
 * This file is part of a BlackDevilCreations project and may not be distributed, copied,
 * or modified without prior written permission from BlackDevilCreations.
 * Unreal Engine and its associated trademarks are property of Epic Games, Inc.
 * and are used with permission.
 */
#include "BDC_InteractionSubsystem.h"

#include "DrawDebugHelpers.h"
#include "BDC_InteractionSettings.h"
#include "Components/InteractionInstigator.h"
#include "Components/InteractionReceiver.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"

void UBDC_InteractionSubsystem::GetLastInteraction(FInteractionReceivers& LastReceiver) const
{
	LastReceiver = LastInteractedWith;
}

void UBDC_InteractionSubsystem::SetInstigator(UInteractionInstigatorComponent* NewInstigator)
{
	Instigator = NewInstigator;
}

void UBDC_InteractionSubsystem::InjectInteraction()
{
	if (!Instigator) return;

	UInteractionReceiverComponent* BestReceiver = Cast<UInteractionReceiverComponent>(CurrentBestFittingReceiver.InteractionComponent);

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
	const UWorld* World = GetWorld();
	if (!Settings || !World) return;

	TArray<UInteractionReceiverComponent*> NewReceiversInField;
	TArray<UInteractionReceiverComponent*> AddedReceivers;
	TArray<UInteractionReceiverComponent*> RemovedReceivers;

	AActor* InstigatorActor = Instigator ? Instigator->GetOwner() : nullptr;

	if (!Instigator && InstigatorsOfLevel.Num() > 0)
	{
		Instigator = InstigatorsOfLevel[0];
		InstigatorActor = Instigator->GetOwner();
	}

	const FName FinalInstigatorName = Instigator ? Instigator->NameOfInstigator : NAME_None;

	for (const FInteractionReceivers& ReceiverData : ReceiversOfLevel)
	{
		UInteractionReceiverComponent* ReceiverComp = Cast<UInteractionReceiverComponent>(ReceiverData.InteractionComponent);
		if (!ReceiverComp) continue;

		const FVector ReceiverLocation = ReceiverComp->GetReceiverTransform().GetLocation();
		const float DistanceXY = FVector::DistXY(InstigatorLocation, ReceiverLocation);

		if (const float EffectiveDistanceXY = FMath::Max(0.0f, DistanceXY - ReceiverComp->ReceiverRadius); EffectiveDistanceXY <= Settings->InteractionRange)
		{
			FHitResult HitResult;
			FCollisionQueryParams TraceParams(FName(TEXT("UpdateInteractionTrace")), true, InstigatorActor);
			
			const bool bHit = World->LineTraceSingleByChannel(HitResult, InstigatorLocation, ReceiverLocation, ECC_Visibility, TraceParams);

			if (const bool bLineOfSightClear = !bHit || HitResult.GetActor() == ReceiverData.InteractionActor)
			{
				NewReceiversInField.Add(ReceiverComp);
				if (!ReceiversInField.Contains(ReceiverComp))
				{
					AddedReceivers.Add(ReceiverComp);
					ReceiverComp->OnEntersInteractionField.Broadcast(InstigatorActor, FinalInstigatorName);
				}
			}
			else if (bHit)
			{
				// UE_LOG(LogTemp, Warning, TEXT("[Interaction] Trace hit %s instead of %s"), *HitResult.GetActor()->GetName(), *ReceiverData.InteractionActor->GetName());
			}
		}
	}

	TArray<UInteractionReceiverComponent*> NewReceiversInView;
	FRotator AdjustedInstigatorRotation = InstigatorRotation;
	if (Instigator)
	{
		AdjustedInstigatorRotation.Yaw += Instigator->InstigatorOffsetViewRotation;
	}
	const FVector InstigatorForward = AdjustedInstigatorRotation.Vector();
	const float HalfFoVInRadians = FMath::DegreesToRadians(Settings->InteractionFoV * 0.5f);
	const float MinDotProduct = FMath::Cos(HalfFoVInRadians);

	for (UInteractionReceiverComponent* Receiver : NewReceiversInField)
	{
		const FVector ReceiverLocation = Receiver->GetReceiverTransform().GetLocation();
		const FVector DirectionToReceiver = (ReceiverLocation - InstigatorLocation).GetSafeNormal2D();

		if (const float DotProduct = FVector::DotProduct(InstigatorForward, DirectionToReceiver); DotProduct >= MinDotProduct)
		{
			NewReceiversInView.Add(Receiver);
		}
	}

	NewReceiversInView.Sort([InstigatorLocation](const UInteractionReceiverComponent& A, const UInteractionReceiverComponent& B) {
		const float DistA = FMath::Max(0.0f, FVector::Dist2D(InstigatorLocation, A.GetReceiverTransform().GetLocation()) - A.ReceiverRadius);
		const float DistB = FMath::Max(0.0f, FVector::Dist2D(InstigatorLocation, B.GetReceiverTransform().GetLocation()) - B.ReceiverRadius);
		return DistA < DistB;
	});

	bool bViewChanged = (ReceiversInView.Num() != NewReceiversInView.Num());
	if (!bViewChanged)
	{
		for (int32 Index = 0; Index < ReceiversInView.Num(); ++Index)
		{
			if (ReceiversInView[Index] != NewReceiversInView[Index])
			{
				bViewChanged = true;
				break;
			}
		}
	}

	ReceiversInView = NewReceiversInView;

	UInteractionReceiverComponent* NewBestReceiver = nullptr;
	if (ReceiversInView.Num() > 0)
	{
		if (bViewChanged || !ReceiversInView.IsValidIndex(CurrentBestReceiverIndex))
		{
			CurrentBestReceiverIndex = 0;
		}
		NewBestReceiver = ReceiversInView[CurrentBestReceiverIndex];
	}
	else
	{
		CurrentBestReceiverIndex = 0;
	}

	if (UInteractionReceiverComponent* OldBestReceiver = Cast<UInteractionReceiverComponent>(CurrentBestFittingReceiver.InteractionComponent); OldBestReceiver != NewBestReceiver)
	{
		if (OldBestReceiver)
		{
			OldBestReceiver->OnIsNotBestFitting.Broadcast();
		}

		if (NewBestReceiver)
		{
			NewBestReceiver->OnIsBestFitting.Broadcast();
		}

		CurrentBestFittingReceiver.InteractionComponent = NewBestReceiver;
		CurrentBestFittingReceiver.InteractionActor = NewBestReceiver ? NewBestReceiver->GetOwner() : nullptr;
	}

	if (Instigator)
	{
		RemovedReceivers.Empty();
		for (UInteractionReceiverComponent* Receiver : ReceiversInField)
		{
			if (!NewReceiversInField.Contains(Receiver))
			{
				RemovedReceivers.Add(Receiver);
				if (Receiver)
				{
					Receiver->OnLeavesInteractionField.Broadcast(InstigatorActor, FinalInstigatorName);
				}
			}
		}
	}

	ReceiversInField = NewReceiversInField;

	for (UInteractionInstigatorComponent* InstigatorComp : InstigatorsOfLevel)
	{
		if (InstigatorComp && InstigatorComp->bShowDebugging)
		{
			const FVector CurrentInstigatorLocation = InstigatorComp->GetInstigatorTransform().GetLocation();
			FRotator CurrentInstigatorRotation = InstigatorComp->GetInstigatorTransform().Rotator();
			CurrentInstigatorRotation.Yaw += InstigatorComp->InstigatorOffsetViewRotation;
			
			const FVector DebugLocation = CurrentInstigatorLocation + FVector(0, 0, 10);

			DrawDebugCircle(World, DebugLocation, Settings->InteractionRange, 36, FColor::Yellow, false, -1, 0, 2, FVector(1, 0, 0), FVector(0, 1, 0), false);

			const float HalfFoVInDegrees = Settings->InteractionFoV * 0.5f;
			const FVector Forward = CurrentInstigatorRotation.Vector();
			const FVector RightBound = Forward.RotateAngleAxis(HalfFoVInDegrees, FVector::UpVector);
			const FVector LeftBound = Forward.RotateAngleAxis(-HalfFoVInDegrees, FVector::UpVector);

			DrawDebugLine(World, DebugLocation, DebugLocation + RightBound * Settings->InteractionRange, FColor::Red, false, -1, 0, 2);
			DrawDebugLine(World, DebugLocation, DebugLocation + LeftBound * Settings->InteractionRange, FColor::Red, false, -1, 0, 2);

			constexpr int32 ArcSegments = 12;
			FVector LastPoint = DebugLocation + LeftBound * Settings->InteractionRange;
			for (int32 i = 1; i <= ArcSegments; ++i)
			{
				const float CurrentAngle = -HalfFoVInDegrees + (Settings->InteractionFoV * i / ArcSegments);
				const FVector CurrentPoint = DebugLocation + Forward.RotateAngleAxis(CurrentAngle, FVector::UpVector) * Settings->InteractionRange;
				DrawDebugLine(World, LastPoint, CurrentPoint, FColor::Red, false, -1, 0, 2);
				LastPoint = CurrentPoint;
			}
		}
	}

	if (AddedReceivers.Num() > 0)
	{
		OnFoundReceivers.Broadcast(AddedReceivers);
	}

	if (RemovedReceivers.Num() > 0)
	{
		OnLostReceivers.Broadcast(RemovedReceivers);
	}
}

void UBDC_InteractionSubsystem::GetAllReceiversField(TArray<UInteractionReceiverComponent*>& Receivers) const
{
	Receivers = ReceiversInField;
}

void UBDC_InteractionSubsystem::GetAllReceiversOfLevel(TArray<FInteractionReceivers>& Receivers) const
{
	Receivers = ReceiversOfLevel;
}

void UBDC_InteractionSubsystem::GetReceiverByTag(FGameplayTag OfReceiverTag, FInteractionReceivers& ReceiverData) const
{
	for (const FInteractionReceivers& R : ReceiversOfLevel)
	{
		if (const UInteractionReceiverComponent* Comp = Cast<UInteractionReceiverComponent>(R.InteractionComponent))
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

void UBDC_InteractionSubsystem::GetReceiverByName(FName OfReceiverName, FInteractionReceivers& ReceiverData) const
{
	for (const FInteractionReceivers& R : ReceiversOfLevel)
	{
		if (UInteractionReceiverComponent* Comp = Cast<UInteractionReceiverComponent>(R.InteractionComponent))
		{
			if (Comp->NameOfReceiver == OfReceiverName)
			{
				ReceiverData = R;
				return;
			}
		}
	}
	ReceiverData = FInteractionReceivers();
}

void UBDC_InteractionSubsystem::GetInstigatorByTag(FGameplayTag OfInstigatorTag, FInteractionReceivers& InstigatorData) const
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

void UBDC_InteractionSubsystem::GetInstigatorByName(FName OfInstigatorName, FInteractionReceivers& InstigatorData) const
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
	ReceiversInView.Remove(ReceiverComponent);
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

void UBDC_InteractionSubsystem::GetAllReceiversInView(TArray<FInteractionReceivers>& OutReceiversInView) const
{
	OutReceiversInView.Empty();
	for (UInteractionReceiverComponent* Comp : ReceiversInView)
	{
		if (Comp)
		{
			FInteractionReceivers Data;
			Data.InteractionActor = Comp->GetOwner();
			Data.InteractionComponent = Comp;
			OutReceiversInView.Add(Data);
		}
	}
}

void UBDC_InteractionSubsystem::CalcNextBest()
{
	if (ReceiversInView.Num() <= 1) return;

	if (const UInteractionReceiverComponent* OldBestReceiver = Cast<UInteractionReceiverComponent>(CurrentBestFittingReceiver.InteractionComponent))
	{
		OldBestReceiver->OnIsNotBestFitting.Broadcast();
	}

	CurrentBestReceiverIndex = (CurrentBestReceiverIndex + 1) % ReceiversInView.Num();

	if (UInteractionReceiverComponent* NewBestReceiver = ReceiversInView[CurrentBestReceiverIndex])
	{
		NewBestReceiver->OnIsBestFitting.Broadcast();
		CurrentBestFittingReceiver.InteractionComponent = NewBestReceiver;
		CurrentBestFittingReceiver.InteractionActor = NewBestReceiver->GetOwner();
	}
}

void UBDC_InteractionSubsystem::CalcPrevBest()
{
	if (ReceiversInView.Num() <= 1) return;

	if (const UInteractionReceiverComponent* OldBestReceiver = Cast<UInteractionReceiverComponent>(CurrentBestFittingReceiver.InteractionComponent))
	{
		OldBestReceiver->OnIsNotBestFitting.Broadcast();
	}

	CurrentBestReceiverIndex = (CurrentBestReceiverIndex - 1 + ReceiversInView.Num()) % ReceiversInView.Num();

	if (UInteractionReceiverComponent* NewBestReceiver = ReceiversInView[CurrentBestReceiverIndex])
	{
		NewBestReceiver->OnIsBestFitting.Broadcast();
		CurrentBestFittingReceiver.InteractionComponent = NewBestReceiver;
		CurrentBestFittingReceiver.InteractionActor = NewBestReceiver->GetOwner();
	}
}

void UBDC_InteractionSubsystem::GetCurrentBestFitting(FInteractionReceivers& BestFit) const
{
	BestFit = CurrentBestFittingReceiver;
}