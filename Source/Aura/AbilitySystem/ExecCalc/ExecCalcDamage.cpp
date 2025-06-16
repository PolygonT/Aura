// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalcDamage.h"
#include "AbilitySystem/DefaultAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "DefaultGameplayTags.h"
#include "GameplayEffectAggregator.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GameplayEffectTypes.h"
#include "Math/UnrealMathUtility.h"

struct DefaultDamageStatics {
    DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
    DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)
    DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration)

    DefaultDamageStatics() {

        DEFINE_ATTRIBUTE_CAPTUREDEF(UDefaultAttributeSet, Armor, Target, false /* bSnapshot */ )
        DEFINE_ATTRIBUTE_CAPTUREDEF(UDefaultAttributeSet, BlockChance, Target, false /* bSnapshot */ )
        DEFINE_ATTRIBUTE_CAPTUREDEF(UDefaultAttributeSet, ArmorPenetration, Target, false /* bSnapshot */ )
    }
};

static const DefaultDamageStatics& DamageStatics() {
    static DefaultDamageStatics DStatics;

    return DStatics;
}

UExecCalcDamage::UExecCalcDamage() {
    RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
    RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
    RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
}

void UExecCalcDamage::Execute_Implementation(
    const FGameplayEffectCustomExecutionParameters &ExecutionParams,
    FGameplayEffectCustomExecutionOutput &OutExecutionOutput) const {
    const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
    const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

    const AActor* SourceAvatarActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
    const AActor* TargetAvatarActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags(); 
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags(); 
    FAggregatorEvaluateParameters EvaluationParameters;
    EvaluationParameters.SourceTags = SourceTags;
    EvaluationParameters.TargetTags = TargetTags;

    float Damage = Spec.GetSetByCallerMagnitude(FDefaultGameplayTags::Damage);

    float Armor { 0.f };
    float BlockChance { 0.f };
    float ArmorPenetration { 0.f };
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, Armor);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, BlockChance);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParameters, ArmorPenetration);
    Armor = FMath::Max<float>(0.f, Armor);
    BlockChance = FMath::Max<float>(0.f, BlockChance);
    ArmorPenetration = FMath::Max<float>(0.f, ArmorPenetration);

    // 是否触发防御
    float bBlocked = FMath::RandRange(1, 100) <= BlockChance;
    if (bBlocked) {
        Damage /= 2.f;
    }

    // 护甲减伤

    BlockChance = FMath::Max<float>(0.f, BlockChance);
    FGameplayModifierEvaluatedData EvaluatedData { UDefaultAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage };
    OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
