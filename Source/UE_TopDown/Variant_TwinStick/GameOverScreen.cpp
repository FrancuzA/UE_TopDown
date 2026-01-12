#include "GameOverScreen.h"
#include "Components/WidgetComponent.h"
#include "Components/Button.h"
#include "Components/TextBlock.h" 
#include "Kismet/GameplayStatics.h"

AGameOverScreen::AGameOverScreen()
{
    PrimaryActorTick.bCanEverTick = false;

    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
    WidgetComponent->SetupAttachment(RootComponent);
    WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    WidgetComponent->SetDrawAtDesiredSize(true);
    WidgetComponent->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));
}

void AGameOverScreen::BeginPlay()
{
    Super::BeginPlay();

    if (UUserWidget* Widget = WidgetComponent->GetUserWidgetObject())
    {
        WidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        WidgetComponent->SetGenerateOverlapEvents(false);

        // WIĄZANIE PRZYCISKÓW
        if (UButton* RestartButton = Cast<UButton>(Widget->GetWidgetFromName(TEXT("RestartButton"))))
        {
            RestartButton->OnClicked.AddDynamic(this, &AGameOverScreen::OnRestartClicked);
        }

        if (UButton* MenuButton = Cast<UButton>(Widget->GetWidgetFromName(TEXT("MainMenuButton"))))
        {
            MenuButton->OnClicked.AddDynamic(this, &AGameOverScreen::OnMainMenuClicked);
        }
    }
}

void AGameOverScreen::SetFinalScore(int32 Score)
{
    if (UUserWidget* Widget = WidgetComponent->GetUserWidgetObject())
    {
        // AKTUALIZACJA TEKSTU WYNIKU
        if (UTextBlock* ScoreText = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("ScoreText"))))
        {
            ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), Score)));
        }
    }
}

void AGameOverScreen::OnRestartClicked()
{
    UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this)));
}

void AGameOverScreen::OnMainMenuClicked()
{
    UGameplayStatics::OpenLevel(this, TEXT("MainMenu"));
}