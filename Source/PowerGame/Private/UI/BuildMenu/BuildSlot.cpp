#include "UI/BuildMenu/BuildSlot.h"

#include "UI/MainHUD.h"

#include "Building/Builds/Build.h"
#include "Building/ConstructionModeManager.h"

#include "Player/MainPlayerCharacter.h"
#include "Player/MainPlayerController.h"

#include <Components/Button.h>
#include <Components/Image.h>

void UBuildSlot::NativeConstruct() {

	Super::NativeConstruct();

	button->OnClicked.AddDynamic(this, &UBuildSlot::Select);

}

void UBuildSlot::SetBuild(UBuild* build) {

	m_build = build;

	if (build != nullptr) {

		icon->SetBrushFromTexture(build->buildMenuIcon);
		icon->SetOpacity(1.0f);

	} else {

		icon->SetBrushFromTexture(nullptr);
		icon->SetOpacity(0.0f);

	}

}

void UBuildSlot::Select() {

	AMainPlayerController* controller = Cast<AMainPlayerController>(GetOwningPlayer());
	PW_ASSERT(controller != nullptr, LogUI, TEXT("Could not cast GetOwningPlayer() to AMainPlayerController."));

	AMainPlayerCharacter* character = Cast<AMainPlayerCharacter>(controller->GetCharacter());
	PW_ASSERT(character != nullptr, LogUI, TEXT("Could not cast controller character to AMainPlayerCharacter."));

	UConstructionModeManager* manager = character->GetConstructionModeManager();
	PW_ASSERT(manager != nullptr, LogUI, TEXT("Could not get UConstructionModeManager."));

	manager->SelectBuild(m_build);

}