// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DefaultGameModeBase.generated.h"

class UCharacterClassInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyCountChangeSignature, int32, EnemyCount);

/**
 * 
 */
UCLASS()
class AURA_API ADefaultGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
    TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

    void OnEnemySpawn();

    void OnEnemyDestory();

  protected:
    int32 LevelEnemyCount {0};

    UPROPERTY(BlueprintAssignable)
    FOnEnemyCountChangeSignature OnEnemyCountChange;
};
