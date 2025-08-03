// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DefaultSpawner.h"
#include "Character/Enemy.h"
#include "Components/StaticMeshComponent.h"
#include "Game/EnemyWaveGameMode.h"
#include "HAL/Platform.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystem/Data/EnemyWaveInfo.h"

// Sets default values
ADefaultSpawner::ADefaultSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
    Mesh->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ADefaultSpawner::BeginPlay()
{
	Super::BeginPlay();
	
    
    if (AEnemyWaveGameMode* EnemyWaveGameMode = 
        Cast<AEnemyWaveGameMode>(UGameplayStatics::GetGameMode(this))) {

        EnemyWaveGameMode->GetSpawnEnemiesDelegate()
            .AddDynamic(this, &ThisClass::OnSpawnEnemies);
    }
}

// Called every frame
void ADefaultSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADefaultSpawner::OnSpawnEnemies(int32 SpawnLevel, TArray<FEnemyInfo>& EnemySpawnInfos) {
    FVector SpawnLocation = Mesh->GetSocketLocation("SpawnEnemySocket");
    FVector SpawnForwardVector = FVector {0.f, 0.f, 0.f} - SpawnLocation;

    double BiasDegree = 60.f;
    FRotator BiasRotator = {0.f, -BiasDegree, 0.f};

    for (const auto& SpawnInfo : EnemySpawnInfos) {
        int32 SpawnNums = SpawnInfo.SpawnNums + FMath::RandRange(0, SpawnInfo.SpawnNumsDeviation);
        if (SpawnNums > 0) {

            for (int32 i = 0; i < SpawnNums; i++) {
                int32 SpawnLevelWithDeviation = SpawnLevel + FMath::RandRange(0, SpawnInfo.EnemyLevelDeviation);
                BiasRotator.Yaw += BiasDegree;
                FTransform SpawnTransform {SpawnLocation + BiasRotator.RotateVector(GetActorForwardVector()) * 70};
                SpawnTransform.SetRotation(SpawnForwardVector.ToOrientationQuat());

                AEnemy* Enemy = GetWorld()->SpawnActorDeferred<AEnemy>(SpawnInfo.EnemyClass, SpawnTransform, this);
                Enemy->SpawnDefaultController();
                Enemy->SetPlayerLevel(SpawnLevelWithDeviation);

                Enemy->FinishSpawning(SpawnTransform);
            }
        }

    }
}

