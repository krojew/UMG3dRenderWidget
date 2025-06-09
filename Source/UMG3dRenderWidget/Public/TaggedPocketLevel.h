// Copyright 2025 Kamil Rojewski. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PocketLevel.h"
#include "TaggedPocketLevel.generated.h"

/**
 * Pocket level with a tag.
 */
UCLASS()
class UMG3DRENDERWIDGET_API UTaggedPocketLevel : public UPocketLevel
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag Tag;
};
