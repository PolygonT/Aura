// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "AI/DefaultAIController.h"
#include "AbilitySystem/DefaultAbilitySystemComponent.h"
#include "AbilitySystem/DefaultAttributeSet.h"
#include "Aura/Aura.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "DefaultGameplayTags.h"
#include "UI/Widget/DefaultUserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BehaviorTree.h"


AEnemy::AEnemy() {
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

    AbilitySystemComponent = CreateDefaultSubobject<UDefaultAbilitySystemComponent>("AbilitySystemComponent");
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

    AttributeSet = CreateDefaultSubobject<UDefaultAttributeSet>("AttributeSet");

    HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
    HealthBar->SetupAttachment(GetRootComponent());
    // TODO 5 这里的Replicate是否合适，是为了修复客户端敌人死后血条往下掉的BUG加上的
    HealthBar->SetIsReplicated(true);

    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
    bUseControllerRotationYaw = false;

    GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void AEnemy::BeginPlay() {
    Super::BeginPlay();
    GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpped;
    InitAbilityActorInfo();
    AddCharactorAbilities();
    AddCharactorGameplayCues();

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
    if (HasAuthority()) {
        AbilitySystemComponent->RegisterGameplayTagEvent(FDefaultGameplayTags::Get().Effect_HitReact, EGameplayTagEventType::NewOrRemoved)
            .AddUObject(this, &ThisClass::HitReact);
    }
        // .AddLambda([this] (const FGameplayTag GameplayTag, int32 NewCount) {
        //     bHitReacting = NewCount > 0;
        //     GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpped;
        //     DefaultAIController->GetBlackboardComponent()->SetValueAsBool("HitReacting", bHitReacting);
        // });

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
    HealthBar->SetVisibility(false);
    DefaultAIController->GetBlackboardComponent()->SetValueAsBool("Dead", true);

    SetLifeSpan(5.f);

    Super::Die();
}

void AEnemy::PossessedBy(AController *NewController) {
    Super::PossessedBy(NewController);


    if (!HasAuthority()) {
        return;
    }

    DefaultAIController = Cast<ADefaultAIController>(NewController);
    DefaultAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
    DefaultAIController->GetBlackboardComponent()->SetValueAsBool("HitReacting", false);
    DefaultAIController->GetBlackboardComponent()->SetValueAsBool("RangeAttacker", CharacterClass != ECharacterClass::Warrior);

    DefaultAIController->RunBehaviorTree(BehaviorTree);
}

void AEnemy::HitReact(const FGameplayTag GameplayTag, int32 NewCount) {
    bHitReacting = NewCount > 0;
    GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpped;
    DefaultAIController->GetBlackboardComponent()->SetValueAsBool("HitReacting", bHitReacting);
}


void AEnemy::SetCombatTarget_Implementation(AActor *InCombatTarget) {
    CombatTarget = InCombatTarget;
}

AActor *AEnemy::GetCombatTarget_Implementation() const {
    return CombatTarget;
}
