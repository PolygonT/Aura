// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystem/Data/EnemyWaveInfo.h"
#include "CoreMinimal.h"
#include "Game/DefaultGameModeBase.h"
#include "EnemyWaveGameMode.generated.h"

class AEnemy;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpawnEnemiesBroadcast, int32, Level, TArray<FEnemyInfo>&, SpawnEnemyInfos);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNextWave);
/**
 * 
 */
UCLASS()
class AURA_API AEnemyWaveGameMode : public ADefaultGameModeBase
{
	GENERATED_BODY()
	
public:
    AEnemyWaveGameMode();

    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FOnSpawnEnemiesBroadcast SpawnEnemiesDelegate;

    // UPROPERTY(BlueprintCallable)
    // FNextWave NextWaveDelegate;

    UFUNCTION(BlueprintImplementableEvent)
    void WaveStart(const FText& Message, float StartTime, int32 OutWaveCount);

    FOnSpawnEnemiesBroadcast &GetSpawnEnemiesDelegate();

    virtual void Tick(float DeltaSeconds) override;

    UPROPERTY()
    TMap<int32, AEnemy*> EnemyTrackMap;

protected:
    // start from 0 as index
    int32 CurrentLevel {0};

    // start from 0 as index
    int32 CurrentWave {0};

    int32 WaveCount {0};

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UEnemyWaveInfo> WaveInfo;

    UFUNCTION(BlueprintCallable)
    void NextWave();
};
