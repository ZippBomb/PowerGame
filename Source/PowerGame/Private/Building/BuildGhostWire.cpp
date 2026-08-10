#include "Building/BuildGhostWire.h"
#include "Building/Builds/Build.h"

#include "Building/ConstructionModeManager.h"

#include "Building/Instances/BuildInstance.h"
#include "Building/Instances/SplineBuildInstance.h"

#include "Building/Instances/Power/Wire.h"

void ABuildGhostWire::SetBuild(UBuild* build) {

	Super::SetBuild(build);

	// TODO: I don't like this :c
	SetActorHiddenInGame(true);

}

void ABuildGhostWire::Update(const FVector& hitLocation, ABuildInstance* hitBuildInstance) {

	// Handle snapping to poles and buildings

}

void ABuildGhostWire::Confirm(const FVector& location, ABuildInstance* targetBuildInstance) {

	if (targetBuildInstance == nullptr) return;

	UBuild* build = targetBuildInstance->GetBuild();

	switch (m_state) {

	case EBuildGhostState::Preview: {

		m_startBuildInstance = targetBuildInstance;
		m_startWireLocation = targetBuildInstance->GetActorTransform().TransformPosition(build->wireConnectionLocation);

		m_state = EBuildGhostState::StartSelected;

		break;

	}
	case EBuildGhostState::StartSelected: {

		if (targetBuildInstance == m_startBuildInstance) break;

		PW_ASSERT(m_build->buildInstanceClass != nullptr, LogBuilding, TEXT("Can't confirm build with invalid Build Instance class. Build: '%s'"), *GetNameSafe(m_build));

		ASplineBuildInstance* splineInstance = GetWorld()->SpawnActor<ASplineBuildInstance>(m_build->buildInstanceClass, m_startWireLocation, GetActorRotation());
		PW_ASSERT(splineInstance != nullptr, LogBuilding, TEXT("Could not spawn actor of type ABuildInstance."));

		splineInstance->SetBuild(m_build);
		splineInstance->SetStartAndEnd(m_startWireLocation, targetBuildInstance->GetActorTransform().TransformPosition(build->wireConnectionLocation));

		AWire* wire = Cast<AWire>(splineInstance);
		if (wire != nullptr) {

			PW_ASSERT(m_startBuildInstance != nullptr, LogBuilding, TEXT("Start build instance was not set in Wire build ghost."));
			wire->Connect(m_startBuildInstance, targetBuildInstance);

		}

		// Reset back to preview for the next wire

		m_state = EBuildGhostState::Preview;

		break;

	}
	default: break;

	}

}