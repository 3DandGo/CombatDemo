#pragma once

#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"

#define DEBUG_MESSAGE(Color, Message) if (GEngine) GEngine->AddOnScreenDebugMessage(1, 3.f, Color, Message);
#define DEBUG_SPHERE(Location, Color, Time) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 14.f, 18, Color, false, Time);