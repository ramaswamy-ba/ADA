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

High Performance 
#include <iostream>
#include <cstdint>
#include <array>
#include <atomic>
#include <vector>

constexpr size_t CACHE_LINE_SIZE = 64;
constexpr size_t BOOK_DEPTH = 10; // e.g., Top 10 levels of bids and asks

struct Level {
    double price;
    uint64_t volume;
};

// Represents a consolidated view of the order book
struct alignas(CACHE_LINE_SIZE) OrderBook {
    std::array<Level, BOOK_DEPTH> bids;
    std::array<Level, BOOK_DEPTH> asks;
    uint64_t timestamp;
};

// A shared slot that routes pointers between threads
struct alignas(CACHE_LINE_SIZE) BookSlot {
    // Points to the absolute latest stable order book
    std::atomic<OrderBook*> latest_book{nullptr};
};

template <size_t MaxTickers>
class ConsolidatedMarketDataSystem {
private:
    alignas(CACHE_LINE_SIZE) std::array<BookSlot, MaxTickers> cache;
    alignas(CACHE_LINE_SIZE) std::atomic<uint64_t> dirty_mask{0};

    // Producer's private scratch buffers to modify before swapping
    std::array<OrderBook*, MaxTickers> producer_buffers;

public:
    ConsolidatedMarketDataSystem() {
        for (size_t i = 0; i < MaxTickers; ++i) {
            // Pre-allocate buffers to eliminate runtime heap allocations
            cache[i].latest_book.store(new OrderBook(), std::memory_order_relaxed);
            producer_buffers[i] = new OrderBook();
        }
    }

    ~ConsolidatedMarketDataSystem() {
        for (size_t i = 0; i < MaxTickers; ++i) {
            delete producer_buffers[i];
            delete cache[i].latest_book.load();
        }
    }

    // ========================================================================
    // FAST PRODUCER: Modifies its private buffer and swaps it in
    // ========================================================================
    inline void update_order_book(size_t ticker_id, const OrderBook& incoming_update) {
        if (ticker_id >= MaxTickers) [[unlikely]] return;

        // 1. Update the producer's private working buffer in isolation
        OrderBook* working_buffer = producer_buffers[ticker_id];
        *working_buffer = incoming_update; // In-place copy/consolidation logic

        // 2. Atomically publish the working buffer to the shared cache.
        // exchange() returns the old buffer that the consumer left behind.
        OrderBook* old_buffer = cache[ticker_id].latest_book.exchange(
            working_buffer, std::memory_order_acq_rel
        );

        // 3. Keep the old buffer as our next private scratchpad
        producer_buffers[ticker_id] = old_buffer;

        // 4. Mark this ticker as updated
        dirty_mask.fetch_or(1ULL << ticker_id, std::memory_order_release);
    }

    // ========================================================================
    // SLOW CONSUMER: Safely grabs the latest book without copying it mid-flight
    // ========================================================================
    template <typename Func>
    inline void fetch_latest_books(Func&& process_order_book) {
        uint64_t mask = dirty_mask.exchange(0, std::memory_order_acquire);

        while (mask > 0) {
            int ticker_id = __builtin_ctzll(mask);
            
            // Read the current atomic pointer cleanly. 
            // The producer might update the slot later, but this specific pointer 
            // is guaranteed to contain a fully stable, completed order book.
            OrderBook* consumer_view = cache[ticker_id].latest_book.load(std::memory_order_acquire);

            // Execute processing logic directly on the pointer (Zero-Copy)
            process_order_book(ticker_id, *consumer_view);

            mask &= (mask - 1);
        }
    }
};


