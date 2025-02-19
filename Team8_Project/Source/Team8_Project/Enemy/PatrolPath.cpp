#include "PatrolPath.h"

APatrolPath::APatrolPath()
{
	PrimaryActorTick.bCanEverTick = false;

}

FVector APatrolPath::GetWaypoint(int32 Index) const
{
	if (Waypoints.IsValidIndex(Index))
		return Waypoints[Index];

	return FVector::Zero();
}

int32 APatrolPath::Num() const
{
	return Waypoints.Num();
}

