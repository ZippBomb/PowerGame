#include "UI/Buildings/Machines/CoalBoilerUI.h"

#include "UI/Inventory/MachineSlot.h"

#include "UI/MainHUD.h"

#include "Building/Instances/Machines/CoalBoilerInstance.h"

#include "Player/MainPlayerController.h"
#include "Player/MainPlayerCharacter.h"

#include "Inventory/PlayerInventoryComponent.h"

#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

#include <Components/TextBlock.h>
#include <Components/Button.h>

void UCoalBoilerUI::NativeConstruct() {

	Super::NativeConstruct();

	m_controller = Cast<AMainPlayerController>(GetOwningPlayer());
	SetVisibility(ESlateVisibility::Hidden);

	UEnhancedInputComponent* inputComponent = Cast<UEnhancedInputComponent>(m_controller->InputComponent);
	inputComponent->BindAction(closeAction, ETriggerEvent::Triggered, this, &UCoalBoilerUI::Close);

	inputSlot->onItemAdded.AddUObject(this, &UCoalBoilerUI::OnInputAdded);

}

void UCoalBoilerUI::Open(ACoalBoilerInstance* instance) {

	if (GetVisibility() == ESlateVisibility::Visible) return;
	SetVisibility(ESlateVisibility::Visible);

	if (!IsInViewport())
		AddToViewport();

	m_instance = instance;

	PW_ASSERT(m_controller != nullptr, LogUI, TEXT("Coal Boiler UI was not assigned a player controller, make sure you called UCoalBoilerUI::InitializeUI()."));

	m_controller->SetShowMouseCursor(true);
	m_controller->SetInputMode(FInputModeGameAndUI());
	m_controller->DisableDefaultIMC();
	m_controller->AddMappingContext(uiIMC);

	// Load Player inventory

	AMainPlayerCharacter* character = Cast<AMainPlayerCharacter>(m_controller->GetCharacter());
	PW_ASSERT(character != nullptr, LogUI, TEXT("Could not get AMainPlayerCharacter from AMainPlayerController->GetCharacter()."));

	character->GetInventoryComponent()->DisplayInventory(inventoryPanel);

}
void UCoalBoilerUI::Close() {

	if (GetVisibility() == ESlateVisibility::Hidden) return;
	SetVisibility(ESlateVisibility::Hidden);

	m_instance->CloseUI();
	m_instance = nullptr;

	PW_ASSERT(m_controller != nullptr, LogUI, TEXT("Coal Boiler UI was not assigned a player controller, make sure you called UCoalBoilerUI::InitializeUI()."));

	m_controller->SetShowMouseCursor(false);
	m_controller->SetInputMode(FInputModeGameOnly());
	m_controller->EnableDefaultIMC();
	m_controller->RemoveMappingContext(uiIMC);

}

void UCoalBoilerUI::UpdateUI(const FItemSlot& input, float fuelProgress, float steam) {

	inputSlot->SetData(input);

	coalText->SetText(FText::FromString(FString::Printf(TEXT("%4.2f"), fuelProgress)));
	steamText->SetText(FText::FromString(FString::Printf(TEXT("%4.2f"), steam)));

}

void UCoalBoilerUI::OnInputAdded(UItemData* item, uint32 quantity) {

	m_instance->OnInputAdded(item, quantity);

}