#include "Inventory/InventoryComponent.h"
#include "Inventory/ItemData.h"

#include "Player/MainPlayerCharacter.h"
#include "Player/MainPlayerController.h"

#include "UI/MainLayout.h"

#include "UI/Inventory/InventoryMenu.h"
#include "UI/Inventory/InventorySlot.h"

#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

UInventoryComponent::UInventoryComponent() {
	
	PrimaryComponentTick.bCanEverTick = false;

}

void UInventoryComponent::BeginPlay() {

	Super::BeginPlay();

	AMainPlayerCharacter* character = Cast<AMainPlayerCharacter>(GetOwner());
	PW_ASSERT(character != nullptr, LogCharacter, TEXT("UInventoryComponent must be attached to an actor of AMainPlayerCharacter type."));

	m_ui = character->GetUI()->GetInventoryMenu();

	// Setup Input

	UEnhancedInputComponent* inputComponent = Cast<UEnhancedInputComponent>(character->InputComponent);
	PW_ASSERT(inputComponent != nullptr, LogCharacter, TEXT("Could not retrieve UEnhancedInputComponent from '%s'."), *GetNameSafe(character));

	inputComponent->BindAction(closeAction, ETriggerEvent::Triggered, this, &UInventoryComponent::Close);

}

void UInventoryComponent::Open() {

	m_ui->Open();

}
void UInventoryComponent::Close() {

	m_ui->Close();

}

void UInventoryComponent::AddItem(UItemData* item, uint32 quantity) {

	uint32 remaining = quantity;
	if (storedItems.Contains(item)) {

		storedItems[item] += quantity;

		for (FItemSlot& slot : slots) {

			if (slot.item != item) continue;

			if (slot.quantity + remaining > item->stackSize) {

				remaining -= item->stackSize - slot.quantity;
				slot.quantity = item->stackSize;

			} else {

				slot.quantity += remaining;
				remaining = 0;

			}

			slot.uiSlot->UpdateQuantity(slot.quantity);

		}

	} else
		storedItems.Add(item, quantity);

	for (; remaining > 0;) {

		FItemSlot slot;
		slot.item = item;

		if (remaining > item->stackSize) {

			remaining -= item->stackSize - slot.quantity;
			slot.quantity = item->stackSize;

		} else {

			slot.quantity = remaining;
			remaining = 0;

		}

		slot.uiSlot = m_ui->AddSlot(slot);
		slots.Add(slot);

	}

}
bool UInventoryComponent::RemoveItem(UItemData* item, uint32 quantity) {

	if (!storedItems.Contains(item)) return false;

	if (storedItems[item] > quantity)
		storedItems[item] -= quantity;
	else
		storedItems.Remove(item);

	return true;

}