// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "DefaultAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UDefaultAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
    static UDefaultAssetManager &Get();

protected:
    virtual void StartInitialLoading() override;
};
