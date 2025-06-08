// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "DerivedAura.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API ADerivedAura : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
    ADerivedAura();

    void PossessedBy(AController *NewController) override;

    void OnRep_PlayerState() override;

    // ===== CombatInterface
    virtual int32 GetPlayerLevel() override;

    // protected:
    // UPROPERTY()
    // TObjectPtr<ADefaultPlayerState> AuraPlayerState;
  private:
    virtual void InitAbilityActorInfo() override;
};
