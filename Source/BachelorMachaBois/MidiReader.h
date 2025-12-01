// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MidiReader.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ENoteInputType : uint8
{
	None UMETA(DisplayName = "None"),
	W UMETA(DisplayName = "W"),
	A UMETA(DisplayName = "A"),
	S UMETA(DisplayName = "S"),
	D UMETA(DisplayName = "D"),
	Up UMETA(DisplayName = "Up"),
	Down UMETA(DisplayName = "Down"),
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right")
};

USTRUCT(BlueprintType)
struct FMidiNoteEvent
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "MIDI")
	float TimeSeconds = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "MIDI")
	float DurationSeconds = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "MIDI")
	int32 NoteNumber = 0;

	UPROPERTY(BlueprintReadOnly, Category = "MIDI")
	ENoteInputType InputType = ENoteInputType::None;
};

UCLASS(Blueprintable)
class BACHELORMACHABOIS_API UMidiReader : public UObject
{
	GENERATED_BODY()

public:
	UMidiReader();
	~UMidiReader();

	/** Load and parse the MIDI file. Returns true on success. */
	bool LoadMidiFile(const FString& FilePath);

	/** Access parsed notes from C++ without copying */
	const TArray<FMidiNoteEvent>& GetParsedNotes() const { return ParsedNotes; }

	/** Default mapping editable in editor (optional) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MIDI")
	TMap<int32, ENoteInputType> NoteToInputMap;

protected:
	/** Parsed and ready-to-use note events */
	UPROPERTY(BlueprintReadOnly)
	TArray<FMidiNoteEvent> ParsedNotes;

	ENoteInputType MapMidiNoteToInput(int32 NoteNumber);

private:
	bool ParseMidiData(const TArray<uint8>& Data);
};