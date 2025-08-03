// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/EnemyWaveGameMode.h"
#include "Aura.h"
#include "Templates/UnrealTemplate.h"

AEnemyWaveGameMode::AEnemyWaveGameMode() {
    // NextWaveDelegate.AddDynamic(this, &ThisClass::NextWave);
}

FOnSpawnEnemiesBroadcast& AEnemyWaveGameMode::GetSpawnEnemiesDelegate() {
    return SpawnEnemiesDelegate;
}

void AEnemyWaveGameMode::Tick(float DeltaSeconds) {
}

void AEnemyWaveGameMode::NextWave() {
    check(WaveInfo);

    TArray<FWaveLevel> Levels = WaveInfo->Levels;
    if (Levels.IsValidIndex(CurrentLevel)) {
        FWaveLevel& WaveLevel = Levels[CurrentLevel];
        int32 SpawnEnemyLevel = WaveLevel.Level;
        auto& Waves = WaveLevel.Waves;

        if (Waves.IsValidIndex(CurrentWave)) {
            FWave& Wave = Waves[CurrentWave];
            FTimerHandle TimerHandle;

            WaveStart(Wave.WaveMessage, Wave.WaitTime, ++WaveCount);

            // TODO 解决这里产生的复制
            GetWorld()->GetTimerManager().SetTimer(TimerHandle, [Wave, SpawnEnemyLevel, this] () {
                TArray<FEnemyInfo> EnemySpawnInfos = Wave.EnemySpawnInfos;
                // call delegate
                SpawnEnemiesDelegate.Broadcast(SpawnEnemyLevel, EnemySpawnInfos);
            }, Wave.WaitTime, false);
            
            // TODO 需要手动清理Timer吗

        }

        

        if (++CurrentWave >= Waves.Num()) {
            CurrentWave = 0;
            ++CurrentLevel;
        }

        if (CurrentLevel >= Levels.Num()) {
            // Compeleted All Levels
        }

    }
}

