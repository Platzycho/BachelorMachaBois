// Fill out your copyright notice in the Description page of Project Settings.


#include "MidiReader.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFileManager.h"

UMidiReader::UMidiReader()
{
	NoteToInputMap.Add(48, ENoteInputType::One); 
	NoteToInputMap.Add(49, ENoteInputType::Two); 
	NoteToInputMap.Add(50, ENoteInputType::Three); 
	NoteToInputMap.Add(51, ENoteInputType::Four); 
	NoteToInputMap.Add(52, ENoteInputType::Five); 
}

UMidiReader::~UMidiReader()
{
}

bool UMidiReader::LoadMidiFile(const FString& FilePath)
{
	TArray<uint8> Data;
	if (!FFileHelper::LoadFileToArray(Data, *FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load MIDI file: %s"), *FilePath);
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("Loaded %d bytes from %s"), Data.Num(), *FilePath);
	return ParseMidiData(Data);
}

TArray<FMidiNoteEvent> UMidiReader::GetParsedNotes() const
{
	return ParsedNotes;
}

ENoteInputType UMidiReader::MapMidiNoteToInput(int32 NoteNumber)
{
	
	/*UE_LOG(LogTemp, Log, TEXT("MapMidiNoteToInput called for note %d"), NoteNumber);

	if (NoteNumber % 4 == 0) return ENoteInputType::Left;
	if (NoteNumber % 4 == 1) return ENoteInputType::Right;
	if (NoteNumber % 4 == 2) return ENoteInputType::Up;
	if (NoteNumber % 4 == 3) return ENoteInputType::Down;*/

	return ENoteInputType::None;
	
}

bool UMidiReader::ParseMidiData(const TArray<uint8>& Data)
{

	ParsedNotes.Empty();

	if (Data.Num() < 14)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid MIDI data"));
		return false;
	}

	int32 Index = 0;

	// --- Header ---
	if (!(Data[0] == 'M' && Data[1] == 'T' && Data[2] == 'h' && Data[3] == 'd'))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid header"));
		return false;
	}
	Index += 8; // Skip "MThd" + chunk length

	uint16 Format = (Data[Index] << 8) | Data[Index + 1];
	uint16 TrackCount = (Data[Index + 2] << 8) | Data[Index + 3];
	uint16 Division = (Data[Index + 4] << 8) | Data[Index + 5];
	Index += 6;

	UE_LOG(LogTemp, Log, TEXT("MIDI Format %d | Tracks %d | Division %d"), Format, TrackCount, Division);

	// --- Parse each track ---
	for (int t = 0; t < TrackCount; ++t)
	{
		if (Index + 8 >= Data.Num()) break;

		// Find "MTrk"
		if (!(Data[Index] == 'M' && Data[Index + 1] == 'T' && Data[Index + 2] == 'r' && Data[Index + 3] == 'k'))
		{
			UE_LOG(LogTemp, Warning, TEXT("No 'MTrk' found at track %d"), t);
			break;
		}
		Index += 4;

		uint32 TrackLength = (Data[Index] << 24) | (Data[Index + 1] << 16) | (Data[Index + 2] << 8) | Data[Index + 3];
		Index += 4;

		int32 TrackEnd = Index + TrackLength;
		double CurrentTicks = 0.0;
		double Tempo = 500000.0; // default 120 BPM
		double SecondsPerTick = Tempo / 1000000.0 / Division;

		uint8 RunningStatus = 0;

		while (Index < TrackEnd)
		{
			// --- Delta time (VLQ) ---
			uint32 Delta = 0;
			uint8 Byte = 0;
			do
			{
				Byte = Data[Index++];
				Delta = (Delta << 7) | (Byte & 0x7F);
			} while ((Byte & 0x80) && Index < TrackEnd);

			CurrentTicks += Delta;
			double CurrentTimeSec = CurrentTicks * SecondsPerTick;

			if (Index >= TrackEnd)
				break;

			uint8 StatusByte = Data[Index++];

			// Handle running status (if byte < 0x80)
			if (StatusByte < 0x80)
			{
				// It’s actually data, not status — use previous running status
				if (RunningStatus == 0)
				{
					UE_LOG(LogTemp, Warning, TEXT("Unexpected data byte without running status at %d"), Index);
					break;
				}
				Index--; // step back one, reuse as note
				StatusByte = RunningStatus;
			}
			else
			{
				RunningStatus = StatusByte;
			}

			uint8 EventType = StatusByte & 0xF0;

			// --- Meta Event ---
			if (StatusByte == 0xFF)
			{
				if (Index + 1 >= TrackEnd) break;
				uint8 MetaType = Data[Index++];
				uint8 MetaLength = Data[Index++];
				if (MetaType == 0x51 && MetaLength == 3)
				{
					Tempo = (Data[Index] << 16) | (Data[Index + 1] << 8) | Data[Index + 2];
					SecondsPerTick = Tempo / 1000000.0 / Division;
				}
				Index += MetaLength;
				continue;
			}
			// --- SysEx Event ---
			else if (StatusByte == 0xF0 || StatusByte == 0xF7)
			{
				if (Index >= TrackEnd) break;
				uint8 Length = Data[Index++];
				Index += Length;
				continue;
			}

			// --- Channel Event (Note On / Note Off) ---
			if (EventType == 0x80 || EventType == 0x90)
			{
				if (Index + 1 >= TrackEnd) break;

				uint8 Note = Data[Index++];
				uint8 Velocity = Data[Index++];

				if (EventType == 0x90 && Velocity > 0)
				{
					FMidiNoteEvent Event;
					Event.TimeSeconds = CurrentTimeSec;
					Event.NoteNumber = Note;
					Event.InputType = MapMidiNoteToInput(Note);

					if (Event.InputType != ENoteInputType::None)
					{
						ParsedNotes.Add(Event);
					}
				}
			}
			else
			{
				// Skip other unhandled events (like pitch bend, CC, etc.)
				int Skip = 0;
				switch (EventType)
				{
				case 0xA0:
				case 0xB0:
				case 0xE0:
					Skip = 2; break;
				case 0xC0:
				case 0xD0:
					Skip = 1; break;
				default:
					break;
				}
				Index += Skip;
			}
		}

		Index = TrackEnd;
	}

	UE_LOG(LogTemp, Log, TEXT("Parsed %d notes total"), ParsedNotes.Num());

	return true;
}

uint32 UMidiReader::ReadVariableLength(const uint8*& Ptr)
{
	uint32 Value = 0;
	uint8 Byte;

	do
	{
		Byte = *Ptr++;
		Value = (Value << 7) | (Byte & 0x7F);
	} while (Byte & 0x80);

	return Value;
}
