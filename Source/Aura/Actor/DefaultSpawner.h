// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/Enemy.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DefaultSpawner.generated.h"

struct FEnemyInfo;

UCLASS()
class AURA_API ADefaultSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADefaultSpawner();

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UFUNCTION()
        
    void OnSpawnEnemies(int32 SpawnLevel, TArray<FEnemyInfo>& EnemySpawnInfos);

    // UPROPERTY(EditAnywhere, Category = "SpawnInfo")
    // TArray<FEnemySpawnInfo> SpawnInfos;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UStaticMeshComponent> Mesh;
};
