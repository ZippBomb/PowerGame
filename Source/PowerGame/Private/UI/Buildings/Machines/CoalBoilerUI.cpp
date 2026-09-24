#include "UI/Buildings/Machines/CoalBoilerUI.h"

#include "UI/Inventory/MachineSlot.h"

#include "UI/MainHUD.h"

#include "Building/Instances/Machines/CoalBoilerInstance.h"

#include <Components/TextBlock.h>
#include <Components/Slider.h>

void UCoalBoilerUI::NativeConstruct() {

	Super::NativeConstruct();

	inputSlot->onItemAdded.AddUObject(this, &UCoalBoilerUI::OnInputAdded);
	inputSlot->onItemRemoved.AddUObject(this, &UCoalBoilerUI::OnInputRemoved);

	outputLevelSlider->OnValueChanged.AddDynamic(this, &UCoalBoilerUI::OnOutputLevelChanged);

}

void UCoalBoilerUI::UpdateUI(const FItemSlot& input, float fuelProgress, float steam) {

	inputSlot->SetData(input);

	coalText->SetText(FText::FromString(FString::Printf(TEXT("%4.2f"), fuelProgress)));
	steamText->SetText(FText::FromString(FString::Printf(TEXT("%4.2f"), steam)));

	float outputLevel = GetInstance<AGenerator>()->GetOutputLevel();
	float maxOutput = GetInstance<AGenerator>()->GetMaxOutput();

	outputLevelSlider->SetValue(outputLevel);
	outputLevelText->SetText(FText::FromString(FString::Printf(TEXT("%3.0f%%"), outputLevel * 100.0f)));

	outputText->SetText(FText::FromString(FString::Printf(TEXT("%3.0f / %3.0fkW"), maxOutput * outputLevel, maxOutput)));

}

void UCoalBoilerUI::OnInputAdded(UItemData* item, uint32 quantity) {

	GetInstance<ACoalBoilerInstance>()->OnInputAdded(item, quantity);

}
void UCoalBoilerUI::OnInputRemoved(UItemData* item, uint32 quantity) {
	
	GetInstance<ACoalBoilerInstance>()->OnInputRemoved(item, quantity);

}

void UCoalBoilerUI::OnOutputLevelChanged(float value) {

	outputLevelText->SetText(FText::FromString(FString::Printf(TEXT("%3.0f%%"), value * 100.0f)));
	GetInstance<AGenerator>()->SetOutputLevel(value);

	float maxOutput = GetInstance<AGenerator>()->GetMaxOutput();
	outputText->SetText(FText::FromString(FString::Printf(TEXT("%3.0f / %3.0fkW"), maxOutput * value, maxOutput)));

}
