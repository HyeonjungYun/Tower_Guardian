#include "PatrolPath.h"

APatrolPath::APatrolPath()
{
	PrimaryActorTick.bCanEverTick = false;

}

FVector APatrolPath::GetWaypoint(int32 Index) const
{
	if (Waypoints.IsValidIndex(Index))
		return GetActorLocation() + Waypoints[Index];

	UE_LOG(LogTemp, Warning, TEXT("Waypoint 반환 실패"));
	return FVector::Zero();
}

int32 APatrolPath::Num() const
{
	return Waypoints.Num();
}

