#include "UI/Buildings/Machines/MachineUI.h"

#include "UI/MainHUD.h"
#include "UI/MainLayout.h"

#include "Building/Instances/Machines/MachineBuildInstance.h"

#include "Player/MainPlayerController.h"
#include "Player/MainPlayerCharacter.h"

#include "Inventory/PlayerInventoryComponent.h"

#include <Blueprint/WidgetTree.h>

#include <Components/CanvasPanel.h>

#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

void UMachineUI::NativeOnInitialized() {

	Super::NativeOnInitialized();

}

void UMachineUI::NativeConstruct() {

	Super::NativeConstruct();

	controller = Cast<AMainPlayerController>(GetOwningPlayer());
	SetVisibility(ESlateVisibility::Hidden);

	UEnhancedInputComponent* inputComponent = Cast<UEnhancedInputComponent>(controller->InputComponent);
	inputComponent->BindAction(closeAction, ETriggerEvent::Triggered, this, &UMachineUI::Close);

}

void UMachineUI::Open(AMachineBuildInstance* openedInstance) {

	if (GetVisibility() == ESlateVisibility::Visible) return;
	SetVisibility(ESlateVisibility::Visible);

	if (!IsInViewport())
		AddToViewport();

	instance = openedInstance;

	PW_ASSERT(controller != nullptr, LogUI, TEXT("Machine UI was not assigned a player controller, make sure you set its OwningPlayer."));

	controller->SetShowMouseCursor(true);
	controller->SetInputMode(FInputModeGameAndUI());
	controller->DisableDefaultIMC();
	controller->AddMappingContext(uiIMC);

	// Load Player inventory

	AMainPlayerCharacter* character = Cast<AMainPlayerCharacter>(controller->GetCharacter());
	PW_ASSERT(character != nullptr, LogUI, TEXT("Could not get AMainPlayerCharacter from AMainPlayerController->GetCharacter()."));

	character->GetInventoryComponent()->DisplayInventory(inventoryPanel);

}
void UMachineUI::Close() {

	if (GetVisibility() == ESlateVisibility::Hidden) return;
	SetVisibility(ESlateVisibility::Hidden);

	instance->CloseUI();
	instance = nullptr;

	PW_ASSERT(controller != nullptr, LogUI, TEXT("Machine UI was not assigned a player controller, make sure you set its OwningPlayer."));

	controller->SetShowMouseCursor(false);
	controller->SetInputMode(FInputModeGameOnly());
	controller->EnableDefaultIMC();
	controller->RemoveMappingContext(uiIMC);

}