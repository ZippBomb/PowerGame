#include "Building/Instances/Machines/SmelterInstance.h"

#include "UI/Buildings/Machines/SmelterUI.h"

#include "Inventory/RecipeData.h"

ASmelterInstance::ASmelterInstance() {

	PrimaryActorTick.bCanEverTick = true;

}

void ASmelterInstance::Tick(float deltaTime) {

	Super::Tick(deltaTime);

	if (!smelting) {

		if (recipe != nullptr && inputSlot.item == recipe->input.item && inputSlot.quantity >= recipe->input.amount)
			StartSmelting();
		else
			return;

	}

	PW_ASSERT(recipe != nullptr, LogMachines, TEXT("Smelter is burning with no recipe set."));

	if (smeltTimer <= 0.0f) {

		// Produce the output

		outputSlot.item = recipe->output.item;
		outputSlot.quantity += recipe->output.amount;

		// Consume the input (if possible)

		if (inputSlot.quantity >= recipe->input.amount)
			ConsumeInput();
		else
			StopSmelting();

	} else
		smeltTimer -= deltaTime;

	if (uiOpen)
		GetUI<USmelterUI>()->UpdateUI(inputSlot, outputSlot, recipe, CalculateProgress());

}

void ASmelterInstance::Interact(AMainPlayerCharacter* character) {

	Super::Interact(character);

	GetUI<USmelterUI>()->UpdateUI(inputSlot, outputSlot, recipe, CalculateProgress());

}

void ASmelterInstance::OnInputAdded(UItemData* item, uint32 quantity) {

	if (inputSlot.item != nullptr && inputSlot.item != item) return;

	inputSlot.item = item;
	inputSlot.quantity += quantity;

	if (recipe != nullptr && item == recipe->input.item && inputSlot.quantity >= recipe->input.amount)
		StartSmelting();

}
void ASmelterInstance::OnInputRemoved(UItemData* item, uint32 quantity) {

	PW_ASSERT(inputSlot.item == item, LogMachines, TEXT("Item mismatch in input slot."));

	inputSlot.quantity -= quantity;

}

void ASmelterInstance::OnOutputRemoved(UItemData* item, uint32 quantity) {

	PW_ASSERT(outputSlot.item == item, LogMachines, TEXT("Item mismatch in output slot."));

	outputSlot.quantity -= quantity;
	if (outputSlot.quantity == 0)
		outputSlot.item = nullptr;

}

void ASmelterInstance::ConsumeInput() {

	inputSlot.quantity -= recipe->input.amount;
	if (inputSlot.quantity == 0)
		inputSlot.item = nullptr;

	smeltTimer = recipe->recipeLength / smeltingSpeed;

}

void ASmelterInstance::StartSmelting() {

	if (smelting) return;

	smelting = true;
	ConsumeInput();

}
void ASmelterInstance::StopSmelting() {

	smelting = false;
	smeltTimer = 0.0f;

	if (inputSlot.quantity == 0)
		inputSlot.item = nullptr;

}

float ASmelterInstance::CalculateProgress() {

	if (recipe == nullptr) return 0.0f;
	return 1 - (smeltTimer * smeltingSpeed) / recipe->recipeLength;

}