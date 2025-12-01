// Fill out your copyright notice in the Description page of Project Settings.


#include "RhythmPlayerController.h"
#include "RythmGameMode.h"
#include "MidiReader.h" 
#include "Kismet/GameplayStatics.h"

ARhythmPlayerController::ARhythmPlayerController()
{
	bShowMouseCursor = false;
}

void ARhythmPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Make sure keyboard input goes to the game, not UI
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	UE_LOG(LogTemp, Log, TEXT("RythmPlayerController::BeginPlay - input mode set to GameOnly"));
}

void ARhythmPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent(); // CRITICAL for any BP bindings / engine stuff

	check(InputComponent);

	// These action names must exist in Project Settings ? Input ? Action Mappings

	InputComponent->BindAction("Hit_Up", IE_Pressed, this, &ARhythmPlayerController::OnHitUp);
	InputComponent->BindAction("Hit_Down", IE_Pressed, this, &ARhythmPlayerController::OnHitDown);
	InputComponent->BindAction("Hit_Left", IE_Pressed, this, &ARhythmPlayerController::OnHitLeft);
	InputComponent->BindAction("Hit_Right", IE_Pressed, this, &ARhythmPlayerController::OnHitRight);

	InputComponent->BindAction("Hit_W", IE_Pressed, this, &ARhythmPlayerController::OnHitW);
	InputComponent->BindAction("Hit_A", IE_Pressed, this, &ARhythmPlayerController::OnHitA);
	InputComponent->BindAction("Hit_S", IE_Pressed, this, &ARhythmPlayerController::OnHitS);
	InputComponent->BindAction("Hit_D", IE_Pressed, this, &ARhythmPlayerController::OnHitD);
}

void ARhythmPlayerController::SendLaneInput(int32 LaneInputType)
{
	UWorld* World = GetWorld();
	if (!World) return;

	ARythmGameMode* GM = World->GetAuthGameMode<ARythmGameMode>();
	if (!GM)
	{
		UE_LOG(LogTemp, Warning, TEXT("RythmPlayerController: No RythmGameMode found"));
		return;
	}

	GM->HandleLaneInput(LaneInputType);
}

// ----- Individual key handlers -----

void ARhythmPlayerController::OnHitUp()
{
	UE_LOG(LogTemp, Verbose, TEXT("OnHitUp"));
	SendLaneInput((int32)ENoteInputType::Up);
}

void ARhythmPlayerController::OnHitDown()
{
	UE_LOG(LogTemp, Verbose, TEXT("OnHitDown"));
	SendLaneInput((int32)ENoteInputType::Down);
}

void ARhythmPlayerController::OnHitLeft()
{
	UE_LOG(LogTemp, Verbose, TEXT("OnHitLeft"));
	SendLaneInput((int32)ENoteInputType::Left);
}

void ARhythmPlayerController::OnHitRight()
{
	UE_LOG(LogTemp, Verbose, TEXT("OnHitRight"));
	SendLaneInput((int32)ENoteInputType::Right);
}

void ARhythmPlayerController::OnHitW()
{
	UE_LOG(LogTemp, Verbose, TEXT("OnHitW"));
	SendLaneInput((int32)ENoteInputType::W);
}

void ARhythmPlayerController::OnHitA()
{
	UE_LOG(LogTemp, Verbose, TEXT("OnHitA"));
	SendLaneInput((int32)ENoteInputType::A);
}

void ARhythmPlayerController::OnHitS()
{
	UE_LOG(LogTemp, Verbose, TEXT("OnHitS"));
	SendLaneInput((int32)ENoteInputType::S);
}

void ARhythmPlayerController::OnHitD()
{
	UE_LOG(LogTemp, Verbose, TEXT("OnHitD"));
	SendLaneInput((int32)ENoteInputType::D);
}