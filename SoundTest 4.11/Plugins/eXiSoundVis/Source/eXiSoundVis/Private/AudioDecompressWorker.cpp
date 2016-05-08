// Fill out your copyright notice in the Description page of Project Settings.

#include "eXiSoundVisPrivatePCH.h"

#include "AudioDecompressWorker.h"
#include "SoundVisComponent.h"

#include "AudioDevice.h"
#include "Developer/TargetPlatform/Public/Interfaces/IAudioFormat.h"

FAudioDecompressWorker* FAudioDecompressWorker::Runnable = NULL;
int32 FAudioDecompressWorker::ThreadCounter = 0;

//Note: AudioInfo is null, shuffled around to see if we could instantiate it properly
FAudioDecompressWorker::FAudioDecompressWorker(class USoundWave* InSoundWaveRef)
	: Thread(NULL)
    , SoundWaveRef(InSoundWaveRef)
	, AudioInfo(NULL)
{
	if (GEngine && GEngine->GetMainAudioDevice())
	{

        //@NOTE: Experiment to deterine if the Compression name is valid
        if(InSoundWaveRef->CompressionName.IsValid())
        {
            UE_LOG(LogeXiSoundVis, Warning, TEXT("FAudioDecompressWorker::FAudioDecompressionWorker; Compression name is valid."));
        }
        else
        {
            UE_LOG(LogeXiSoundVis, Warning, TEXT("FAudioDecompressWorker::FAudioDecompressionWorker; Compression name is invalid!"));
        }
        
        
        //@NOTE: Experiment to determine what the compression name is
        FString CompressionNameString = InSoundWaveRef->CompressionName.ToString();
        UE_LOG(LogeXiSoundVis, Warning, TEXT("FAudioDecompressWorker::FAudioDecompressWorker; InSoundWaveRef->CompressionName is %s"), *CompressionNameString);
        
        
        //@NOTE: Maybe refer to this code block if the data is null when running?
		AudioInfo = GEngine->GetMainAudioDevice()->CreateCompressedAudioInfo(SoundWaveRef);
        UE_LOG(LogeXiSoundVis, Warning, TEXT("FAudioDecompressWorker::FAudioDecompressionWorker; AudioInfo constructed!"));
        if(AudioInfo == nullptr)
        {
            UE_LOG(LogeXiSoundVis, Warning, TEXT("FAudioDecompressWorker::FAudioDecompressionWorker; AudioInfo was null after construction!"));
        }
	}
    else
    {
        UE_LOG(LogeXiSoundVis, Warning, TEXT("FAudioDecompressWorker::FAudioDecompressionWorker; GetMainAudioDevice or GEngine not present!"));
    }

	// Higher overall ThreadCounter to avoid duplicated names
	FAudioDecompressWorker::ThreadCounter++;

	Thread = FRunnableThread::Create(this, *FString::Printf(TEXT("FAudioDecompressWorker%d"), FAudioDecompressWorker::ThreadCounter), 0, EThreadPriority::TPri_Normal);
}

FAudioDecompressWorker::~FAudioDecompressWorker()
{
	delete Thread;
	Thread = NULL;
}

FAudioDecompressWorker* FAudioDecompressWorker::InitializeWorker(class USoundWave* InSoundWaveRef)
{
	Runnable = new FAudioDecompressWorker(InSoundWaveRef);

	return Runnable;
}

void FAudioDecompressWorker::ShutdownWorker()
{
	if (Runnable)
	{
		Runnable->EnsureCompletion();
		delete Runnable;
		Runnable = NULL;
	}
}

bool FAudioDecompressWorker::Init()
{
	// Make sure the Worker is marked is not finished
	bIsFinished = false;

	return true;
}

uint32 FAudioDecompressWorker::Run()
{
	if (!SoundWaveRef)
	{
        UE_LOG(LogeXiSoundVis, Warning, TEXT("FAudioDecompressWorker::Run; Passed SoundWave pointer is a nullptr!"));
		return 0;
	}

	if (AudioInfo != NULL)
	{
		FSoundQualityInfo QualityInfo = { 0 };

		// Parse the audio header for the relevant information
		if (!(SoundWaveRef->ResourceData))
		{
            UE_LOG(LogeXiSoundVis, Warning, TEXT("FAudioDecompressWorker::Run; Could not find resource data!"));
			return 0;
		}

		if (AudioInfo->ReadCompressedInfo(SoundWaveRef->ResourceData, SoundWaveRef->ResourceSize, &QualityInfo))
		{
			FScopeCycleCounterUObject WaveObject(SoundWaveRef);

			// Extract the data
			SoundWaveRef->SampleRate = QualityInfo.SampleRate;
			SoundWaveRef->NumChannels = QualityInfo.NumChannels;

			if (QualityInfo.Duration > 0.0f)
			{
				SoundWaveRef->Duration = QualityInfo.Duration;
			}

			const uint32 PCMBufferSize = SoundWaveRef->Duration * SoundWaveRef->SampleRate * SoundWaveRef->NumChannels;
            
            
			SoundWaveRef->CachedRealtimeFirstBuffer = new uint8[PCMBufferSize * 2];
            UE_LOG(LogeXiSoundVis, Warning,TEXT("FAudioDecompressWorker::Run; PCM Buffer size: %d"), PCMBufferSize );

			AudioInfo->SeekToTime(0.0f);
			AudioInfo->ReadCompressedData(SoundWaveRef->CachedRealtimeFirstBuffer, false, PCMBufferSize * 2);
		}
		else if (SoundWaveRef->DecompressionType == DTYPE_RealTime || SoundWaveRef->DecompressionType == DTYPE_Native)
		{
			SoundWaveRef->RemoveAudioResource();
            UE_LOG(LogeXiSoundVis, Warning, TEXT("FAudioDecompressWorker::Run; Removing audio resource."));
		}
        else
        {
            UE_LOG(LogeXiSoundVis, Warning, TEXT("FAudioDecompressWorker::Run; Edge case detected. Deleting audio info."));
        }

		delete AudioInfo;
	}
    else
    {
        UE_LOG(LogeXiSoundVis, Warning, TEXT("FAudioDecompressWorker::Run; Audio info is null!"));
        
        //Check if we can access the Game
        
        if (GEngine && GEngine->GetMainAudioDevice())
        {
            //@NOTE: Maybe refer to this code block if the data is null when running?
            AudioInfo = GEngine->GetMainAudioDevice()->CreateCompressedAudioInfo(SoundWaveRef);
            if(AudioInfo == nullptr)
            {
                UE_LOG(LogeXiSoundVis, Warning, TEXT("FAudioDecompressWorker::Run; AudioInfo could not be constructed even when GEngine and GetMainAudioDevice are present!"));
            }
            else
            {
                UE_LOG(LogeXiSoundVis, Warning, TEXT("FAudioDecompressWorker::Run; Audio info reconstructed at runtime!"));
            }
        }
        else
        {
            UE_LOG(LogeXiSoundVis, Warning, TEXT("FAudioDecompressWorker::Run; GetMainAudioDevice or GEngine not present!"));
        }
        
    }

	return 0;
}

void FAudioDecompressWorker::Stop()
{
	StopTaskCounter.Increment();
}

void FAudioDecompressWorker::Exit()
{
	// Make sure to mark Thread as finished
	bIsFinished = true;
}

void FAudioDecompressWorker::EnsureCompletion()
{
	Stop();

	if (Thread != NULL) {

		Thread->WaitForCompletion();
	}		
}
