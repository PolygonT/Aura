// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MMC/MMC_MaxHealth.h"
#include "AbilitySystem/DefaultAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth() {
    VigorDef.AttributeToCapture = UDefaultAttributeSet::GetVigorAttribute();
    VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
    VigorDef.bSnapshot = false;

    RelevantAttributesToCapture.Add(MoveTemp(VigorDef));
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(
    const FGameplayEffectSpec &Spec) const {
    auto SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    auto TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters EvaluationParameters;

    EvaluationParameters.SourceTags = SourceTags;
    EvaluationParameters.TargetTags = TargetTags;

    float Vigor;
    GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);
    Vigor = FMath::Max<float>(Vigor, 0);

    const auto PlayerLevel = 
        Cast<ICombatInterface>(Spec.GetContext().GetSourceObject())->GetPlayerLevel();
    return 80.f + 2.5f * Vigor + 10.f * PlayerLevel;
}
