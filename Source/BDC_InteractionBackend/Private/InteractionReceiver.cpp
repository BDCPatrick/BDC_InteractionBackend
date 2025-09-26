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
#include "InteractionReceiver.h"
#include "BDC_InteractionSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "Components/SceneComponent.h"
#include "Components/PrimitiveComponent.h"

UInteractionReceiverComponent::UInteractionReceiverComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractionReceiverComponent::BeginPlay()
{
    bAutoActivate = true;
	Super::BeginPlay();

    if (UWorld* World = GetWorld())
    {
        if (UGameInstance* GameInstance = World->GetGameInstance())
        {
            if (UBDC_InteractionSubsystem* InteractionSubsystem = GameInstance->GetSubsystem<UBDC_InteractionSubsystem>())
            {
                InteractionSubsystem->AddReceiver(this);
            }
        }
    }
}

void UInteractionReceiverComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (UWorld* World = GetWorld())
    {
        if (UGameInstance* GameInstance = World->GetGameInstance())
        {
            if (UBDC_InteractionSubsystem* InteractionSubsystem = GameInstance->GetSubsystem<UBDC_InteractionSubsystem>())
            {
                InteractionSubsystem->RemoveReceiver(this);
            }
        }
    }

    Super::EndPlay(EndPlayReason);
}

FVector UInteractionReceiverComponent::GetInteractionWorldLocation() const
{
    // 1) Explicitly tracked component wins
    if (TrackingComponent)
    {
        return TrackingComponent->GetComponentLocation();
    }

    const AActor* Owner = GetOwner();
    if (!Owner)
    {
        return FVector::ZeroVector;
    }

    // 2) If the root itself simulates physics, trust it
    if (const USceneComponent* Root = Owner->GetRootComponent())
    {
        if (const UPrimitiveComponent* PrimRoot = Cast<const UPrimitiveComponent>(Root))
        {
            if (PrimRoot->IsSimulatingPhysics())
            {
                return Root->GetComponentLocation();
            }
        }
    }

    // 3) Otherwise, try to find any simulating primitive component (e.g., a physics StaticMeshComponent)
    TArray<const UActorComponent*> PrimitiveComponents;
    Owner->GetComponents(UPrimitiveComponent::StaticClass(), PrimitiveComponents);
    for (const UActorComponent* Comp : PrimitiveComponents)
    {
        const UPrimitiveComponent* Prim = Cast<const UPrimitiveComponent>(Comp);
        if (Prim && Prim->IsSimulatingPhysics())
        {
            return Prim->GetComponentLocation();
        }
    }

    // 4) If no simulating primitive, prefer any Movable scene component (covers sprites, billboards, particle systems, skeletal meshes not simulating, etc.)
    TArray<USceneComponent*> SceneComponents;
    Owner->GetComponents<USceneComponent>(SceneComponents);
    for (const USceneComponent* SC : SceneComponents)
    {
        if (SC && SC->Mobility == EComponentMobility::Movable)
        {
            return SC->GetComponentLocation();
        }
    }

    // 5) Otherwise, pick the primitive component with the largest bounds as a reasonable proxy for the actor’s visual/movable center
    const UPrimitiveComponent* BestPrim = nullptr;
    float BestRadius = -1.f;
    for (const UActorComponent* Comp : PrimitiveComponents)
    {
        const UPrimitiveComponent* Prim = Cast<const UPrimitiveComponent>(Comp);
        if (Prim)
        {
            const float Radius = Prim->Bounds.SphereRadius;
            if (Radius > BestRadius)
            {
                BestRadius = Radius;
                BestPrim = Prim;
            }
        }
    }
    if (BestPrim)
    {
        return BestPrim->GetComponentLocation();
    }

    // 6) Fallbacks: root component, then actor location
    if (const USceneComponent* Root = Owner->GetRootComponent())
    {
        return Root->GetComponentLocation();
    }

    return Owner->GetActorLocation();
}

FVector UInteractionReceiverComponent::GetSurfacePointToward(const FVector& FromWorld) const
{
    const FVector Center = GetInteractionWorldLocation();
    if (BoundsExtent.IsNearlyZero())
    {
        return Center;
    }

    const USceneComponent* Basis = TrackingComponent ? TrackingComponent : (GetOwner() ? GetOwner()->GetRootComponent() : nullptr);
    const FQuat Rot = Basis ? Basis->GetComponentQuat() : FQuat::Identity;

    const FVector LocalDelta = Rot.UnrotateVector(FromWorld - Center);

    const FVector Clamped(
        FMath::Clamp(LocalDelta.X, -BoundsExtent.X, BoundsExtent.X),
        FMath::Clamp(LocalDelta.Y, -BoundsExtent.Y, BoundsExtent.Y),
        FMath::Clamp(LocalDelta.Z, -BoundsExtent.Z, BoundsExtent.Z));

    const FVector LocalPoint = Clamped;
    const FVector WorldPoint = Center + Rot.RotateVector(LocalPoint);
    return WorldPoint;
}