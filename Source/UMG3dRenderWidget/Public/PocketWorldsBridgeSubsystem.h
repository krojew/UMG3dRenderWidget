// Copyright 2025 Kamil Rojewski. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/WorldSubsystem.h"
#include "PocketWorldsBridgeSubsystem.generated.h"

class APocketLevelStageManager;
class UPocketLevelInstance;
class UTaggedPocketLevel;

/**
 * Bridge between pocket worlds and current game.
 */
UCLASS()
class UMG3DRENDERWIDGET_API UPocketWorldsBridgeSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UPocketLevelInstance* SpawnPocketLevel(ULocalPlayer& LocalPlayer, UTaggedPocketLevel& PocketLevel,
                                           const FVector& DesiredSpawnPoint);

    UPocketLevelInstance* GetLevelInstance(const FGameplayTag& Tag) const;

    UFUNCTION(BlueprintCallable, BlueprintPure)
    APocketLevelStageManager* GetStageManager(const FGameplayTag& Tag) const;

private:
    UPROPERTY(Transient)
    TMap<FGameplayTag, TObjectPtr<UPocketLevelInstance>> SpawnedLevels;
};
