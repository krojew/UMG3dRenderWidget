// Copyright 2025 Kamil Rojewski. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PocketLevelWidget.generated.h"

class APocketLevelStageManager;
class UPocketLevelInstance;
class UImage;
class UTaggedPocketLevel;

/**
 * Widget rendering a pocket level stage.
 */
UCLASS()
class UMG3DRENDERWIDGET_API UPocketLevelWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Transient, BlueprintReadOnly)
    TObjectPtr<APocketLevelStageManager> StageManager;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UImage> Image;

    virtual void NativePreConstruct() override;

    virtual void NativeConstruct() override;

    virtual void NativeDestruct() override;

    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
    UPROPERTY(EditAnywhere, Category = "Capture")
    TObjectPtr<UTaggedPocketLevel> PocketLevel;

    UPROPERTY(EditAnywhere, Category = "Capture")
    TObjectPtr<UMaterialInterface> CameraRenderMaterial;

    UPROPERTY(EditAnywhere, Category = "Capture")
    FVector DesiredSpawnPoint;

    FDelegateHandle PocketLevelReadyDelegateHandle;

    void PrepareCapture(UPocketLevelInstance* Instance);
};
