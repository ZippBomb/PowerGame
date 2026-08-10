#include "Building/ConstructionModeManager.h"

#include "Building/Builds/Build.h"
#include "Building/BuildGhost.h"

#include "Building/Instances/BuildInstance.h"

#include "Player/MainPlayerCharacter.h"
#include "Player/MainPlayerController.h"

#include "UI/BuildMenu/BuildMenu.h"

#include <Camera/CameraComponent.h>

#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include <InputAction.h>

DEFINE_LOG_CATEGORY(LogBuilding);

UConstructionModeManager::UConstructionModeManager() {
	
	PrimaryComponentTick.bCanEverTick = true;

}

void UConstructionModeManager::BeginPlay() {

	Super::BeginPlay();

	m_character = Cast<AMainPlayerCharacter>(GetOwner());
	PW_ASSERT(m_character != nullptr, LogBuilding, TEXT("Build Mode Manager must be attached to actor of type AMainPlayerCharacter."));

	m_camera = m_character->GetCamera();
	PW_ASSERT(m_camera != nullptr, LogBuilding, TEXT("Could not retrieve UCameraComponent from '%s'."), *GetNameSafe(m_character));

	// Setup input

	m_controller = Cast<AMainPlayerController>(m_character->Controller);
	PW_ASSERT(m_controller != nullptr, LogBuilding, TEXT("Could not cast controller of '%s' to AMainPlayerController."), *GetNameSafe(m_character));
	PW_ASSERT(constructionModeIMC != nullptr, LogBuilding, TEXT("No Build mode IMC assigned to the Build Mode Manager "));

	UEnhancedInputComponent* inputComponent = Cast<UEnhancedInputComponent>(m_character->InputComponent);
	PW_ASSERT(inputComponent != nullptr, LogBuilding, TEXT("Could not retrieve UEnhancedInputComponent from '%s'."), *GetNameSafe(m_character));

	inputComponent->BindAction(rotateGhostAction, ETriggerEvent::Started, this, &UConstructionModeManager::RotateBuildGhost);

	inputComponent->BindAction(confirmAction, ETriggerEvent::Started, this, &UConstructionModeManager::Confirm);
	inputComponent->BindAction(exitAction, ETriggerEvent::Started, this, &UConstructionModeManager::ExitConstructionMode);

	inputComponent->BindAction(toggleGridSnapAction, ETriggerEvent::Started, this, &UConstructionModeManager::ToggleGridSnap);

}

void UConstructionModeManager::SpawnBuildGhost(TSubclassOf<ABuildGhost> buildGhostClass) {

	PW_ASSERT(buildGhostClass != nullptr, LogBuilding, TEXT("Can't spawn Build Ghost actor with invalid Build Ghost class."));

	FActorSpawnParameters params;
	params.Owner = GetOwner();
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	m_buildGhost = GetWorld()->SpawnActor<ABuildGhost>(buildGhostClass, FTransform::Identity, params);
	PW_ASSERT(m_buildGhost != nullptr, LogBuilding, TEXT("Could not spawn Build Ghost actor."));

	m_buildGhost->SetActorHiddenInGame(true);

}

void UConstructionModeManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (m_tool) {

	case EConstructionTool::BuildTool: HandleBuildTool(); break;
	case EConstructionTool::DeconstructTool: HandleDeconstructTool(); break;
	default: break;

	}

}

void UConstructionModeManager::HandleBuildTool() {

	if (m_selectedBuild == nullptr) return;

	FHitResult hit = FHitResult(EForceInit::ForceInit);
	VisibilityTrace(hit);

	if (!hit.bBlockingHit) {

		m_buildGhost->SetActorHiddenInGame(true);
		return;

	}

	FVector location = hit.ImpactPoint;
	if (m_gridSnap) {

		location.X = FMath::RoundToInt(location.X / gridSize) * gridSize;
		location.Y = FMath::RoundToInt(location.Y / gridSize) * gridSize;

	}

	m_buildGhost->Update(location, Cast<ABuildInstance>(hit.GetActor()));

}
void UConstructionModeManager::HandleDeconstructTool() {

	FHitResult hit = FHitResult(EForceInit::ForceInit);
	VisibilityTrace(hit);

	if (hit.bBlockingHit && Cast<ABuildInstance>(hit.GetActor()) != nullptr) {

		ABuildInstance* buildInstance = Cast<ABuildInstance>(hit.GetActor());
		if (m_highlightedBuildInstance != nullptr && m_highlightedBuildInstance != buildInstance)
			m_highlightedBuildInstance->ResetHighlightMaterial();

		m_highlightedBuildInstance = buildInstance;
		m_highlightedBuildInstance->SetHighlightMaterial(deconstructHighlightMaterial);

		return;

	}

	// Reset if changed

	if (m_highlightedBuildInstance == nullptr) return;
	
	m_highlightedBuildInstance->ResetHighlightMaterial();
	m_highlightedBuildInstance = nullptr;

}

void UConstructionModeManager::VisibilityTrace(FHitResult& hit) {

	FVector start = m_camera->GetComponentLocation();
	FVector end = start + (m_camera->GetForwardVector() * buildToolRange);

	FCollisionQueryParams params = FCollisionQueryParams(TEXT("Construction mode visibility trace"), true, m_character);
	params.AddIgnoredActor(m_buildGhost);
	params.bReturnPhysicalMaterial = false;
	params.bDebugQuery = true;

	GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility, params);

}

void UConstructionModeManager::SelectBuildTool() {

	if (m_tool == EConstructionTool::BuildTool) {

		ExitConstructionMode();
		return;

	}
	
	PW_LOG(LogBuilding, TEXT("Selecting build tool."));

	// First deselect the deconstruct tool if selected

	if (m_tool == EConstructionTool::DeconstructTool) {

		if (m_highlightedBuildInstance != nullptr) {

			PW_LOG(LogBuilding, TEXT("Deselecting deconstruct tool."));

			m_highlightedBuildInstance->ResetHighlightMaterial();
			m_highlightedBuildInstance = nullptr;

		}

	} else // If deconstruct tool was selected, it added the IMC already.
		m_controller->AddMappingContext(constructionModeIMC);

	// Then do the actual selecting

	m_tool = EConstructionTool::BuildTool;
	m_buildMenu->Open();

}
void UConstructionModeManager::SelectDeconstructTool() {

	if (m_tool == EConstructionTool::DeconstructTool) {

		ExitConstructionMode();
		return;

	}

	PW_LOG(LogBuilding, TEXT("Selecting deconstruct tool."));

	// First deselect the build tool if selected

	if (m_tool == EConstructionTool::BuildTool) {

		m_selectedBuild = nullptr;

		m_buildGhost->Reset();
		m_buildMenu->Close();

	} else // If build tool was selected prior, it added the IMC already.
		m_controller->AddMappingContext(constructionModeIMC);
	
	// Then do the actual selecting

	m_tool = EConstructionTool::DeconstructTool;

}

void UConstructionModeManager::ExitConstructionMode() {

	PW_LOG(LogBuilding, TEXT("Exiting construction mode."));

	switch (m_tool) {

	case EConstructionTool::BuildTool: {

		m_selectedBuild = nullptr;

		m_buildGhost->Reset();
		m_buildMenu->Close();

		break;

	}
	case EConstructionTool::DeconstructTool: {

		if (m_highlightedBuildInstance == nullptr) break;

		m_highlightedBuildInstance->ResetHighlightMaterial();
		m_highlightedBuildInstance = nullptr;

		break;

	}
	default: return;

	}

	m_controller->RemoveMappingContext(constructionModeIMC);
	m_tool = EConstructionTool::None;

}

void UConstructionModeManager::SelectBuild(UBuild* build) {

	m_selectedBuild = build;

	if (m_buildGhost == nullptr)
		SpawnBuildGhost(build->buildGhostClass);
	else if (m_buildGhost->GetClass() != build->buildGhostClass) {

		GetWorld()->DestroyActor(m_buildGhost);
		SpawnBuildGhost(build->buildGhostClass);

	}

	m_buildGhost->SetBuild(build);

	// Handle the build menu
	
	m_buildMenu->Close();

}

void UConstructionModeManager::Confirm() {

	switch (m_tool) {

	case EConstructionTool::BuildTool: ConfirmBuildTool(); break;
	case EConstructionTool::DeconstructTool: ConfirmDeconstructTool(); break;
	default: break;

	}

}
void UConstructionModeManager::ConfirmBuildTool() {

	if (m_selectedBuild == nullptr) return;
	if (!m_buildGhost->IsValidPlacement()) {

		PW_LOG_ERROR(LogBuilding, TEXT("Invalid build placement."));
		return;

	}

	PW_LOG(LogBuilding, TEXT("Build action confirmed."));

	// There is no 100% guarantee that TickComponent will be called, and therefore update the location
	// of the build ghost, before this function, so we have to do the ray again to get the correct position.

	FHitResult hit = FHitResult(EForceInit::ForceInit);
	VisibilityTrace(hit);

	if (!hit.bBlockingHit) {

		PW_LOG_ERROR(LogBuilding, TEXT("Invalid build placement."));
		return;

	}

	// Get the location

	FVector location = hit.ImpactPoint;
	if (m_gridSnap) {

		location.X = FMath::RoundToInt(location.X / gridSize) * gridSize;
		location.Y = FMath::RoundToInt(location.Y / gridSize) * gridSize;

	}

	m_buildGhost->Confirm(location, Cast<ABuildInstance>(hit.GetActor()));

}
void UConstructionModeManager::ConfirmDeconstructTool() {

	if (m_highlightedBuildInstance == nullptr) return;

	m_highlightedBuildInstance->Deconstruct();
	GetWorld()->DestroyActor(m_highlightedBuildInstance);

	m_highlightedBuildInstance = nullptr;

}

void UConstructionModeManager::BindUI(UBuildMenu* buildMenu) {

	m_buildMenu = buildMenu;
	m_buildMenu->UpdateSlots(availableBuilds);

}

void UConstructionModeManager::ToggleGridSnap() {

	if (m_tool != EConstructionTool::BuildTool) return;
	m_gridSnap = !m_gridSnap;

}

void UConstructionModeManager::RotateBuildGhost() {

	if (m_buildGhost == nullptr || m_buildGhost->IsHidden()) return;
	if (!m_selectedBuild->rotatable) return;

	m_buildGhost->AddActorWorldRotation(FRotator(0.0f, rotationAngle, 0.0f));

}