#pragma once

#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"

#define DEBUG_MESSAGE(Color, Message) if (GEngine) GEngine->AddOnScreenDebugMessage(1, 3.f, Color, Message);