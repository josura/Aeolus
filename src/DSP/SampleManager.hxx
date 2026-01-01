#pragma once
#include "SampleBufferData.h"

class SampleManager {
public:
    // Called by the UI Thread (File Loader)
    void updateCurrentBuffer(SampleBufferData::Ptr newBuffer) {
        // Atomic exchange of the pointer
        currentBuffer.store(newBuffer.get());
        // Ensure the old buffer stays alive until the swap is complete
        retainedBuffer = newBuffer; 
    }

    // Called by the Audio Thread (Lock-Free)
    SampleBufferData::Ptr getCurrentBuffer() const {
        return retainedBuffer;
    }

private:
    std::atomic<SampleBufferData*> currentBuffer{ nullptr };
    SampleBufferData::Ptr retainedBuffer; // Simple way to keep it alive
};