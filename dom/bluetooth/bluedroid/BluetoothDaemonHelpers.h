/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_bluetooth_bluedroid_BluetoothDaemonHelpers_h
#define mozilla_dom_bluetooth_bluedroid_BluetoothDaemonHelpers_h

#include "BluetoothCommon.h"
#include "mozilla/ArrayUtils.h"
#include "mozilla/dom/bluetooth/BluetoothTypes.h"
#include "mozilla/ipc/DaemonSocketPDU.h"
#include "mozilla/ipc/DaemonSocketPDUHelpers.h"

BEGIN_BLUETOOTH_NAMESPACE

using mozilla::ipc::DaemonSocketPDU;
using mozilla::ipc::DaemonSocketPDUHeader;
using mozilla::ipc::DaemonSocketPDUHelpers::Convert;
using mozilla::ipc::DaemonSocketPDUHelpers::PackPDU;
using mozilla::ipc::DaemonSocketPDUHelpers::UnpackPDU;

using namespace mozilla::ipc::DaemonSocketPDUHelpers;

//
// Helper structures
//

enum BluetoothAclState {
  ACL_STATE_CONNECTED,
  ACL_STATE_DISCONNECTED
};

struct BluetoothAddress {
  uint8_t mAddr[6];
};

struct BluetoothAvrcpAttributeTextPairs {
  BluetoothAvrcpAttributeTextPairs(const uint8_t* aAttr,
                                   const char** aText,
                                   size_t aLength)
    : mAttr(aAttr)
    , mText(aText)
    , mLength(aLength)
  { }

  const uint8_t* mAttr;
  const char** mText;
  size_t mLength;
};

struct BluetoothAvrcpAttributeValuePairs {
  BluetoothAvrcpAttributeValuePairs(const uint8_t* aAttr,
                                    const uint8_t* aValue,
                                    size_t aLength)
    : mAttr(aAttr)
    , mValue(aValue)
    , mLength(aLength)
  { }

  const uint8_t* mAttr;
  const uint8_t* mValue;
  size_t mLength;
};

struct BluetoothAvrcpEventParamPair {
  BluetoothAvrcpEventParamPair(BluetoothAvrcpEvent aEvent,
                               const BluetoothAvrcpNotificationParam& aParam)
    : mEvent(aEvent)
    , mParam(aParam)
  { }

  BluetoothAvrcpEvent mEvent;
  const BluetoothAvrcpNotificationParam& mParam;
};

struct BluetoothConfigurationParameter {
  uint8_t mType;
  uint16_t mLength;
  nsAutoArrayPtr<uint8_t> mValue;
};

struct BluetoothPinCode {
  uint8_t mPinCode[16];
  uint8_t mLength;
};

struct BluetoothRemoteName {
  uint8_t mName[249];
};

struct BluetoothServiceName {
  uint8_t mName[256];
};

//
// Conversion
//

nsresult
Convert(bool aIn, BluetoothScanMode& aOut);

nsresult
Convert(int32_t aIn, BluetoothTypeOfDevice& aOut);

nsresult
Convert(int32_t aIn, BluetoothScanMode& aOut);

nsresult
Convert(uint8_t aIn, BluetoothA2dpAudioState& aOut);

nsresult
Convert(uint8_t aIn, BluetoothA2dpConnectionState& aOut);

nsresult
Convert(uint8_t aIn, BluetoothAclState& aOut);

nsresult
Convert(uint8_t aIn, BluetoothAvrcpEvent& aOut);

nsresult
Convert(uint8_t aIn, BluetoothAvrcpMediaAttribute& aOut);

nsresult
Convert(uint8_t aIn, BluetoothAvrcpPlayerAttribute& aOut);

nsresult
Convert(uint8_t aIn, BluetoothAvrcpRemoteFeature& aOut);

nsresult
Convert(uint8_t aIn, BluetoothHandsfreeAudioState& aOut);

nsresult
Convert(uint8_t aIn, BluetoothHandsfreeCallHoldType& aOut);

nsresult
Convert(uint8_t aIn, BluetoothHandsfreeConnectionState& aOut);

nsresult
Convert(uint8_t aIn, BluetoothHandsfreeNRECState& aOut);

nsresult
Convert(uint8_t aIn, BluetoothHandsfreeVoiceRecognitionState& aOut);

nsresult
Convert(uint8_t aIn, BluetoothHandsfreeVolumeType& aOut);

nsresult
Convert(uint8_t aIn, BluetoothHandsfreeWbsConfig& aOut);

nsresult
Convert(uint8_t aIn, BluetoothBondState& aOut);

nsresult
Convert(uint8_t aIn, BluetoothTypeOfDevice& aOut);

nsresult
Convert(uint8_t aIn, BluetoothPropertyType& aOut);

nsresult
Convert(uint8_t aIn, BluetoothScanMode& aOut);

nsresult
Convert(uint8_t aIn, BluetoothSspVariant& aOut);

nsresult
Convert(uint8_t aIn, BluetoothStatus& aOut);

nsresult
Convert(int32_t aIn, BluetoothGattStatus& aOut);

nsresult
Convert(const nsAString& aIn, BluetoothAddress& aOut);

nsresult
Convert(const nsAString& aIn, BluetoothPinCode& aOut);

nsresult
Convert(const nsAString& aIn, BluetoothPropertyType& aOut);

nsresult
Convert(const nsAString& aIn, BluetoothServiceName& aOut);

nsresult
Convert(BluetoothAclState aIn, bool& aOut);

nsresult
Convert(const BluetoothAddress& aIn, nsAString& aOut);

nsresult
Convert(BluetoothAvrcpEvent aIn, uint8_t& aOut);

nsresult
Convert(BluetoothAvrcpNotification aIn, uint8_t& aOut);

nsresult
Convert(BluetoothAvrcpPlayerAttribute aIn, uint8_t& aOut);

nsresult
Convert(BluetoothAvrcpRemoteFeature aIn, unsigned long& aOut);

nsresult
Convert(BluetoothAvrcpStatus aIn, uint8_t& aOut);

nsresult
Convert(BluetoothHandsfreeAtResponse aIn, uint8_t& aOut);

nsresult
Convert(BluetoothHandsfreeCallAddressType aIn, uint8_t& aOut);

nsresult
Convert(BluetoothHandsfreeCallDirection aIn, uint8_t& aOut);

nsresult
Convert(BluetoothHandsfreeCallState aIn, uint8_t& aOut);

nsresult
Convert(BluetoothHandsfreeCallMode aIn, uint8_t& aOut);

nsresult
Convert(BluetoothHandsfreeCallMptyType aIn, uint8_t& aOut);

nsresult
Convert(BluetoothHandsfreeNetworkState aIn, uint8_t& aOut);

nsresult
Convert(BluetoothHandsfreeServiceType aIn, uint8_t& aOut);

nsresult
Convert(BluetoothHandsfreeVolumeType aIn, uint8_t& aOut);

nsresult
Convert(BluetoothHandsfreeWbsConfig aIn, uint8_t& aOut);

nsresult
Convert(BluetoothPropertyType aIn, uint8_t& aOut);

nsresult
Convert(const BluetoothRemoteName& aIn, nsAString& aOut);

nsresult
Convert(BluetoothScanMode aIn, uint8_t& aOut);

nsresult
Convert(BluetoothSocketType aIn, uint8_t& aOut);

nsresult
Convert(BluetoothSspVariant aIn, uint8_t& aOut);

nsresult
Convert(ControlPlayStatus aIn, uint8_t& aOut);

nsresult
Convert(BluetoothGattAuthReq aIn, int32_t& aOut);

nsresult
Convert(BluetoothGattWriteType aIn, int32_t& aOut);

nsresult
Convert(nsresult aIn, BluetoothStatus& aOut);

//
// Packing
//

nsresult
PackPDU(bool aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(const BluetoothAddress& aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(const BluetoothAvrcpAttributeTextPairs& aIn,
        DaemonSocketPDU& aPDU);

nsresult
PackPDU(const BluetoothAvrcpAttributeValuePairs& aIn,
        DaemonSocketPDU& aPDU);

nsresult
PackPDU(const BluetoothAvrcpElementAttribute& aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(BluetoothAvrcpEvent aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(const BluetoothAvrcpEventParamPair& aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(BluetoothAvrcpNotification aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(BluetoothAvrcpPlayerAttribute aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(BluetoothAvrcpStatus aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(const BluetoothConfigurationParameter& aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(const BluetoothHandsfreeAtResponse& aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(const BluetoothHandsfreeCallAddressType& aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(const BluetoothHandsfreeCallDirection& aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(const BluetoothHandsfreeCallMode& aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(const BluetoothHandsfreeCallMptyType& aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(const BluetoothHandsfreeCallState& aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(const BluetoothHandsfreeNetworkState& aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(const BluetoothHandsfreeServiceType& aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(const BluetoothHandsfreeVolumeType& aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(const BluetoothHandsfreeWbsConfig& aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(const BluetoothNamedValue& aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(const BluetoothPinCode& aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(BluetoothPropertyType aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(const BluetoothServiceName& aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(BluetoothSocketType aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(BluetoothSspVariant aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(BluetoothScanMode aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(ControlPlayStatus aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(const BluetoothUuid& aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(const BluetoothGattId& aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(const BluetoothGattServiceId& aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(BluetoothGattAuthReq aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(BluetoothGattWriteType aIn, DaemonSocketPDU& aPDU);

nsresult
PackPDU(BluetoothTransport aIn, DaemonSocketPDU& aPDU);

/* |PackConversion| is a helper for packing converted values. Pass
 * an instance of this structure to |PackPDU| to convert a value from
 * the input type to the output type and and write it to the PDU.
 */
template<typename Tin, typename Tout>
struct PackConversion {
  PackConversion(const Tin& aIn)
  : mIn(aIn)
  { }

  const Tin& mIn;
};

template<typename Tin, typename Tout>
inline nsresult
PackPDU(const PackConversion<Tin, Tout>& aIn, DaemonSocketPDU& aPDU)
{
  Tout out;

  nsresult rv = Convert(aIn.mIn, out);
  if (NS_FAILED(rv)) {
    return rv;
  }
  return PackPDU(out, aPDU);
}

/* |PackArray| is a helper for packing arrays. Pass an instance
 * of this structure as the first argument to |PackPDU| to pack
 * an array. The array's maximum default length is 255 elements.
 */
template <typename T>
struct PackArray
{
  PackArray(const T* aData, size_t aLength)
  : mData(aData)
  , mLength(aLength)
  { }

  const T* mData;
  size_t mLength;
};

/* This implementation of |PackPDU| packs the length of an array
 * and the elements of the array one-by-one.
 */
template<typename T>
inline nsresult
PackPDU(const PackArray<T>& aIn, DaemonSocketPDU& aPDU)
{
  for (size_t i = 0; i < aIn.mLength; ++i) {
    nsresult rv = PackPDU(aIn.mData[i], aPDU);
    if (NS_FAILED(rv)) {
      return rv;
    }
  }
  return NS_OK;
}

template<>
inline nsresult
PackPDU<uint8_t>(const PackArray<uint8_t>& aIn, DaemonSocketPDU& aPDU)
{
  /* Write raw bytes in one pass */
  return aPDU.Write(aIn.mData, aIn.mLength);
}

template<>
inline nsresult
PackPDU<char>(const PackArray<char>& aIn, DaemonSocketPDU& aPDU)
{
  /* Write raw bytes in one pass */
  return aPDU.Write(aIn.mData, aIn.mLength);
}

/* |PackCString0| is a helper for packing 0-terminated C string,
 * including the \0 character. Pass an instance of this structure
 * as the first argument to |PackPDU| to pack a string.
 */
struct PackCString0
{
  PackCString0(const nsCString& aString)
  : mString(aString)
  { }

  const nsCString& mString;
};

/* This implementation of |PackPDU| packs a 0-terminated C string.
 */
inline nsresult
PackPDU(const PackCString0& aIn, DaemonSocketPDU& aPDU)
{
  return PackPDU(
    PackArray<uint8_t>(reinterpret_cast<const uint8_t*>(aIn.mString.get()),
                       aIn.mString.Length() + 1), aPDU);
}

/* |PackReversed| is a helper for packing data in reversed order. Pass an
 * instance of this structure as the first argument to |PackPDU| to pack data
 * in reversed order.
 */
template<typename T>
struct PackReversed
{
  PackReversed(const T& aValue)
    : mValue(aValue)
  { }

  const T& mValue;
};

/* No general rules to pack data in reversed order. Signal a link error if the
 * type |T| of |PackReversed| is not defined explicitly.
 */
template<typename T>
nsresult
PackPDU(const PackReversed<T>& aIn, DaemonSocketPDU& aPDU);

/* This implementation of |PackPDU| packs elements in |PackArray| in reversed
 * order. (ex. reversed GATT UUID, see bug 1171866)
 */
template<typename U>
inline nsresult
PackPDU(const PackReversed<PackArray<U>>& aIn, DaemonSocketPDU& aPDU)
{
  for (size_t i = 0; i < aIn.mValue.mLength; ++i) {
    nsresult rv = PackPDU(aIn.mValue.mData[aIn.mValue.mLength - i - 1], aPDU);
    if (NS_FAILED(rv)) {
      return rv;
    }
  }
  return NS_OK;
}

/* This implementation of |PackPDU| packs |BluetoothUuid| in reversed order.
 * (ex. reversed GATT UUID, see bug 1171866)
 */
template <>
inline nsresult
PackPDU<BluetoothUuid>(const PackReversed<BluetoothUuid>& aIn,
                       DaemonSocketPDU& aPDU)
{
 return PackPDU(
   PackReversed<PackArray<uint8_t>>(
     PackArray<uint8_t>(aIn.mValue.mUuid, sizeof(aIn.mValue.mUuid))),
   aPDU);
}

template <typename T1, typename T2>
inline nsresult
PackPDU(const T1& aIn1, const T2& aIn2, DaemonSocketPDU& aPDU)
{
  nsresult rv = PackPDU(aIn1, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  return PackPDU(aIn2, aPDU);
}

template <typename T1, typename T2, typename T3>
inline nsresult
PackPDU(const T1& aIn1, const T2& aIn2, const T3& aIn3,
        DaemonSocketPDU& aPDU)
{
  nsresult rv = PackPDU(aIn1, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn2, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  return PackPDU(aIn3, aPDU);
}

template <typename T1, typename T2, typename T3, typename T4>
inline nsresult
PackPDU(const T1& aIn1, const T2& aIn2, const T3& aIn3, const T4& aIn4,
        DaemonSocketPDU& aPDU)
{
  nsresult rv = PackPDU(aIn1, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn2, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn3, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  return PackPDU(aIn4, aPDU);
}

template <typename T1, typename T2, typename T3,
          typename T4, typename T5>
inline nsresult
PackPDU(const T1& aIn1, const T2& aIn2, const T3& aIn3,
        const T4& aIn4, const T5& aIn5,
        DaemonSocketPDU& aPDU)
{
  nsresult rv = PackPDU(aIn1, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn2, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn3, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn4, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  return PackPDU(aIn5, aPDU);
}

template <typename T1, typename T2, typename T3,
          typename T4, typename T5, typename T6>
inline nsresult
PackPDU(const T1& aIn1, const T2& aIn2, const T3& aIn3,
        const T4& aIn4, const T5& aIn5, const T6& aIn6,
        DaemonSocketPDU& aPDU)
{
  nsresult rv = PackPDU(aIn1, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn2, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn3, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn4, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn5, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  return PackPDU(aIn6, aPDU);
}

template <typename T1, typename T2, typename T3,
          typename T4, typename T5, typename T6,
          typename T7>
inline nsresult
PackPDU(const T1& aIn1, const T2& aIn2, const T3& aIn3,
        const T4& aIn4, const T5& aIn5, const T6& aIn6,
        const T7& aIn7, DaemonSocketPDU& aPDU)
{
  nsresult rv = PackPDU(aIn1, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn2, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn3, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn4, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn5, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn6, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  return PackPDU(aIn7, aPDU);
}

template <typename T1, typename T2, typename T3,
          typename T4, typename T5, typename T6,
          typename T7, typename T8>
inline nsresult
PackPDU(const T1& aIn1, const T2& aIn2, const T3& aIn3,
        const T4& aIn4, const T5& aIn5, const T6& aIn6,
        const T7& aIn7, const T8& aIn8, DaemonSocketPDU& aPDU)
{
  nsresult rv = PackPDU(aIn1, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn2, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn3, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn4, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn5, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn6, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn7, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  return PackPDU(aIn8, aPDU);
}

template <typename T1, typename T2, typename T3,
          typename T4, typename T5, typename T6,
          typename T7, typename T8, typename T9,
          typename T10, typename T11, typename T12,
          typename T13>
inline nsresult
PackPDU(const T1& aIn1, const T2& aIn2, const T3& aIn3,
        const T4& aIn4, const T5& aIn5, const T6& aIn6,
        const T7& aIn7, const T8& aIn8, const T9& aIn9,
        const T10& aIn10, const T11& aIn11, const T12& aIn12,
        const T13& aIn13, DaemonSocketPDU& aPDU)
{
  nsresult rv = PackPDU(aIn1, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn2, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn3, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn4, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn5, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn6, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn7, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn8, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn9, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn10, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn11, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  rv = PackPDU(aIn12, aPDU);
  if (NS_FAILED(rv)) {
    return rv;
  }
  return PackPDU(aIn13, aPDU);
}

//
// Unpacking
//

nsresult
UnpackPDU(DaemonSocketPDU& aPDU, bool& aOut);

nsresult
UnpackPDU(DaemonSocketPDU& aPDU, char& aOut);

nsresult
UnpackPDU(DaemonSocketPDU& aPDU, BluetoothA2dpAudioState& aOut);

nsresult
UnpackPDU(DaemonSocketPDU& aPDU, BluetoothA2dpConnectionState& aOut);

nsresult
UnpackPDU(DaemonSocketPDU& aPDU, BluetoothAclState& aOut);

inline nsresult
UnpackPDU(DaemonSocketPDU& aPDU, BluetoothAddress& aOut)
{
  return aPDU.Read(aOut.mAddr, sizeof(aOut.mAddr));
}

nsresult
UnpackPDU(DaemonSocketPDU& aPDU, BluetoothAvrcpEvent& aOut);

nsresult
UnpackPDU(DaemonSocketPDU& aPDU, BluetoothAvrcpMediaAttribute& aOut);

nsresult
UnpackPDU(DaemonSocketPDU& aPDU, BluetoothAvrcpPlayerAttribute& aOut);

nsresult
UnpackPDU(DaemonSocketPDU& aPDU, BluetoothAvrcpPlayerSettings& aOut);

nsresult
UnpackPDU(DaemonSocketPDU& aPDU, BluetoothAvrcpRemoteFeature& aOut);

nsresult
UnpackPDU(DaemonSocketPDU& aPDU, BluetoothBondState& aOut);

nsresult
UnpackPDU(DaemonSocketPDU& aPDU, BluetoothTypeOfDevice& aOut);

nsresult
UnpackPDU(DaemonSocketPDU& aPDU, BluetoothHandsfreeAudioState& aOut);

nsresult
UnpackPDU(DaemonSocketPDU& aPDU, BluetoothHandsfreeCallHoldType& aOut);

nsresult
UnpackPDU(DaemonSocketPDU& aPDU, BluetoothHandsfreeConnectionState& aOut);

nsresult
UnpackPDU(DaemonSocketPDU& aPDU, BluetoothHandsfreeNRECState& aOut);

nsresult
UnpackPDU(DaemonSocketPDU& aPDU,
          BluetoothHandsfreeVoiceRecognitionState& aOut);

nsresult
UnpackPDU(DaemonSocketPDU& aPDU, BluetoothHandsfreeVolumeType& aOut);

nsresult
UnpackPDU(DaemonSocketPDU& aPDU, BluetoothRemoteInfo& aOut);

inline nsresult
UnpackPDU(DaemonSocketPDU& aPDU, BluetoothRemoteName& aOut)
{
  return aPDU.Read(aOut.mName, sizeof(aOut.mName));
}

nsresult
UnpackPDU(DaemonSocketPDU& aPDU, BluetoothProperty& aOut);

nsresult
UnpackPDU(DaemonSocketPDU& aPDU, BluetoothPropertyType& aOut);

nsresult
UnpackPDU(DaemonSocketPDU& aPDU, BluetoothScanMode& aOut);

nsresult
UnpackPDU(DaemonSocketPDU& aPDU, BluetoothServiceRecord& aOut);

nsresult
UnpackPDU(DaemonSocketPDU& aPDU, BluetoothSspVariant& aOut);

nsresult
UnpackPDU(DaemonSocketPDU& aPDU, BluetoothStatus& aOut);

nsresult
UnpackPDU(DaemonSocketPDU& aPDU, BluetoothGattStatus& aOut);

inline nsresult
UnpackPDU(DaemonSocketPDU& aPDU, BluetoothUuid& aOut)
{
  return aPDU.Read(aOut.mUuid, sizeof(aOut.mUuid));
}

nsresult
UnpackPDU(DaemonSocketPDU& aPDU, BluetoothGattId& aOut);

nsresult
UnpackPDU(DaemonSocketPDU& aPDU, BluetoothGattServiceId& aOut);

nsresult
UnpackPDU(DaemonSocketPDU& aPDU, BluetoothGattReadParam& aOut);

nsresult
UnpackPDU(DaemonSocketPDU& aPDU, BluetoothGattWriteParam& aOut);

nsresult
UnpackPDU(DaemonSocketPDU& aPDU, BluetoothGattNotifyParam& aOut);

/* |UnpackConversion| is a helper for convering unpacked values. Pass
 * an instance of this structure to |UnpackPDU| to read a value from
 * the PDU in the input type and convert it to the output type.
 */
template<typename Tin, typename Tout>
struct UnpackConversion {
  UnpackConversion(Tout& aOut)
  : mOut(aOut)
  { }

  Tout& mOut;
};

template<typename Tin, typename Tout>
inline nsresult
UnpackPDU(DaemonSocketPDU& aPDU, const UnpackConversion<Tin, Tout>& aOut)
{
  Tin in;
  nsresult rv = UnpackPDU(aPDU, in);
  if (NS_FAILED(rv)) {
    return rv;
  }
  return Convert(in, aOut.mOut);
}

/* |UnpackArray| is a helper for unpacking arrays. Pass an instance
 * of this structure as the second argument to |UnpackPDU| to unpack
 * an array.
 */
template <typename T>
struct UnpackArray
{
  UnpackArray(T* aData, size_t aLength)
  : mData(aData)
  , mLength(aLength)
  { }

  UnpackArray(nsAutoArrayPtr<T>& aData, size_t aLength)
  : mData(nullptr)
  , mLength(aLength)
  {
    aData = new T[mLength];
    mData = aData.get();
  }

  UnpackArray(nsAutoArrayPtr<T>& aData, size_t aSize, size_t aElemSize)
  : mData(nullptr)
  , mLength(aSize / aElemSize)
  {
    aData = new T[mLength];
    mData = aData.get();
  }

  T* mData;
  size_t mLength;
};

template<typename T>
inline nsresult
UnpackPDU(DaemonSocketPDU& aPDU, const UnpackArray<T>& aOut)
{
  for (size_t i = 0; i < aOut.mLength; ++i) {
    nsresult rv = UnpackPDU(aPDU, aOut.mData[i]);
    if (NS_FAILED(rv)) {
      return rv;
    }
  }
  return NS_OK;
}

template<typename T>
inline nsresult
UnpackPDU(DaemonSocketPDU& aPDU, UnpackArray<T>& aOut)
{
  for (size_t i = 0; i < aOut.mLength; ++i) {
    nsresult rv = UnpackPDU(aPDU, aOut.mData[i]);
    if (NS_FAILED(rv)) {
      return rv;
    }
  }
  return NS_OK;
}

template<>
inline nsresult
UnpackPDU<uint8_t>(DaemonSocketPDU& aPDU, const UnpackArray<uint8_t>& aOut)
{
  /* Read raw bytes in one pass */
  return aPDU.Read(aOut.mData, aOut.mLength);
}

template<typename T>
inline nsresult
UnpackPDU(DaemonSocketPDU& aPDU, nsTArray<T>& aOut)
{
  for (typename nsTArray<T>::size_type i = 0; i < aOut.Length(); ++i) {
    nsresult rv = UnpackPDU(aPDU, aOut[i]);
    if (NS_FAILED(rv)) {
      return rv;
    }
  }
  return NS_OK;
}

/* |UnpackReversed| is a helper for unpacking data in reversed order. Pass an
 * instance of this structure as the second argument to |UnpackPDU| to unpack
 * data in reversed order.
 */
template<typename T>
struct UnpackReversed
{
  UnpackReversed(T& aValue)
    : mValue(&aValue)
  { }

  UnpackReversed(T&& aValue)
    : mValue(&aValue)
  { }

  T* mValue;
};

/* No general rules to unpack data in reversed order. Signal a link error if
 * the type |T| of |UnpackReversed| is not defined explicitly.
 */
template<typename T>
nsresult
UnpackPDU(DaemonSocketPDU& aPDU, const UnpackReversed<T>& aOut);

template<typename U>
inline nsresult
UnpackPDU(DaemonSocketPDU& aPDU, const UnpackReversed<UnpackArray<U>>& aOut)
{
  for (size_t i = 0; i < aOut.mValue->mLength; ++i) {
    nsresult rv = UnpackPDU(aPDU,
                            aOut.mValue->mData[aOut.mValue->mLength - i - 1]);
    if (NS_FAILED(rv)) {
      return rv;
    }
  }
  return NS_OK;
}

/* This implementation of |UnpackPDU| unpacks |BluetoothUuid| in reversed
 * order. (ex. reversed GATT UUID, see bug 1171866)
 */
template<>
inline nsresult
UnpackPDU<BluetoothUuid>(DaemonSocketPDU& aPDU,
                         const UnpackReversed<BluetoothUuid>& aOut)
{
  return UnpackPDU(
    aPDU,
    UnpackReversed<UnpackArray<uint8_t>>(
      UnpackArray<uint8_t>(aOut.mValue->mUuid, sizeof(aOut.mValue->mUuid))));
}

//
// Init operators
//

// |UnpackPDUInitOp| is a general-purpose init operator for all variants
// of |BluetoothResultRunnable| and |BluetoothNotificationRunnable|. The
// call operators of |UnpackPDUInitOp| unpack a PDU into the supplied
// arguments.
class UnpackPDUInitOp final : private PDUInitOp
{
public:
  UnpackPDUInitOp(DaemonSocketPDU& aPDU)
  : PDUInitOp(aPDU)
  { }

  nsresult operator () () const
  {
    WarnAboutTrailingData();
    return NS_OK;
  }

  template<typename T1>
  nsresult operator () (T1& aArg1) const
  {
    nsresult rv = UnpackPDU(GetPDU(), aArg1);
    if (NS_FAILED(rv)) {
      return rv;
    }
    WarnAboutTrailingData();
    return NS_OK;
  }

  template<typename T1, typename T2>
  nsresult operator () (T1& aArg1, T2& aArg2) const
  {
    DaemonSocketPDU& pdu = GetPDU();

    nsresult rv = UnpackPDU(pdu, aArg1);
    if (NS_FAILED(rv)) {
      return rv;
    }
    rv = UnpackPDU(pdu, aArg2);
    if (NS_FAILED(rv)) {
      return rv;
    }
    WarnAboutTrailingData();
    return NS_OK;
  }

  template<typename T1, typename T2, typename T3>
  nsresult operator () (T1& aArg1, T2& aArg2, T3& aArg3) const
  {
    DaemonSocketPDU& pdu = GetPDU();

    nsresult rv = UnpackPDU(pdu, aArg1);
    if (NS_FAILED(rv)) {
      return rv;
    }
    rv = UnpackPDU(pdu, aArg2);
    if (NS_FAILED(rv)) {
      return rv;
    }
    rv = UnpackPDU(pdu, aArg3);
    if (NS_FAILED(rv)) {
      return rv;
    }
    WarnAboutTrailingData();
    return NS_OK;
  }

  template<typename T1, typename T2, typename T3, typename T4>
  nsresult operator () (T1& aArg1, T2& aArg2, T3& aArg3, T4& aArg4) const
  {
    DaemonSocketPDU& pdu = GetPDU();

    nsresult rv = UnpackPDU(pdu, aArg1);
    if (NS_FAILED(rv)) {
      return rv;
    }
    rv = UnpackPDU(pdu, aArg2);
    if (NS_FAILED(rv)) {
      return rv;
    }
    rv = UnpackPDU(pdu, aArg3);
    if (NS_FAILED(rv)) {
      return rv;
    }
    rv = UnpackPDU(pdu, aArg4);
    if (NS_FAILED(rv)) {
      return rv;
    }
    WarnAboutTrailingData();
    return NS_OK;
  }

  template<typename T1, typename T2, typename T3, typename T4, typename T5>
  nsresult operator () (T1& aArg1, T2& aArg2, T3& aArg3, T4& aArg4,
                        T5& aArg5) const
  {
    DaemonSocketPDU& pdu = GetPDU();

    nsresult rv = UnpackPDU(pdu, aArg1);
    if (NS_FAILED(rv)) {
      return rv;
    }
    rv = UnpackPDU(pdu, aArg2);
    if (NS_FAILED(rv)) {
      return rv;
    }
    rv = UnpackPDU(pdu, aArg3);
    if (NS_FAILED(rv)) {
      return rv;
    }
    rv = UnpackPDU(pdu, aArg4);
    if (NS_FAILED(rv)) {
      return rv;
    }
    rv = UnpackPDU(pdu, aArg5);
    if (NS_FAILED(rv)) {
      return rv;
    }
    WarnAboutTrailingData();
    return NS_OK;
  }
};

END_BLUETOOTH_NAMESPACE

#endif // mozilla_dom_bluetooth_bluedroid_BluetoothDaemonHelpers_h
