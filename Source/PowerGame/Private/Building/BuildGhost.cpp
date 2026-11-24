#include "Building/BuildGhost.h"
#include "Building/Builds/Build.h"

#include "Building/ConstructionModeManager.h"

#include "Building/Instances/BuildInstance.h"

ABuildGhost::ABuildGhost() {

	PrimaryActorTick.bCanEverTick = false;

}

void ABuildGhost::BeginPlay() {

	Super::BeginPlay();

}

void ABuildGhost::SetBuild(UBuild* build) {

	m_state = EBuildGhostState::Preview;
	m_build = build;

	m_mesh->SetStaticMesh(build->mesh);

	SetActorHiddenInGame(false);

}

void ABuildGhost::Update(const FVector& hitLocation, ABuildInstance* hitBuildInstance) {

	// In case the build ghost was hidden for a period of time and suddenly brought up again
	SetActorHiddenInGame(false);

	SetActorLocation(hitLocation);

}
void ABuildGhost::Rotate(float angle) {

	AddActorWorldRotation(FRotator(0.0f, angle, 0.0f));

}

ABuildInstance* ABuildGhost::Confirm(const FVector& location, ABuildInstance* targetBuildInstance) {

	PW_ASSERT(m_state != EBuildGhostState::None, LogBuilding, TEXT("Invalid Build Ghost placement state."));
	PW_ASSERT(m_build->buildInstanceClass != nullptr, LogBuilding, TEXT("Can't confirm build with invalid Build Instance class. Build: '%s'"), *GetNameSafe(m_build));

	ABuildInstance* buildInstance = GetWorld()->SpawnActor<ABuildInstance>(m_build->buildInstanceClass, location, GetActorRotation());
	PW_ASSERT(buildInstance != nullptr, LogBuilding, TEXT("Could not spawn actor of type ABuildInstance."));

	buildInstance->SetBuild(m_build);

	return buildInstance;

}

void ABuildGhost::Reset() {

	m_build = nullptr;
	SetActorHiddenInGame(true);

}