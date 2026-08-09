#include "UI/PauseMenu.h"
#include "UI/MainHUD.h"

#include "Saving/WorldSaveSubsystem.h"

#include "Player/MainPlayerController.h"
#include "Player/MainPlayerCharacter.h"

#include <Components/Button.h>

#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

#include <Kismet/GameplayStatics.h>

void UPauseMenu::InitializeUI(AMainPlayerController* controller) {

	PW_ASSERT(controller != nullptr, LogUI, TEXT("Can't initialize PauseMenu UI with an invalid controller."));

	m_controller = controller;
	SetOwningPlayer(controller);

	SetVisibility(ESlateVisibility::Hidden);

	UEnhancedInputComponent* inputComponent = Cast<UEnhancedInputComponent>(controller->InputComponent);
	PW_ASSERT(inputComponent != nullptr, LogUI, TEXT("Could not retrieve UEnhancedInputComponent from '%s'."), *GetNameSafe(m_controller));

	inputComponent->BindAction(closeAction, ETriggerEvent::Triggered, this, &UPauseMenu::Close);

}

void UPauseMenu::NativeConstruct() {

	Super::NativeConstruct();

	resumeButton->OnClicked.AddDynamic(this, &UPauseMenu::Resume);
	quitButton->OnClicked.AddDynamic(this, &UPauseMenu::Quit);

	saveButton->OnClicked.AddDynamic(this, &UPauseMenu::Save);
	loadButton->OnClicked.AddDynamic(this, &UPauseMenu::Load);

}

void UPauseMenu::Open() {

	if (GetVisibility() == ESlateVisibility::Visible) return;

	if (!IsInViewport())
		AddToViewport();

	SetVisibility(ESlateVisibility::Visible);

	PW_ASSERT(m_controller != nullptr, LogUI, TEXT("PauseMenu was not assigned a player controller, make sure you called UPauseMenu::InitializeUI()."));

	m_controller->SetShowMouseCursor(true);
	m_controller->SetInputMode(FInputModeGameAndUI());
	m_controller->DisableDefaultIMC();
	m_controller->AddMappingContext(uiIMC);

}
void UPauseMenu::Close() {

	if (GetVisibility() == ESlateVisibility::Hidden) return;

	SetVisibility(ESlateVisibility::Hidden);

	PW_ASSERT(m_controller != nullptr, LogUI, TEXT("PauseMenu was not assigned a player controller, make sure you called UPauseMenu::InitializeUI()."));

	m_controller->SetShowMouseCursor(false);
	m_controller->SetInputMode(FInputModeGameOnly());
	m_controller->EnableDefaultIMC();
	m_controller->RemoveMappingContext(uiIMC);

}

void UPauseMenu::Resume() {

	Close();

}
void UPauseMenu::Quit() {

	FGenericPlatformMisc::RequestExit(false);

}

void UPauseMenu::Save() {

	UWorldSaveSubsystem* saveSubsystem = GetGameInstance()->GetSubsystem<UWorldSaveSubsystem>();
	PW_ASSERT(saveSubsystem != nullptr, LogUI, TEXT("Could not get UWorldSaveSubsystem from game instance."));

	saveSubsystem->SaveWorld("SaveSlot1");

}
void UPauseMenu::Load() {

	UPowerGameInstance* gameInstance = Cast<UPowerGameInstance>(GetGameInstance());
	PW_ASSERT(gameInstance != nullptr, LogUI, TEXT("Could not get UPowerGameInstance."));

	gameInstance->pendingSaveSlot = "SaveSlot1";
	gameInstance->pendingSaveLoad = true;

	UGameplayStatics::OpenLevel(GetWorld(), FName(*UGameplayStatics::GetCurrentLevelName(this)));

	Close();

}