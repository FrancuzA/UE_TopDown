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
#include "GameFramework/CharacterMovementComponent.h"

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
        Attributes->OnHealthChanged.AddDynamic(this, &ABasePlayerCharacter::OnHealthChanged);
        Attributes->OnStaminaChanged.AddDynamic(this, &ABasePlayerCharacter::OnStaminaChanged);
        Attributes->OnDeath.AddDynamic(this, &ABasePlayerCharacter::OnDeath);
    }
}

// Called every frame
void ABasePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
    /*FVector2D AxisValue = Value.Get<FVector2D>();
    if (Controller)
    {
        AddControllerYawInput(AxisValue.X);
        AddControllerPitchInput(-AxisValue.Y);
    }*/
}

void ABasePlayerCharacter::Move(const FInputActionValue& Value)
{
    FVector2D moveValue = Value.Get<FVector2D>();

    if (Controller)
    {

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

    // Update state if health is critical
    if (CurrentHealth <= 0.0f)
    {
      
    }
}

void ABasePlayerCharacter::OnStaminaChanged(float CurrentStamina, float MaxStamina)
{
    ABasePlayerController* PC = Cast<ABasePlayerController>(GetController());
    if (PC)
    {
        PC->UpdateStaminaBar(CurrentStamina, MaxStamina);
    }

}

void ABasePlayerCharacter::OnDeath()
{
   
    DisableInput(nullptr);
    GetCharacterMovement()->DisableMovement();

}

void ABasePlayerCharacter::Interact()
{
    InteractionComponent->TryInteract();
}

void ABasePlayerCharacter::Attack()
{
    
    Attributes->PayStamina(StaminaCost_Attack);

    if ( AttackMontage && GetMesh() && GetMesh()->GetAnimInstance())
    {
        UE_LOG(LogTemp, Warning, TEXT("Wykonano atak"));
        GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage);
    }
}

void ABasePlayerCharacter::GetHit_Implementation(FVector HitLocation)
{


    if (Attributes)
    {
        Attributes->ApplyDamage(10.0f); // Przykładowe obrażenia
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
