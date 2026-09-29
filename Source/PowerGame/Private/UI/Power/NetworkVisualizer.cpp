#include "UI/Power/NetworkVisualizer.h"
#include "UI/Power/NetworkSineWaveVisualizer.h"

#include "UI/MainHUD.h"

#include "Power/PowerNetwork.h"

#include "Player/MainPlayerController.h"

#include <Components/TextBlock.h>
#include <Components/CheckBox.h>

#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

#define LOCTEXT_NAMESPACE "PowerGame"

void UNetworkVisualizer::InitializeUI(AMainPlayerController* controller) {

	PW_ASSERT(controller != nullptr, LogPower, TEXT("Can't initialize NetworkVisualizer UI with invalid controller."));

	m_controller = controller;
	SetOwningPlayer(controller);

	SetVisibility(ESlateVisibility::Hidden);

	UEnhancedInputComponent* inputComponent = Cast<UEnhancedInputComponent>(m_controller->InputComponent);
	PW_ASSERT(inputComponent != nullptr, LogPower, TEXT("Could not retrieve UEnhancedInputComponent from '%s'."), *GetNameSafe(m_controller));

	inputComponent->BindAction(closeAction, ETriggerEvent::Triggered, this, &UNetworkVisualizer::Close);

	aliveToggle->OnCheckStateChanged.AddDynamic(this, &UNetworkVisualizer::OnAliveToggled);

}

void UNetworkVisualizer::NativeTick(const FGeometry& geometry, float deltaTime) {

	Super::NativeTick(geometry, deltaTime);

	if (m_network == nullptr) return;

	frequencyText->SetText(FText::FromString(FString::Printf(TEXT("Frequency: %.2f"), m_network->GetFrequency())));
	voltageText->SetText(FText::FromString(FString::Printf(TEXT("Voltage: %.2f"), m_network->GetVoltage())));

	supplyText->SetText(FText::FromString(FString::Printf(TEXT("Supply: %.2f"), m_network->GetSupply())));
	demandText->SetText(FText::FromString(FString::Printf(TEXT("Demand: %.2f"), m_network->GetDemand())));

	// This is a bit of weird logic, the checkbox is flipped compared to if the network is dead so if they are
	// equal (checkbox is checked but network is dead) it means theres a mismatch.

	if (aliveToggle->IsChecked() == m_network->IsDead()) {

		ECheckBoxState checkedState = m_network->IsDead() ? ECheckBoxState::Unchecked : ECheckBoxState::Checked;
		aliveToggle->SetCheckedState(checkedState);

	}

}

void UNetworkVisualizer::Open(APowerNetwork* network) {

	if (GetVisibility() == ESlateVisibility::Visible) return;

	if (!IsInViewport())
		AddToViewport();

	SetVisibility(ESlateVisibility::Visible);
	m_network = network;

	PW_ASSERT(m_controller != nullptr, LogUI, TEXT("NetworkVisualizer was not assigned a player controller, make sure you called UNetworkInitializer::InitializeUI()."));

	m_controller->OpenUI(this);
	sineWaveVisualizer->SetNetwork(m_network);

}
void UNetworkVisualizer::Close() {

	if (GetVisibility() == ESlateVisibility::Hidden) return;

	SetVisibility(ESlateVisibility::Hidden);
	m_network = nullptr;

	PW_ASSERT(m_controller != nullptr, LogUI, TEXT("NetworkVisualizer was not assigned a player controller, make sure you called UNetworkInitializer::InitializeUI()."));

	m_controller->CloseUI();
	sineWaveVisualizer->SetNetwork(nullptr);

}

void UNetworkVisualizer::OnAliveToggled(bool alive) {

	if (m_network == nullptr) return;
	m_network->SetIsDead(!alive);

}