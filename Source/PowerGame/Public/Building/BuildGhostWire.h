#pragma once

#include <CoreMinimal.h>
#include <Building/BuildGhost.h>

#include "BuildGhostWire.generated.h"

class ABuildInstance;

UCLASS()
class POWERGAME_API ABuildGhostWire : public ABuildGhost {

	GENERATED_BODY()

public:
	virtual void SetBuild(UBuild* build) override;

	virtual void Update(const FVector& hitLocation, ABuildInstance* hitBuildInstance) override;

	virtual ABuildInstance* Confirm(const FVector& location, ABuildInstance* targetBuildInstance) override;

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ABuildInstance> m_startBuildInstance = nullptr;
	UPROPERTY(VisibleAnywhere)
	FVector m_startWireLocation = FVector::ZeroVector;

};