// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/Enemy.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyWaveInfo.generated.h"

USTRUCT()
struct FEnemyInfo {
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AEnemy> EnemyClass;

    int32 EnemyLevel;

    UPROPERTY(EditDefaultsOnly)
    int32 EnemyLevelDeviation;

    UPROPERTY(EditDefaultsOnly)
    int32 SpawnNums;

    UPROPERTY(EditDefaultsOnly)
    int32 SpawnNumsDeviation;
};

USTRUCT()
struct FWave {
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    float WaitTime;

    UPROPERTY(EditDefaultsOnly)
    TArray<FEnemyInfo> EnemySpawnInfos;


};

USTRUCT()
struct FWaveLevel {
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    int32 Level;

    UPROPERTY(EditDefaultsOnly)
    
    TArray<FWave> Waves;

    // TODO Message Prompt
};


/**
 * 
 */
UCLASS()
class AURA_API UEnemyWaveInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditDefaultsOnly)
    TArray<FWaveLevel> Levels;
};
