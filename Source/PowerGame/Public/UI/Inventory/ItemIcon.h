#pragma once

#include <CoreMinimal.h>
#include "UI/UserWidgetBase.h"

#include "ItemIcon.generated.h"

class UImage;

UCLASS(Abstract)
class POWERGAME_API UItemIcon : public UUserWidgetBase {

	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetIcon(UTexture2D* sprite);
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UImage> icon = nullptr;

};