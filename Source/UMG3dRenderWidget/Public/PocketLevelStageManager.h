// Copyright 2025 Kamil Rojewski. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PocketLevelStageManager.generated.h"

class UPocketCapture;
class UCameraComponent;

/**
 * Base for pocket level stage manager. A stage manager should define the properties of the stage, e.g. camera settings,
 * along with capture options, e.g. pocket capture settings.
 */
UCLASS(Abstract)
class UMG3DRENDERWIDGET_API APocketLevelStageManager : public AActor
{
    GENERATED_BODY()

public:
    APocketLevelStageManager();

    FORCEINLINE TObjectPtr<UPocketCapture> GetPocketCapture() const
    {
        return PocketCapture;
    }

    void CaptureDiffuse() const;
    void CaptureAlphaMask();

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
    TObjectPtr<USceneComponent> Scene;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UCameraComponent> Camera;

    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "Capture")
    int32 CaptureWidth = 1024;

    UPROPERTY(EditAnywhere, Category = "Capture")
    int32 CaptureHeight = 2048;

    /**
     * Actors which should be excluded from alpha mask rendering.
     */
    UPROPERTY(EditAnywhere, Category = "Capture")
    TArray<TObjectPtr<AActor>> ExcludedFromAlphaMask;

    /**
     * PocketWorlds capture configuration.
     */
    UPROPERTY(EditDefaultsOnly, Category = "Capture")
    TSubclassOf<UPocketCapture> PocketCaptureClass;

    /**
     * Material used by actors when rendering the alpha mask.
     */
    UPROPERTY(EditDefaultsOnly, Category = "Capture")
    TObjectPtr<UMaterialInterface> ActorAlphaMask;

    UPROPERTY(Transient)
    TObjectPtr<UPocketCapture> PocketCapture;

    UPROPERTY(Transient)
    TArray<AActor*> AttachedActors;

    TArray<UMaterialInterface*> SetActorAlphaMaskAndReturnOriginal();
    void RestoreActorMaterials(const TArray<UMaterialInterface*>& OriginalMaterials);
};
