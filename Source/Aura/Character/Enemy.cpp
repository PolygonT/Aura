// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "AbilitySystem/DefaultAbilitySystemComponent.h"
#include "AbilitySystem/DefaultAttributeSet.h"
#include "Aura/Aura.h"
#include "Components/SkeletalMeshComponent.h"


AEnemy::AEnemy() {
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

    AbilitySystemComponent = CreateDefaultSubobject<UDefaultAbilitySystemComponent>("AbilitySystemComponent");
    AbilitySystemComponent->SetIsReplicated(true);

    AttributeSet = CreateDefaultSubobject<UDefaultAttributeSet>("AttributeSet");
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
}

void AEnemy::BeginPlay() {
    Super::BeginPlay();
    InitAbilityActorInfo();
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
}

int32 AEnemy::GetPlayerLevel() {
    return PlayerLevel;
}
