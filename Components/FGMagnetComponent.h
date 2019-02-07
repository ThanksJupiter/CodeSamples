#pragma once

#include "Components/ActorComponent.h"
#include "FGMagnetComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSMOnStayPositiveField, AActor*, Instigator, float, Force);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSMOnStayNegativeField, AActor*, Instigator, float, Force);

UCLASS(meta=(BlueprintSpawnableComponent))
class UFGMagnetComponent : public UActorComponent {

	GENERATED_BODY()

public:

	UFUNCTION()
	void EnableMagnetism();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void NotifyPositiveField(AActor* Instigator, float Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void NotifyNegativeField(AActor* Instigator, float Value);

	UPROPERTY(BlueprintAssignable, Category = Magnetism)
	FSMOnStayPositiveField OnStayPositiveField;

	UPROPERTY(BlueprintAssignable, Category = Magnetism)
	FSMOnStayNegativeField OnStayNegativeField;

	UPROPERTY()
	bool bIsDisrupted = false;
};
