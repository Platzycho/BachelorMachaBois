// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ENoteInputType.generated.h"
/**
 * 
 */
UENUM(BlueprintType)
enum class ENoteInputType : uint8
{
	None	UMETA(DisplayName = "None"),
	Left	UMETA(DisplayName = "Left"),
	Right	UMETA(DisplayName = "Right"),
	Up		UMETA(DisplayName = "Up"),
	Down	UMETA(DisplayName = "Down")
};
