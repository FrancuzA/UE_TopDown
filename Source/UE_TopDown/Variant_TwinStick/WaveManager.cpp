#include "WaveManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

AWaveManager::AWaveManager()
{
    PrimaryActorTick.bCanEverTick = false; // Tick nie jest potrzebny
}

void AWaveManager::BeginPlay()
{
    Super::BeginPlay();
    StartNextWave(); // Rozpocznij od pierwszej fali
}

void AWaveManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AWaveManager::StartNextWave()
{
    if (CurrentWaveIndex >= Waves.Num()) return;

    const FEnemyWave& CurrentWave = Waves[CurrentWaveIndex];
    float SpawnDelay = 0.0f;

    for (int32 i = 0; i < CurrentWave.EnemyCount; ++i)
    {
        // Losowy punkt spawnu
        int32 RandomIndex = FMath::RandRange(0, SpawnLocations.Num() - 1);
        FVector SpawnLocation = SpawnLocations[RandomIndex];

        // Opóźnione spawnowanie wrogów
        FTimerHandle SingleSpawnTimer;
        GetWorldTimerManager().SetTimer(
            SingleSpawnTimer,
            [this, CurrentWave, SpawnLocation]()
            {
                SpawnEnemy(CurrentWave.EnemyClass, SpawnLocation);
            },
            SpawnDelay,
            false
        );

        SpawnDelay += CurrentWave.SpawnInterval;
    }

    CurrentWaveIndex++;
}

void AWaveManager::SpawnEnemy(TSubclassOf<AEnemy> EnemyClass, const FVector& Location)
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    AEnemy* NewEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyClass, Location, FRotator::ZeroRotator, SpawnParams);
    if (NewEnemy)
    {
        EnemiesAlive++;
        // Śledź zniszczenie wroga
        NewEnemy->OnDestroyed.AddDynamic(this, &AWaveManager::OnEnemyDestroyed);
    }
}

void AWaveManager::OnEnemyDestroyed(AActor* DestroyedActor)
{
    EnemiesAlive = FMath::Max(0, EnemiesAlive - 1);

    // Jeśli nie ma wrogów i są kolejne fale
    if (EnemiesAlive == 0 && CurrentWaveIndex < Waves.Num())
    {
        // Opóźnienie przed następną falą
        GetWorldTimerManager().SetTimer(
            WaveTimer,
            this,
            &AWaveManager::StartNextWave,
            3.0f,
            false
        );
    }
}

void AWaveManager::StopAllWaves()
{
    GetWorldTimerManager().ClearTimer(WaveTimer);
    // Zatrzymaj wszystkie aktywne timery spawnów
    GetWorldTimerManager().ClearAllTimersForObject(this);
    CurrentWaveIndex = Waves.Num(); // Zablokuj nowe fale
}

void AWaveManager::ResetWaves()
{
    if (!GetWorld()) return;
    // Wyczyść wszystkie istniejące wrogowie
    TArray<AActor*> AllEnemies;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), AllEnemies);
    for (AActor* Enemy : AllEnemies)
    {
        Enemy->Destroy();
    }

    // Zresetuj stan menadżera
    CurrentWaveIndex = 0;
    EnemiesAlive = 0;
    GetWorldTimerManager().ClearAllTimersForObject(this);

    // Natychmiast rozpocznij od nowa
    StartNextWave();
}