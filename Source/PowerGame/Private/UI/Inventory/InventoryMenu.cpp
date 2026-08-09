#include "UI/Inventory/InventoryMenu.h"
#include "UI/Inventory/InventorySlot.h"

#include "UI/MainHUD.h"

#include "Player/MainPlayerController.h"

#include <Components/WrapBox.h>

void UInventoryMenu::InitializeUI(AMainPlayerController* controller) {

	PW_ASSERT(controller != nullptr, LogUI, TEXT("Can't initialize InventoryMenu UI with an invalid controller."));
	
	SetOwningPlayer(controller);
	m_controller = controller;

	SetVisibility(ESlateVisibility::Hidden);

}

void UInventoryMenu::Open() {

	if (GetVisibility() == ESlateVisibility::Visible) return;

	if (!IsInViewport())
		AddToViewport();

	SetVisibility(ESlateVisibility::Visible);

	PW_ASSERT(m_controller != nullptr, LogUI, TEXT("Inventory menu was not assigned a player controller, make sure you called UInventoryMenu::InitializeUI()."));

	m_controller->SetShowMouseCursor(true);
	m_controller->SetInputMode(FInputModeGameAndUI());
	m_controller->DisableDefaultIMC();
	m_controller->AddMappingContext(uiIMC);

}
void UInventoryMenu::Close() {

	if (GetVisibility() == ESlateVisibility::Hidden) return;

	SetVisibility(ESlateVisibility::Hidden);

	PW_ASSERT(m_controller != nullptr, LogUI, TEXT("Inventory menu was not assigned a player controller, make sure you called UInventoryMenu::InitializeUI()."));

	m_controller->SetShowMouseCursor(false);
	m_controller->SetInputMode(FInputModeGameOnly());
	m_controller->EnableDefaultIMC();
	m_controller->RemoveMappingContext(uiIMC);

}

inline UInventorySlot* UInventoryMenu::AddSlot(const FItemSlot& data) {

	PW_ASSERT(inventorySlotClass != nullptr, LogUI, TEXT("Inventory slot class must be assigned."));

	UInventorySlot* slot = CreateWidget<UInventorySlot>(GetOwningPlayer(), inventorySlotClass);
	slot->SetData(data);

	slotContainer->AddChild(slot);
	slots.Add(slot);

	return slot;

}