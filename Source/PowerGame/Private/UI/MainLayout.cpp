#include "UI/MainLayout.h"
#include "UI/MainHUD.h"
#include "UI/PauseMenu.h"

#include "UI/Inventory/InventoryMenu.h"

#include "UI/BuildMenu/BuildMenu.h"

#include "UI/Power/NetworkVisualizer.h"

#include "UI/Buildings/Machines/MachineUI.h"

#include "Player/MainPlayerController.h"
#include "Player/MainPlayerCharacter.h"

#include "Building/ConstructionModeManager.h"

#include "Power/PowerNetwork.h"

#include <Blueprint/WidgetTree.h>

void UMainLayout::NativeConstruct() {

	Super::NativeConstruct();

	AMainPlayerController* controller = Cast<AMainPlayerController>(GetOwningPlayer());
	PW_ASSERT(controller != nullptr, LogUI, TEXT("Could not retrieve first player controller."));

	AMainPlayerCharacter* character = Cast<AMainPlayerCharacter>(controller->GetCharacter());
	PW_ASSERT(character != nullptr, LogUI, TEXT("Could not retrieve player character from player controller."));

	PW_ASSERT(pauseMenu != nullptr, LogUI, TEXT("Main layouts PauseMenu ui was not assigned."));
	PW_ASSERT(inventoryMenu != nullptr, LogUI, TEXT("Main layouts InventoryMenu ui was not assigned."));
	PW_ASSERT(buildMenu != nullptr, LogUI, TEXT("Main layouts BuildMenu ui was not assigned."));

	PW_ASSERT(networkVisualizer != nullptr, LogUI, TEXT("Main layouts NetworkVisualizer ui was not assigned."));

	character->GetConstructionModeManager()->BindUI(buildMenu);
	character->BindUI(this);

	pauseMenu->InitializeUI(controller);
	inventoryMenu->InitializeUI(controller);
	buildMenu->InitializeUI(controller);

	networkVisualizer->InitializeUI(controller);

	RegisterMachineUIs();

}

void UMainLayout::RegisterMachineUIs() {
	
	WidgetTree->ForEachWidget([this](UWidget* widget) {

		UMachineUI* machineUI = Cast<UMachineUI>(widget);
		if (machineUI == nullptr) return;

		m_machineUIs.Add(machineUI->GetClass(), machineUI);
		machineUI->SetOwningPlayer(GetOwningPlayer());

	});

}

UMachineUI* UMainLayout::GetMachineUI(TSubclassOf<UMachineUI> uiClass) {

	PW_ASSERT(m_machineUIs.Contains(uiClass), LogUI, TEXT("Machine UI '%s' was not found in UMainLayout::machineUIs."), *GetNameSafe(uiClass));
	return m_machineUIs[uiClass];

}