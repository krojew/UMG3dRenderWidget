// Copyright 2025 Kamil Rojewski. All Rights Reserved.


#include "PocketLevelStageManager.h"

#include "PocketCapture.h"
#include "PocketCaptureSubsystem.h"
#include "Camera/CameraComponent.h"


APocketLevelStageManager::APocketLevelStageManager()
{
    PrimaryActorTick.bCanEverTick = false;

    Scene = CreateDefaultSubobject<USceneComponent>("Scene");
    Scene->SetMobility(EComponentMobility::Static);
    RootComponent = Scene;

    Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
    Camera->SetupAttachment(RootComponent);
}

void APocketLevelStageManager::CaptureDiffuse() const
{
    PocketCapture->CaptureDiffuse();
}

void APocketLevelStageManager::CaptureAlphaMask()
{
    const auto OriginalMaterials = SetActorAlphaMaskAndReturnOriginal();
    PocketCapture->CaptureAlphaMask();
    RestoreActorMaterials(OriginalMaterials);
}

void APocketLevelStageManager::BeginPlay()
{
    Super::BeginPlay();

    GetAttachedActors(AttachedActors);

    const auto PocketWorlds = GetWorld()->GetSubsystem<UPocketCaptureSubsystem>();
    PocketCapture = PocketWorlds->CreateThumbnailRenderer(PocketCaptureClass);
    PocketCapture->SetRenderTargetSize(CaptureWidth, CaptureHeight);
    PocketCapture->SetCaptureTarget(this);
    PocketCapture->SetAlphaMaskedActors(AttachedActors);
}

TArray<UMaterialInterface*> APocketLevelStageManager::SetActorAlphaMaskAndReturnOriginal()
{
    if (!ActorAlphaMask)
    {
        return {};
    }

    TArray<UMaterialInterface*> Materials;

    for (const auto& Actor : AttachedActors)
    {
        if (ExcludedFromAlphaMask.Contains(Actor))
        {
            continue;
        }

        for (TInlineComponentArray<UPrimitiveComponent*> Components(Actor); const auto Component : Components)
        {
            const auto NumMaterials = Component->GetNumMaterials();
            Materials.Reserve(Materials.Num() + NumMaterials);

            for (auto i = 0; i < NumMaterials; ++i)
            {
                Materials.Emplace(Component->GetMaterial(i));
                Component->SetMaterial(i, ActorAlphaMask);
            }
        }
    }

    return Materials;
}

void APocketLevelStageManager::RestoreActorMaterials(const TArray<UMaterialInterface*>& OriginalMaterials)
{
    if (OriginalMaterials.IsEmpty())
    {
        return;
    }

    auto MaterialIt = OriginalMaterials.CreateConstIterator();

    for (const auto& Actor : AttachedActors)
    {
        if (ExcludedFromAlphaMask.Contains(Actor))
        {
            continue;
        }

        for (TInlineComponentArray<UPrimitiveComponent*> Components(Actor); const auto Component : Components)
        {
            const auto NumMaterials = Component->GetNumMaterials();
            for (auto i = 0; i < NumMaterials; ++i)
            {
                Component->SetMaterial(i, *MaterialIt);
                ++MaterialIt;
            }
        }
    }
}
