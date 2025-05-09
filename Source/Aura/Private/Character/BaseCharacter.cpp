// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"
#include "Components/SkeletalMeshComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

    Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
    Weapon->SetupAttachment(GetMesh(), "WeaponHandSocket");
    Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
