#include "Building/Instances/Machines/CoalBoilerInstance.h"

#include "Player/MainPlayerCharacter.h"

#include "UI/MainLayout.h"

#include "UI/Buildings/Machines/CoalBoilerUI.h"

ACoalBoilerInstance::ACoalBoilerInstance() {

	PrimaryActorTick.bCanEverTick = true;

}

void ACoalBoilerInstance::Tick(float deltaTime) {

	Super::Tick(deltaTime);

	if (fuel <= 0.0f) return;

	fuel -= fuelConsumption * deltaTime;
	steam += steamProduction * deltaTime;

	if (fuel < 0.0f)
		fuel = 0.0f;

	if (uiOpen)
		m_ui->UpdateUI(fuel, steam);

}

void ACoalBoilerInstance::Interact(AMainPlayerCharacter* character) {

	if (m_ui == nullptr)
		m_ui = character->GetUI()->GetCoalBoilerUI();

	m_ui->Open(this);
	m_ui->UpdateUI(fuel, steam);

	uiOpen = true;

}

void ACoalBoilerInstance::AddCoal() {

	fuel += 1.0f;

}