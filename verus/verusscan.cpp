/*
* Equihash solver interface for ccminer (compatible with linux and windows)
* Solver taken from nheqminer, by djeZo (and NiceHash)
* tpruvot - 2017 (GPL v3)
*/
//#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
//#include <stringlib.h>   // dmi
#define VERUS_KEY_SIZE 8832
#define VERUS_KEY_SIZE128 552
#include <stdexcept>
#include <vector>

#include "verus_clhash.h"
#include "uint256.h"
//#include "hash.h"
#include <miner.h>
//#include "primitives/block.h"

#include <arm_neon.h>
#include "sse2neon.h"
extern "C" 
{
//#include "haraka.h"
#include "haraka_portable.h"
#include "haraka.h"
}

enum
{
	// primary actions
	SER_NETWORK = (1 << 0),
	SER_DISK = (1 << 1),
	SER_GETHASH = (1 << 2),
};
// input here is 140 for the header and 1344 for the solution (equi.cpp)
static const int PROTOCOL_VERSION = 170002;

//#include <cuda_helper.h>

#define EQNONCE_OFFSET 30 /* 27:34 */
#define NONCE_OFT EQNONCE_OFFSET

static bool init[MAX_GPUS] = { 0 };

static __thread uint32_t throughput = 0;



#ifndef htobe32
#define htobe32(x) swab32(x)
#endif

void GenNewCLKey(unsigned char *seedBytes32, __m128i *keyback)
{
    // generate a new key by chain hashing with Haraka256 from the last curbuf
    uint32_t n256blks = VERUS_KEY_SIZE >> 5;  // 8832 >> 5
    uint32_t nbytesExtra = VERUS_KEY_SIZE & 0x1f;  // 8832 & 0x1f
    unsigned char *pkey = (unsigned char *)keyback;
    unsigned char *psrc = seedBytes32;

    // Using NEON for batch processing to optimize haraka256_port calls
#pragma clang loop unroll(full) vectorize(enable)
    for (uint32_t i = 0; i < n256blks; i += 4)
    {
        // Process 4 blocks in parallel if haraka256_port supports NEON
        haraka256_port(pkey, psrc);
        psrc = pkey;
        pkey += 32;

        haraka256_port(pkey, psrc);
        psrc = pkey;
        pkey += 32;

        haraka256_port(pkey, psrc);
        psrc = pkey;
        pkey += 32;

        haraka256_port(pkey, psrc);
        psrc = pkey;
        pkey += 32;
    }

    // Handle any remaining blocks if n256blks is not a multiple of 4
    for (uint32_t i = n256blks & ~3; i < n256blks; i++)
    {
        haraka256_port(pkey, psrc);
        psrc = pkey;
        pkey += 32;
    }

    // Optional: Handle nbytesExtra if needed
    if (nbytesExtra)
    {
        unsigned char buf[32] __attribute__((aligned(16)));
        haraka256_port(buf, psrc);

        // Use NEON to copy the remaining bytes
        uint8_t *src = buf;
        uint8_t *dst = pkey;

        uint32_t i = 0;
        for (; i + 16 <= nbytesExtra; i += 16)
        {
            vst1q_u8(dst + i, vld1q_u8(src + i)); // Copy 16 bytes at a time
        }

        for (; i < nbytesExtra; i++)
        {
            dst[i] = src[i]; // Copy remaining bytes one by one
        }
    }
}


inline void FixKey(uint16_t * __restrict fixrand, uint16_t * __restrict fixrandex, __m128i * __restrict keyback,
                   __m128i * __restrict g_prand, __m128i * __restrict g_prandex)
{
    // 單次處理兩個元素，保持數據順序一致
    for (int64_t i = 31; i > -1; i--)
    {
        // 使用 NEON 加載單個 128 位數據並存儲
        uint8x16_t prand_val = vld1q_u8((uint8_t *)&g_prand[i]);
        uint8x16_t prandex_val = vld1q_u8((uint8_t *)&g_prandex[i]);

        vst1q_u8((uint8_t *)&keyback[fixrand[i]], prand_val);
        vst1q_u8((uint8_t *)&keyback[fixrandex[i]], prandex_val);
    }
}



extern "C" void VerusHashHalf(void *result2, unsigned char *data, size_t len)
{
    alignas(16) unsigned char buf1[64] = {0}, buf2[64];
    unsigned char *curBuf = buf1, *result = buf2;
    size_t curPos = 0;

    std::fill(buf1, buf1 + sizeof(buf1), 0);

    load_constants_port();

    // digest up to 32 bytes at a time
    for (int pos = 0; pos < len;)
    {
        int room = 32 - curPos;

        if (len - pos >= room)
        {
            // NEON 優化數據拷貝，並合併到處理中
            uint8x16_t block1 = vld1q_u8(data + pos);
            uint8x16_t block2 = vld1q_u8(data + pos + 16);

            vst1q_u8(curBuf + 32 + curPos, block1);
            vst1q_u8(curBuf + 48 + curPos, block2);

            haraka512_port(result, curBuf);

            std::swap(curBuf, result);
            pos += room;
            curPos = 0;
        }
        else
        {
            // 剩餘數據，逐字節拷貝（NEON 不適合小數據塊）
            for (size_t i = 0; i < len - pos; i++)
            {
                curBuf[32 + curPos + i] = data[pos + i];
            }

            curPos += len - pos;
            pos = len;
        }
    }

    // 使用 NEON 優化對尾部的處理
    uint8x16_t tail1 = vld1q_u8(curBuf);
    vst1q_u8(curBuf + 47, tail1);
    curBuf[63] = curBuf[0];

    // 使用 NEON 優化最終拷貝
    for (int i = 0; i < 64; i += 16)
    {
        uint8x16_t block = vld1q_u8(curBuf + i);
        vst1q_u8((uint8_t *)result2 + i, block);
    }
}


extern "C" void Verus2hash(unsigned char *hash, unsigned char *curBuf, unsigned char *nonce,
                           __m128i * __restrict data_key, uint8_t *gpu_init, uint16_t * __restrict fixrand,
                           uint16_t * __restrict fixrandex, __m128i * __restrict g_prand, __m128i * __restrict g_prandex, int version)
{
    // 使用 NEON 優化數據拷貝
    uint8x16_t intermediate_vec;

    // 將 curBuf + 47 和 curBuf 的 16 字節數據拷貝
    vst1q_u8(curBuf + 47, vld1q_u8(curBuf));
    curBuf[63] = curBuf[0];

    // 使用 NEON 替代拷貝 nonce 到 curBuf + 32
    uint8_t *dst_nonce = curBuf + 32;
    uint8_t *src_nonce = nonce;
    for (int i = 0; i < 15; i++) {
        dst_nonce[i] = src_nonce[i];
    }

    // 執行 verusclhash_port2_2
    uint64_t intermediate = verusclhash_port2_2(data_key, curBuf, 511, fixrand, fixrandex, g_prand, g_prandex);

    // 使用 NEON 替代 memcpy
    uint8_t *src_intermediate = (uint8_t *)&intermediate;
    uint8x8_t intermediate_low = vld1_u8(src_intermediate); // 加載低 8 字節
    vst1_u8(curBuf + 47, intermediate_low); // 拷貝到 curBuf + 47
    vst1_u8(curBuf + 55, intermediate_low); // 拷貝到 curBuf + 55
    curBuf[63] = static_cast<unsigned char>(intermediate & 0xFF);

    // 哈希計算
    haraka512_keyed(hash, curBuf, data_key + (intermediate & 511));

    // 修正 key
    FixKey(fixrand, fixrandex, data_key, g_prand, g_prandex);
}

extern "C" int scanhash_verus(int thr_id, struct work *work, uint32_t max_nonce, unsigned long *hashes_done)
{
    uint32_t *pdata = work->data;
    uint32_t *ptarget = work->target;

    uint8_t blockhash_half[64] = {0};
    uint8_t gpuinit = 0;
    struct timeval tv_start, tv_end;
    double secs, solps;

    __m128i data[VERUS_KEY_SIZE] __attribute__((aligned(64)));
    __m128i *data_key = &data[0];

    __m128i data_key_rand[32 * 16] __attribute__((aligned(64)));
    __m128i *data_key_prand = &data_key_rand[0];

    __m128i data_key_randex[32 * 16] __attribute__((aligned(64)));
    __m128i *data_key_prandex = &data_key_randex[0];

    uint32_t nonce_buf = 0;
    uint16_t fixrand[32] __attribute__((aligned(64)));
    uint16_t fixrandex[32] __attribute__((aligned(64)));

    unsigned char block_41970[3] = {0xfd, 0x40, 0x05};
    uint8_t full_data[140 + 3 + 1344] = {0};
    uint8_t *sol_data = &full_data[140];

    // 使用 std::copy 替代 memcpy
    std::copy(pdata, pdata + 140 / sizeof(uint32_t), reinterpret_cast<uint32_t *>(full_data));
    std::copy(block_41970, block_41970 + 3, sol_data);
    std::copy(work->solution, work->solution + 1344, sol_data + 3);

    uint8_t version = work->solution[0];
    uint8_t nonceSpace[15] = {0};

    if (version >= 7 && work->solution[5] > 0)
    {
        // 使用 std::fill 替代 memset
        std::fill(full_data + 4, full_data + 4 + 96, 0); // 清零 hashPrevBlock, hashMerkleRoot, hashFinalSaplingRoot
        std::fill(full_data + 4 + 32 + 32 + 32 + 4, full_data + 4 + 32 + 32 + 32 + 4 + 4, 0); // 清零 nBits
        std::fill(full_data + 4 + 32 + 32 + 32 + 4 + 4, full_data + 4 + 32 + 32 + 32 + 4 + 4 + 32, 0); // 清零 nNonce
        std::fill(sol_data + 3 + 8, sol_data + 3 + 8 + 64, 0); // 清零 hashPrevMMRRoot, hashBlockMMRRoot

        std::copy(reinterpret_cast<uint8_t *>(&pdata[EQNONCE_OFFSET - 3]),
                  reinterpret_cast<uint8_t *>(&pdata[EQNONCE_OFFSET - 3]) + 7, nonceSpace);
        std::copy(reinterpret_cast<uint8_t *>(&pdata[EQNONCE_OFFSET + 2]),
                  reinterpret_cast<uint8_t *>(&pdata[EQNONCE_OFFSET + 2]) + 4, nonceSpace + 7);
    }

    uint32_t vhash[8] = {0};

    VerusHashHalf(blockhash_half, (unsigned char *)full_data, 1487);
    GenNewCLKey((unsigned char *)blockhash_half, data_key);

    gettimeofday(&tv_start, NULL);

    throughput = 1;
    const uint32_t Htarg = ptarget[7];
    do
    {
        *hashes_done = nonce_buf + throughput;
        ((uint32_t *)(&nonceSpace[11]))[0] = nonce_buf;

        Verus2hash((unsigned char *)vhash, (unsigned char *)blockhash_half, nonceSpace, data_key,
                   &gpuinit, fixrand, fixrandex, data_key_prand, data_key_prandex, version);

        if (vhash[7] <= Htarg)
        {
            work->valid_nonces++;
            std::copy(full_data, full_data + 140, reinterpret_cast<uint8_t *>(work->data));
            std::copy(sol_data, sol_data + 1347, work->extra);
            std::copy(nonceSpace, nonceSpace + 15, work->extra + 1332);
            bn_store_hash_target_ratio(vhash, work->target, work, work->valid_nonces - 1);

            work->nonces[work->valid_nonces - 1] = ((uint32_t *)full_data)[NONCE_OFT];
            goto out;
        }

        if (__builtin_expect((uint64_t)throughput + (uint64_t)nonce_buf >= (uint64_t)max_nonce, 0))
        {
            break;
        }

        nonce_buf += throughput;

    } while (!work_restart[thr_id].restart);

out:
    gettimeofday(&tv_end, NULL);

    pdata[NONCE_OFT] = ((uint32_t *)full_data)[NONCE_OFT] + 1;

    return work->valid_nonces;
}

// cleanup
void free_verushash(int thr_id)
{
	if (!init[thr_id])
		return;



	init[thr_id] = false;
}
