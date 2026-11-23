#include "Building/BuildGhostConveyor.h"
#include "Building/ConstructionModeManager.h"

#include "Building/Instances/BuildInstance.h"

#include "Logistics/ItemReceiver.h"
#include "Logistics/Conveyor.h"

#include <Components/SplineComponent.h>
#include <Components/SplineMeshComponent.h>

ABuildGhostConveyor::ABuildGhostConveyor() {

	m_spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	m_spline->SetMobility(EComponentMobility::Movable);
	SetRootComponent(m_spline);

	m_splineMesh = CreateDefaultSubobject<USplineMeshComponent>(TEXT("Spline mesh"));
	m_splineMesh->SetMobility(EComponentMobility::Movable);
	m_splineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_splineMesh->SetupAttachment(m_spline);

	m_mesh = m_splineMesh;

}

void ABuildGhostConveyor::Update(const FVector& hitLocation, ABuildInstance* hitBuildInstance) {

	// Get the location and rotation
	// In the case we hit close enough to a machine slot, it will snap to it, otherwie it will just be
	// the hit location and actor rotation

	// NOTE: The addition is just a temporary thing before conveyor stands and shit is implemented
	FVector location = hitLocation + FVector(0.0f, 0.0f, 70.0f);
	FQuat rotation = GetActorRotation().Quaternion();

	machineSlot = GetMachineSlot(hitLocation, hitBuildInstance);
	if (machineSlot != nullptr) {

		FTransform connectionPoint = machineSlot->transform * hitBuildInstance->GetActorTransform();

		location = connectionPoint.GetLocation();
		rotation = connectionPoint.GetRotation();

	}

	switch (m_state) {

	case EBuildGhostState::Preview: {

		SetActorLocation(location);
		SetActorRotation(rotation);

		break;

	}
	case EBuildGhostState::StartSelected: {

		m_spline->SetLocationAtSplinePoint(1, location, ESplineCoordinateSpace::World);
		if (machineSlot != nullptr) {

			FTransform transform = machineSlot->transform * hitBuildInstance->GetActorTransform();
			FVector tangent = transform.GetUnitAxis(EAxis::X) * tangentLength;
			m_spline->SetTangentsAtSplinePoint(1, tangent, tangent, ESplineCoordinateSpace::World);

		}

		FVector startPos, startTangent;
		FVector endPos, endTangent;

		m_spline->GetLocationAndTangentAtSplinePoint(0, startPos, startTangent, ESplineCoordinateSpace::Local);
		m_spline->GetLocationAndTangentAtSplinePoint(1, endPos, endTangent, ESplineCoordinateSpace::Local);

		m_splineMesh->SetStartAndEnd(startPos, startTangent, endPos, endTangent);

		break;

	}

	}

	SetActorHiddenInGame(false);

}

ABuildInstance* ABuildGhostConveyor::Confirm(const FVector& location, ABuildInstance* targetBuildInstance) {

	switch (m_state) {

	case EBuildGhostState::Preview: {

		m_startBuildInstance = targetBuildInstance;
		m_startSlot = machineSlot;

		m_spline->SetLocationAtSplinePoint(0, GetActorLocation(), ESplineCoordinateSpace::World);
		m_spline->SetLocationAtSplinePoint(1, GetActorLocation(), ESplineCoordinateSpace::World);

		if (machineSlot != nullptr) {

			FTransform transform = machineSlot->transform * targetBuildInstance->GetActorTransform();
			FVector tangent = transform.GetUnitAxis(EAxis::X) * tangentLength;
			m_spline->SetTangentsAtSplinePoint(0, tangent, tangent, ESplineCoordinateSpace::World);

		}

		m_state = EBuildGhostState::StartSelected;

		return nullptr;

	}
	case EBuildGhostState::StartSelected: {

		// Spawn the conveyor belt

		AConveyor* instance = Cast<AConveyor>(Super::Confirm(GetActorLocation(), targetBuildInstance));
		PW_ASSERT(instance != nullptr, LogBuilding, TEXT("Could not cast spawned actor of type ABuildInstance to AConveyor."));

		// Setup the slots and conveyor belt

		if (m_startSlot != nullptr)
			m_startSlot->attachedConveyor = instance;
		if (machineSlot != nullptr) {

			machineSlot->attachedConveyor = instance;

			if (machineSlot->input)
				instance->connectedReceiver = targetBuildInstance;

		}

		instance->SetSpline(m_spline);

		// Reset the build ghost

		FVector pos = m_spline->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::World);
		SetActorLocation(pos);

		m_startBuildInstance = instance;
		m_startSlot = &instance->slots[1]; // TODO: This needs to be defined better for when the conveyor belt is reversed

		ResetSpline();

		FTransform transform = m_startSlot->transform * instance->GetActorTransform();
		FVector tangent = transform.GetUnitAxis(EAxis::X) * tangentLength;
		m_spline->SetTangentsAtSplinePoint(0, tangent, tangent, ESplineCoordinateSpace::World);
		m_spline->SetTangentsAtSplinePoint(1, tangent, tangent, ESplineCoordinateSpace::World);

		return instance;

	}
	default: return nullptr;

	}

}

void ABuildGhostConveyor::Reset() {

	Super::Reset();

	ResetSpline();

}

FMachineItemSlot* ABuildGhostConveyor::GetMachineSlot(const FVector& location, ABuildInstance* buildInstance) const {

	if (buildInstance == nullptr || !buildInstance->Implements<UItemReceiver>()) return nullptr;

	IItemReceiver* receiver = Cast<IItemReceiver>(buildInstance);
	PW_ASSERT(receiver != nullptr, LogLogistics, TEXT("Build instance that implements IItemReceiver failed to cast."));

	TArray<FMachineItemSlot>& slots = receiver->GetMachineSlots();

	FMachineItemSlot* bestSlot = nullptr;
	float bestDistance = MAX_FLT;
	for (FMachineItemSlot& slot : slots) {

		if (slot.attachedConveyor != nullptr) continue;

		const FVector worldLocation = buildInstance->GetActorTransform().TransformPosition(slot.transform.GetLocation());
		const float distance = FVector::DistSquared(location, worldLocation);

		if (distance >= bestDistance) continue;

		bestDistance = distance;
		bestSlot = &slot;

	}
	if (bestDistance > maxSnapDistance * maxSnapDistance)
		bestSlot = nullptr;

	return bestSlot;

}

void ABuildGhostConveyor::ResetSpline() {

	m_spline->ResetToDefault();
	m_splineMesh->SetStartAndEnd(FVector::ZeroVector, FVector::ZeroVector, FVector(100.0f, 0.0f, 0.0f), FVector(100.0f, 0.0f, 0.0f));

}
