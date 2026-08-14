#include "Building/Instances/Machines/CoalBoilerInstance.h"

#include "Player/MainPlayerCharacter.h"

#include "UI/MainLayout.h"

#include "UI/Buildings/Machines/CoalBoilerUI.h"

ACoalBoilerInstance::ACoalBoilerInstance() {

	PrimaryActorTick.bCanEverTick = true;

}

void ACoalBoilerInstance::Tick(float deltaTime) {

	Super::Tick(deltaTime);

	if (!burning) return;

	if (consumeTimer <= 0.0f) {

		// Try consuming fuel

		if (inputSlot.quantity > 0) {

			// Consume another piece of fuel

			inputSlot.quantity--;
			if (inputSlot.quantity == 0)
				inputSlot.item = nullptr;

			consumeTimer = 1 / fuelConsumption;

		} else // No more fuel left, stop burning
			return StopBurning();

	} else
		consumeTimer -= deltaTime;

	// Produce steam

	steam += steamProduction * deltaTime;

	// UI update

	if (uiOpen)
		Cast<UCoalBoilerUI>(ui)->UpdateUI(inputSlot, consumeTimer * fuelConsumption, steam);

}

void ACoalBoilerInstance::Interact(AMainPlayerCharacter* character) {

	Super::Interact(character);

	UCoalBoilerUI* coalBoilerUI = Cast<UCoalBoilerUI>(ui);
	coalBoilerUI->UpdateUI(inputSlot, consumeTimer * fuelConsumption, steam);

}

void ACoalBoilerInstance::OnInputAdded(UItemData* item, uint32 quantity) {

	if (inputSlot.item != nullptr && inputSlot.item != item) return;

	inputSlot.item = item;
	inputSlot.quantity += quantity;

	StartBurning();

}
void ACoalBoilerInstance::OnInputRemoved(UItemData* item, uint32 quantity) {

	PW_ASSERT(item == inputSlot.item, LogMachines, TEXT("item mismatch in input slot."));

	inputSlot.quantity -= quantity;

}

void ACoalBoilerInstance::StartBurning() {

	burning = true;
	consumeTimer = 0.0f;

}
void ACoalBoilerInstance::StopBurning() {

	burning = false;
	consumeTimer = 0.0f;

	inputSlot.item = nullptr;
	inputSlot.quantity = 0;

}