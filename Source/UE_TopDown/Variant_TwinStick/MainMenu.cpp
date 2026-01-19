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

    MainMenuWidget = WidgetComponent->GetUserWidgetObject();
    if (!MainMenuWidget) return;

    // Pobierz panele UI
    MainMenuPanel = MainMenuWidget->GetWidgetFromName(TEXT("MainMenuPanel"));
    OptionsPanel = MainMenuWidget->GetWidgetFromName(TEXT("OptionsPanel"));

    if (OptionsPanel) OptionsPanel->SetVisibility(ESlateVisibility::Hidden);

    // Główny ekran
    if (UButton* NewGameBtn = Cast<UButton>(MainMenuWidget->GetWidgetFromName(TEXT("NewGameButton"))))
        NewGameBtn->OnClicked.AddDynamic(this, &AMainMenu::OnNewGameClicked);

    if (UButton* OptionsBtn = Cast<UButton>(MainMenuWidget->GetWidgetFromName(TEXT("OptionsButton"))))
        OptionsBtn->OnClicked.AddDynamic(this, &AMainMenu::OnOptionsClicked);

    if (UButton* QuitBtn = Cast<UButton>(MainMenuWidget->GetWidgetFromName(TEXT("QuitButton"))))
        QuitBtn->OnClicked.AddDynamic(this, &AMainMenu::OnQuitClicked);

    // Opcje
    if (UButton* BackOptionsBtn = Cast<UButton>(MainMenuWidget->GetWidgetFromName(TEXT("BackFromOptionsButton"))))
        BackOptionsBtn->OnClicked.AddDynamic(this, &AMainMenu::OnBackFromOptionsClicked);

    if (USlider* VolumeSlider = Cast<USlider>(MainMenuWidget->GetWidgetFromName(TEXT("MasterVolumeSlider"))))
    {
        // USTAW DOMYŚLNĄ WARTOŚĆ 
        VolumeSlider->SetValue(1.0f); 

        VolumeSlider->OnValueChanged.AddDynamic(this, &AMainMenu::OnMasterVolumeChanged);
    }

    if (UCheckBox* FullscreenBox = Cast<UCheckBox>(MainMenuWidget->GetWidgetFromName(TEXT("FullscreenCheckBox"))))
    {
        UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
        FullscreenBox->SetIsChecked(Settings->GetFullscreenMode() == EWindowMode::Fullscreen);
        FullscreenBox->OnCheckStateChanged.AddDynamic(this, &AMainMenu::OnFullscreenToggled);
    }
}

void AMainMenu::OnNewGameClicked()
{
    UGameplayStatics::OpenLevel(this, TEXT("MainLevel"));
}

void AMainMenu::OnOptionsClicked()
{
    if (MainMenuPanel) MainMenuPanel->SetVisibility(ESlateVisibility::Hidden);
    if (OptionsPanel) OptionsPanel->SetVisibility(ESlateVisibility::Visible);
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
    if (OptionsPanel) OptionsPanel->SetVisibility(ESlateVisibility::Hidden);
    if (MainMenuPanel) MainMenuPanel->SetVisibility(ESlateVisibility::Visible);
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
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        FString Command = FString::Printf(TEXT("VOL Master %.2f"), Value);
        PC->ConsoleCommand(*Command, true);
    }
}

void AMainMenu::OnFullscreenToggled(bool bIsChecked)
{
    UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
    Settings->SetFullscreenMode(bIsChecked ? EWindowMode::Fullscreen : EWindowMode::Windowed);
    Settings->ApplySettings(true);
}