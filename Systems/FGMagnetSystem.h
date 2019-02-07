#pragma once

#include "Components/PrimitiveComponent.h"
#include "SMMagnetSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSMOnPositiveField, TArray<USMMagnetComponent*>, MagnetsInRange);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSMOnNegativeField, TArray<USMMagnetComponent*>, MagnetsInRange);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSMOnDisableDisruptionField);

class USMMagnetComponent;
class AActor;

UCLASS(meta = (BlueprintSpawnableComponent))
class UFGMagnetSystem : public UPrimitiveComponent {

	GENERATED_BODY()

public:
	void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void Initialize(AActor* InActor);

	UFUNCTION()
	void EngagePositiveField(float Value);

	UFUNCTION()
	void EngageNegativeField(float Value);

	UFUNCTION(BlueprintCallable)
	void EngageDisruptionField();

	UFUNCTION(BlueprintCallable)
	void DisableDisruptionField();

	UFUNCTION(BlueprintCallable)
	void BeginOverlapMagneticObjects();

	UFUNCTION()
	void StopOverlap();

	void HandleAsyncOverlap(const FTraceHandle& TraceHandle, FOverlapDatum& OverlapDatum);

	UPROPERTY(BlueprintAssignable, Category = Magnetism)
	FSMOnPositiveField OnEngagePositiveField;

	UPROPERTY(BlueprintAssignable, Category = Magnetism)
	FSMOnNegativeField OnEngageNegativeField;

	UPROPERTY(BlueprintAssignable, Category = Magnetism)
	FSMOnDisableDisruptionField OnDisableDisruption;

	UPROPERTY(EditAnywhere, Category = Magnetism)
	float MagneticRange = 300.0f;

	UPROPERTY()
	AActor* OwnerActor = nullptr;

	UPROPERTY()
	TArray<USMMagnetComponent*> FoundMagnets;

	UPROPERTY()
	TArray<AActor*> AccessibleActors;

	FOverlapDelegate OverlapDelegate;

	FCollisionShape CachedCollisionShape;

	UPROPERTY(EditAnywhere, Category = Collision)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	UPROPERTY(EditAnywhere, Category = Collision)
	TArray<TSubclassOf<AActor>> ActorsToIgnore;

	FCollisionObjectQueryParams CachedCollisionObjectParams;

	FCollisionQueryParams CachedCollisionQueryParams;

	UPROPERTY()
	bool bIsOverlapping = false;
};
