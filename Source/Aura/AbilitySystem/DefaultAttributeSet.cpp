// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultAttributeSet.h"
#include "AbilitySystem/DefaultAbilitySystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/Enemy.h"
#include "DefaultAssetManager.h"
#include "DefaultGameplayTags.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "GameplayTagContainer.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/EnvDamageInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Player/DefaultPlayerController.h"
#include "ScalableFloat.h"
#include "Utils/GameplayAbilityUtils.h"
#include "UObject/ConstructorHelpers.h"

// UDefaultAttributeSet::TMap<FGameplayTag, FGameplayAttribute> StackingTagAttributeMap;


UDefaultAttributeSet::UDefaultAttributeSet() {
    AttributeGetMap.Add(FDefaultGameplayTags::Get().Attributes_Vital_Health, UDefaultAttributeSet::GetHealthAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Get().Attributes_Vital_Mana, UDefaultAttributeSet::GetManaAttribute);

    AttributeGetMap.Add(FDefaultGameplayTags::Get().Attributes_Primary_Strength, UDefaultAttributeSet::GetStrengthAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Get().Attributes_Primary_Resilience, UDefaultAttributeSet::GetResilienceAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Get().Attributes_Primary_Intelligence, UDefaultAttributeSet::GetIntelligenceAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Get().Attributes_Primary_Vigor, UDefaultAttributeSet::GetVigorAttribute);

    AttributeGetMap.Add(FDefaultGameplayTags::Get().Attributes_Secondary_Armor                , UDefaultAttributeSet::GetArmorAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Get().Attributes_Secondary_ArmorPenetration     , UDefaultAttributeSet::GetArmorPenetrationAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Get().Attributes_Secondary_BlockChance          , UDefaultAttributeSet::GetBlockChanceAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Get().Attributes_Secondary_CriticalHitChance    , UDefaultAttributeSet::GetCriticalHitChanceAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Get().Attributes_Secondary_CriticalHitDamage    , UDefaultAttributeSet::GetCriticalHitDamageAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Get().Attributes_Secondary_CriticalHitResistance, UDefaultAttributeSet::GetCriticalHitResistanceAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Get().Attributes_Secondary_HealthRegeneration   , UDefaultAttributeSet::GetHealthRegenerationAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Get().Attributes_Secondary_ManaRegeneration     , UDefaultAttributeSet::GetManaRegenerationAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Get().Attributes_Secondary_MaxHealth            , UDefaultAttributeSet::GetMaxHealthAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Get().Attributes_Secondary_MaxMana              , UDefaultAttributeSet::GetMaxManaAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Get().Attributes_Resistance_Fire                , UDefaultAttributeSet::GetResistanceFireAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Get().Attributes_Resistance_Lightning           , UDefaultAttributeSet::GetResistanceLightningAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Get().Attributes_Resistance_Arcane              , UDefaultAttributeSet::GetResistanceArcaneAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Get().Attributes_Resistance_Physical            , UDefaultAttributeSet::GetResistancePhysicalAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Get().Attributes_Vital_FireStacking             , UDefaultAttributeSet::GetResistancePhysicalAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Get().Attributes_Vital_LightningStacking        , UDefaultAttributeSet::GetResistancePhysicalAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Get().Attributes_Secondary_MaxFireStacking      , UDefaultAttributeSet::GetResistancePhysicalAttribute);
    AttributeGetMap.Add(FDefaultGameplayTags::Get().Attributes_Secondary_MaxLightningStacking , UDefaultAttributeSet::GetResistancePhysicalAttribute);

    PrimaryAttributeLevelUpMap.Add(FDefaultGameplayTags::Get().Attributes_Primary_Strength, [this] () {
        if (!SpendAttributePoint()) { return; }
        SetStrength(GetStrength() + 1);
    });
    PrimaryAttributeLevelUpMap.Add(FDefaultGameplayTags::Get().Attributes_Primary_Resilience, [this] () {
        if (!SpendAttributePoint()) { return; }
        SetResilience(GetResilience() + 1);
    });
    PrimaryAttributeLevelUpMap.Add(FDefaultGameplayTags::Get().Attributes_Primary_Intelligence, [this] () {
        if (!SpendAttributePoint()) { return; }
        SetIntelligence(GetIntelligence() + 1);
    });
    PrimaryAttributeLevelUpMap.Add(FDefaultGameplayTags::Get().Attributes_Primary_Vigor, [this] () {
        if (!SpendAttributePoint()) { return; }
        SetVigor(GetVigor() + 1);
    });


    StackingTagAttributeMap.Add(FDefaultGameplayTags::Get().Stacking_Fire, UDefaultAttributeSet::GetIncomingFireStackingAttribute());
    StackingTagAttributeMap.Add(FDefaultGameplayTags::Get().Stacking_Lightning, UDefaultAttributeSet::GetIncomingLightningStackingAttribute());
}

void UDefaultAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, Mana, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, Strength, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, Armor, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, ResistanceFire, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, ResistanceLightning, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, ResistanceArcane, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, ResistancePhysical, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, FireStacking, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, LightningStacking, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, MaxFireStacking, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, MaxLightningStacking, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, Xp, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, MaxXp, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, Level, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, AttributePoint, COND_None, REPNOTIFY_Always);
}

void UDefaultAttributeSet::PreAttributeChange(const FGameplayAttribute &Attribute, float &NewValue) {
    if (Attribute == GetHealthAttribute()) {
        NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
    }

    if (Attribute == GetManaAttribute()) {
        NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
    }

    if (Attribute == GetAttributePointAttribute()) {
        NewValue = FMath::Clamp(NewValue, 0.f, TNumericLimits<int32>::Max());
    }

    Super::PreAttributeChange(Attribute, NewValue);
}

void UDefaultAttributeSet::PostGameplayEffectExecute(
    const FGameplayEffectModCallbackData &Data) {
    Super::PostGameplayEffectExecute(Data);

    FEffectProperties Props {};
    SetEffectProperties(Data, Props);

    if (Data.EvaluatedData.Attribute == GetHealthAttribute()) {
        SetHealth(FMath::Clamp(GetHealth(), 0, GetMaxHealth()));
    }

    if (Data.EvaluatedData.Attribute == GetManaAttribute()) {
        SetMana(FMath::Clamp(GetMana(), 0, GetMaxMana()));
    }

    if (Data.EvaluatedData.Attribute == GetAttributePointAttribute()) {
        SetAttributePoint(FMath::Clamp(GetAttributePoint(), 0.f, TNumericLimits<int32>::Max()));
    }

    if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute()) {
        DealIncomingDamage(Props);
    }

    if (Data.EvaluatedData.Attribute == GetIncomingFireStackingAttribute()) {
        DealIncomingFireStacking(Props);
    }

    if (Data.EvaluatedData.Attribute == GetIncomingLightningStackingAttribute()) {
        DealIncomingLightningStacking(Props);
    }

    if (Data.EvaluatedData.Attribute == GetIncomingXpAttribute()) {
        DealIncomingXp(Props);
    }

}

void UDefaultAttributeSet::SetEffectProperties(
    const FGameplayEffectModCallbackData &Data, FEffectProperties& Props) {
    Props.EffectContextHandle = Data.EffectSpec.GetContext();
    Props.SourceAbilitySystemComponent = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

    if (IsValid(Props.SourceAbilitySystemComponent) &&
        Props.SourceAbilitySystemComponent->AbilityActorInfo.IsValid() &&
        Props.SourceAbilitySystemComponent->AbilityActorInfo->AvatarActor.IsValid()) {

        Props.SourceAvatarActor = Props.SourceAbilitySystemComponent->AbilityActorInfo->AvatarActor.Get();
        Props.SourceController = Props.SourceAbilitySystemComponent->AbilityActorInfo->PlayerController.Get();
        
        if (!Props.SourceController && Props.SourceAvatarActor) {
            if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor)) {
                Props.SourceController = Pawn->GetController();
            }
        }

        if (Props.SourceController) {
            Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
        }
    }

    if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid()) {
        Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
        Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
        Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
        Props.TargetAbilitySystemComponent = &Data.Target;
    }
}

void UDefaultAttributeSet::OnRep_Health(const FGameplayAttributeData OldHealth) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, Health, OldHealth);
}

void UDefaultAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData OldMaxHealth) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, MaxHealth, OldMaxHealth);
}

void UDefaultAttributeSet::OnRep_Mana(const FGameplayAttributeData OldMana) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, Mana, OldMana);
}

void UDefaultAttributeSet::OnRep_MaxMana(const FGameplayAttributeData OldMaxMana) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, MaxMana, OldMaxMana);
}

void UDefaultAttributeSet::OnRep_Strength(const FGameplayAttributeData OldStrength) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, Strength, OldStrength);
}

void UDefaultAttributeSet::OnRep_Intelligence(const FGameplayAttributeData OldIntelligence) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, Intelligence, OldIntelligence);
}

void UDefaultAttributeSet::OnRep_Resilience(const FGameplayAttributeData OldResilience) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, Resilience, OldResilience);
}

void UDefaultAttributeSet::OnRep_Vigor(const FGameplayAttributeData OldVigor) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, Vigor, OldVigor);
}

void UDefaultAttributeSet::OnRep_Armor(const FGameplayAttributeData OldArmor) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, Armor, OldArmor);
}

void UDefaultAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData OldArmorPenetration) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UDefaultAttributeSet::OnRep_BlockChance(const FGameplayAttributeData OldBlockChance) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, BlockChance, OldBlockChance);
}

void UDefaultAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData OldCriticalHitChance) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UDefaultAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData OldCriticalHitDamage) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UDefaultAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData OldCriticalHitResistance) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}


void UDefaultAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData OldHealthRegeneration) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UDefaultAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData OldManaRegeneration) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UDefaultAttributeSet::OnRep_ResistanceFire(const FGameplayAttributeData OldResistanceFire) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, ResistanceFire, OldResistanceFire);
}

void UDefaultAttributeSet::OnRep_ResistanceLightning(const FGameplayAttributeData OldResistanceLightning) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, ResistanceLightning, OldResistanceLightning);
}

void UDefaultAttributeSet::OnRep_ResistanceArcane(const FGameplayAttributeData OldResistanceArcane) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, ResistanceArcane, OldResistanceArcane);
}

void UDefaultAttributeSet::OnRep_ResistancePhysical(const FGameplayAttributeData OldResistancePhysical) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, ResistancePhysical, OldResistancePhysical);
}

void UDefaultAttributeSet::OnRep_FireStacking(const FGameplayAttributeData OldFireStacking) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, FireStacking, OldFireStacking);
}

void UDefaultAttributeSet::OnRep_LightningStacking(const FGameplayAttributeData OldLightningStacking) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, LightningStacking, OldLightningStacking);
}

void UDefaultAttributeSet::OnRep_MaxFireStacking(const FGameplayAttributeData OldMaxFireStacking) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, MaxFireStacking, OldMaxFireStacking);
}

void UDefaultAttributeSet::OnRep_MaxLightningStacking(const FGameplayAttributeData OldMaxLightningStacking) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, MaxLightningStacking, OldMaxLightningStacking);
}

void UDefaultAttributeSet::OnRep_Xp(const FGameplayAttributeData OldXp) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, Xp, OldXp);
}

void UDefaultAttributeSet::OnRep_MaxXp(const FGameplayAttributeData OldMaxXp) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, MaxXp, OldMaxXp);
}

void UDefaultAttributeSet::OnRep_Level(const FGameplayAttributeData OldLevel) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, Level, OldLevel);
}

void UDefaultAttributeSet::OnRep_AttributePoint(const FGameplayAttributeData OldLevel) const {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, AttributePoint, OldLevel);
}


void UDefaultAttributeSet::DealIncomingDamage(FEffectProperties& Props) {
    const float LocalIncomingDamge = GetIncomingDamage();
    SetIncomingDamage(0.f);
    if (LocalIncomingDamge > 0.f && GetHealth() > 0.f) {

        const float NewHealth = GetHealth() - LocalIncomingDamge;
        SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

        const bool bFatal = NewHealth <= 0.f;

        if (bFatal) {
            // Death
            ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor);
            if (CombatInterface) {
                float XpDrop = CombatInterface->GetXpDrop();
                if (!CombatInterface->IsPlayer() && XpDrop > 0.f && Props.SourceAbilitySystemComponent) {
                    // Gain Xp to Player
                    auto EffectSpec = GameplayAbilityUtils::ConstructEffectSpec(
                        nullptr, Props.SourceAbilitySystemComponent, Props.TargetAbilitySystemComponent, UDefaultAssetManager::Get().GainXpEffectClass);

                    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
                        *EffectSpec, FDefaultGameplayTags::Get().SetByCaller_GE_GainXp, CombatInterface->GetXpDrop());

                    Props.SourceAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpec->Data);
                }
                CombatInterface->Die();
                // Props.SourceAbilitySystemComponent->ApplyGameplayEffect
            }
        } else {

            // Hit React
            FGameplayTagContainer TagContainer { FDefaultGameplayTags::Get().PunishAbility_HitReact };
            Props.TargetAbilitySystemComponent->TryActivateAbilitiesByTag(TagContainer);
        }

        // float damage text
        const bool bBlockedHit = UDefaultAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle);
        const bool bCriticalHit = UDefaultAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);

        if (auto DefaultPlayerController = Cast<ADefaultPlayerController>(Props.SourceCharacter->GetController())) {

            DefaultPlayerController->ShowDamageNumber(LocalIncomingDamge, Props.TargetCharacter, bBlockedHit, bCriticalHit);
        } else if (auto TargetPlayerController = Cast<ADefaultPlayerController>(Props.TargetCharacter->GetController())) {

            TargetPlayerController->ShowDamageNumber(LocalIncomingDamge, Props.TargetCharacter, bBlockedHit, bCriticalHit);
        }

    }


}

void UDefaultAttributeSet::DealIncomingFireStacking(FEffectProperties &Props) {
    auto& TargetASC = Props.TargetAbilitySystemComponent;
    auto& SourceASC = Props.SourceAbilitySystemComponent;

    // if (TargetASC->HasMatchingGameplayTag(FDefaultGameplayTags::Get().Stacking_Fire_Triggered)) {
    //     // 如果触发了效果，不再叠加值 (这个逻辑转移到ExeCalc里)
    //     return;
    // }
    const float LocalIncomingFireStacking = GetIncomingFireStacking();
    SetIncomingFireStacking(0.f);

    if (LocalIncomingFireStacking == 0.f) { return; }

    if (LocalIncomingFireStacking < 0.f && GetFireStacking() == 0.f) { return; }

    const float NewFireStacking = FMath::Clamp(GetFireStacking() + LocalIncomingFireStacking, 0.f, GetMaxFireStacking());
    SetFireStacking(NewFireStacking);

    if (GetFireStacking() == GetMaxFireStacking()) {
        // Apply OnFire Effect
        if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor)) {
            auto EffectSpec = GameplayAbilityUtils::ConstructEffectSpec(
                nullptr, SourceASC, TargetASC, CombatInterface->GetOnFireEffect());

            UObject* SourceObject = Props.EffectContextHandle.GetSourceObject();
            
            if (IEnvDamageInterface* EnvDamageInterface = Cast<IEnvDamageInterface>(SourceObject)) {

                for (const auto& Pair : EnvDamageInterface->GetDamageTypesMap()) {
                    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(*EffectSpec, Pair.Key, Pair.Value.GetValue());
                }

                for (const auto& Pair : EnvDamageInterface->GetStackingTypesMap()) {
                    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(*EffectSpec, Pair.Key, Pair.Value.GetValue());
                }

                TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpec->Data);

                SetFireStacking(0.f);
            }

        }
    }
}

void UDefaultAttributeSet::DealIncomingLightningStacking(
    FEffectProperties &Props) {

    const float LocalIncomingLightningStacking = GetIncomingLightningStacking();
    SetIncomingLightningStacking(0.f);

    if (LocalIncomingLightningStacking == 0.f) { return; }

    if (LocalIncomingLightningStacking < 0.f && GetLightningStacking() == 0.f) { return; }

    const float NewLightningStacking = GetLightningStacking() + LocalIncomingLightningStacking;
    SetLightningStacking(FMath::Clamp(NewLightningStacking, 0.f, GetMaxLightningStacking()));

    if (GetLightningStacking() == GetMaxLightningStacking()) {
        // Activate Stun Ability
        auto& TargetASC = Props.TargetAbilitySystemComponent;
        auto& SourceASC = Props.SourceAbilitySystemComponent;

        if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor)) {
            // auto EffectSpec = GameplayAbilityUtils::ConstructEffectSpec(
            //     nullptr, TargetASC, TargetASC, CombatInterface->GetOnLightningEffect());
            //
            // TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpec->Data);

            FGameplayTagContainer TagContainer { FDefaultGameplayTags::Get().PunishAbility_Stun };
            Props.TargetAbilitySystemComponent->TryActivateAbilitiesByTag(TagContainer);

            SetLightningStacking(0.f);
        }
    }
}

void UDefaultAttributeSet::DealIncomingXp(FEffectProperties& Props) {
    ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor);
    auto& TargetASC = Props.TargetAbilitySystemComponent;

    if (!CombatInterface || !CombatInterface->IsPlayer()) { return; }
    const float LocalIncomingXp = GetIncomingXp();
    const float LocalMaxXp = GetMaxXp();
    SetIncomingXp(0.f);

    if (LocalIncomingXp <= 0.f) { return; }


    const float NewXp = FMath::Clamp(GetXp() + LocalIncomingXp, 0.f, LocalMaxXp);
    SetXp(NewXp);

    if (GetXp() == LocalMaxXp) {
        // apply level up effect
        auto EffectSpec = GameplayAbilityUtils::ConstructEffectSpec(
            nullptr, TargetASC, TargetASC, CombatInterface->GetLevelUpEffect()
        );

        TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpec->Data);

        SetXp(0.f);
        SetMaxXp(CombatInterface->GetCurrentMaxXp());
    }

    // 溢出经验重新apply GE
    if (LocalIncomingXp > LocalMaxXp) {
        float OverflowXp = LocalIncomingXp - LocalMaxXp;
        auto EffectSpec = GameplayAbilityUtils::ConstructEffectSpec(
            nullptr, TargetASC, TargetASC, UDefaultAssetManager::Get().GainXpEffectClass
        );

        UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(*EffectSpec, FDefaultGameplayTags::Get().SetByCaller_GE_GainXp, OverflowXp);

        TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpec->Data);
    }
}

bool UDefaultAttributeSet::SpendAttributePoint() {
    if (GetAttributePoint() >= 1.f) {
        SetAttributePoint(GetAttributePoint() - 1);
        return true;
    }

    return false;
}

