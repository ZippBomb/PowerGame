#include "Building/Instances/Machines/SmelterInstance.h"

#include "Power/PowerNetwork.h"

#include "Logistics/Conveyor.h"

#include "UI/Buildings/Machines/SmelterUI.h"

#include "Inventory/RecipeData.h"

ASmelterInstance::ASmelterInstance() {

	PrimaryActorTick.bCanEverTick = true;

}

void ASmelterInstance::Tick(float deltaTime) {

	Super::Tick(deltaTime);

	if (slots[outputSlot].quantity != 0)
		AttemptOutput();

	if (!smelting) {

		if (CanStartSmelting())
			StartSmelting();
		else
			return;

	}

	PW_ASSERT(recipe != nullptr, LogMachines, TEXT("Smelter is burning with no recipe set."));

	if (m_powerNetwork == nullptr || m_powerNetwork->IsDead())
		StopSmelting();
	else if (smeltTimer <= 0.0f) {

		// Produce the output

		slots[outputSlot].item = recipe->output.item;
		slots[outputSlot].quantity += recipe->output.amount;

		// Consume the input (if possible)

		if (slots[inputSlot].quantity >= recipe->input.amount)
			ConsumeInput();
		else
			StopSmelting();

	} else
		smeltTimer -= deltaTime;

	if (uiOpen)
		GetUI<USmelterUI>()->UpdateUI(slots[inputSlot], slots[outputSlot], recipe, CalculateProgress());

}

void ASmelterInstance::Interact(AMainPlayerCharacter* character) {

	Super::Interact(character);

	GetUI<USmelterUI>()->UpdateUI(slots[inputSlot], slots[outputSlot], recipe, CalculateProgress());

}

void ASmelterInstance::OnInputAdded(UItemData* item, uint32 quantity) {

	if (slots[inputSlot].item != nullptr && slots[inputSlot].item != item) return;

	slots[inputSlot].item = item;
	slots[inputSlot].quantity += quantity;

	if (CanStartSmelting())
		StartSmelting();

}
void ASmelterInstance::OnInputRemoved(UItemData* item, uint32 quantity) {

	PW_ASSERT(slots[inputSlot].item == item, LogMachines, TEXT("Item mismatch in input slot."));

	slots[inputSlot].quantity -= quantity;

}

void ASmelterInstance::OnOutputRemoved(UItemData* item, uint32 quantity) {

	PW_ASSERT(slots[outputSlot].item == item, LogMachines, TEXT("Item mismatch in output slot."));

	slots[outputSlot].quantity -= quantity;
	if (slots[outputSlot].quantity == 0)
		slots[outputSlot].item = nullptr;

}

void ASmelterInstance::ConsumeInput() {

	slots[inputSlot].quantity -= recipe->input.amount;
	if (slots[inputSlot].quantity == 0)
		slots[inputSlot].item = nullptr;

	smeltTimer = recipe->recipeLength / smeltingSpeed;

}

void ASmelterInstance::AttemptOutput() {

	AConveyor* conveyor = slots[outputSlot].attachedConveyor;
	if (conveyor == nullptr || !conveyor->CanReceiveItem(slots[outputSlot].item)) return;

	conveyor->InsertItem(slots[outputSlot].item);
	OnOutputRemoved(slots[outputSlot].item);

}

void ASmelterInstance::StartSmelting() {

	if (smelting) return;

	smelting = true;
	demand = powerDemand;

	ConsumeInput();

}
void ASmelterInstance::StopSmelting() {

	smelting = false;
	demand = 0.0f;

	smeltTimer = 0.0f;

	if (slots[inputSlot].quantity == 0)
		slots[inputSlot].item = nullptr;

}
bool ASmelterInstance::CanStartSmelting() const {

	if (GetNetwork() == nullptr) return false;
	return !GetNetwork()->IsDead() && recipe != nullptr && slots[inputSlot].item == recipe->input.item && slots[inputSlot].quantity >= recipe->input.amount;

}

float ASmelterInstance::CalculateProgress() {

	if (recipe == nullptr) return 0.0f;
	return 1 - (smeltTimer * smeltingSpeed) / recipe->recipeLength;

}