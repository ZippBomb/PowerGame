#pragma once

#include <CoreMinimal.h>
#include "Building/BuildGhost.h"

#include "BuildGhostNormal.generated.h"

class UBoxComponent;

UCLASS(Abstract)
class POWERGAME_API ABuildGhostNormal : public ABuildGhost {

	GENERATED_BODY()
	
public:
	ABuildGhostNormal();

	virtual inline bool IsValidPlacement() const override { return m_validPlacement; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	bool m_validPlacement = true;

	UPROPERTY(EditAnywhere, Category = "Preview")
	TObjectPtr<UMaterialInterface> m_validMaterial = nullptr;
	UPROPERTY(EditAnywhere, Category = "Preview")
	TObjectPtr<UMaterialInterface> m_invalidMaterial = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> validityTrigger = nullptr;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* overlappedComp, AActor* other, UPrimitiveComponent* otherComp, int32 otherIndex, bool fromSweep, const FHitResult& sweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* overlappedComp, AActor* other, UPrimitiveComponent* otherComp, int32 otherIndex);

};