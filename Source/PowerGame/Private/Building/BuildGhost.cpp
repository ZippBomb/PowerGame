#include "Building/BuildGhost.h"
#include "Building/Builds/Build.h"

#include "Building/ConstructionModeManager.h"

#include "Building/Instances/BuildInstance.h"

ABuildGhost::ABuildGhost() {

	PrimaryActorTick.bCanEverTick = false;

	m_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	m_mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(m_mesh);

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

void ABuildGhost::Confirm(const FVector& location, ABuildInstance* targetBuildInstance) {

	PW_ASSERT(m_state == EBuildGhostState::Preview, LogBuilding, TEXT("Invalid Build Ghost placement state (must be EBuildGhostPlacementState::Preview)."));
	PW_ASSERT(m_build->buildInstanceClass != nullptr, LogBuilding, TEXT("Can't confirm build with invalid Build Instance class. Build: '%s'"), *GetNameSafe(m_build));

	ABuildInstance* buildInstance = GetWorld()->SpawnActor<ABuildInstance>(m_build->buildInstanceClass, location, GetActorRotation());
	PW_ASSERT(buildInstance != nullptr, LogBuilding, TEXT("Could not spawn actor of type ABuildInstance."));

	buildInstance->SetBuild(m_build);

}

void ABuildGhost::Reset() {

	m_build = nullptr;
	SetActorHiddenInGame(true);

}