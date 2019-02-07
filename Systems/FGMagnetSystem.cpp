#include "FGMagnetSystem.h"
#include "WorldCollision.h"
#include "Engine/CollisionProfile.h"
#include "Components/SMMagnetComponent.h"
#include "DrawDebugHelpers.h"


void UFGMagnetSystem::BeginPlay() {
	Super::BeginPlay();

	CachedCollisionShape = FCollisionShape::MakeSphere(MagneticRange);
	CachedCollisionQueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(SphereOverlapComponents), false);

	for (auto Iter = ObjectTypes.CreateConstIterator(); Iter; ++Iter) {
		const ECollisionChannel& Channel = UCollisionProfile::Get()->ConvertToCollisionChannel(false, *Iter);
		CachedCollisionObjectParams.AddObjectTypesToQuery(Channel);
	}

	OverlapDelegate.BindUObject(this, &UFGMagnetSystem::HandleAsyncOverlap);
}

void UFGMagnetSystem::Initialize(AActor* InActor)
{
	OwnerActor = InActor;
}

void UFGMagnetSystem::EngagePositiveField(float Value)
{
	if (Value > KINDA_SMALL_NUMBER) {
		
		DrawDebugSphere(GetWorld(), OwnerActor->GetActorLocation(), MagneticRange, 50, FColor::Red, false, -1.0f, (uint8)'\000', 0.5f);
		OnEngagePositiveField.Broadcast(FoundMagnets);

		for (USMMagnetComponent* Magnet : FoundMagnets) {
			Magnet->NotifyPositiveField(OwnerActor, Value);
		}
	}
}

void UFGMagnetSystem::EngageNegativeField(float Value)
{
	if (Value > KINDA_SMALL_NUMBER) {
		
		DrawDebugSphere(GetWorld(), OwnerActor->GetActorLocation(), MagneticRange, 50, FColor::Blue, false, -1.0f, (uint8)'\000', 0.5f);
		OnEngageNegativeField.Broadcast(FoundMagnets);

		for (USMMagnetComponent* Magnet : FoundMagnets) {
			Magnet->NotifyNegativeField(OwnerActor, Value);
		}
	}
}

void UFGMagnetSystem::EngageDisruptionField() {

	for (USMMagnetComponent* Magnet : FoundMagnets) {
		Magnet->bIsDisrupted = true;

		OnDisableDisruption.AddUniqueDynamic(Magnet, &USMMagnetComponent::EnableMagnetism);
	}

	StopOverlap();
}

void UFGMagnetSystem::DisableDisruptionField() {
	OnDisableDisruption.Broadcast();
	OnDisableDisruption.RemoveAll(this);
}

void UFGMagnetSystem::BeginOverlapMagneticObjects() {
	
	if (!GetWorld())
		return;

	if (!OwnerActor) {
		return;
	}

	if (!bIsOverlapping) {
		bIsOverlapping = true;

		FCollisionQueryParams CollisionQueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(SphereOverlapActors), false);
		CollisionQueryParams.AddIgnoredActor(OwnerActor);

		GetWorld()->AsyncOverlapByObjectType(OwnerActor->GetActorLocation(), FQuat::Identity, CachedCollisionObjectParams, CachedCollisionShape, CollisionQueryParams, &OverlapDelegate);
	}
}

void UFGMagnetSystem::StopOverlap() {
	bIsOverlapping = false;
}

void UFGMagnetSystem::HandleAsyncOverlap(const FTraceHandle& TraceHandle, FOverlapDatum& OverlapDatum) {
	UWorld* World = GetWorld();

	if (!World)
		return;

	AccessibleActors.Empty();
	FoundMagnets.Empty();

	const FVector StartLocation = OwnerActor->GetActorLocation();
	FHitResult Hit;
	FCollisionQueryParams LineTraceCollisionQueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(LineTraceSingle), false);

	TArray<AActor*> TargetsInLineOfSight;

	for (FOverlapResult& Overlap : OverlapDatum.OutOverlaps) {

		if (!Overlap.Actor->IsValidLowLevel()) {
			continue;
		}

		for (TSubclassOf<AActor> ClassToIgnore : ActorsToIgnore)
		{
			if (Overlap.Actor->IsA(ClassToIgnore)) {
				continue;
			}
		}

		if (!Overlap.Actor->FindComponentByClass(USMMagnetComponent::StaticClass())) {
			continue;
		}

		const bool bHit = World->LineTraceSingleByChannel(Hit, StartLocation, Overlap.Actor->GetActorLocation(), ECC_Visibility, LineTraceCollisionQueryParams);
		AActor* FoundActor = Hit.GetActor();
		AActor* OverlappedActor = Overlap.GetActor();

		if (!bHit || FoundActor == OverlappedActor) {
			TargetsInLineOfSight.Add(OverlappedActor);
		}

		for (AActor* AvailableTarget : TargetsInLineOfSight) {
			if (!AccessibleActors.Contains(AvailableTarget)) {
				AccessibleActors.Add(AvailableTarget);
			}

			USMMagnetComponent* MagComp = Cast<USMMagnetComponent>(AvailableTarget->GetComponentByClass(USMMagnetComponent::StaticClass()));
			
			if (!FoundMagnets.Contains(MagComp)) {
				FoundMagnets.Add(MagComp);
			}
		}
	}
}
