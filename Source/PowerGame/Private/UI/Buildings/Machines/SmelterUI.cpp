#include "UI/Buildings/Machines/SmelterUI.h"

#include "UI/Inventory/MachineSlot.h"

#include "Building/Instances/Machines/SmelterInstance.h"

#include "Building/Builds/Machine.h"

#include "Inventory/RecipeData.h"

#include <Components/TextBlock.h>
#include <Components/ComboBoxString.h>

void USmelterUI::NativeConstruct() {

	Super::NativeConstruct();

	recipeSelect->OnSelectionChanged.AddDynamic(this, &USmelterUI::OnRecipeSelected);

	inputSlot->onItemAdded.AddUObject(this, &USmelterUI::OnInputAdded);
	inputSlot->onItemRemoved.AddUObject(this, &USmelterUI::OnInputRemoved);

	outputSlot->onItemRemoved.AddUObject(this, &USmelterUI::OnOutputRemoved);

}

void USmelterUI::UpdateUI(const FItemSlot& input, const FItemSlot& output, URecipeData* recipe, float progress) {

	inputSlot->SetData(input);
	outputSlot->SetData(output);

	progressText->SetText(FText::FromString(FString::Printf(TEXT("%4.2f"), progress)));

	// Populate the recipe select combo box
	
	UMachine* machine = Cast<UMachine>(instance->GetBuild());
	PW_ASSERT(machine != nullptr, LogUI, TEXT("Could not get UMachine from machine instance."));

	recipeSelect->ClearOptions();

	for (URecipeData* recipe : machine->recipes)
		recipeSelect->AddOption(recipe->displayName.ToString());

	if (recipe != nullptr)
		recipeSelect->SetSelectedOption(recipe->displayName.ToString());

}

void USmelterUI::OnRecipeSelected(FString recipeName, ESelectInfo::Type selectionType) {

	UMachine* machine = Cast<UMachine>(GetInstance<ASmelterInstance>()->GetBuild());
	PW_ASSERT(machine != nullptr, LogUI, TEXT("Could not get UMachine from machine instance."));

	for (URecipeData* recipe : machine->recipes) {

		if (recipe->displayName.ToString() != recipeName) continue;
		
		GetInstance<ASmelterInstance>()->SetRecipe(recipe);
		return;

	}

}

void USmelterUI::OnInputAdded(UItemData* item, uint32 quantity) {

	GetInstance<ASmelterInstance>()->OnInputAdded(item, quantity);

}
void USmelterUI::OnInputRemoved(UItemData* item, uint32 quantity) {

	GetInstance<ASmelterInstance>()->OnInputRemoved(item, quantity);

}

void USmelterUI::OnOutputRemoved(UItemData* item, uint32 quantity) {

	GetInstance<ASmelterInstance>()->OnOutputRemoved(item, quantity);

}