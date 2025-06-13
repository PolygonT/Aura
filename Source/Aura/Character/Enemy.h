// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Enemy.generated.h"

class UWidgetComponent;

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

    // // ==== Default WidgetController Override ====
    // virtual void BroadcastInitialValues() override;
    //
    // virtual void BindCallbacksToDependencies() override;

    // ==== Member Variable ====
    UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
    FOnFloatValueChangedSignature OnMaxHealthChanged;

    UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
    FOnFloatValueChangedSignature OnHealthChanged;

    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    bool bHitReacting { false };

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    float BaseWalkSpped { 250.f };

protected:
    virtual void BeginPlay() override;

    virtual void InitAbilityActorInfo() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
    int32 PlayerLevel = 1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UWidgetComponent> HealthBar;

};

