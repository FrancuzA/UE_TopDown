#include "WaveManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"

AWaveManager::AWaveManager()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AWaveManager::BeginPlay()
{
    Super::BeginPlay();
    StartWaveSystem();
}

void AWaveManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AWaveManager::StartWaveSystem()
{
    if (bWaveInProgress || EnemyTypes.Num() == 0 || SpawnLocations.Num() == 0) return;

    CurrentWave = 0;
    EnemiesAlive = 0;
    bWaveInProgress = true;

    // Pierwsza fala po 3 sekundach
    GetWorldTimerManager().SetTimer(WaveTimer, this, &AWaveManager::SpawnWave, 3.0f, false);
}

void AWaveManager::StopWaveSystem()
{
    GetWorldTimerManager().ClearTimer(WaveTimer);
    GetWorldTimerManager().ClearTimer(SpawnTimer);
    bWaveInProgress = false;
}

void AWaveManager::SpawnWave()
{
    if (!bWaveInProgress) return;

    CurrentWave++;
    int32 EnemyCount = CalculateEnemyCountForWave(CurrentWave);
    FEnemyStats EnemyStats = CalculateEnemyStatsForWave(CurrentWave);

    UE_LOG(LogTemp, Log, TEXT("Wave %d started - %d enemies"), CurrentWave, EnemyCount);

    // Spawnowanie wrogów z opóźnieniem
    float SpawnDelay = 0.0f;
    for (int32 i = 0; i < EnemyCount; i++)
    {
        FTimerHandle SingleSpawnTimer;
        GetWorldTimerManager().SetTimer(
            SingleSpawnTimer,
            [this, EnemyStats]()
            {
                TSubclassOf<AEnemy> EnemyClass = GetRandomEnemyTypeForWave(CurrentWave);
                SpawnSingleEnemy(EnemyClass, EnemyStats);
            },
            SpawnDelay,
            false
        );
        SpawnDelay += 0.8f; // 0.8s między wrogami
    }

    // Następna fala po zakończeniu obecnej + czas przerwy
    GetWorldTimerManager().SetTimer(
        WaveTimer,
        this,
        &AWaveManager::SpawnWave,
        TimeBetweenWaves + (EnemyCount * 0.8f),
        false
    );
}

void AWaveManager::SpawnSingleEnemy(TSubclassOf<AEnemy> EnemyClass, const FEnemyStats& Stats)
{
    if (!EnemyClass || SpawnLocations.Num() == 0) return;

    // Losowa lokalizacja na krawędzi mapy
    FVector SpawnLocation = SpawnLocations[FMath::RandRange(0, SpawnLocations.Num() - 1)];

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    AEnemy* NewEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
    if (NewEnemy)
    {
        // Modyfikacja statystyk wroga
        NewEnemy->MaxHealth *= Stats.HealthMultiplier;
        NewEnemy->CurrentHealth = NewEnemy->MaxHealth;
        NewEnemy->DamageOnHit *= Stats.DamageMultiplier;

        // Modyfikacja prędkości (tylko dla CharacterMovement)
        if (NewEnemy->GetCharacterMovement())
        {
            NewEnemy->GetCharacterMovement()->MaxWalkSpeed *= Stats.SpeedMultiplier;
        }

        EnemiesAlive++;
        NewEnemy->OnDestroyed.AddDynamic(this, &AWaveManager::OnEnemyDestroyed);
    }
}

void AWaveManager::OnEnemyDestroyed(AActor* DestroyedActor)
{
    EnemiesAlive = FMath::Max(0, EnemiesAlive - 1);
}

int32 AWaveManager::CalculateEnemyCountForWave(int32 WaveNumber) const
{
    return BaseEnemyCount + FMath::FloorToInt((WaveNumber - 1) / WavesPerNewEnemyType * EnemyCountIncrement);
}

FEnemyStats AWaveManager::CalculateEnemyStatsForWave(int32 WaveNumber) const
{
    FEnemyStats Stats;
    Stats.HealthMultiplier = 1.0f + (WaveNumber - 1) * HealthScalePerWave;
    Stats.DamageMultiplier = 1.0f + (WaveNumber - 1) * DamageScalePerWave;
    Stats.SpeedMultiplier = 1.0f; // Stała prędkość dla balansu

    // Dodatkowy boost dla późnych fal
    if (WaveNumber > 10)
    {
        Stats.HealthMultiplier *= 1.5f;
        Stats.DamageMultiplier *= 1.3f;
    }

    return Stats;
}

TSubclassOf<AEnemy> AWaveManager::GetRandomEnemyTypeForWave(int32 WaveNumber) const
{
    if (EnemyTypes.Num() == 0) return nullptr;

    // Liczba dostępnych typów wrogów dla danej fali
    int32 AvailableTypes = FMath::Min(EnemyTypes.Num(), 1 + (WaveNumber - 1) / WavesPerNewEnemyType);
    int32 RandomIndex = FMath::RandRange(0, AvailableTypes - 1);

    return EnemyTypes[RandomIndex];
}