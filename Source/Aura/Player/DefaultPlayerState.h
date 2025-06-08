// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "DefaultPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

/**
 * 
 */
UCLASS()
class AURA_API ADefaultPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
    ADefaultPlayerState();

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    virtual void GetLifetimeReplicatedProps(
        TArray<FLifetimeProperty> &OutLifetimeProps) const override;

    UAttributeSet* GetAttributeSet() const { return AttributeSet; }

    FORCEINLINE int32 GetPlayerLevel() const { return PlayerLevel; }
protected:

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY()
    TObjectPtr<UAttributeSet> AttributeSet;

private:
    UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_PlayerLevel)
    int32 PlayerLevel = 1;

    UFUNCTION()
    void OnRep_PlayerLevel(int32 OldLevel);


};
