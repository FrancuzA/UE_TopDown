#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.h"
#include "WaveManager.generated.h"

USTRUCT(BlueprintType)
struct FEnemyWave
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    TSubclassOf<AEnemy> EnemyClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    int32 EnemyCount = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    float SpawnInterval = 2.0f;
};

UCLASS()
class UE_TOPDOWN_API AWaveManager : public AActor
{
    GENERATED_BODY()

public:
    AWaveManager();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable)
    void StartNextWave();

    UFUNCTION(BlueprintCallable)
    void StopAllWaves();

    UFUNCTION(BlueprintCallable)
    void ResetWaves();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TArray<FVector> SpawnLocations;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves")
    TArray<FEnemyWave> Waves;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waves")
    int32 CurrentWaveIndex = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    int32 EnemiesAlive = 0;

    FTimerHandle WaveTimer;

    void SpawnEnemy(TSubclassOf<AEnemy> EnemyClass, const FVector& Location);
    void OnEnemyDestroyed(AActor* DestroyedActor);
};