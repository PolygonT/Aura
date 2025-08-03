// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultGameModeBase.h"

void ADefaultGameModeBase::OnEnemySpawn() { 
    OnEnemyCountChange.Broadcast(++LevelEnemyCount);
}

void ADefaultGameModeBase::OnEnemyDestory() { 
    OnEnemyCountChange.Broadcast(--LevelEnemyCount);
}

