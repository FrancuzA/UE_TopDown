// Fill out your copyright notice in the Description page of Project Settings.


#include "Variant_TwinStick/BasePlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InteractionInterface.h"
#include "InteractionComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "BasePlayerController.h"
#include "Projectile.h"
#include "WaveManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABasePlayerCharacter::ABasePlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
    InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));

    GetCharacterMovement()->bOrientRotationToMovement = false;
    bUseControllerRotationPitch = true;
    bUseControllerRotationYaw = true;
    bUseControllerRotationRoll = false;

    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(GetRootComponent());
    CameraBoom->TargetArmLength = 150.f;
    CameraBoom->bUsePawnControlRotation = true;
    ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
    ViewCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    ViewCamera->bUsePawnControlRotation = false;

    Attributes = CreateDefaultSubobject<UAttributesComponent>(TEXT("Attributes"));

    CurrentProjectileClass = ProjectileClass;
}

// Called when the game starts or when spawned
void ABasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
    if (ABasePlayerController* PC = Cast<ABasePlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            if (MappingContext)
            {
                Subsystem->AddMappingContext(MappingContext, 0);
            }
        }
    }

    if (Attributes)
    {
        Attributes->OnDeath.AddDynamic(this, &ABasePlayerCharacter::HandlePlayerDeath);
        Attributes->OnHealthChanged.AddDynamic(this, &ABasePlayerCharacter::OnHealthChanged);
        Attributes->OnManaChanged.AddDynamic(this, &ABasePlayerCharacter::OnManaChanged);
    }
}

// Called every frame
// Called every frame
void ABasePlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Tylko w Play In Editor (nie w wyeksportowanej grze!)
    if (GetWorld() && GetWorld()->IsPlayInEditor())
    {
        APlayerController* PC = GetController<APlayerController>();
        if (!PC) return;

        // Klawisz H: Odejmij 10 HP
        if (PC->WasInputKeyJustPressed(EKeys::H))
        {
            if (Attributes)
            {
                UE_LOG(LogTemp, Warning, TEXT("DEBUG: Taking 10 damage"));
                Attributes->ApplyDamage(10.0f);
            }
        }

        // Klawisz M: Zużyj 20 Many
        if (PC->WasInputKeyJustPressed(EKeys::M))
        {
            if (Attributes && Attributes->CanPayManaCost(20.0f))
            {
                UE_LOG(LogTemp, Warning, TEXT("DEBUG: Paying 20 mana"));
                Attributes->PayMana(20.0f);
            }
        }

        // Klawisz P: Dodaj 50 punktów
        if (PC->WasInputKeyJustPressed(EKeys::P))
        {
            if (Attributes)
            {
                UE_LOG(LogTemp, Warning, TEXT("DEBUG: Adding 50 score"));
                Attributes->AddScore(50);
            }
        }

        // Klawisz R: Pełny respawn
        if (PC->WasInputKeyJustPressed(EKeys::R))
        {
            if (Attributes)
            {
                UE_LOG(LogTemp, Warning, TEXT("DEBUG: Full respawn"));
                Attributes->SetHealth(Attributes->GetMaxHealth());
                Attributes->SetMana(Attributes->GetMaxMana());
            }
        }
    }
}

// Called to bind functionality to input
void ABasePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    if (UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::Move);
        EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::Look);
        EIC->BindAction(InteractAction, ETriggerEvent::Started, this, &ABasePlayerCharacter::Interact);
        EIC->BindAction(AttackAction, ETriggerEvent::Started, this, &ABasePlayerCharacter::Attack);
    }
}

void ABasePlayerCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();
    AddControllerYawInput(LookAxisVector.X);
    AddControllerPitchInput(LookAxisVector.Y);

}

void ABasePlayerCharacter::Move(const FInputActionValue& Value)
{
    FVector2D moveValue = Value.Get<FVector2D>();

    if (Controller)
    {
        if (moveValue.GetAbsMax() > 0 && AttackMontage && GetMesh() && GetMesh()->GetAnimInstance())
        {
                GetMesh()->GetAnimInstance()->Montage_Play(RunningMontage);
        }
        if (moveValue.X != 0.f)
        {
            const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
            const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
            AddMovementInput(Direction, moveValue.X);
        }

        if (moveValue.Y != 0.f)
        {
            const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
            const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
            AddMovementInput(Direction, moveValue.Y);
        }
    }
}

void ABasePlayerCharacter::OnHealthChanged(float CurrentHealth, float MaxHealth)
{
    ABasePlayerController* PC = Cast<ABasePlayerController>(GetController());
    if (PC)
    {
        PC->UpdateHealthBar(CurrentHealth, MaxHealth);
    }

    if (CurrentHealth <= 0.0f)
    {
      
    }
}

void ABasePlayerCharacter::OnManaChanged(float CurrentMana, float MaxMana)
{
    ABasePlayerController* PC = Cast<ABasePlayerController>(GetController());
    if (PC)
    {
        PC->UpdateManaBar(CurrentMana, MaxMana);
    }

}

void ABasePlayerCharacter::OnDeath()
{
    if (AttackMontage && GetMesh() && GetMesh()->GetAnimInstance())
    {
        GetMesh()->GetAnimInstance()->Montage_Play(DeathMontage);
    }
    DisableInput(nullptr);
    GetCharacterMovement()->DisableMovement();

}

void ABasePlayerCharacter::Interact()
{
    InteractionComponent->TryInteract();
}

void ABasePlayerCharacter::Attack()
{
    if (!Attributes || !CurrentProjectileClass) return;


    // Sprawdź czy jest wystarczająca mana
    if (Attributes->CanPayManaCost(ManaCost_Attack)) // Ta funkcja zostanie zmieniona w kolejnym kroku
    {
        Attributes->PayMana(ManaCost_Attack); // Zostanie zmienione na PayMana

        if (AttackMontage && GetMesh() && GetMesh()->GetAnimInstance())
        {
            GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage);
        }

        // Strzelanie
        FVector SpawnLocation = GetActorLocation() + FVector(0, 0, 50);
        FRotator SpawnRotation = GetControlRotation();

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();

        if (CurrentProjectileClass)
        {
            AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
                CurrentProjectileClass, // Używaj aktualnej broni
                SpawnLocation,
                SpawnRotation,
                SpawnParams
            );

            if (Projectile)
            {
                FVector LaunchDirection = SpawnRotation.Vector();
                Projectile->FireInDirection(LaunchDirection);
            }
        }
    }
}

void ABasePlayerCharacter::GetHit_Implementation(int DMG)
{
    UE_LOG(LogTemp, Log, TEXT("Player Got Hit"));
    if (Attributes)
    {
        Attributes->ApplyDamage(DMG); // Przykładowe obrażenia
    }
}

FVector ABasePlayerCharacter::GetCameraLocation()
{
    return ViewCamera ? ViewCamera->GetComponentLocation() : FVector::ZeroVector;
}

FVector ABasePlayerCharacter::GetCameraForwardVector()
{
    return ViewCamera ? ViewCamera->GetForwardVector() : FVector::ForwardVector;
}

void ABasePlayerCharacter::HandlePlayerDeath()
{
    // POPRAWNE WYŁĄCZENIE INPUTU
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        DisableInput(PC);
    }

    GetCharacterMovement()->DisableMovement();
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);


    TArray<AActor*> FoundManagers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaveManager::StaticClass(), FoundManagers);
    if (FoundManagers.Num() > 0)
    {
        AWaveManager* WaveManager = Cast<AWaveManager>(FoundManagers[0]);
        if (WaveManager)
        {
            WaveManager->StopWaveSystem();
        }
    }
    // Spawnowanie ekranu końcowego
    if (AGameOverScreen* GameOver = GetWorld()->SpawnActor<AGameOverScreen>(AGameOverScreen::StaticClass()))
    {
        if (Attributes)
        {
            GameOver->SetFinalScore(Attributes->GetScore()); // ← bez RoundToInt!
        }
    }

    InteractionComponent->Deactivate();
    CameraBoom->Deactivate();
    ViewCamera->Deactivate();
}