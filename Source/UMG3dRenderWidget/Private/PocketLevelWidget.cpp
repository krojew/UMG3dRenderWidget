// Copyright 2025 Kamil Rojewski. All Rights Reserved.


#include "PocketLevelWidget.h"

#include "PocketCapture.h"
#include "PocketLevelInstance.h"
#include "PocketLevelStageManager.h"
#include "Components/Image.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"
#include "PocketWorldsBridgeSubsystem.h"
#include "TaggedPocketLevel.h"

void UPocketLevelWidget::NativePreConstruct()
{
    Super::NativePreConstruct();

    if (Image)
    {
        Image->SetBrushFromMaterial(CameraRenderMaterial);
    }
}

void UPocketLevelWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!PocketLevel)
    {
        return;
    }

    const auto Bridge = GetWorld()->GetSubsystem<UPocketWorldsBridgeSubsystem>();
    const auto Instance = Bridge->SpawnPocketLevel(*GetOwningLocalPlayer(), *PocketLevel, DesiredSpawnPoint);

    PocketLevelReadyDelegateHandle = Instance->AddReadyCallback(
        FPocketLevelInstanceEvent::FDelegate::CreateUObject(this, &UPocketLevelWidget::PrepareCapture));

    Instance->StreamIn();
}

void UPocketLevelWidget::NativeDestruct()
{
    if (PocketLevel)
    {
        const auto Bridge = GetWorld()->GetSubsystem<UPocketWorldsBridgeSubsystem>();
        if (const auto Instance = Bridge->GetLevelInstance(PocketLevel->Tag))
        {
            Instance->RemoveReadyCallback(PocketLevelReadyDelegateHandle);
            Instance->StreamOut();
        }
    }

    Super::NativeDestruct();
}

void UPocketLevelWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (StageManager)
    {
        StageManager->CaptureDiffuse();
        StageManager->CaptureAlphaMask();
    }
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void UPocketLevelWidget::PrepareCapture(UPocketLevelInstance* Instance)
{
    if (!Image)
    {
        return;
    }

    StageManager = Cast<APocketLevelStageManager>(
        UGameplayStatics::GetActorOfClass(Instance->GetWorld(), APocketLevelStageManager::StaticClass()));

    const auto PocketCapture = StageManager->GetPocketCapture();

    const auto Material = Image->GetDynamicMaterial();
    Material->SetTextureParameterValue("Diffuse", PocketCapture->GetOrCreateDiffuseRenderTarget());
    Material->SetTextureParameterValue("AlphaMask", PocketCapture->GetOrCreateAlphaMaskRenderTarget());
}
