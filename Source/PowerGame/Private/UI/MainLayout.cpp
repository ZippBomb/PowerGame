#include "UI/MainLayout.h"
#include "UI/MainHUD.h"
#include "UI/PauseMenu.h"

#include "UI/Inventory/InventoryMenu.h"

#include "UI/BuildMenu/BuildMenu.h"

#include "UI/Power/NetworkVisualizer.h"

#include "Player/MainPlayerController.h"
#include "Player/MainPlayerCharacter.h"

#include "Building/ConstructionModeManager.h"

#include "Power/PowerNetwork.h"

void UMainLayout::NativeConstruct() {

	Super::NativeConstruct();

	AMainPlayerController* controller = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController());
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

}