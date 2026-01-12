#include "MainMenu.h"
#include "Components/WidgetComponent.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/CheckBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameUserSettings.h"

AMainMenu::AMainMenu()
{
    PrimaryActorTick.bCanEverTick = false;

    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
    WidgetComponent->SetupAttachment(RootComponent);
    WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    WidgetComponent->SetDrawAtDesiredSize(true);
}

void AMainMenu::BeginPlay()
{
    Super::BeginPlay();

    if (UUserWidget* Widget = WidgetComponent->GetUserWidgetObject())
    {
        // Główny ekran
        if (UButton* NewGameBtn = Cast<UButton>(Widget->GetWidgetFromName(TEXT("NewGameButton"))))
            NewGameBtn->OnClicked.AddDynamic(this, &AMainMenu::OnNewGameClicked);

        if (UButton* OptionsBtn = Cast<UButton>(Widget->GetWidgetFromName(TEXT("OptionsButton"))))
            OptionsBtn->OnClicked.AddDynamic(this, &AMainMenu::OnOptionsClicked);

        if (UButton* CreditsBtn = Cast<UButton>(Widget->GetWidgetFromName(TEXT("CreditsButton"))))
            CreditsBtn->OnClicked.AddDynamic(this, &AMainMenu::OnCreditsClicked);

        if (UButton* QuitBtn = Cast<UButton>(Widget->GetWidgetFromName(TEXT("QuitButton"))))
            QuitBtn->OnClicked.AddDynamic(this, &AMainMenu::OnQuitClicked);

        // Opcje
        if (UButton* BackOptionsBtn = Cast<UButton>(Widget->GetWidgetFromName(TEXT("BackFromOptionsButton"))))
            BackOptionsBtn->OnClicked.AddDynamic(this, &AMainMenu::OnBackFromOptionsClicked);

        if (USlider* VolumeSlider = Cast<USlider>(Widget->GetWidgetFromName(TEXT("MasterVolumeSlider"))))
            VolumeSlider->OnValueChanged.AddDynamic(this, &AMainMenu::OnMasterVolumeChanged);

        if (UCheckBox* FullscreenBox = Cast<UCheckBox>(Widget->GetWidgetFromName(TEXT("FullscreenCheckBox"))))
            FullscreenBox->OnCheckStateChanged.AddDynamic(this, &AMainMenu::OnFullscreenToggled);

        // Kredyty
        if (UButton* BackCreditsBtn = Cast<UButton>(Widget->GetWidgetFromName(TEXT("BackFromCreditsButton"))))
            BackCreditsBtn->OnClicked.AddDynamic(this, &AMainMenu::OnBackFromCreditsClicked);
    }
}

void AMainMenu::OnNewGameClicked()
{
    UGameplayStatics::OpenLevel(this, TEXT("MainLevel"));
}

void AMainMenu::OnOptionsClicked()
{
    if (UUserWidget* Widget = WidgetComponent->GetUserWidgetObject())
    {
        Widget->GetWidgetFromName(TEXT("MainMenuPanel"))->SetVisibility(ESlateVisibility::Hidden);
        Widget->GetWidgetFromName(TEXT("OptionsPanel"))->SetVisibility(ESlateVisibility::Visible);
    }
}

void AMainMenu::OnCreditsClicked()
{
    if (UUserWidget* Widget = WidgetComponent->GetUserWidgetObject())
    {
        Widget->GetWidgetFromName(TEXT("MainMenuPanel"))->SetVisibility(ESlateVisibility::Hidden);
        Widget->GetWidgetFromName(TEXT("CreditsPanel"))->SetVisibility(ESlateVisibility::Visible);
    }
}

void AMainMenu::OnBackFromOptionsClicked()
{
    if (UUserWidget* Widget = WidgetComponent->GetUserWidgetObject())
    {
        Widget->GetWidgetFromName(TEXT("OptionsPanel"))->SetVisibility(ESlateVisibility::Hidden);
        Widget->GetWidgetFromName(TEXT("MainMenuPanel"))->SetVisibility(ESlateVisibility::Visible);
    }
}

void AMainMenu::OnBackFromCreditsClicked()
{
    if (UUserWidget* Widget = WidgetComponent->GetUserWidgetObject())
    {
        Widget->GetWidgetFromName(TEXT("CreditsPanel"))->SetVisibility(ESlateVisibility::Hidden);
        Widget->GetWidgetFromName(TEXT("MainMenuPanel"))->SetVisibility(ESlateVisibility::Visible);
    }
}

void AMainMenu::OnQuitClicked()
{
    UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

void AMainMenu::OnMasterVolumeChanged(float Value)
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        FString Command = FString::Printf(TEXT("VOL Master %.2f"), Value);
        PlayerController->ConsoleCommand(*Command, true);
    }
}

void AMainMenu::OnFullscreenToggled(bool bIsChecked)
{
    UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
    Settings->SetFullscreenMode(bIsChecked ? EWindowMode::Fullscreen : EWindowMode::Windowed); 
    Settings->ApplySettings(true);
}