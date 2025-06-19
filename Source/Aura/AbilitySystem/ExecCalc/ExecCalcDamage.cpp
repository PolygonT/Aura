// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalcDamage.h"
#include "AbilitySystem/DefaultAbilitySystemLibrary.h"
#include "AbilitySystem/DefaultAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "DefaultAbilityTypes.h"
#include "DefaultGameplayTags.h"
#include "GameplayEffectAggregator.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GameplayEffectTypes.h"
#include "Interaction/CombatInterface.h"
#include "Math/UnrealMathUtility.h"

struct DefaultDamageStatics {
    DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
    DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)
    DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration)
    DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance)
    DECLARE_ATTRIBUTE_CAPTUREDEF(ResistanceFire)
    DECLARE_ATTRIBUTE_CAPTUREDEF(ResistanceLightning)
    DECLARE_ATTRIBUTE_CAPTUREDEF(ResistanceArcane)
    DECLARE_ATTRIBUTE_CAPTUREDEF(ResistancePhysical)

    DefaultDamageStatics() {

        DEFINE_ATTRIBUTE_CAPTUREDEF(UDefaultAttributeSet, Armor, Target, false /* bSnapshot */ )
        DEFINE_ATTRIBUTE_CAPTUREDEF(UDefaultAttributeSet, BlockChance, Target, false /* bSnapshot */ )
        DEFINE_ATTRIBUTE_CAPTUREDEF(UDefaultAttributeSet, ArmorPenetration, Source, false /* bSnapshot */ )
        DEFINE_ATTRIBUTE_CAPTUREDEF(UDefaultAttributeSet, CriticalHitChance, Source, false /* bSnapshot */ )
        DEFINE_ATTRIBUTE_CAPTUREDEF(UDefaultAttributeSet, ResistanceFire, Target, false /* bSnapshot */ )
        DEFINE_ATTRIBUTE_CAPTUREDEF(UDefaultAttributeSet, ResistanceLightning, Target, false /* bSnapshot */ )
        DEFINE_ATTRIBUTE_CAPTUREDEF(UDefaultAttributeSet, ResistanceArcane, Target, false /* bSnapshot */ )
        DEFINE_ATTRIBUTE_CAPTUREDEF(UDefaultAttributeSet, ResistancePhysical, Target, false /* bSnapshot */ )

        TagsToCaptureDefMap.Add(FDefaultGameplayTags::Get().Attributes_Secondary_Armor, ArmorDef);
        TagsToCaptureDefMap.Add(FDefaultGameplayTags::Get().Attributes_Secondary_BlockChance, BlockChanceDef);
        TagsToCaptureDefMap.Add(FDefaultGameplayTags::Get().Attributes_Secondary_ArmorPenetration, ArmorPenetrationDef);
        TagsToCaptureDefMap.Add(FDefaultGameplayTags::Get().Attributes_Secondary_CriticalHitChance, CriticalHitChanceDef);
        TagsToCaptureDefMap.Add(FDefaultGameplayTags::Get().Attributes_Resistance_Fire, ResistanceFireDef);
        TagsToCaptureDefMap.Add(FDefaultGameplayTags::Get().Attributes_Resistance_Lightning, ResistanceLightningDef);
        TagsToCaptureDefMap.Add(FDefaultGameplayTags::Get().Attributes_Resistance_Arcane, ResistanceArcaneDef);
        TagsToCaptureDefMap.Add(FDefaultGameplayTags::Get().Attributes_Resistance_Physical, ResistancePhysicalDef);
    }

    TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefMap;
};

static const DefaultDamageStatics& DamageStatics() {
    static DefaultDamageStatics DStatics;

    return DStatics;
}

UExecCalcDamage::UExecCalcDamage() {
    RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
    RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
    RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
    RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
    RelevantAttributesToCapture.Add(DamageStatics().ResistanceFireDef);
    RelevantAttributesToCapture.Add(DamageStatics().ResistanceLightningDef);
    RelevantAttributesToCapture.Add(DamageStatics().ResistanceArcaneDef);
    RelevantAttributesToCapture.Add(DamageStatics().ResistancePhysicalDef);
}

void UExecCalcDamage::Execute_Implementation(
    const FGameplayEffectCustomExecutionParameters &ExecutionParams,
    FGameplayEffectCustomExecutionOutput &OutExecutionOutput) const {
    const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
    const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

    const AActor* SourceAvatarActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
    const AActor* TargetAvatarActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

    const ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatarActor);
    const ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatarActor);

    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags(); 
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags(); 
    FAggregatorEvaluateParameters EvaluationParameters;
    EvaluationParameters.SourceTags = SourceTags;
    EvaluationParameters.TargetTags = TargetTags;

    float Damage { 0.f };
    for (const auto& Pair : FDefaultGameplayTags::Get().DamageTypeAndResistanceMap) {
        const FGameplayTag DamageTypeTag = Pair.Key;
        const FGameplayTag ResistanceTypeTag = Pair.Value;

        checkf(DamageStatics().TagsToCaptureDefMap.Contains(ResistanceTypeTag),
               TEXT("error TagsToCaptureDefMap does't contains resistance type "
                    "tag: [%s]"),
               *ResistanceTypeTag.ToString());


        float Resistance { 0.f };

        // Get Set By Caller
        float DamageVal = Spec.GetSetByCallerMagnitude(Pair.Key);

        ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
            DamageStatics().TagsToCaptureDefMap[ResistanceTypeTag],
            EvaluationParameters, Resistance);

        Resistance = FMath::Clamp(Resistance, 0.f, 100.f);

        DamageVal *= ( 100.f - Resistance ) / 100.f;
        Damage += DamageVal;
    }

    float Armor { 0.f };
    float BlockChance { 0.f };
    float ArmorPenetration { 0.f };
    float CriticalHitChance { 0.f };
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, Armor);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, BlockChance);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParameters, ArmorPenetration);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters, CriticalHitChance);
    Armor = FMath::Max<float>(0.f, Armor);
    BlockChance = FMath::Max<float>(0.f, BlockChance);
    ArmorPenetration = FMath::Max<float>(0.f, ArmorPenetration);


    // 是否触发防御
    bool bBlocked = FMath::RandRange(1, 100) <= BlockChance;
    if (bBlocked) {
        Damage /= 2.f;
    }

    // 护甲减伤、暴击
    auto CharacterClassInfo = UDefaultAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatarActor);
    auto DamageCalcCoefficientsCT = CharacterClassInfo->DamageCalcCoefficientsCT;
    const auto ArmorPenetrationCurve = DamageCalcCoefficientsCT->FindCurve("ArmorPenetrationCoefficient", FString());
    const auto EffectiveArmorCurve = DamageCalcCoefficientsCT->FindCurve("EffectiveArmorCoefficient", FString());
    const auto CriticalHitCurve = DamageCalcCoefficientsCT->FindCurve("CriticalHitCoefficient", FString());
    
    const auto ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel());
    const auto EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());
    const auto CriticalCoefficient = CriticalHitCurve->Eval(SourceCombatInterface->GetPlayerLevel());


    float EffectiveArmor = Armor * (100.f - ArmorPenetration * ArmorPenetrationCoefficient) / 100.f;
    Damage *= (100.f - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;

    bool bCriticalHit = FMath::RandRange(1, 100) <= CriticalHitChance;
    if (bCriticalHit) {
        Damage *= 2 * CriticalCoefficient; 
    }


    // Set Custom ContextHandle
    FGameplayEffectContextHandle Context = Spec.GetContext();
    UDefaultAbilitySystemLibrary::SetIsBlockedHit(Context, bBlocked);
    UDefaultAbilitySystemLibrary::SetIsCriticalHit(Context, bCriticalHit);

    FGameplayModifierEvaluatedData EvaluatedData { UDefaultAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage };
    OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
