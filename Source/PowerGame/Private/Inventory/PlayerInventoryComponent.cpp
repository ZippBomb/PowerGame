#include "Inventory/PlayerInventoryComponent.h"

#include "Player/MainPlayerCharacter.h"
#include "Player/MainPlayerController.h"

#include "UI/MainLayout.h"

#include "UI/Inventory/InventoryMenu.h"
#include "UI/Inventory/InventoryPanel.h"

#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

UPlayerInventoryComponent::UPlayerInventoryComponent() {

	PrimaryComponentTick.bCanEverTick = false;

}

void UPlayerInventoryComponent::BeginPlay() {

	Super::BeginPlay();

	// UI Setup

	AMainPlayerCharacter* character = Cast<AMainPlayerCharacter>(GetOwner());
	PW_ASSERT(character != nullptr, LogInventory, TEXT("UInventoryComponent must be attached to an actor of AMainPlayerCharacter type."));

	m_ui = character->GetUI()->GetInventoryMenu();

	inventoryPanel = m_ui->GetInventoryPanel();
	inventoryPanel->inventory = this;

	for (uint32 i = 0; i < numOfSlots; i++) {

		FItemSlot slot;

		slot.item = nullptr;
		slot.quantity = 0;
		slot.slotIndex = slots.Num();

		slots.Add(slot);

		m_ui->GetInventoryPanel()->AddSlot();

	}

	// Setup Input

	UEnhancedInputComponent* inputComponent = Cast<UEnhancedInputComponent>(character->InputComponent);
	PW_ASSERT(inputComponent != nullptr, LogInventory, TEXT("Could not retrieve UEnhancedInputComponent from '%s'."), *GetNameSafe(character));

	inputComponent->BindAction(closeAction, ETriggerEvent::Triggered, this, &UPlayerInventoryComponent::Close);

}

void UPlayerInventoryComponent::Open() {

	m_ui->Open();

}
void UPlayerInventoryComponent::Close() {

	m_ui->Close();

}