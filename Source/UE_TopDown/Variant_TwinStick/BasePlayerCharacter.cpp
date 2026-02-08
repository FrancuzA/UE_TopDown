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
#include "GameFramework/Controller.h" // ✅ DODANE – dla GetController()
#include "Kismet/GameplayStatics.h"

ABasePlayerCharacter::ABasePlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));

    GetCharacterMovement()->bOrientRotationToMovement = false;
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
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
        // ✅ PODPIĘCIE EVENTU SCORE:
        Attributes->OnScoreChanged.AddDynamic(this, &ABasePlayerCharacter::OnScoreChanged);
    }

    // ✅ TWÓRZ HUD TYLKO GDY JEST POSTAĆ GRACZA
    if (ABasePlayerController* PC = Cast<ABasePlayerController>(GetController()))
    {
        PC->CreateHUD();
    }
}

void ABasePlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (GetWorld() && GetWorld()->IsPlayInEditor())
    {
        APlayerController* PC = GetController<APlayerController>();
        if (!PC) return;

        if (PC->WasInputKeyJustPressed(EKeys::H))
        {
            if (Attributes) Attributes->ApplyDamage(10.0f);
        }

        if (PC->WasInputKeyJustPressed(EKeys::M))
        {
            if (Attributes && Attributes->CanPayManaCost(20.0f)) Attributes->PayMana(20.0f);
        }

        if (PC->WasInputKeyJustPressed(EKeys::P))
        {
            if (Attributes) Attributes->AddScore(50);
        }

        if (PC->WasInputKeyJustPressed(EKeys::R))
        {
            if (Attributes)
            {
                Attributes->SetHealth(Attributes->GetMaxHealth());
                Attributes->SetMana(Attributes->GetMaxMana());
            }
        }
    }
}

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
    FVector2D MoveValue = Value.Get<FVector2D>();

    if (Controller && (MoveValue.X != 0.0f || MoveValue.Y != 0.0f))
    {
        AddMovementInput(FVector(1.0f, 0.0f, 0.0f), MoveValue.X);
        AddMovementInput(FVector(0.0f, 1.0f, 0.0f), MoveValue.Y);
    }
}

void ABasePlayerCharacter::OnHealthChanged(float CurrentHealth, float MaxHealth)
{
    ABasePlayerController* PC = Cast<ABasePlayerController>(GetController());
    if (PC)
    {
        PC->UpdateHealthBar(CurrentHealth, MaxHealth);
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

// ✅ POPRAWIONA FUNKCJA SCORE:
void ABasePlayerCharacter::OnScoreChanged(int32 NewScore)
{
    ABasePlayerController* PC = Cast<ABasePlayerController>(GetController());
    if (PC)
    {
        PC->UpdateScore(NewScore);
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
    if (!CurrentProjectileClass) CurrentProjectileClass = ProjectileClass;
    if (!Attributes || !CurrentProjectileClass) return;

    if (Attributes->CanPayManaCost(ManaCost_Attack))
    {
        Attributes->PayMana(ManaCost_Attack);

        if (AttackMontage && GetMesh() && GetMesh()->GetAnimInstance())
        {
            GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage);
        }

        FVector SpawnLocation = GetActorLocation() + FVector(0, 0, 50);
        FRotator SpawnRotation = GetActorRotation();

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = this;

        if (CurrentProjectileClass)
        {
            AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
                CurrentProjectileClass,
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
        Attributes->ApplyDamage(DMG);
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
        if (AWaveManager* WaveManager = Cast<AWaveManager>(FoundManagers[0]))
        {
            WaveManager->StopWaveSystem();
        }
    }

    if (ABasePlayerController* PC = Cast<ABasePlayerController>(GetController()))
    {
        int32 FinalScore = Attributes ? Attributes->GetScore() : 0;
        PC->ShowGameOverScreen(FinalScore);
    }

    if (InteractionComponent) InteractionComponent->Deactivate();
    if (CameraBoom) CameraBoom->Deactivate();
    if (ViewCamera) ViewCamera->Deactivate();

    if (DeathMontage && GetMesh() && GetMesh()->GetAnimInstance())
    {
        GetMesh()->GetAnimInstance()->Montage_Play(DeathMontage);
    }
}