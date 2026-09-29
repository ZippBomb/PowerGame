#include "Player/MainPlayerController.h"
#include "Player/MainPlayerCharacter.h"

#include <Blueprint/UserWidget.h>

#include <Engine/LocalPlayer.h>

#include <EnhancedInputSubsystems.h>

void AMainPlayerController::BeginPlay() {

	Super::BeginPlay();

	m_inputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	PW_ASSERT(m_inputSubsystem != nullptr, LogCharacter, TEXT("'%s' could not retrieve Enhanced input local player subsystem."), *GetNameSafe(this));

	EnableDefaultIMC();

}

void AMainPlayerController::OpenUI(UUserWidget* widget) {

	FInputModeGameAndUI inputMode;

	inputMode.SetWidgetToFocus(widget->TakeWidget());
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	inputMode.SetHideCursorDuringCapture(false);

	SetInputMode(inputMode);
	SetShowMouseCursor(true);

	DisableDefaultIMC();
	AddMappingContext(uiIMC);

}
void AMainPlayerController::CloseUI() {

	SetInputMode(FInputModeGameOnly());
	SetShowMouseCursor(false);

	EnableDefaultIMC();
	RemoveMappingContext(uiIMC);

}

void AMainPlayerController::EnableDefaultIMC() {

	AddMappingContext(defaultIMC, 0);

}
void AMainPlayerController::DisableDefaultIMC() {

	RemoveMappingContext(defaultIMC);

}

void AMainPlayerController::AddMappingContext(UInputMappingContext* imc, int priority) {

	m_inputSubsystem->AddMappingContext(imc, priority);

}
void AMainPlayerController::RemoveMappingContext(UInputMappingContext* imc) {

	m_inputSubsystem->RemoveMappingContext(imc);

}