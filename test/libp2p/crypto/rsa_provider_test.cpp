/**
 * Copyright Quadrivium LLC
 * All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 */

#include "libp2p/crypto/rsa_provider/rsa_provider_impl.hpp"

#include <gtest/gtest.h>
#include "testutil/outcome.hpp"

using libp2p::crypto::rsa::PrivateKey;
using libp2p::crypto::rsa::PublicKey;
using libp2p::crypto::rsa::RsaProviderImpl;
using libp2p::crypto::rsa::Signature;

#define SAMPLE_RSA_PRIVATE_KEY_BYTES                                          \
  0x30, 0x82, 0x04, 0xa3, 0x02, 0x01, 0x00, 0x02, 0x82, 0x01, 0x01, 0x00,     \
      0xa7, 0x20, 0xe9, 0xcb, 0xb9, 0x67, 0x23, 0x5a, 0x9e, 0x65, 0xb8, 0x43, \
      0xd3, 0xf6, 0x4e, 0x58, 0x69, 0x5f, 0x01, 0x88, 0xcc, 0xde, 0x7d, 0x73, \
      0x44, 0xbb, 0x2e, 0xc8, 0x12, 0xc7, 0x4f, 0xf2, 0xb4, 0x51, 0x9a, 0x2b, \
      0x21, 0xad, 0xce, 0xcc, 0x46, 0xe4, 0x59, 0xe4, 0xa6, 0xac, 0xd6, 0x35, \
      0xd1, 0x7f, 0xaa, 0x9f, 0x4a, 0x78, 0x76, 0x15, 0x2a, 0x13, 0x9e, 0x58, \
      0x44, 0x0c, 0x94, 0xda, 0x13, 0x16, 0x9a, 0xa1, 0x61, 0x57, 0x4c, 0x62, \
      0x9a, 0x6e, 0xb5, 0x02, 0x94, 0x3e, 0x8d, 0x20, 0x6e, 0x40, 0x95, 0x90, \
      0x2a, 0x5e, 0x1f, 0x64, 0x17, 0x32, 0xce, 0xda, 0xc8, 0x4a, 0x45, 0x86, \
      0xd3, 0x4d, 0xd4, 0xb3, 0x6d, 0x0d, 0xf6, 0x8a, 0xbd, 0xb3, 0xa8, 0x23, \
      0x21, 0x55, 0x21, 0x72, 0xec, 0xbd, 0x08, 0x74, 0xb9, 0x26, 0xab, 0xab, \
      0xe2, 0xd5, 0x1a, 0x30, 0xa8, 0x3b, 0x13, 0x4f, 0xc1, 0x96, 0x43, 0x3f, \
      0x04, 0x18, 0xfd, 0x69, 0x51, 0x86, 0x84, 0xa0, 0x48, 0x29, 0x3e, 0x21, \
      0x24, 0x9d, 0xb7, 0x32, 0x74, 0x36, 0x03, 0x5f, 0x36, 0x15, 0xe6, 0xf8, \
      0x49, 0xdb, 0x12, 0x0b, 0x21, 0x1a, 0x04, 0xf1, 0xd5, 0xaa, 0xc3, 0xc7, \
      0xa9, 0xa4, 0xf2, 0x56, 0xa4, 0x80, 0xc3, 0x94, 0x85, 0x84, 0x40, 0xb1, \
      0xd1, 0xea, 0xfa, 0x64, 0x63, 0xe3, 0xd7, 0x3c, 0x1a, 0x2a, 0xe3, 0x23, \
      0x6a, 0x34, 0x1f, 0x57, 0x58, 0x4b, 0xd6, 0x9b, 0x97, 0x97, 0x63, 0xaa, \
      0x38, 0xc0, 0xdd, 0x59, 0xfc, 0x86, 0xdd, 0x48, 0x0b, 0x39, 0x85, 0xb5, \
      0x94, 0xf1, 0xff, 0xea, 0xb3, 0x18, 0x06, 0xe5, 0xd4, 0x44, 0xe9, 0xad, \
      0x51, 0x7d, 0x1d, 0x8f, 0x26, 0x7a, 0x50, 0xce, 0x70, 0xd2, 0x90, 0xae, \
      0xcf, 0xe9, 0x6a, 0xf8, 0xef, 0x33, 0x2f, 0x2d, 0x9a, 0x7c, 0x3c, 0x97, \
      0x59, 0x51, 0x9c, 0x27, 0x02, 0x03, 0x01, 0x00, 0x01, 0x02, 0x82, 0x01, \
      0x00, 0x09, 0xc9, 0x95, 0xa7, 0x86, 0x9e, 0x11, 0xa5, 0xf6, 0xec, 0xc2, \
      0x13, 0xc6, 0xcf, 0xff, 0xda, 0x91, 0xa1, 0x2c, 0xb7, 0xd4, 0x37, 0x9b, \
      0x65, 0x2a, 0xbb, 0x43, 0x8c, 0x17, 0xfd, 0x70, 0x23, 0x28, 0x88, 0x36, \
      0xdd, 0x1b, 0xc5, 0xcf, 0xfc, 0x8a, 0x92, 0x48, 0xe9, 0x69, 0x11, 0x6a, \
      0x22, 0x10, 0xb8, 0xc7, 0x47, 0x38, 0xd7, 0x41, 0x23, 0x10, 0x29, 0xc1, \
      0xf6, 0x2d, 0x56, 0x07, 0x49, 0x96, 0x20, 0x96, 0xc1, 0xa8, 0xaf, 0xb2, \
      0x1b, 0x7c, 0x2b, 0xcb, 0x91, 0xf1, 0x18, 0x90, 0xaf, 0x3b, 0x85, 0xb0, \
      0xbc, 0x96, 0x9e, 0x98, 0x7c, 0x06, 0x17, 0xc5, 0xa1, 0x6e, 0x65, 0x5b, \
      0x19, 0x00, 0x3a, 0xe8, 0x9f, 0xf1, 0x5d, 0x05, 0xf9, 0x2e, 0x0e, 0xdd, \
      0x63, 0xe1, 0x49, 0xd9, 0x01, 0x9f, 0xf7, 0x9a, 0x0d, 0x2b, 0xe2, 0xa5, \
      0xdf, 0x15, 0x13, 0xa1, 0xcb, 0x03, 0xaf, 0x21, 0xfc, 0x4b, 0x79, 0xbf, \
      0xf5, 0x7e, 0x48, 0x31, 0x74, 0x83, 0x79, 0xb4, 0x71, 0x4e, 0x99, 0xa5, \
      0x61, 0xdb, 0x88, 0x32, 0xd3, 0xda, 0x16, 0x11, 0x8d, 0xf5, 0x01, 0xf6, \
      0xd1, 0x5a, 0x7f, 0xf1, 0xb0, 0x38, 0xbe, 0xf2, 0x8f, 0x63, 0xe2, 0xe8, \
      0xec, 0x7f, 0x79, 0x3e, 0x94, 0xc3, 0x93, 0x79, 0x5a, 0x76, 0xc6, 0x40, \
      0x03, 0xbe, 0x03, 0x7e, 0x0f, 0x98, 0xef, 0xaa, 0x22, 0x2a, 0x80, 0xee, \
      0xd1, 0x36, 0xdd, 0x94, 0xb7, 0xbf, 0xa8, 0xaf, 0x54, 0x5e, 0xca, 0x0a, \
      0x0a, 0x00, 0x53, 0x15, 0x74, 0x66, 0x8f, 0x57, 0xda, 0xae, 0x4c, 0x83, \
      0x1f, 0x5e, 0x4c, 0xca, 0x42, 0x9f, 0x13, 0x42, 0x86, 0x66, 0x53, 0xc9, \
      0x33, 0x1d, 0x70, 0xd6, 0xdd, 0x6a, 0x56, 0x31, 0xbf, 0x12, 0xa9, 0xf0, \
      0x68, 0x79, 0x3e, 0x5c, 0xa6, 0xe8, 0xd3, 0xa9, 0x6a, 0x8d, 0x71, 0x11, \
      0xe9, 0x4b, 0xbe, 0xf4, 0x61, 0x02, 0x81, 0x81, 0x00, 0xc4, 0x86, 0x60, \
      0xd4, 0x71, 0x1c, 0xe5, 0x72, 0x23, 0x60, 0xea, 0x54, 0x14, 0x3e, 0x06, \
      0x60, 0x1b, 0xe0, 0xcd, 0x75, 0xd9, 0xaa, 0xfb, 0xb3, 0x41, 0x1d, 0xcf, \
      0x78, 0xe0, 0xed, 0x2a, 0x71, 0xc7, 0xc2, 0x0a, 0x8d, 0xf8, 0x91, 0x75, \
      0x8c, 0x5d, 0xc6, 0x90, 0x56, 0xa7, 0xaf, 0xa5, 0x39, 0x08, 0x02, 0x21, \
      0x61, 0x39, 0x79, 0x7e, 0xa7, 0xc3, 0xd0, 0x1e, 0x70, 0x64, 0xb0, 0xd1, \
      0x85, 0x23, 0xa3, 0x7f, 0x84, 0x1e, 0x0f, 0x65, 0xac, 0xe6, 0xe8, 0x21, \
      0xc1, 0xee, 0x85, 0x7f, 0x76, 0xfb, 0x96, 0x4c, 0xb5, 0xc4, 0x8a, 0x4e, \
      0xa8, 0x84, 0xa1, 0xd5, 0x34, 0x20, 0x50, 0x5a, 0xd8, 0xb2, 0xf6, 0x42, \
      0xec, 0xdb, 0x5f, 0xed, 0x17, 0x5a, 0x65, 0x2f, 0x6e, 0x27, 0x10, 0x62, \
      0x78, 0x82, 0xe9, 0x7a, 0x5f, 0xe0, 0xf5, 0x20, 0x77, 0x40, 0xeb, 0x2d, \
      0x41, 0xca, 0x91, 0x0c, 0x19, 0x02, 0x81, 0x81, 0x00, 0xd9, 0xb5, 0x13, \
      0x28, 0xa8, 0x37, 0x79, 0xb3, 0xe2, 0x17, 0xbb, 0xfb, 0x28, 0x61, 0xf2, \
      0x8d, 0x14, 0x57, 0x1a, 0x4a, 0xc0, 0x08, 0xda, 0x50, 0x23, 0x66, 0x07, \
      0x00, 0x52, 0x90, 0x82, 0x9a, 0x1e, 0xd5, 0xd2, 0xbf, 0x36, 0xfe, 0x09, \
      0x91, 0x94, 0xdb, 0x71, 0xbd, 0xad, 0x06, 0xaf, 0x3c, 0xaa, 0x6a, 0xdb, \
      0x8b, 0xf5, 0x35, 0x94, 0x9c, 0x46, 0x7a, 0xc5, 0x58, 0xce, 0x7b, 0xb7, \
      0x07, 0x26, 0xc3, 0x14, 0x10, 0x4d, 0x04, 0x58, 0x95, 0x2b, 0x41, 0x25, \
      0x16, 0xf4, 0xea, 0x45, 0xc6, 0x74, 0x59, 0xa8, 0x95, 0x11, 0x3e, 0x65, \
      0x15, 0x3d, 0x64, 0x8a, 0x44, 0xd3, 0x38, 0xb0, 0xe0, 0x62, 0x20, 0xec, \
      0xfe, 0xb7, 0xc0, 0x52, 0x11, 0x87, 0xce, 0x6b, 0xdd, 0x20, 0x33, 0xe6, \
      0xd0, 0x67, 0x7d, 0xbe, 0xe3, 0x21, 0x03, 0x9c, 0x5f, 0x2e, 0x3c, 0x64, \
      0x6b, 0xb9, 0xb0, 0xf2, 0x3f, 0x02, 0x81, 0x80, 0x56, 0xed, 0xcd, 0xd5, \
      0x0a, 0xb7, 0xaa, 0x72, 0x63, 0x5d, 0xd2, 0x29, 0x20, 0xcf, 0x12, 0xc8, \
      0xde, 0x6c, 0x00, 0xd5, 0xf9, 0x53, 0x5d, 0x84, 0xbb, 0xef, 0xf0, 0xbe, \
      0xda, 0x57, 0x6d, 0xe7, 0xe8, 0x61, 0x57, 0x83, 0xd7, 0x08, 0x98, 0x83, \
      0x5f, 0x53, 0x9e, 0xb3, 0x05, 0xf7, 0x76, 0xd1, 0x14, 0x4e, 0xc4, 0xef, \
      0x19, 0x9a, 0x39, 0x41, 0x02, 0x27, 0x83, 0xdf, 0x59, 0x5a, 0xb1, 0x43, \
      0x00, 0x31, 0xa9, 0x0d, 0x0a, 0xb2, 0xea, 0xd9, 0xa1, 0x45, 0xda, 0xf0, \
      0xca, 0x8d, 0xf8, 0x84, 0xeb, 0xe2, 0x5e, 0xca, 0x24, 0x46, 0xbd, 0x66, \
      0x63, 0x4c, 0xa6, 0x5f, 0x29, 0x50, 0x01, 0xef, 0x4a, 0xb2, 0x0d, 0x86, \
      0x71, 0xbf, 0x2b, 0xc3, 0xa1, 0xb3, 0x92, 0x58, 0x5a, 0x25, 0x30, 0x39, \
      0x78, 0x46, 0xc8, 0x5f, 0xb2, 0xc5, 0x1a, 0xf8, 0x45, 0x1a, 0x01, 0xe9, \
      0xd1, 0x2d, 0x79, 0xb9, 0x02, 0x81, 0x80, 0x4c, 0x36, 0x05, 0x34, 0xf0, \
      0x5e, 0x2b, 0x2e, 0x7b, 0x71, 0x4f, 0x8f, 0x06, 0x7d, 0xf3, 0x48, 0xcb, \
      0xdc, 0x0a, 0x0a, 0x2c, 0xdb, 0x6c, 0x81, 0x2d, 0x43, 0x24, 0x24, 0x4a, \
      0xc2, 0xe7, 0xd5, 0x48, 0x02, 0x16, 0x3a, 0x75, 0xe7, 0xcc, 0x35, 0xb8, \
      0x64, 0x71, 0x96, 0xb2, 0xa1, 0x6a, 0x6a, 0xd4, 0x45, 0x80, 0x01, 0xa8, \
      0x9f, 0x4f, 0x1c, 0xbd, 0xd7, 0x70, 0x8a, 0x3c, 0x0e, 0x36, 0xee, 0x3b, \
      0xf5, 0x82, 0x6a, 0x8a, 0x6b, 0x81, 0x0f, 0x07, 0x0b, 0x51, 0x0e, 0x8e, \
      0x42, 0x0c, 0x97, 0x02, 0x57, 0xf8, 0x99, 0xe6, 0x65, 0x6f, 0x41, 0x56, \
      0x32, 0x81, 0xec, 0xc1, 0xfc, 0x50, 0x7e, 0xdc, 0x1b, 0x26, 0x9b, 0xac, \
      0x7b, 0x98, 0xbf, 0x26, 0x49, 0x6e, 0x9d, 0x04, 0x0e, 0xe7, 0x38, 0x01, \
      0x62, 0x95, 0x96, 0x31, 0x12, 0x70, 0x85, 0x29, 0x4c, 0x5e, 0xe3, 0x0f, \
      0x28, 0x10, 0x45, 0x02, 0x81, 0x81, 0x00, 0x8d, 0xff, 0xaa, 0xda, 0x02, \
      0xdb, 0xb7, 0xba, 0xd1, 0x35, 0x98, 0x09, 0x23, 0x94, 0x0f, 0xf5, 0x18, \
      0xdc, 0x90, 0x4e, 0xf5, 0x59, 0xd0, 0x88, 0x56, 0xaf, 0x08, 0x28, 0xcf, \
      0xd3, 0xc9, 0x77, 0x55, 0x00, 0x20, 0xc9, 0xfd, 0x10, 0xca, 0x24, 0x56, \
      0x61, 0x3f, 0x47, 0xc6, 0xff, 0x33, 0xd6, 0x9a, 0x5c, 0xb6, 0x7d, 0xc0, \
      0xcf, 0x39, 0xcc, 0x73, 0x9a, 0x0d, 0x36, 0x95, 0x6a, 0x83, 0x45, 0x5f, \
      0x22, 0xdf, 0x96, 0x6c, 0x24, 0xc5, 0x59, 0xae, 0xc9, 0x70, 0x9d, 0x55, \
      0xd6, 0x59, 0xf7, 0x00, 0x60, 0x00, 0x5a, 0x0f, 0x7a, 0x72, 0x1d, 0x3d, \
      0xdb, 0x17, 0x85, 0x7f, 0xef, 0x52, 0xbc, 0xbc, 0x5c, 0x1e, 0xb2, 0x9b, \
      0x84, 0xc9, 0x0b, 0x05, 0x33, 0x03, 0xd3, 0x2f, 0x8b, 0x8f, 0x6a, 0xa6, \
      0xb4, 0x9a, 0xbe, 0x83, 0xde, 0xc3, 0x6e, 0xcf, 0x71, 0x7d, 0xb6, 0x98, \
      0x22, 0xf0, 0x45

#define SAMPLE_RSA_PUBLIC_KEY_BYTES                                           \
  0x30, 0x82, 0x01, 0x22, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86,     \
      0xf7, 0x0d, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x82, 0x01, 0x0f, 0x00, \
      0x30, 0x82, 0x01, 0x0a, 0x02, 0x82, 0x01, 0x01, 0x00, 0xa7, 0x20, 0xe9, \
      0xcb, 0xb9, 0x67, 0x23, 0x5a, 0x9e, 0x65, 0xb8, 0x43, 0xd3, 0xf6, 0x4e, \
      0x58, 0x69, 0x5f, 0x01, 0x88, 0xcc, 0xde, 0x7d, 0x73, 0x44, 0xbb, 0x2e, \
      0xc8, 0x12, 0xc7, 0x4f, 0xf2, 0xb4, 0x51, 0x9a, 0x2b, 0x21, 0xad, 0xce, \
      0xcc, 0x46, 0xe4, 0x59, 0xe4, 0xa6, 0xac, 0xd6, 0x35, 0xd1, 0x7f, 0xaa, \
      0x9f, 0x4a, 0x78, 0x76, 0x15, 0x2a, 0x13, 0x9e, 0x58, 0x44, 0x0c, 0x94, \
      0xda, 0x13, 0x16, 0x9a, 0xa1, 0x61, 0x57, 0x4c, 0x62, 0x9a, 0x6e, 0xb5, \
      0x02, 0x94, 0x3e, 0x8d, 0x20, 0x6e, 0x40, 0x95, 0x90, 0x2a, 0x5e, 0x1f, \
      0x64, 0x17, 0x32, 0xce, 0xda, 0xc8, 0x4a, 0x45, 0x86, 0xd3, 0x4d, 0xd4, \
      0xb3, 0x6d, 0x0d, 0xf6, 0x8a, 0xbd, 0xb3, 0xa8, 0x23, 0x21, 0x55, 0x21, \
      0x72, 0xec, 0xbd, 0x08, 0x74, 0xb9, 0x26, 0xab, 0xab, 0xe2, 0xd5, 0x1a, \
      0x30, 0xa8, 0x3b, 0x13, 0x4f, 0xc1, 0x96, 0x43, 0x3f, 0x04, 0x18, 0xfd, \
      0x69, 0x51, 0x86, 0x84, 0xa0, 0x48, 0x29, 0x3e, 0x21, 0x24, 0x9d, 0xb7, \
      0x32, 0x74, 0x36, 0x03, 0x5f, 0x36, 0x15, 0xe6, 0xf8, 0x49, 0xdb, 0x12, \
      0x0b, 0x21, 0x1a, 0x04, 0xf1, 0xd5, 0xaa, 0xc3, 0xc7, 0xa9, 0xa4, 0xf2, \
      0x56, 0xa4, 0x80, 0xc3, 0x94, 0x85, 0x84, 0x40, 0xb1, 0xd1, 0xea, 0xfa, \
      0x64, 0x63, 0xe3, 0xd7, 0x3c, 0x1a, 0x2a, 0xe3, 0x23, 0x6a, 0x34, 0x1f, \
      0x57, 0x58, 0x4b, 0xd6, 0x9b, 0x97, 0x97, 0x63, 0xaa, 0x38, 0xc0, 0xdd, \
      0x59, 0xfc, 0x86, 0xdd, 0x48, 0x0b, 0x39, 0x85, 0xb5, 0x94, 0xf1, 0xff, \
      0xea, 0xb3, 0x18, 0x06, 0xe5, 0xd4, 0x44, 0xe9, 0xad, 0x51, 0x7d, 0x1d, \
      0x8f, 0x26, 0x7a, 0x50, 0xce, 0x70, 0xd2, 0x90, 0xae, 0xcf, 0xe9, 0x6a, \
      0xf8, 0xef, 0x33, 0x2f, 0x2d, 0x9a, 0x7c, 0x3c, 0x97, 0x59, 0x51, 0x9c, \
      0x27, 0x02, 0x03, 0x01, 0x00, 0x01

#define SAMPLE_MESSAGE_BYTES                                                  \
  0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x21, 0x20, 0x61, 0x6e, 0x64, 0x20, 0x77,     \
      0x65, 0x6c, 0x63, 0x6f, 0x6d, 0x65, 0x20, 0x74, 0x6f, 0x20, 0x73, 0x6f, \
      0x6d, 0x65, 0x20, 0x61, 0x77, 0x65, 0x73, 0x6f, 0x6d, 0x65, 0x20, 0x63, \
      0x72, 0x79, 0x70, 0x74, 0x6f, 0x20, 0x70, 0x72, 0x69, 0x6d, 0x69, 0x74, \
      0x69, 0x76, 0x65, 0x73

#define SAMPLE_RSA_SIGNATURE_BYTES                                            \
  0x41, 0x2c, 0xaa, 0xaf, 0xc6, 0x0c, 0x46, 0x33, 0x1c, 0x5c, 0x92, 0xf1,     \
      0x0b, 0x64, 0xe0, 0x61, 0x51, 0xf6, 0x61, 0x5c, 0xff, 0xe4, 0xe8, 0x43, \
      0xb5, 0x02, 0x6d, 0xb6, 0x5e, 0x96, 0x52, 0xbf, 0xdb, 0x5b, 0xea, 0x2c, \
      0x4c, 0xb2, 0x9a, 0xb3, 0xaa, 0x63, 0xe2, 0x27, 0x04, 0x14, 0xe7, 0x2f, \
      0x18, 0xee, 0xd3, 0x96, 0x72, 0x9c, 0xdd, 0x1b, 0xbc, 0x59, 0x57, 0x81, \
      0x58, 0xd7, 0x88, 0xcb, 0x10, 0xcc, 0x6d, 0x2b, 0x0b, 0x59, 0x57, 0x82, \
      0xb2, 0x22, 0x58, 0xd2, 0x1c, 0xbe, 0x4f, 0x3d, 0xc6, 0x00, 0xa5, 0x23, \
      0x58, 0xac, 0x95, 0xe6, 0x7c, 0x11, 0x9a, 0xee, 0x02, 0xc3, 0xbc, 0x4a, \
      0xb1, 0xa2, 0xdf, 0xa7, 0xaf, 0x9c, 0x8f, 0xf0, 0xb5, 0x72, 0xfa, 0x27, \
      0x05, 0x70, 0x21, 0xc6, 0xbb, 0x24, 0x34, 0x7d, 0x26, 0x95, 0xc9, 0x58, \
      0xa7, 0xe3, 0xb4, 0xce, 0x98, 0x47, 0xcf, 0xf9, 0x21, 0x60, 0x21, 0xa4, \
      0x06, 0x10, 0xa5, 0x0c, 0x92, 0xd7, 0xcc, 0xf0, 0xba, 0x95, 0x41, 0xe4, \
      0x4d, 0x2f, 0x8e, 0x67, 0xc7, 0xab, 0xd6, 0x62, 0xd8, 0x20, 0x98, 0xb1, \
      0xea, 0xba, 0xcf, 0x7a, 0xd9, 0x2d, 0x12, 0x5d, 0xe1, 0x2c, 0x5f, 0x90, \
      0xe3, 0x60, 0x68, 0xf2, 0x45, 0xd8, 0xdf, 0x1d, 0x9f, 0x17, 0xaf, 0x82, \
      0xf8, 0x80, 0x1f, 0x06, 0xf6, 0xd6, 0xa9, 0x88, 0xe1, 0xab, 0x96, 0xb7, \
      0x3d, 0xf0, 0xec, 0xba, 0xda, 0x53, 0x6f, 0x1b, 0x4c, 0x8c, 0x64, 0x8c, \
      0xef, 0x95, 0xec, 0x62, 0xec, 0x53, 0x54, 0x11, 0xb8, 0xbc, 0xac, 0x56, \
      0xbe, 0x81, 0x7f, 0xcd, 0x13, 0x02, 0xb2, 0xd6, 0x0f, 0x04, 0x7d, 0xdb, \
      0x3a, 0x9a, 0x71, 0x63, 0xba, 0x27, 0xca, 0x13, 0x80, 0xb7, 0x7c, 0x51, \
      0x4b, 0x8f, 0x41, 0x18, 0xe1, 0x4f, 0xdb, 0x66, 0xd7, 0xd3, 0x84, 0x92, \
      0x37, 0x27, 0xe2, 0x9b

#define SAMPLE_RSA_INVALID_SIGNATURE_BYTES                                    \
  0x6d, 0xfd, 0x8b, 0xd9, 0xb6, 0x63, 0xb6, 0xd2, 0xc3, 0x5b, 0xfc, 0x6f,     \
      0xc1, 0x1f, 0x40, 0x69, 0x61, 0xa7, 0x66, 0x23, 0x4a, 0xf5, 0x2d, 0xb4, \
      0x4c, 0xc3, 0x79, 0x36, 0x98, 0xec, 0x76, 0xf2, 0xac, 0xdc, 0x2f, 0x8f, \
      0xfc, 0xd2, 0xda, 0xe0, 0xda, 0x18, 0xd5, 0xd2, 0xfe, 0x10, 0x74, 0x4c, \
      0x8e, 0x5a, 0xd5, 0x55, 0x50, 0xb1, 0x13, 0xd4, 0x78, 0xa8, 0x4b, 0x73, \
      0x80, 0xe0, 0xe3, 0x70, 0x00, 0xc8, 0x1a, 0xd7, 0xef, 0xe4, 0x92, 0x1c, \
      0x39, 0x7b, 0x14, 0xa3, 0xdc, 0xbd, 0x01, 0xea, 0x72, 0xdf, 0xc0, 0x3e, \
      0x9f, 0xc0, 0xe3, 0x22, 0x91, 0xcb, 0xa1, 0x5b, 0xe7, 0x46, 0x22, 0x71, \
      0x59, 0x59, 0xa6, 0x20, 0xdf, 0x43, 0x12, 0xcf, 0x1e, 0x8b, 0x1d, 0x3d, \
      0xfa, 0x50, 0xa9, 0x98, 0x8b, 0x80, 0x7b, 0x49, 0x6d, 0xfd, 0x85, 0xe2, \
      0x7c, 0x01, 0xea, 0x52, 0xbb, 0xee, 0x8c, 0xab, 0x9a, 0xd5, 0x96, 0xe0, \
      0xca, 0x73, 0xd6, 0xee, 0x52, 0x20, 0xb9, 0x6e, 0x0d, 0x5c, 0xa8, 0x3e, \
      0x8d, 0x19, 0xfc, 0xb1, 0x69, 0x27, 0x17, 0xc4, 0x73, 0xa6, 0xee, 0x39, \
      0xc5, 0x76, 0x31, 0xc7, 0x55, 0x28, 0xd0, 0xcb, 0x2a, 0x88, 0xeb, 0x55, \
      0x34, 0xbe, 0x97, 0xdd, 0x6e, 0xfc, 0xed, 0x61, 0x9c, 0x26, 0x01, 0x78, \
      0xd8, 0x3c, 0x1c, 0xb5, 0x4b, 0xe9, 0x1c, 0x15, 0x50, 0x51, 0x52, 0xbb, \
      0x57, 0x4e, 0x37, 0x44, 0x1c, 0xa0, 0xa6, 0x8b, 0x4a, 0xd6, 0x0c, 0x8d, \
      0x76, 0x40, 0x72, 0xcf, 0xfc, 0x16, 0x29, 0xb3, 0x56, 0x40, 0xab, 0x84, \
      0x37, 0x79, 0x2f, 0x95, 0x0e, 0x3c, 0x17, 0x0b, 0xe7, 0xcd, 0x13, 0x64, \
      0x1d, 0xd0, 0xfe, 0x66, 0x44, 0x24, 0x6e, 0x0b, 0x6a, 0x9d, 0x03, 0x7d, \
      0xf4, 0x3b, 0xd1, 0x40, 0xda, 0xc0, 0x93, 0x97, 0x6a, 0x72, 0xda, 0xfb, \
      0xb7, 0xe9, 0xd6, 0x51

class RsaProviderTest : public ::testing::Test {
 protected:
  RsaProviderImpl provider_;
  PrivateKey private_key_{SAMPLE_RSA_PRIVATE_KEY_BYTES};
  PublicKey public_key_{SAMPLE_RSA_PUBLIC_KEY_BYTES};
  std::vector<uint8_t> message_{SAMPLE_MESSAGE_BYTES};
  std::vector<uint8_t> signature_{SAMPLE_RSA_SIGNATURE_BYTES};
};

/**
 * @given Pre-generated RSA public key and signature using reference golang
 *        implementation
 * @when Verifying RSA signature
 * @then Pre-generated signature must be valid
 */
TEST_F(RsaProviderTest, SignatureVerificationSuccess) {
  EXPECT_OUTCOME_TRUE(result,
                      provider_.verify(message_, signature_, public_key_));
  ASSERT_TRUE(result);
}

/**
 * @given Pre-generated RSA public key and invalid RSA signature
 * @when Verifying RSA signature
 * @then Signature verification must be successful with result "Invalid
 * signature"
 */
TEST_F(RsaProviderTest, InvalidSignatureVerificationFailure) {
  Signature different_signature{SAMPLE_RSA_INVALID_SIGNATURE_BYTES};
  EXPECT_OUTCOME_TRUE(
      result, provider_.verify(message_, different_signature, public_key_));
  ASSERT_FALSE(result);
}

/**
 * @given Pre-generated RSA signature and invalid public key
 * @when Verifying RSA signature
 * @then Signature verification must be successful with result "Invalid
 * signature"
 */
TEST_F(RsaProviderTest, InvalidPublicKeySignatureVerificationFailure) {
  PublicKey invalid_key = public_key_;
  invalid_key[32] ^= 8;
  EXPECT_OUTCOME_TRUE(result,
                      provider_.verify(message_, signature_, invalid_key));
  ASSERT_FALSE(result);
}

/**
 * @given Pre-generated RSA private key
 * @when Signing message
 * @then Generated signature matches pre-generated
 */
TEST_F(RsaProviderTest, SigningSample) {
  EXPECT_OUTCOME_TRUE(signature, provider_.sign(message_, private_key_));
  EXPECT_EQ(signature, signature_);
}
