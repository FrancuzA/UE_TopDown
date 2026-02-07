#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.h"
#include "WaveManager.generated.h"

USTRUCT(BlueprintType)
struct FEnemyStats
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float HealthMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float DamageMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float SpeedMultiplier = 1.0f;
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
    void StartWaveSystem();

    UFUNCTION(BlueprintCallable)
    void StopWaveSystem();

protected:
    // Konfiguracja trudności
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Difficulty")
    TArray<TSubclassOf<AEnemy>> EnemyTypes; // Grunt -> Speedster -> Tank

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Difficulty")
    int32 WavesPerNewEnemyType = 3; // Co 3 fale nowy typ wroga

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Difficulty")
    int32 BaseEnemyCount = 5;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Difficulty")
    float EnemyCountIncrement = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Difficulty")
    float HealthScalePerWave = 0.1f; // +10% HP co falę

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Difficulty")
    float DamageScalePerWave = 0.05f; // +5% obrażeń co falę

    // Spawn
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TArray<FVector> SpawnLocations; // Punkty na krawędziach mapy

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    float TimeBetweenWaves = 5.0f;

    // Stan
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
    int32 CurrentWave = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
    int32 EnemiesAlive = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
    bool bWaveInProgress = false;

    FTimerHandle WaveTimer;
    FTimerHandle SpawnTimer;

    void SpawnWave();
    void SpawnSingleEnemy(TSubclassOf<AEnemy> EnemyClass, const FEnemyStats& Stats);

    UFUNCTION()
    void OnEnemyDestroyed(AActor* DestroyedActor);

    int32 CalculateEnemyCountForWave(int32 WaveNumber) const;
    FEnemyStats CalculateEnemyStatsForWave(int32 WaveNumber) const;
    TSubclassOf<AEnemy> GetRandomEnemyTypeForWave(int32 WaveNumber) const;
};