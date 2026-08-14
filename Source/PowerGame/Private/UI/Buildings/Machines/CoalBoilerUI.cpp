#include "UI/Buildings/Machines/CoalBoilerUI.h"

#include "UI/Inventory/MachineSlot.h"

#include "UI/MainHUD.h"

#include "Building/Instances/Machines/CoalBoilerInstance.h"

#include <Components/TextBlock.h>

void UCoalBoilerUI::NativeConstruct() {

	Super::NativeConstruct();

	inputSlot->onItemAdded.AddUObject(this, &UCoalBoilerUI::OnInputAdded);
	inputSlot->onItemRemoved.AddUObject(this, &UCoalBoilerUI::OnInputRemoved);

}

void UCoalBoilerUI::UpdateUI(const FItemSlot& input, float fuelProgress, float steam) {

	inputSlot->SetData(input);

	coalText->SetText(FText::FromString(FString::Printf(TEXT("%4.2f"), fuelProgress)));
	steamText->SetText(FText::FromString(FString::Printf(TEXT("%4.2f"), steam)));

}

void UCoalBoilerUI::OnInputAdded(UItemData* item, uint32 quantity) {

	PW_ASSERT(instance != nullptr, LogUI, TEXT("Invalid machine instance."));

	ACoalBoilerInstance* coalBoiler = Cast<ACoalBoilerInstance>(instance);
	PW_ASSERT(coalBoiler != nullptr, LogUI, TEXT("Coal boiler UI was not given ACoalBoilerInstance instance."));

	coalBoiler->OnInputAdded(item, quantity);

}
void UCoalBoilerUI::OnInputRemoved(UItemData* item, uint32 quantity) {

	PW_ASSERT(instance != nullptr, LogUI, TEXT("Invalid machine instance."));

	ACoalBoilerInstance* coalBoiler = Cast<ACoalBoilerInstance>(instance);
	PW_ASSERT(coalBoiler != nullptr, LogUI, TEXT("Coal boiler UI was not given ACoalBoilerInstance instance."));

	coalBoiler->OnInputRemoved(item, quantity);

}