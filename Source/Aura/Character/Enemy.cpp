// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "AbilitySystem/DefaultAbilitySystemComponent.h"
#include "AbilitySystem/DefaultAttributeSet.h"
#include "Aura/Aura.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "DefaultGameplayTags.h"
#include "UI/Widget/DefaultUserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"


AEnemy::AEnemy() {
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

    AbilitySystemComponent = CreateDefaultSubobject<UDefaultAbilitySystemComponent>("AbilitySystemComponent");
    AbilitySystemComponent->SetIsReplicated(true);

    AttributeSet = CreateDefaultSubobject<UDefaultAttributeSet>("AttributeSet");
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

    HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
    HealthBar->SetupAttachment(GetRootComponent());
}

void AEnemy::BeginPlay() {
    Super::BeginPlay();
    GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpped;
    InitAbilityActorInfo();
    AddCharactorAbilities();

    // Set Widget Controller
    
    if (auto DefaultUserWidget = Cast<UDefaultUserWidget>(HealthBar->GetUserWidgetObject())) {
        DefaultUserWidget->SetWidgetController(this);
    }

    auto DefaultAttributeSet = Cast<UDefaultAttributeSet>(AttributeSet);

    // Init Broadcast
    OnHealthChanged.Broadcast(DefaultAttributeSet->GetHealth());
    OnMaxHealthChanged.Broadcast(DefaultAttributeSet->GetMaxHealth());

    // Bind Callbacks 
    if (DefaultAttributeSet) {
        AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
            DefaultAttributeSet->GetHealthAttribute()).AddLambda([this] (const FOnAttributeChangeData& Data) {
                OnHealthChanged.Broadcast(Data.NewValue);
            });
        AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
            DefaultAttributeSet->GetMaxHealthAttribute()).AddLambda([this] (const FOnAttributeChangeData& Data) {
                OnMaxHealthChanged.Broadcast(Data.NewValue);
            });
    }

    // Register Gameplay Tag Event (Hit React)
    AbilitySystemComponent->RegisterGameplayTagEvent(FDefaultGameplayTags::Get().Effect_HitReact, EGameplayTagEventType::NewOrRemoved)
        .AddLambda([this] (const FGameplayTag GameplayTag, int32 NewCount) {
            bHitReacting = NewCount > 0;
            GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpped;
        });

}

void AEnemy::HighlightActor() {
    GetMesh()->SetRenderCustomDepth(true);
    GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
    Weapon->SetRenderCustomDepth(true);
    Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AEnemy::UnHighlightActor() {
    GetMesh()->SetRenderCustomDepth(false);
    GetMesh()->SetCustomDepthStencilValue(0);

    Weapon->SetRenderCustomDepth(false);
    Weapon->SetCustomDepthStencilValue(0);
}


void AEnemy::InitAbilityActorInfo() {
    AbilitySystemComponent->InitAbilityActorInfo(this, this);
    CastChecked<UDefaultAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

    InitPrimaryAttributes();
    InitSecondaryAttributes();
    InitVitalAttributes();
}

int32 AEnemy::GetPlayerLevel() const {
    return PlayerLevel;
}

void AEnemy::Die() {
    Super::Die();

    HealthBar->SetVisibility(false);
    SetLifeSpan(5.f);
}
