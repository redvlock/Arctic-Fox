/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et cindent: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/TaskQueue.h"

#include "H264Converter.h"
#include "ImageContainer.h"
#include "MediaInfo.h"
#include "mp4_demuxer/AnnexB.h"
#include "mp4_demuxer/H264.h"

namespace mozilla
{

H264Converter::H264Converter(PlatformDecoderModule* aPDM,
                             const VideoInfo& aConfig,
                             layers::LayersBackend aLayersBackend,
                             layers::ImageContainer* aImageContainer,
                             FlushableTaskQueue* aVideoTaskQueue,
                             MediaDataDecoderCallback* aCallback)
  : mPDM(aPDM)
  , mCurrentConfig(aConfig)
  , mLayersBackend(aLayersBackend)
  , mImageContainer(aImageContainer)
  , mVideoTaskQueue(aVideoTaskQueue)
  , mCallback(aCallback)
  , mDecoder(nullptr)
  , mNeedAVCC(aPDM->DecoderNeedsConversion(aConfig) == PlatformDecoderModule::kNeedAVCC)
  , mDecoderInitializing(false)
  , mLastError(NS_OK)
{
  CreateDecoder();
}

H264Converter::~H264Converter()
{
}

nsRefPtr<MediaDataDecoder::InitPromise>
H264Converter::Init()
{
  if (mDecoder) {
    return mDecoder->Init();
  }

  return MediaDataDecoder::InitPromise::CreateAndReject(
           MediaDataDecoder::DecoderFailureReason::INIT_ERROR, __func__);
}

nsresult
H264Converter::Input(MediaRawData* aSample)
{
  if (!mNeedAVCC) {
    if (!mp4_demuxer::AnnexB::ConvertSampleToAnnexB(aSample)) {
      return NS_ERROR_FAILURE;
    }
  } else {
    if (!mp4_demuxer::AnnexB::ConvertSampleToAVCC(aSample)) {
      return NS_ERROR_FAILURE;
    }
  }

  if (mDecoderInitializing) {
    mMediaRawSamples.AppendElement(aSample);
    return NS_OK;
  }

  nsresult rv;
  if (!mDecoder) {
    // It is not possible to create an AVCC H264 decoder without SPS.
    // As such, creation will fail if the extra_data just extracted doesn't
    // contain a SPS.
    rv = CreateDecoderAndInit(aSample);
    if (rv == NS_ERROR_NOT_INITIALIZED) {
      // We are missing the required SPS to create the decoder.
      // Ignore for the time being, the MediaRawData will be dropped.
      return NS_OK;
    }
  } else {
    rv = CheckForSPSChange(aSample);
  }
  NS_ENSURE_SUCCESS(rv, rv);

  aSample->mExtraData = mCurrentConfig.mExtraData;

  return mDecoder->Input(aSample);
}

nsresult
H264Converter::Flush()
{
  if (mDecoder) {
    return mDecoder->Flush();
  }
  return mLastError;
}

nsresult
H264Converter::Drain()
{
  if (mDecoder) {
    return mDecoder->Drain();
  }
  mCallback->DrainComplete();
  return mLastError;
}

nsresult
H264Converter::Shutdown()
{
  if (mDecoder) {
    nsresult rv = mDecoder->Shutdown();
    mInitPromiseRequest.DisconnectIfExists();
    mDecoder = nullptr;
    return rv;
  }
  return NS_OK;
}

bool
H264Converter::IsHardwareAccelerated(nsACString& aFailureReason) const
{
  if (mDecoder) {
    return mDecoder->IsHardwareAccelerated(aFailureReason);
  }
  return MediaDataDecoder::IsHardwareAccelerated(aFailureReason);
}

nsresult
H264Converter::CreateDecoder()
{
  if (mNeedAVCC && !mp4_demuxer::AnnexB::HasSPS(mCurrentConfig.mExtraData)) {
    // nothing found yet, will try again later
    return NS_ERROR_NOT_INITIALIZED;
  }
  UpdateConfigFromExtraData(mCurrentConfig.mExtraData);

  mDecoder = mPDM->CreateVideoDecoder(mCurrentConfig,
                                      mLayersBackend,
                                      mImageContainer,
                                      mVideoTaskQueue,
                                      mCallback);
  if (!mDecoder) {
    mLastError = NS_ERROR_FAILURE;
    return NS_ERROR_FAILURE;
  }
  return NS_OK;
}

nsresult
H264Converter::CreateDecoderAndInit(MediaRawData* aSample)
{
  nsRefPtr<MediaByteBuffer> extra_data =
    mp4_demuxer::AnnexB::ExtractExtraData(aSample);
  if (!mp4_demuxer::AnnexB::HasSPS(extra_data)) {
    return NS_ERROR_NOT_INITIALIZED;
  }
  UpdateConfigFromExtraData(extra_data);

  nsresult rv = CreateDecoder();

  if (NS_SUCCEEDED(rv)) {
    mDecoderInitializing = true;
    nsRefPtr<H264Converter> self = this;

    // The mVideoTaskQueue is flushable which can't be used in MediaPromise. So
    // we get the current AbstractThread instead of it. The MOZ_ASSERT above
    // ensures we are running in AbstractThread so we won't get a nullptr.
    mInitPromiseRequest.Begin(mDecoder->Init()
      ->Then(AbstractThread::GetCurrent(), __func__, this,
             &H264Converter::OnDecoderInitDone,
             &H264Converter::OnDecoderInitFailed));
  }
  return rv;
}

void
H264Converter::OnDecoderInitDone(const TrackType aTrackType)
{
  mInitPromiseRequest.Complete();
  for (uint32_t i = 0 ; i < mMediaRawSamples.Length(); i++) {
    if (NS_FAILED(mDecoder->Input(mMediaRawSamples[i]))) {
      mCallback->Error();
    }
  }
  mMediaRawSamples.Clear();
  mDecoderInitializing = false;
}

void
H264Converter::OnDecoderInitFailed(MediaDataDecoder::DecoderFailureReason aReason)
{
  mInitPromiseRequest.Complete();
  mCallback->Error();
}

nsresult
H264Converter::CheckForSPSChange(MediaRawData* aSample)
{
  nsRefPtr<MediaByteBuffer> extra_data =
    mp4_demuxer::AnnexB::ExtractExtraData(aSample);
  if (!mp4_demuxer::AnnexB::HasSPS(extra_data) ||
      mp4_demuxer::AnnexB::CompareExtraData(extra_data,
                                            mCurrentConfig.mExtraData)) {
        return NS_OK;
      }
  if (!mNeedAVCC) {
    UpdateConfigFromExtraData(extra_data);
    mDecoder->ConfigurationChanged(mCurrentConfig);
    return NS_OK;
  }
  // The SPS has changed, signal to flush the current decoder and create a
  // new one.
  mDecoder->Flush();
  Shutdown();
  return CreateDecoderAndInit(aSample);
}

void
H264Converter::UpdateConfigFromExtraData(MediaByteBuffer* aExtraData)
{
  mp4_demuxer::SPSData spsdata;
  if (mp4_demuxer::H264::DecodeSPSFromExtraData(aExtraData, spsdata) &&
      spsdata.pic_width > 0 && spsdata.pic_height > 0) {
    mp4_demuxer::H264::EnsureSPSIsSane(spsdata);
    mCurrentConfig.mImage.width = spsdata.pic_width;
    mCurrentConfig.mImage.height = spsdata.pic_height;
    mCurrentConfig.mDisplay.width = spsdata.display_width;
    mCurrentConfig.mDisplay.height = spsdata.display_height;
  }
  mCurrentConfig.mExtraData = aExtraData;
}

/* static */
bool
H264Converter::IsH264(const TrackInfo& aConfig)
{
  return aConfig.mMimeType.EqualsLiteral("video/avc") ||
    aConfig.mMimeType.EqualsLiteral("video/mp4");
}

} // namespace mozilla
