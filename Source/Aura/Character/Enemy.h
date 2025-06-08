// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Interaction/EnemyInterface.h"
#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AEnemy : public ABaseCharacter, public IEnemyInterface
{
	GENERATED_BODY()
	
public:
    AEnemy();

    // ==== Enemy Interface ====
    virtual void HighlightActor() override;
    virtual void UnHighlightActor() override;

    // ==== Combat Interface ====
    virtual int32 GetPlayerLevel() override;

  protected:
    virtual void BeginPlay() override;

    virtual void InitAbilityActorInfo() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
    int32 PlayerLevel = 1;
};
