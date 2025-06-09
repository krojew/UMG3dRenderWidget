// Copyright 2025 Kamil Rojewski. All Rights Reserved.


#include "PocketWorldsBridgeSubsystem.h"

#include "PocketLevelInstance.h"
#include "PocketLevelSystem.h"
#include "Kismet/GameplayStatics.h"
#include "PocketLevelStageManager.h"
#include "TaggedPocketLevel.h"

UPocketLevelInstance* UPocketWorldsBridgeSubsystem::SpawnPocketLevel(ULocalPlayer& LocalPlayer,
                                                                     UTaggedPocketLevel& PocketLevel,
                                                                     const FVector& DesiredSpawnPoint)
{
    if (const auto Instance = SpawnedLevels.Find(PocketLevel.Tag))
    {
        return *Instance;
    }

    const auto Instance = GetWorld()->GetSubsystem<UPocketLevelSubsystem>()->GetOrCreatePocketLevelFor(
        &LocalPlayer, &PocketLevel, DesiredSpawnPoint);

    SpawnedLevels.Emplace(PocketLevel.Tag, Instance);

    return Instance;
}

UPocketLevelInstance* UPocketWorldsBridgeSubsystem::GetLevelInstance(const FGameplayTag& Tag) const
{
    const auto Instance = SpawnedLevels.Find(Tag);
    return Instance ? *Instance : nullptr;
}

APocketLevelStageManager* UPocketWorldsBridgeSubsystem::GetStageManager(const FGameplayTag& Tag) const
{
    const auto Instance = GetLevelInstance(Tag);
    return Instance
               ? Cast<APocketLevelStageManager>(
                   UGameplayStatics::GetActorOfClass(Instance->GetWorld(), APocketLevelStageManager::StaticClass()))
               : nullptr;
}
