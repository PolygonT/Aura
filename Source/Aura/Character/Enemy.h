// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Enemy.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class ADefaultAIController;

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

    virtual void
    SetCombatTarget_Implementation(AActor *InCombatTarget) override;

    virtual AActor *GetCombatTarget_Implementation() const override;

    // ==== Combat Interface ====
    virtual int32 GetPlayerLevel() const override;

    virtual void Die() override;

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

    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    TObjectPtr<AActor> CombatTarget;

    UFUNCTION(BlueprintImplementableEvent)
    void CriticalHitEvent();

    virtual void PossessedBy(AController *NewController) override;

    void HitReact(const FGameplayTag GameplayTag, int32 NewCount);

  protected:
    virtual void BeginPlay() override;

    virtual void InitAbilityActorInfo() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
    int32 PlayerLevel = 1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UWidgetComponent> HealthBar;

    UPROPERTY(EditAnywhere, Category = "AI")
    TObjectPtr<UBehaviorTree> BehaviorTree;

    UPROPERTY()
    TObjectPtr<ADefaultAIController> DefaultAIController;

};

