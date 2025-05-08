// Fill out your copyright notice in the Description page of Project Settings.


#include "Charactor/BaseCharactor.h"
#include "Components/SkeletalMeshComponent.h"

ABaseCharactor::ABaseCharactor()
{
	PrimaryActorTick.bCanEverTick = true;

    Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
    Weapon->SetupAttachment(GetMesh(), "WeaponHandSocket");
    Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharactor::BeginPlay()
{
	Super::BeginPlay();
	
}
