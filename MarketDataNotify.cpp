#include <iostream>
#include <cstdint>
#include <array>
#include <atomic>
#include <type_traits>

constexpr size_t CACHE_LINE_SIZE = 64;

// 1. Structure representing your market data payload
struct alignas(32) MarketData {
    double bid;
    double ask;
    uint64_t volume;
};

// 2. A thread-safe slot wrapper using sequence numbers for lock-free reading
struct alignas(CACHE_LINE_SIZE) ConflatedSlot {
    // Sequence number: Even = stable data, Odd = producer is currently writing
    std::atomic<uint64_t> sequence{0};
    MarketData data{};
};

// 3. The Unified Conflated System
template <size_t MaxTickers>
class ConflatedMarketDataSystem {
private:
    // Core data cache: Index matches Ticker ID
    alignas(CACHE_LINE_SIZE) std::array<ConflatedSlot, MaxTickers> cache{};

    // Reuse our atomic bitmask from earlier to track which tickers are "dirty" (updated)
    // (If MaxTickers > 64, swap this out for the AtomicSparseDenseStrategy)
    alignas(CACHE_LINE_SIZE) std::atomic<uint64_t> dirty_mask{0};

public:
    // Fast Producer Loop: Overwrites in-place and flags the mask. Never blocks.
    inline void update_market_data(size_t ticker_id, const MarketData& new_data) {
        if (ticker_id >= MaxTickers) [[unlikely]] return;

        ConflatedSlot& slot = cache[ticker_id];
        
        // Read current sequence and increment to ODD (signals a write is in progress)
        uint64_t seq = slot.sequence.load(std::memory_order_relaxed);
        slot.sequence.store(seq + 1, std::memory_order_relaxed);

        // Enforce fence so data writes don't reorder before/after the sequence steps
        std::atomic_thread_fence(std::memory_order_release);

        // Overwrite the historical data completely (Conflation)
        slot.data = new_data;

        std::atomic_thread_fence(std::memory_order_release);
        // Increment sequence to EVEN (signals data is stable and readable)
        slot.sequence.store(seq + 2, std::memory_order_release);

        // Mark this ticker as updated in our global mask
        dirty_mask.fetch_or(1ULL << ticker_id, std::memory_order_release);
    }

    // Slow Consumer Loop: Fetches only when it wants to, instantly grabbing the latest snapshot
    template <typename Func>
    inline void fetch_latest_updates(Func&& process_market_data) {
        // Grab a snapshot of all tickers that changed, and clear the mask
        uint64_t mask = dirty_mask.exchange(0, std::memory_order_acquire);

        while (mask > 0) {
            int ticker_id = __builtin_ctzll(mask);
            ConflatedSlot& slot = cache[ticker_id];
            MarketData stable_snapshot;
            uint64_t seq1, seq2;

            // Optimistic Concurrency Control read loop (Lock-Free)
            do {
                seq1 = slot.sequence.load(std::memory_order_acquire);
                stable_snapshot = slot.data;
                std::atomic_thread_fence(std::memory_order_acquire);
                seq2 = slot.sequence.load(std::memory_order_relaxed);
                // Loop continues if a write was in progress (odd) OR if the producer 
                // updated the values mid-read (seq1 != seq2)
            } while ((seq1 & 1) || (seq1 != seq2));

            // Process the latest clean snapshot, skipping intermediate iterations
            process_market_data(ticker_id, stable_snapshot);

            mask &= (mask - 1); // Clear bit and move to next dirty ticker
        }
    }
};
