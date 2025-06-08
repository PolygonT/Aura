// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MMC/MMC_MaxMana.h"
#include "AbilitySystem/DefaultAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana() {
    IntelligenceDef.AttributeToCapture = UDefaultAttributeSet::GetIntelligenceAttribute();
    IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
    IntelligenceDef.bSnapshot = false;

    RelevantAttributesToCapture.Add(MoveTemp(IntelligenceDef));
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(
    const FGameplayEffectSpec &Spec) const {

    auto SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    auto TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters EvaluationParameters;

    EvaluationParameters.SourceTags = SourceTags;
    EvaluationParameters.TargetTags = TargetTags;

    float Intelligence;
    GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParameters, Intelligence);
    Intelligence = FMath::Max<float>(Intelligence, 0);

    const auto PlayerLevel = 
        Cast<ICombatInterface>(Spec.GetContext().GetSourceObject())->GetPlayerLevel();
    return 80.f + 2.5f * Intelligence + 10.f * PlayerLevel;
}
