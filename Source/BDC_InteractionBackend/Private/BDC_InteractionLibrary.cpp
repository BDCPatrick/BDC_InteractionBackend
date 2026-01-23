/* Copyright © beginning at 2026 - BlackDevilCreations
 * Author: Patrick Wenzel
 * All rights reserved.
 * This file is part of a BlackDevilCreations project and may not be distributed, copied,
 * or modified without prior written permission from BlackDevilCreations.
 * Unreal Engine and its associated trademarks are property of Epic Games, Inc.
 * and are used with permission.
 */
#include "BDC_InteractionLibrary.h"
#include "BDC_InteractionSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"

void UBDC_InteractionLibrary::SetInstigator(const UObject* WorldContextObject, UInteractionInstigatorComponent* NewInstigator)
{
	if (WorldContextObject)
	{
		if (const UWorld* World = WorldContextObject->GetWorld())
		{
			if (const UGameInstance* GI = World->GetGameInstance())
			{
				if (UBDC_InteractionSubsystem* Subsystem = GI->GetSubsystem<UBDC_InteractionSubsystem>())
				{
					Subsystem->SetInstigator(NewInstigator);
				}
			}
		}
	}
}

void UBDC_InteractionLibrary::GetLastInteraction(const UObject* WorldContextObject, FInteractionReceivers& LastReceiver)
{
	if (WorldContextObject)
	{
		if (const UWorld* World = WorldContextObject->GetWorld())
		{
			if (const UGameInstance* GI = World->GetGameInstance())
			{
				if (const UBDC_InteractionSubsystem* Subsystem = GI->GetSubsystem<UBDC_InteractionSubsystem>())
				{
					Subsystem->GetLastInteraction(LastReceiver);
				}
			}
		}
	}
}

void UBDC_InteractionLibrary::InjectInteraction(const UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (const UWorld* World = WorldContextObject->GetWorld())
		{
			if (const UGameInstance* GI = World->GetGameInstance())
			{
				if (UBDC_InteractionSubsystem* Subsystem = GI->GetSubsystem<UBDC_InteractionSubsystem>())
				{
					Subsystem->InjectInteraction();
				}
			}
		}
	}
}

void UBDC_InteractionLibrary::UpdateInteractions(const UObject* WorldContextObject, FVector InstigatorLocation, FRotator InstigatorRotation)
{
	if (WorldContextObject)
	{
		if (const UWorld* World = WorldContextObject->GetWorld())
		{
			if (const UGameInstance* GI = World->GetGameInstance())
			{
				if (UBDC_InteractionSubsystem* Subsystem = GI->GetSubsystem<UBDC_InteractionSubsystem>())
				{
					Subsystem->UpdateInteractions(InstigatorLocation, InstigatorRotation);
				}
			}
		}
	}
}

void UBDC_InteractionLibrary::GetAllReceiversField(const UObject* WorldContextObject, TArray<UInteractionReceiverComponent*>& Receivers)
{
	if (WorldContextObject)
	{
		if (const UWorld* World = WorldContextObject->GetWorld())
		{
			if (const UGameInstance* GI = World->GetGameInstance())
			{
				if (UBDC_InteractionSubsystem* Subsystem = GI->GetSubsystem<UBDC_InteractionSubsystem>())
				{
					Subsystem->GetAllReceiversField(Receivers);
				}
			}
		}
	}
}

void UBDC_InteractionLibrary::GetAllReceiversOfLevel(const UObject* WorldContextObject, TArray<FInteractionReceivers>& Receivers)
{
	if (WorldContextObject)
	{
		if (const UWorld* World = WorldContextObject->GetWorld())
		{
			if (const UGameInstance* GI = World->GetGameInstance())
			{
				if (const UBDC_InteractionSubsystem* Subsystem = GI->GetSubsystem<UBDC_InteractionSubsystem>())
				{
					Subsystem->GetAllReceiversOfLevel(Receivers);
				}
			}
		}
	}
}

void UBDC_InteractionLibrary::GetReceiverByTag(const UObject* WorldContextObject, FGameplayTag OfReceiverTag, FInteractionReceivers& Receiver)
{
	if (WorldContextObject)
	{
		if (const UWorld* World = WorldContextObject->GetWorld())
		{
			if (const UGameInstance* GI = World->GetGameInstance())
			{
				if (const UBDC_InteractionSubsystem* Subsystem = GI->GetSubsystem<UBDC_InteractionSubsystem>())
				{
					Subsystem->GetReceiverByTag(OfReceiverTag, Receiver);
				}
			}
		}
	}
}

void UBDC_InteractionLibrary::GetReceiverByName(const UObject* WorldContextObject, FName OfReceiverName, FInteractionReceivers& Receiver)
{
	if (WorldContextObject)
	{
		if (const UWorld* World = WorldContextObject->GetWorld())
		{
			if (const UGameInstance* GI = World->GetGameInstance())
			{
				if (const UBDC_InteractionSubsystem* Subsystem = GI->GetSubsystem<UBDC_InteractionSubsystem>())
				{
					Subsystem->GetReceiverByName(OfReceiverName, Receiver);
				}
			}
		}
	}
}

void UBDC_InteractionLibrary::GetInstigatorByTag(const UObject* WorldContextObject, FGameplayTag OfInstigatorTag, FInteractionReceivers& Instigator)
{
	if (WorldContextObject)
	{
		if (const UWorld* World = WorldContextObject->GetWorld())
		{
			if (const UGameInstance* GI = World->GetGameInstance())
			{
				if (const UBDC_InteractionSubsystem* Subsystem = GI->GetSubsystem<UBDC_InteractionSubsystem>())
				{
					Subsystem->GetInstigatorByTag(OfInstigatorTag, Instigator);
				}
			}
		}
	}
}

void UBDC_InteractionLibrary::GetInstigatorByName(const UObject* WorldContextObject, FName OfInstigatorName, FInteractionReceivers& Instigator)
{
	if (WorldContextObject)
	{
		if (const UWorld* World = WorldContextObject->GetWorld())
		{
			if (const UGameInstance* GI = World->GetGameInstance())
			{
				if (const UBDC_InteractionSubsystem* Subsystem = GI->GetSubsystem<UBDC_InteractionSubsystem>())
				{
					Subsystem->GetInstigatorByName(OfInstigatorName, Instigator);
				}
			}
		}
	}
}

void UBDC_InteractionLibrary::GetAllReceiversInView(const UObject* WorldContextObject, TArray<FInteractionReceivers>& OutReceiversInView)
{
	if (WorldContextObject)
	{
		if (const UWorld* World = WorldContextObject->GetWorld())
		{
			if (const UGameInstance* GI = World->GetGameInstance())
			{
				if (const UBDC_InteractionSubsystem* Subsystem = GI->GetSubsystem<UBDC_InteractionSubsystem>())
				{
					Subsystem->GetAllReceiversInView(OutReceiversInView);
				}
			}
		}
	}
}

void UBDC_InteractionLibrary::CalcNextBest(const UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (const UWorld* World = WorldContextObject->GetWorld())
		{
			if (const UGameInstance* GI = World->GetGameInstance())
			{
				if (UBDC_InteractionSubsystem* Subsystem = GI->GetSubsystem<UBDC_InteractionSubsystem>())
				{
					Subsystem->CalcNextBest();
				}
			}
		}
	}
}

void UBDC_InteractionLibrary::CalcPrevBest(const UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (const UWorld* World = WorldContextObject->GetWorld())
		{
			if (const UGameInstance* GI = World->GetGameInstance())
			{
				if (UBDC_InteractionSubsystem* Subsystem = GI->GetSubsystem<UBDC_InteractionSubsystem>())
				{
					Subsystem->CalcPrevBest();
				}
			}
		}
	}
}

void UBDC_InteractionLibrary::GetCurrentBestFitting(const UObject* WorldContextObject, FInteractionReceivers& BestFit)
{
	if (WorldContextObject)
	{
		if (const UWorld* World = WorldContextObject->GetWorld())
		{
			if (const UGameInstance* GI = World->GetGameInstance())
			{
				if (const UBDC_InteractionSubsystem* Subsystem = GI->GetSubsystem<UBDC_InteractionSubsystem>())
				{
					Subsystem->GetCurrentBestFitting(BestFit);
				}
			}
		}
	}
}

