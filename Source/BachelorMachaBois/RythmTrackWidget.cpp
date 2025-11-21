// Fill out your copyright notice in the Description page of Project Settings.


#include "RythmTrackWidget.h"
#include "Components/PanelWidget.h"
#include "Components/Widget.h"
#include "Kismet/GameplayStatics.h"

URythmTrackWidget::URythmTrackWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void URythmTrackWidget::NativeConstruct()
{
	Super::NativeConstruct();

	LaneMap.Empty();

	if (!LaneContainer)
	{
		UE_LOG(LogTemp, Error, TEXT("RythmTrackWidget: LaneContainer is NOT bound!"));
		return;
	}

	// -----------------------------
	// RECURSIVE LANE FINDING (Option A)
	// -----------------------------
	FindLanesRecursive(LaneContainer);

	UE_LOG(LogTemp, Log, TEXT("TrackWidget: Registered %d lanes."), LaneMap.Num());

	for (const auto& Pair : LaneMap)
	{
		UE_LOG(LogTemp, Log, TEXT("   Lane InputType %d mapped to widget %s"),
			Pair.Key,
			*Pair.Value->GetName());
	}

	StartTime = 0.f;
}

void URythmTrackWidget::FindLanesRecursive(UWidget* Widget)
{
	if (!Widget)
		return;

	// If this widget *is* a lane → register it
	if (ULaneWidgetBase* Lane = Cast<ULaneWidgetBase>(Widget))
	{
		LaneMap.Add(Lane->LaneInputType, Lane);
		return;
	}

	// If it's a panel, recurse into children
	if (UPanelWidget* Panel = Cast<UPanelWidget>(Widget))
	{
		const int32 Count = Panel->GetChildrenCount();
		for (int32 i = 0; i < Count; i++)
		{
			UWidget* Child = Panel->GetChildAt(i);
			FindLanesRecursive(Child);
		}
	}
}

void URythmTrackWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void URythmTrackWidget::InitializeTrack(const TArray<FMidiNoteEvent>& Notes)
{
	PendingNotes = Notes;
	PendingNotes.Sort([](const FMidiNoteEvent& A, const FMidiNoteEvent& B)
		{
			return A.TimeSeconds < B.TimeSeconds;
		});

	UE_LOG(LogTemp, Log, TEXT("TrackWidget: Received %d notes"), PendingNotes.Num());
}

void URythmTrackWidget::TickTrackWidget(float CurrentSongTime, float DeltaTime)
{
	if (PendingNotes.Num() == 0) return;

	int32 SpawnCount = 0;

	while (PendingNotes.Num() > 0)
	{
		const FMidiNoteEvent& Event = PendingNotes[0];

		// Too early to spawn → stop
		if (Event.TimeSeconds > CurrentSongTime + SpawnLeadTime)
			break;

		// Find lane
		if (ULaneWidgetBase** LanePtr = LaneMap.Find((int32)Event.InputType))
		{
			if (ULaneWidgetBase* Lane = *LanePtr)
			{
				Lane->SpawnNote(Event);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("TrackWidget: No lane found for input %d"),
				(int32)Event.InputType);
		}

		PendingNotes.RemoveAt(0);
		SpawnCount++;
	}

	if (SpawnCount > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("TrackWidget: Spawned %d notes at time %.3f"),
			SpawnCount,
			CurrentSongTime);
	}
}